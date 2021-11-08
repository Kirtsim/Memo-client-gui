#include "dialog/NewMemoDialog.hpp"
#include "widget/EditMemoWidget.hpp"
#include "manager/MemoCollection.hpp"
#include "manager/TagCollection.hpp"
#include "model/Tag.hpp"

#include <QPushButton>
#include <QVBoxLayout>
#include <QSpacerItem>

NewMemoDialog::NewMemoDialog(QWidget* parent,
                             const std::shared_ptr<memo::MemoCollection>& memos,
                             const std::shared_ptr<memo::TagCollection>& tags)
    : BaseMemoDialog(memos, tags, parent)
{
    auto& tags_ = BaseMemoDialog::tags();
    auto& memos_ = BaseMemoDialog::memos();
    auto& memoWidget = BaseMemoDialog::memoWidget();
    auto& selectedTags = BaseMemoDialog::selectedTags();
    connect(&memoWidget, &EditMemoWidget::userSelectedTagsAdded, this, [&](const QStringList& tagNames) {
        for (const auto& tagName : tagNames)
        {
            if (auto tag = tags_.find(tagName.toStdString()))
            {
                selectedTags.add(tag);
            }
        }
    });
    connect(&memoWidget, &EditMemoWidget::userSelectedTagsRemoved, this, [&](const QStringList& tagNames) {
        for (const auto& tagName : tagNames)
            selectedTags.remove(tagName.toStdString());
    });
    connect(&memoWidget, &EditMemoWidget::titleChanged, this, [&](const QString& newTitle) {
        const bool enableConfirmButton = !memos_.find(newTitle.toStdString());
        setConfirmButtonEnabled(enableConfirmButton);
    });

    connect(&tags_, &memo::TagCollection::tagCacheCleared, &memoWidget, &EditMemoWidget::clearAvailableTags);
    connect(&tags_, &memo::TagCollection::tagCacheCleared, &memoWidget, &EditMemoWidget::clearSelectedTags);
    connect(&tags_, &memo::TagCollection::tagsAdded, this, &NewMemoDialog::splitNewTags);

    BaseMemoDialog::tags().listAll();
}

NewMemoDialog::~NewMemoDialog() = default;

void NewMemoDialog::splitNewTags(const QVector<qulonglong>& tagIds)
{
    QStringList selectedTagNames;
    QStringList availableTagNames;
    for (const auto id : tagIds)
    {
        if (auto tag = tags().find(id))
        {
            const auto name = QString::fromStdString(tag->name());
            if (selectedTags().find(id))
                selectedTagNames << name;
            else
                availableTagNames << name;
        }
    }
    memoWidget().setAvailableTags(availableTagNames);
    memoWidget().setSelectedTags(selectedTagNames);
}
