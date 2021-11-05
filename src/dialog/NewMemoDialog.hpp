#pragma once
#include "manager/TagVault.hpp"
#include <QDialog>

#include <memory>
#include <set>

namespace memo { class MemoCollection; }
namespace memo { class TagCollection; }
class EditMemoWidget;
class QPushButton;

class NewMemoDialog : public QDialog
{
public:
    NewMemoDialog(QWidget* parent,
                  const std::shared_ptr<memo::MemoCollection>& memos,
                  const std::shared_ptr<memo::TagCollection>& tags);

    ~NewMemoDialog() override;

    const EditMemoWidget& memoWidget() const;

    EditMemoWidget& memoWidget();

    void enableConfirmButton(bool enable);

private slots:
    void splitNewTags(const QVector<qulonglong>& tagIds);

private:
    std::unique_ptr<EditMemoWidget> memoWidget_;
    std::unique_ptr<QPushButton> cancelButton_;
    std::unique_ptr<QPushButton> confirmButton_;
    memo::TagVault selectedTags_;
    std::shared_ptr<memo::MemoCollection> memos_;
    std::shared_ptr<memo::TagCollection> tags_;
};


