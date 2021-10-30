#include "MainWindow.hpp"
#include "model/Memo.hpp"
#include "model/Tag.hpp"
#include "remote/GrpcClient.hpp"
#include "remote/GrpcClientAdapter.hpp"
#include "remote/model/ListTags.hpp"
#include "widget/EditMemoWidget.hpp"
#include "dialog/NewMemoDialog.hpp"
#include "ui_MainWindow.h"

#include <QListWIdgetItem>
#include <QDateTime>
#include <QMessageBox>

using namespace memo;
using TagVector = std::vector<std::shared_ptr<memo::model::Tag>>;

namespace {
    std::string serverUrl() { return "127.0.0.1"; }

    std::string serverPort() { return "8000"; }

    std::unique_ptr<memo::IGrpcClientAdapter> CreateMemoClient();

    TagVector collectTags(const QMap<QString, std::shared_ptr<memo::model::Tag>>& tags, const QStringList& tagNames);
}

MainWindow::MainWindow()
    : QMainWindow()
    , ui_(new Ui::MainWindow)
    , memos_(std::make_unique<memo::MemoCollection>(std::move(CreateMemoClient())))
{
    ui_->setupUi(this);
    auto selectionModel = ui_->memoList->selectionModel();

    connect(selectionModel, &QItemSelectionModel::selectionChanged, this, &MainWindow::processMemoSelection);
    connect(ui_->newMemoButton, &QPushButton::clicked, this, &MainWindow::newMemo);
    connect(ui_->refreshButton, &QPushButton::clicked, memos_.get(), &MemoCollection::listAll);
    connect(ui_->deleteButton, &QPushButton::clicked, this, [&]() {
         auto selected = ui_->memoList->selectedItems();
         if (!selected.isEmpty())
         {
             auto rowItem = selected.front();
             const auto memoTitle = rowItem->text();
             auto memo = memos_->find(memoTitle.toStdString());
             if (memo)
                memos_->remove(memo->id());
         }
    });
    connect(memos_.get(), &MemoCollection::memoCacheCleared, this, [&]() { ui_->memoList->clear(); });
    connect(memos_.get(), &MemoCollection::networkOperationFailed, this, [&](int operation) {
        switch (toMemoOperation(operation))
        {
            case MemoOperation::kAdd: QMessageBox::warning(this, "Server error", "Unable to save memo."); break;
            default: QMessageBox::warning(this, "Server error", "Network error occurred."); break;
        }
    });
    connect(memos_.get(), &MemoCollection::memoAdded, this, [&](unsigned long id) {
        if (auto newMemo = memos_->find(id))
        {
            const auto title = QString::fromStdString(newMemo->title());
            auto listItem = new QListWidgetItem(title, ui_->memoList);
            listItem->setData(Qt::ItemDataRole::UserRole, QVariant(static_cast<qlonglong>(id)));
        }
    });

    connect(memos_.get(), &MemoCollection::memoRemoved, this, [&](unsigned long id) {
        for (int i = 0; i < ui_->memoList->count(); ++i)
        {
            const auto item = ui_->memoList->item(i);
            if (item->data(Qt::ItemDataRole::UserRole).toULongLong() == id)
            {
                ui_->memoList->takeItem(i);
                break;
            }
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui_;
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
        auto title = ui_->memoList->model()->data(index).toString().toStdString();
        if (auto memo = memos_->find(title))
        {
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
        const bool enableConfirmButton = !memos_->find(newTitle.toStdString());
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

        memos_->add(memo);
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

    std::unique_ptr<memo::IGrpcClientAdapter> CreateMemoClient()
    {
        auto client = std::make_unique<memo::GrpcClient>(serverUrl(), serverPort());
        auto clientAdapter = std::make_unique<memo::GrpcClientAdapter>(std::move(client));
        return std::move(clientAdapter);
    }

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
