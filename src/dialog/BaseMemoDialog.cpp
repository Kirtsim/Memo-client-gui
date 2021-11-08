#include "dialog/BaseMemoDialog.hpp"
#include "widget/EditMemoWidget.hpp"
#include "manager/MemoCollection.hpp"
#include "manager/TagCollection.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
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
    connect(confirmButton_.get(), &QPushButton::clicked, this, [&]()
    {
        if (onConfirmButtonClicked())
            QDialog::done(QDialog::Accepted);
    });
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
