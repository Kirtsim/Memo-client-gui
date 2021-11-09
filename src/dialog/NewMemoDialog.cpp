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
