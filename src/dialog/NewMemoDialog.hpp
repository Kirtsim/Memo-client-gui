#pragma once
#include <QDialog>

#include <memory>

class EditMemoWidget;
class QPushButton;

class NewMemoDialog : public QDialog
{
public:
    NewMemoDialog(QWidget* parent);

    ~NewMemoDialog() override;

    const EditMemoWidget& memoWidget() const;

    EditMemoWidget& memoWidget();

    void enableConfirmButton(bool enable);

private:
    void updateButtonStates();

private:
    std::unique_ptr<EditMemoWidget> memoWidget_;
    std::unique_ptr<QPushButton> cancelButton_;
    std::unique_ptr<QPushButton> confirmButton_;
};


