#include "MainWindow.hpp"
#include "model/Memo.hpp"
#include "model/Tag.hpp"
#include "remote/GrpcClient.hpp"
#include "remote/GrpcClientAdapter.hpp"
#include "remote/model/ListMemos.hpp"
#include "remote/model/AddMemo.hpp"
#include "remote/model/ListTags.hpp"
#include "widget/EditMemoWidget.hpp"
#include "dialog/NewMemoDialog.hpp"
#include "ui_MainWindow.h"

#include <QListWIdgetItem>
#include <QDateTime>

using TagVector = std::vector<std::shared_ptr<memo::model::Tag>>;

namespace {
    std::string serverUrl() { return "127.0.0.1"; };
    std::string serverPort() { return "8000"; };

    TagVector collectTags(const QMap<QString, std::shared_ptr<memo::model::Tag>>& tags, const QStringList& tagNames);
}

MainWindow::MainWindow()
    : QMainWindow()
    , ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);
    connect(ui_->refreshButton, &QPushButton::pressed, this, &MainWindow::fetchMemos);
    auto selectionModel = ui_->memoList->selectionModel();
    connect(selectionModel, &QItemSelectionModel::selectionChanged, this, &MainWindow::processMemoSelection);
    connect(ui_->newMemoButton, &QPushButton::pressed, this, &MainWindow::newMemo);
}

MainWindow::~MainWindow()
{
    delete ui_;
}

void MainWindow::fetchMemos()
{
    memo::GrpcClientAdapter client(std::make_unique<memo::GrpcClient>(serverUrl(), serverPort()));
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

void MainWindow::fetchTags()
{
    memo::GrpcClientAdapter client(std::make_unique<memo::GrpcClient>(serverUrl(), serverPort()));
    memo::remote::ListTagsRequest request;
    request.uuid = "5555-5555-1111-3333-2222";
    const auto response = client.listTags(request);
    if (!response.ok())
    {
        // TODO: notify the user or log
        return;
    }
    tags_.clear();
    for (const auto& tag : response.body().tags())
    {
        if (tag)
        {
            tags_[QString::fromStdString(tag->name())] = tag;
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

void MainWindow::newMemo()
{
    fetchTags();
    QStringList tagNames = tags_.keys();

    NewMemoDialog newMemoDialog(this);
    auto& memoWidget = newMemoDialog.memoWidget();
    memoWidget.setAvailableTags(tagNames);
    connect(&memoWidget, &EditMemoWidget::titleChanged, this, [&](const QString& newTitle) {
        const bool enableConfirmButton = !memos_.contains(newTitle);
        newMemoDialog.enableConfirmButton(enableConfirmButton);
    });

    if (newMemoDialog.exec() == QDialog::Accepted)
    {
        auto tags = collectTags(tags_, memoWidget.selectedTags());
        auto memo = std::make_shared<memo::model::Memo>();
        memo->setTitle(memoWidget.title().toStdString());
        memo->setDescription(memoWidget.description().toStdString());
        memo->setTimestamp(QDateTime::currentSecsSinceEpoch());
        memo->setTags(tags);

        memo::remote::AddMemoRequest request;
        request.uuid = "aaaa-bbbb-1111-2222-9999";
        request.memo = memo;

        memo::GrpcClientAdapter client(std::make_unique<memo::GrpcClient>(serverUrl(), serverPort()));
        auto response = client.addMemo(request);
        if (!response.ok())
        {
            // TODO: log and inform the user
            return;
        }
        memo->setId(response.body().addedMemoId());

        const auto title = QString::fromStdString(memo->title());
        memos_[title] = memo;
        new QListWidgetItem(title, ui_->memoList);
    }
}

void MainWindow::displayMemo(const memo::model::Memo& memo)
{
    ui_->memoTitle->setText(QString::fromStdString(memo.title()));
    ui_->memoDescription->setText(QString::fromStdString(memo.description()));
    auto dateTime = QDateTime::fromSecsSinceEpoch(static_cast<long long>(memo.timestamp()));
    ui_->date->setText(dateTime.toString("dd MMM yyyy  hh:mm:ss"));
    QStringList tagNames;
    tagNames.reserve(static_cast<qsizetype>(memo.tags().size()));
    for (const auto& tag : memo.tags())
    {
        if (tag)
        {
            tagNames << ("#" + QString::fromStdString(tag->name()));
        }
    }
    ui_->tags->setText(tagNames.join("  "));
}

namespace {
    TagVector collectTags(const QMap<QString, std::shared_ptr<memo::model::Tag>>& tags, const QStringList& tagNames)
    {
        TagVector resultTags;
        for (const auto& tagName : tagNames)
        {
            auto iter = tags.find(tagName);
            if (iter != tags.end() && iter.value())
            {
                auto tag = iter.value();
                resultTags.emplace_back(tag);
            }
        }
        return resultTags;
    }
} // namespace
