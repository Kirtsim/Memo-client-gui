#include "dialog/ManageTagsDialog.hpp"
#include "manager/TagCollection.hpp"
#include "model/Tag.hpp"
#include "model/Color.hpp"
#include "ui_ManageTagsDialog.h"

#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QColorDialog>
#include <QPalette>
#include <QDateTime>


namespace {
    QColor ToQColor(const memo::model::Color& color);
    memo::model::Color ToColor(const QColor& color);
    std::shared_ptr<memo::model::Tag> getSelectedTag(const QListWidget& listWidget, const memo::TagCollection& tags);
}

ManageTagsDialog::ManageTagsDialog(const std::shared_ptr<memo::TagCollection>& tags, QWidget* parent)
    : QDialog(parent)
    , ui_(std::make_unique<Ui::ManageTagsDialog>())
    , tags_(tags)
{
    ui_->setupUi(this);
    connect(ui_->searchBar, &QLineEdit::textChanged, this, &ManageTagsDialog::displayTagsWithPrefix);
    connect(ui_->tagsList, &QListWidget::itemSelectionChanged, this, &ManageTagsDialog::onTagSelectionChanged);
    connect(tags_.get(), &memo::TagCollection::tagsAdded, this, &ManageTagsDialog::populateListWidgetWithTags);
    connect(tags_.get(), &memo::TagCollection::tagAdded, this, [&]() { displayTagsWithPrefix(ui_->searchBar->text()); });
    connect(tags_.get(), &memo::TagCollection::tagUpdated, this, [&]() { displayTagsWithPrefix(ui_->searchBar->text()); });
    connect(tags_.get(), &memo::TagCollection::tagRemoved, this, [&]() { displayTagsWithPrefix(ui_->searchBar->text()); });
    connect(ui_->colorButton, &QPushButton::clicked, this, &ManageTagsDialog::pickColor);
    connect(ui_->addButton, &QPushButton::clicked, this, &ManageTagsDialog::onAddButtonClicked);
    connect(ui_->updateButton, &QPushButton::clicked, this, &ManageTagsDialog::onUpdateButtonClicked);
    connect(ui_->removeButton, &QPushButton::clicked, this, &ManageTagsDialog::onRemoveButtonClicked);
    connect(ui_->okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui_->cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    updateDisplayedColor(QColor(Qt::white));
    updateEnableStates();
    tags_->listAll();
}

ManageTagsDialog::~ManageTagsDialog() = default;

void ManageTagsDialog::onAddButtonClicked()
{
    const auto tagName = ui_->searchBar->text().toStdString();
    if (tags_->find(tagName) == nullptr)
    {
        auto newTag = std::make_shared<memo::model::Tag>();
        newTag->setColor(ToColor(displayedColor_));
        newTag->setName(ui_->searchBar->text().toStdString());
        newTag->setTimestamp(QDateTime::currentSecsSinceEpoch());
        tags_->add(newTag);
    }
}

void ManageTagsDialog::onUpdateButtonClicked()
{
    if (auto selectedTag = getSelectedTag(*ui_->tagsList, *tags_))
    {
        selectedTag->setColor(ToColor(displayedColor_));
        tags_->add(selectedTag);
    }
}

void ManageTagsDialog::onRemoveButtonClicked()
{
    const auto tag = getSelectedTag(*ui_->tagsList, *tags_);
    if (!tag)
        return;

    tags_->remove(tag->id());
}

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
}

void ManageTagsDialog::populateListWidgetWithTags(const QList<qulonglong>& tagIds)
{
    using TagPtr = std::shared_ptr<memo::model::Tag>;
    std::vector<TagPtr> tags;
    std::transform(tagIds.begin(), tagIds.end(),
                   std::back_inserter(tags), [&](const qulonglong id) { return tags_->find(id); });
    auto removeIter = std::remove_if(tags.begin(), tags.end(), [](const TagPtr& tag) { return tag == nullptr; });
    tags.erase(removeIter, tags.end());

    const auto previouslySelectedTag = getSelectedTag(*ui_->tagsList, *tags_);
    for (const auto& tag : tags)
    {
        const auto tagName = QString::fromStdString(tag->name());
        auto item = new QListWidgetItem(tagName, ui_->tagsList);
        item->setData(Qt::ItemDataRole::UserRole, QVariant(static_cast<qlonglong>(tag->id())));
    }

    if (previouslySelectedTag)
    {
        const auto name = QString::fromStdString(previouslySelectedTag->name());
        auto matches = ui_->tagsList->findItems(name, Qt::MatchExactly);
        if (!matches.isEmpty())
            matches.front()->setSelected(true);
    }
}

void ManageTagsDialog::onTagSelectionChanged()
{
    if (auto selectedTag = getSelectedTag(*ui_->tagsList, *tags_))
    {
        updateDisplayedColor(ToQColor(selectedTag->color()));
    }
    updateEnableStates();
}

void ManageTagsDialog::pickColor()
{
    if (!colorDialog_)
        colorDialog_ = std::make_unique<QColorDialog>(this);
    if (colorDialog_->exec() == QDialog::Accepted)
    {
        updateDisplayedColor(colorDialog_->selectedColor());
        updateEnableStates();
    }
}

void ManageTagsDialog::updateEnableStates()
{
    const auto searchText = ui_->searchBar->text().toStdString();
    const auto selectedTag = getSelectedTag(*ui_->tagsList, *tags_);

    const bool canUseSearchTextAsNewTag = !searchText.empty() && tags_->find(searchText) == nullptr;
    ui_->addButton->setEnabled(canUseSearchTextAsNewTag);

    ui_->colorButton->setEnabled(canUseSearchTextAsNewTag || selectedTag != nullptr);

    const bool colorUpdated = selectedTag && displayedColor_ != ToQColor(selectedTag->color());
    ui_->updateButton->setEnabled(colorUpdated);

    ui_->removeButton->setEnabled(selectedTag != nullptr);
}

void ManageTagsDialog::updateDisplayedColor(const QColor& color)
{
    QPixmap pixmap(14, 14);
    pixmap.fill(color);
    ui_->colorButton->setIcon(QIcon(pixmap));

    displayedColor_ = color;
}

namespace {
    QColor ToQColor(const memo::model::Color& color)
    {
        return { color.red, color.green, color.blue };
    }

    memo::model::Color ToColor(const QColor& color)
    {
        return { color.red(), color.green(), color.blue() };
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

    void removeTagFromListWidgetById(QListWidget& listWidget, const unsigned long id)
    {
        for (int i = 0; i < listWidget.count(); ++i)
        {
            auto item = listWidget.item(i);
            const auto item_id = static_cast<unsigned long>(item->data(Qt::ItemDataRole::UserRole).value<qulonglong>());
            if (item_id == id)
            {
                listWidget.takeItem(i);
                delete item;
                break;
            }
        }
    }
}
