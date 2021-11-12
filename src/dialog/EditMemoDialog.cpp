#include "dialog/EditMemoDialog.hpp"
#include "widget/EditMemoWidget.hpp"
#include "manager/MemoCollection.hpp"
#include "manager/TagCollection.hpp"
#include "model/Memo.hpp"
#include "model/Tag.hpp"

#include <QHBoxLayout>
#include <QSpacerItem>
#include <QPushButton>
#include <QDateTime>

EditMemoDialog::EditMemoDialog(const std::shared_ptr<memo::MemoCollection>& memos,
                               const std::shared_ptr<memo::TagCollection>& tags, QWidget* parent)
    : QDialog(parent)
    , memoWidget_(std::make_unique<EditMemoWidget>())
    , cancelButton_(std::make_unique<QPushButton>("Cancel"))
    , confirmButton_(std::make_unique<QPushButton>("Confirm"))
    , memos_(memos)
    , tags_(tags)
{
    auto buttonLayout = new QHBoxLayout();
    auto horizontalSpacer = new QSpacerItem(0, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    buttonLayout->addItem(horizontalSpacer);
    buttonLayout->addWidget(cancelButton_.get());
    buttonLayout->addWidget(confirmButton_.get());

    auto baseLayout = new QVBoxLayout();
    this->setLayout(baseLayout);
    baseLayout->addWidget(memoWidget_.get());
    baseLayout->addLayout(buttonLayout);

    connect(cancelButton_.get(), &QPushButton::clicked, this, [&]() { QDialog::done(QDialog::Rejected); });
    connect(confirmButton_.get(), &QPushButton::clicked, this, [&]() { QDialog::done(QDialog::Accepted); });

    using EMW = EditMemoWidget;
    connect(memoWidget_.get(), &EMW::userSelectedTagsAdded, this, &EditMemoDialog::userAddedToSelectedTagsList);
    connect(memoWidget_.get(), &EMW::userSelectedTagsRemoved, this, &EditMemoDialog::userRemovedFromSelectedTagsList);
    connect(memoWidget_.get(), &EMW::titleChanged, this, &EditMemoDialog::memoTitleChanged);

    connect(tags_.get(), &memo::TagCollection::tagCacheCleared, memoWidget_.get(), &EMW::clearAvailableTags);
    connect(tags_.get(), &memo::TagCollection::tagCacheCleared, memoWidget_.get(), &EMW::clearSelectedTags);
    connect(tags_.get(), &memo::TagCollection::tagsAdded, this, &EditMemoDialog::splitNewTags);
    tags_->listAll();
}

EditMemoDialog::~EditMemoDialog() = default;

EditMemoWidget& EditMemoDialog::memoWidget()
{
    return *memoWidget_;
}

const EditMemoWidget& EditMemoDialog::memoWidget() const
{
    return *memoWidget_;
}

std::shared_ptr<memo::model::Memo> EditMemoDialog::constructMemo()
{
    auto memo = std::make_shared<memo::model::Memo>();
    memo->setTitle(memoWidget_->title().toStdString());
    memo->setDescription(memoWidget_->description().toStdString());
    memo->setTimestamp(QDateTime::currentSecsSinceEpoch());


    return memo;
}

void EditMemoDialog::userAddedToSelectedTagsList(const QStringList& tags)
{
    for (const auto& tagName : tags)
    {
        if (auto tag = tags_->find(tagName.toStdString()))
        {
            selectedTags_.add(tag);
        }
    }
}

void EditMemoDialog::userRemovedFromSelectedTagsList(const QStringList& tags)
{
    for (const auto& tagName : tags)
        selectedTags_.remove(tagName.toStdString());
}

void EditMemoDialog::memoTitleChanged(const QString& newTitle)
{
    const bool enableConfirmButton = !memos_->find(newTitle.toStdString());
    confirmButton_->setEnabled(enableConfirmButton);
}

void EditMemoDialog::splitNewTags(const QVector<qulonglong>& tagIds)
{
    QStringList selectedTagNames;
    QStringList availableTagNames;
    for (const auto id : tagIds)
    {
        if (auto tag = tags_->find(id))
        {
            const auto name = QString::fromStdString(tag->name());
            if (selectedTags_.find(id))
                selectedTagNames << name;
            else
                availableTagNames << name;
        }
    }
    memoWidget_->setAvailableTags(availableTagNames);
    memoWidget_->setSelectedTags(selectedTagNames);
}