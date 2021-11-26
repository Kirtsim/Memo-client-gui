#include "dialog/ManageTagsDialog.hpp"
#include "manager/TagCollection.hpp"
#include "model/Tag.hpp"
#include "ui_ManageTagsDialog.h"

#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QColorDialog>
#include <QPalette>

namespace {
    void updateIconColor(QPushButton& button, const QColor& color);
}

ManageTagsDialog::ManageTagsDialog(const std::shared_ptr<memo::TagCollection>& tags, QWidget* parent)
    : QDialog(parent)
    , ui_(std::make_unique<Ui::ManageTagsDialog>())
    , tags_(tags)
{
    ui_->setupUi(this);
    connect(ui_->searchBar, &QLineEdit::textChanged, this, &ManageTagsDialog::displayTagsWithPrefix);
    connect(tags_.get(), &memo::TagCollection::tagsAdded, this, &ManageTagsDialog::populateListWidgetWithTags);
    connect(ui_->colorButton, &QPushButton::clicked, this, &ManageTagsDialog::pickColor);

    ui_->colorButton->setIcon(QIcon(QPixmap(16, 16)));

    updateEnableStates();
    tags_->listAll();
}

ManageTagsDialog::~ManageTagsDialog() = default;

void ManageTagsDialog::displayTagsWithPrefix(const QString& prefix)
{
    const auto filteredTags = tags_->filterByPrefix(prefix.toStdString());
    ui_->tagsList->clear();
    for (const auto& tag : filteredTags)
    {
        const auto tagName = QString::fromStdString(tag->name());
        auto item = new QListWidgetItem(tagName, ui_->tagsList);
        item->setData(Qt::ItemDataRole::UserRole, QVariant(static_cast<qlonglong>(tag->id())));
    }
    updateEnableStates();
    updateUiElementsBasedOnSearchbar();
}

namespace {
    std::shared_ptr<memo::model::Tag> getSelectedTag(const QListWidget& listWidget, const memo::TagCollection& tags);
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

void ManageTagsDialog::pickColor()
{
    if (!colorDialog_)
        colorDialog_ = std::make_unique<QColorDialog>(this);
    if (colorDialog_->exec() == QDialog::Accepted)
    {
        const auto color = colorDialog_->selectedColor();
        updateIconColor(*ui_->colorButton, color);
    }
}

void ManageTagsDialog::updateEnableStates()
{
    const bool searchBarNotEmpty = !ui_->searchBar->text().isEmpty();

    ui_->updateButton->setEnabled(searchBarNotEmpty);
    ui_->colorButton->setEnabled(searchBarNotEmpty);
}

void ManageTagsDialog::updateUiElementsBasedOnSearchbar()
{
    const auto searchBarText = ui_->searchBar->text();
    ui_->tagName->setText(searchBarText);

    auto searchedTag = tags_->find(searchBarText.toStdString());
    auto selectedTag = getSelectedTag(*ui_->tagsList, *tags_);

    const QString buttonText = (searchedTag || selectedTag) ? "Update" : "Add";
    ui_->updateButton->setText(buttonText);

    auto tag = searchedTag ? searchedTag : selectedTag;
    const auto buttonIconColor = tag ? QColor("red") : QColor("white");
    updateIconColor(*ui_->colorButton, buttonIconColor);
}

namespace {
    void updateIconColor(QPushButton& button, const QColor& color)
    {
        QPixmap pixmap(14, 14);
        pixmap.fill(color);
        button.setIcon(QIcon(pixmap));
    }

    std::shared_ptr<memo::model::Tag> getSelectedTag(const QListWidget& listWidget, const memo::TagCollection& tags)
    {
        const auto items = listWidget.selectedItems();
        if (!items.empty())
        {
            const auto id = items.front()->data(Qt::ItemDataRole::UserRole).toULongLong();
            return tags.find(id);
        }
        return nullptr;
    }
}