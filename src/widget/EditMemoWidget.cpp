#include "widget/EditMemoWidget.hpp"
#include "ui_EditMemoWidget.h"

#include <vector>

namespace {
    void populateListWidget(QListWidget* widget, const QStringList& values);

    void appendValue(QListWidget* widget, const QString& value);

    std::vector<int> indexesOfSelected(const QListWidget& listWidget);
} // namespace

EditMemoWidget::EditMemoWidget(QWidget* parent)
    : QWidget(parent)
    , ui_(new Ui::EditMemoWidget)
{
    ui_->setupUi(this);
    connect(ui_->addButton, &QPushButton::pressed, this, &EditMemoWidget::addTags);
    connect(ui_->removeButton, &QPushButton::pressed, this, &EditMemoWidget::removeTags);
    connect(ui_->availableTags, &QListWidget::itemSelectionChanged, this, &EditMemoWidget::updateButtonStates);
    connect(ui_->selectedTags, &QListWidget::itemSelectionChanged, this, &EditMemoWidget::updateButtonStates);
    connect(ui_->title, &QLineEdit::textChanged, this, &EditMemoWidget::titleChanged);
    updateButtonStates();
}

EditMemoWidget::~EditMemoWidget()
{
    delete ui_;
}

QString EditMemoWidget::title() const
{
    return ui_->title->text();
}

void EditMemoWidget::setTitle(const QString& title)
{
    ui_->title->setText(title);
}

QString EditMemoWidget::description() const
{
    return ui_->description->toPlainText();
}

void EditMemoWidget::setDescription(const QString& description)
{
    ui_->description->setPlainText(description);
}

void EditMemoWidget::setAvailableTags(const QStringList& tags)
{
    populateListWidget(ui_->availableTags, tags);
}

QStringList EditMemoWidget::selectedTags() const
{
    QStringList tagNames;
    auto tagsList = ui_->selectedTags;
    for (int i = 0; i < tagsList->count(); ++i)
    {
        tagNames << tagsList->item(i)->text();
    }
    return tagNames;
}

void EditMemoWidget::setSelectedTags(const QStringList& tags)
{
    populateListWidget(ui_->selectedTags, tags);
}

void EditMemoWidget::clearAvailableTags()
{
    ui_->availableTags->clear();
}

void EditMemoWidget::clearSelectedTags()
{
    ui_->selectedTags->clear();
}

void EditMemoWidget::addTags()
{
    QStringList addedTags;
    const auto indexes = indexesOfSelected(*ui_->availableTags);
    for (int i = static_cast<int>(indexes.size() - 1); i >= 0; --i)
    {
        auto item = ui_->availableTags->takeItem(indexes[i]);
        ui_->selectedTags->addItem(item);
        addedTags << item->text();
    }

    emit userSelectedTagsAdded(addedTags);
    updateButtonStates();
}

void EditMemoWidget::removeTags()
{
    QStringList removedTags;
    const auto indexes = indexesOfSelected(*ui_->selectedTags);
    for (int i = static_cast<int>(indexes.size() - 1); i >= 0; --i)
    {
        auto item = ui_->selectedTags->takeItem(indexes[i]);
        ui_->availableTags->addItem(item);
        removedTags << item->text();
    }

    emit userSelectedTagsRemoved(removedTags);
    updateButtonStates();
}

void EditMemoWidget::updateButtonStates()
{
    const bool availableTagsSelected = !ui_->availableTags->selectedItems().empty();
    const bool selectedTagsSelected = !ui_->selectedTags->selectedItems().empty();
    ui_->addButton->setEnabled(availableTagsSelected);
    ui_->removeButton->setEnabled(selectedTagsSelected);
}

namespace {
    void populateListWidget(QListWidget* widget, const QStringList& values)
    {
        for (const auto& value : values)
            appendValue(widget, value);
    }

    void appendValue(QListWidget* widget, const QString& value)
    {
        new QListWidgetItem(value, widget);
    }

    std::vector<int> indexesOfSelected(const QListWidget& listWidget)
    {
        std::vector<int> indexes;
        for (int i = 0; i < listWidget.count(); ++i)
        {
            if (listWidget.item(i)->isSelected())
                indexes.push_back(i);
        }
        return indexes;
    }
} // namespace