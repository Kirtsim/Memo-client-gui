#pragma once
#include "manager/TagVault.hpp"
#include "dialog/BaseMemoDialog.hpp"
#include <QDialog>

#include <memory>
#include <set>

namespace memo { class MemoCollection; }
namespace memo { class TagCollection; }
class EditMemoWidget;
class QPushButton;

class NewMemoDialog : public BaseMemoDialog
{
public:
    NewMemoDialog(QWidget* parent,
                  const std::shared_ptr<memo::MemoCollection>& memos,
                  const std::shared_ptr<memo::TagCollection>& tags);

    ~NewMemoDialog() override;

private slots:
    void splitNewTags(const QVector<qulonglong>& tagIds);

};


