#include "dialog/BaseMemoDialog.hpp"
#include "widget/EditMemoWidget.hpp"
#include "manager/MemoCollection.hpp"
#include "manager/TagCollection.hpp"

#include <QHBoxLayout>
#include <QSpacerItem>
#include <QPushButton>

BaseMemoDialog::BaseMemoDialog(const std::shared_ptr<memo::MemoCollection>& memos,
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
    connect(confirmButton_.get(), &QPushButton::clicked, this, &BaseMemoDialog::confirmButtonClicked);

    using EMW = EditMemoWidget;
    connect(memoWidget_.get(), &EMW::userSelectedTagsAdded, this, &BaseMemoDialog::userAddedToSelectedTagsList);
    connect(memoWidget_.get(), &EMW::userSelectedTagsRemoved, this, &BaseMemoDialog::userRemovedFromSelectedTagsList);
    connect(memoWidget_.get(), &EMW::titleChanged, this, &BaseMemoDialog::memoTitleChanged);

    connect(tags_.get(), &memo::TagCollection::tagCacheCleared, memoWidget_.get(), &EMW::clearAvailableTags);
    connect(tags_.get(), &memo::TagCollection::tagCacheCleared, memoWidget_.get(), &EMW::clearSelectedTags);
    //connect(tags_.get(), &memo::TagCollection::tagsAdded, this, &NewMemoDialog::splitNewTags);
}

BaseMemoDialog::~BaseMemoDialog() = default;

EditMemoWidget& BaseMemoDialog::memoWidget()
{
    return *memoWidget_;
}

const EditMemoWidget& BaseMemoDialog::memoWidget() const
{
    return *memoWidget_;
}

memo::MemoCollection& BaseMemoDialog::memos()
{
    return *memos_;
}

const memo::MemoCollection& BaseMemoDialog::memos() const
{
    return *memos_;
}

memo::TagCollection& BaseMemoDialog::tags()
{
    return *tags_;
}

const memo::TagCollection& BaseMemoDialog::tags() const
{
    return *tags_;
}

memo::TagVault& BaseMemoDialog::selectedTags()
{
    return selectedTags_;
}

const memo::TagVault& BaseMemoDialog::selectedTags() const
{
    return selectedTags_;
}

void BaseMemoDialog::setConfirmButtonEnabled(bool enable)
{
    confirmButton_->setEnabled(enable);
}

bool BaseMemoDialog::onConfirmButtonClicked()
{
    return true;
}

void BaseMemoDialog::confirmButtonClicked()
{
    if (onConfirmButtonClicked())
        QDialog::done(QDialog::Accepted);
}

void BaseMemoDialog::userAddedToSelectedTagsList(const QStringList& tags)
{
    for (const auto& tagName : tags)
    {
        if (auto tag = tags_->find(tagName.toStdString()))
        {
            selectedTags_.add(tag);
        }
    }
}

void BaseMemoDialog::userRemovedFromSelectedTagsList(const QStringList& tags)
{
    for (const auto& tagName : tags)
        selectedTags_.remove(tagName.toStdString());
}

void BaseMemoDialog::memoTitleChanged(const QString& newTitle)
{
    const bool enableConfirmButton = !memos_->find(newTitle.toStdString());
    setConfirmButtonEnabled(enableConfirmButton);
}
