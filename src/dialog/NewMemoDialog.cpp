#include "dialog/NewMemoDialog.hpp"
#include "widget/EditMemoWidget.hpp"

#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>

NewMemoDialog::NewMemoDialog(QWidget* parent)
    : QDialog(parent)
    , memoWidget_(std::make_unique<EditMemoWidget>())
    , cancelButton_(std::make_unique<QPushButton>("Cancel"))
    , confirmButton_(std::make_unique<QPushButton>("Confirm"))
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
