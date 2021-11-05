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

    connect(memoWidget_.get(), &EditMemoWidget::userSelectedTagsAdded, this, [&](const QStringList& tagNames) {
        for (const auto& tagName : tagNames)
        {
            auto tag = tags_->find(tagName.toStdString());
            if (tag)
                selectedTags_.add(tag);
        }
    });
    connect(memoWidget_.get(), &EditMemoWidget::userSelectedTagsRemoved, this, [&](const QStringList& tagNames) {
        for (const auto& tagName : tagNames)
            selectedTags_.remove(tagName.toStdString());
    });
    connect(memoWidget_.get(), &EditMemoWidget::titleChanged, this, [&](const QString& newTitle) {
        const bool enableConfirmButton = !memos_->find(newTitle.toStdString());
        this->enableConfirmButton(enableConfirmButton);
    });

    connect(tags_.get(), &memo::TagCollection::tagCacheCleared, memoWidget_.get(), &EditMemoWidget::clearAvailableTags);
    connect(tags_.get(), &memo::TagCollection::tagCacheCleared, memoWidget_.get(), &EditMemoWidget::clearSelectedTags);
    connect(tags_.get(), &memo::TagCollection::tagsAdded, this, &NewMemoDialog::splitNewTags);

    tags_->listAll();
}

NewMemoDialog::~NewMemoDialog() = default;

const EditMemoWidget& NewMemoDialog::memoWidget() const
{
    return *memoWidget_;
}

EditMemoWidget& NewMemoDialog::memoWidget()
{
    return *memoWidget_;
}

void NewMemoDialog::enableConfirmButton(bool enable)
{
    confirmButton_->setEnabled(enable);
}

void NewMemoDialog::splitNewTags(const QVector<qulonglong>& tagIds)
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
