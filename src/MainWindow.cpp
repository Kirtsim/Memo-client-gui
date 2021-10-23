#include "MainWindow.hpp"
#include "model/Memo.hpp"
#include "model/Tag.hpp"
#include "remote/GrpcClient.hpp"
#include "remote/GrpcClientAdapter.hpp"
#include "remote/model/ListMemos.hpp"
#include "ui_MainWindow.h"
#include <QListWIdgetItem>
#include <QDateTime>

MainWindow::MainWindow()
    : QMainWindow()
    , ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);
    connect(ui_->refreshButton, &QPushButton::pressed, this, &MainWindow::fetchMemos);
    auto selectionModel = ui_->memoList->selectionModel();
    connect(selectionModel, &QItemSelectionModel::selectionChanged, this, &MainWindow::processMemoSelection);
}

MainWindow::~MainWindow()
{
    delete ui_;
}

void MainWindow::fetchMemos()
{
    memo::GrpcClientAdapter client(std::make_unique<memo::GrpcClient>("127.0.0.1", "8000"));
    memo::remote::ListMemosRequest request;
    request.uuid = "123-123-123-123-123";
    auto response = client.listMemos(request);

    if (response.ok())
    {
        const auto& responseData = response.body();
        ui_->memoList->clear();

        for (const auto& memo : responseData.memos())
        {
            const auto title = QString::fromStdString(memo->title());
            memos_[title] = memo;
            new QListWidgetItem(title, ui_->memoList);
        }
    }
}

void MainWindow::processMemoSelection(const QItemSelection& selected, const QItemSelection& deselected)
{
    if (!selected.indexes().empty())
    {
        auto index = selected.indexes().front();
        auto memoTitle = ui_->memoList->model()->data(index).toString();
        auto iter = memos_.find(memoTitle);
        if (iter != memos_.end() && iter.value())
        {
            const auto memo = iter.value();
            displayMemo(*memo);
        }
    }
}

void MainWindow::displayMemo(const memo::model::Memo& memo)
{
    ui_->memoTitle->setText(QString::fromStdString(memo.title()));
    ui_->memoDescription->setText(QString::fromStdString(memo.description()));
    auto dateTime = QDateTime::fromMSecsSinceEpoch(static_cast<long long>(memo.timestamp()));
    ui_->date->setText(dateTime.toString("dd MMM yyyy  hh:mm:ss"));
    QStringList tagNames;
    tagNames.reserve(static_cast<qsizetype>(memo.tags().size()));
    for (const auto& tag : memo.tags())
    {
        if (tag)
        {
            tagNames << QString::fromStdString(tag->name());
        }
    }
    ui_->tags->setText(tagNames.join("  "));
}
