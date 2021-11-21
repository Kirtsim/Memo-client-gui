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
    connect(memoWidget_.get(), &EMW::titleChanged, this, &EditMemoDialog::updateEnableStates);
    connect(memoWidget_.get(), &EMW::descriptionChanged, this, &EditMemoDialog::updateEnableStates);

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

void EditMemoDialog::setDetailsFromMemo(const memo::model::Memo& memo)
{
    QSet<QString> tags;
    for (const auto& tag : memo.tags())
    {
        if (tag)
        {
            const auto name = QString::fromStdString(tag->name());
            tags << name;
        }
    }
    memoWidget_->setTitle(QString::fromStdString(memo.title()));
    memoWidget_->setDescription(QString::fromStdString(memo.description()));
    memoWidget_->setSelectedTags({tags.begin(), tags.end()});

    baseTitle_ = QString::fromStdString(memo.title());
    baseDescription_ = QString::fromStdString(memo.description());
    baseTags_ = tags;
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
    updateEnableStates();
}

void EditMemoDialog::userRemovedFromSelectedTagsList(const QStringList& tags)
{
    for (const auto& tagName : tags)
        selectedTags_.remove(tagName.toStdString());
    updateEnableStates();
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
            else if (baseTags_ && baseTags_->find(name) != baseTags_->end())
            {
                selectedTags_.add(tag);
                selectedTagNames << name;
            }
            else
                availableTagNames << name;
        }
    }
    memoWidget_->setAvailableTags(availableTagNames);
    memoWidget_->setSelectedTags(selectedTagNames);
}

namespace {
    bool haveSameTags(const QSet<QString>& baseTags, const QStringList& actualTags);
} // namespace

void EditMemoDialog::updateEnableStates()
{
    const auto title = memoWidget_->title();
    const bool enableConfirmButton = !memos_->find(title.toStdString()) &&
            ((baseTitle_ && baseTitle_ != title)
            || (baseDescription_ && baseDescription_ != memoWidget_->description())
            || (baseTags_ && !haveSameTags(baseTags_.value(), memoWidget_->selectedTags())));
    confirmButton_->setEnabled(enableConfirmButton);
}

namespace {
    bool haveSameTags(const QSet<QString>& baseTags, const QStringList& actualTags)
    {
        if (baseTags.size() != actualTags.size())
            return false;
        return std::all_of(actualTags.begin(), actualTags.end(), [&baseTags](const QString& tag)
        { return baseTags.find(tag) != baseTags.end(); });
    }
}
