#pragma once
#include "manager/TagVault.hpp"
#include <QDialog>

#include <memory>

namespace memo { class MemoCollection; }
namespace memo { class TagCollection; }
class EditMemoWidget;
class QPushButton;

class BaseMemoDialog : public QDialog
{
    Q_OBJECT
public:
    BaseMemoDialog(const std::shared_ptr<memo::MemoCollection>& memos,
                   const std::shared_ptr<memo::TagCollection>& tags,
                   QWidget* parent=nullptr);

    ~BaseMemoDialog() override;

    EditMemoWidget& memoWidget();

    const EditMemoWidget& memoWidget() const;

protected slots:

protected:

    memo::MemoCollection& memos();

    const memo::MemoCollection& memos() const;

    memo::TagCollection& tags();

    const memo::TagCollection& tags() const;

    memo::TagVault& selectedTags();

    const memo::TagVault& selectedTags() const;

    void setConfirmButtonEnabled(bool enable);

    virtual bool onConfirmButtonClicked();

private slots:
    void confirmButtonClicked();

    void userAddedToSelectedTagsList(const QStringList& tags);

    void userRemovedFromSelectedTagsList(const QStringList& tags);

    void memoTitleChanged(const QString& newTitle);

private:
    std::unique_ptr<EditMemoWidget> memoWidget_;
    std::unique_ptr<QPushButton> cancelButton_;
    std::unique_ptr<QPushButton> confirmButton_;
    memo::TagVault selectedTags_;
    std::shared_ptr<memo::MemoCollection> memos_;
    std::shared_ptr<memo::TagCollection> tags_;
};


