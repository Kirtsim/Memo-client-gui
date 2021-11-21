#include "dialog/ManageTagsDialog.hpp"
#include "manager/TagCollection.hpp"
#include "model/Tag.hpp"
#include "ui_ManageTagsDialog.h"

#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>

ManageTagsDialog::ManageTagsDialog(const std::shared_ptr<memo::TagCollection>& tags, QWidget* parent)
    : QDialog(parent)
    , ui_(std::make_unique<Ui::ManageTagsDialog>())
    , tags_(tags)
{
    ui_->setupUi(this);
    auto searchBar = ui_->searchBar;
    connect(searchBar, &QLineEdit::textChanged, this, &ManageTagsDialog::displayTagsWithPrefix);
    connect(tags_.get(), &memo::TagCollection::tagsAdded, this, &ManageTagsDialog::populateListWidgetWithTags);

    tags_->listAll();
}

ManageTagsDialog::~ManageTagsDialog() = default;

void ManageTagsDialog::displayTagsWithPrefix(const QString& prefix)
{

}

void ManageTagsDialog::populateListWidgetWithTags(const QList<qulonglong>& tagIds)
{
    using TagPtr = std::shared_ptr<memo::model::Tag>;
    std::vector<TagPtr> tags;
    std::transform(tagIds.begin(), tagIds.end(),
                   std::back_inserter(tags), [&](const qulonglong id) { return tags_->find(id); });
    auto removeIter = std::remove_if(tags.begin(), tags.end(), [](const TagPtr& tag) { return tag == nullptr; });
    tags.erase(removeIter, tags.end());

    for (const auto& tag : tags)
    {
        const auto tagName = QString::fromStdString(tag->name());
        auto item = new QListWidgetItem(tagName, ui_->tagsList);
        item->setData(Qt::ItemDataRole::UserRole, QVariant(static_cast<qlonglong>(tag->id())));
    }
}
