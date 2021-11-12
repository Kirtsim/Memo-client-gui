#pragma once
#include "manager/TagVault.hpp"
#include <QDialog>

#include <memory>

namespace memo { class MemoCollection; }
namespace memo { class TagCollection; }
namespace memo::model { class Memo; }
class EditMemoWidget;
class QPushButton;

class EditMemoDialog : public QDialog
{
    Q_OBJECT
public:
    EditMemoDialog(const std::shared_ptr<memo::MemoCollection>& memos,
                   const std::shared_ptr<memo::TagCollection>& tags,
                   QWidget* parent=nullptr);

    ~EditMemoDialog() override;

    EditMemoWidget& memoWidget();

    const EditMemoWidget& memoWidget() const;

    std::shared_ptr<memo::model::Memo> constructMemo();

private slots:
    void userAddedToSelectedTagsList(const QStringList& tags);

    void userRemovedFromSelectedTagsList(const QStringList& tags);

    void memoTitleChanged(const QString& newTitle);

    void splitNewTags(const QVector<qulonglong>& tagIds);

private:
    std::unique_ptr<EditMemoWidget> memoWidget_;
    std::unique_ptr<QPushButton> cancelButton_;
    std::unique_ptr<QPushButton> confirmButton_;
    memo::TagVault selectedTags_;
    std::shared_ptr<memo::MemoCollection> memos_;
    std::shared_ptr<memo::TagCollection> tags_;
};


