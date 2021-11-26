#pragma once
#include <QDialog>
#include <QWidget>

#include <memory>

namespace Ui { class ManageTagsDialog; }

namespace memo { class TagCollection; }

class QColorDialog;

class ManageTagsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ManageTagsDialog(const std::shared_ptr<memo::TagCollection>& tags, QWidget* parent);

    ~ManageTagsDialog() override;

private slots:
    void displayTagsWithPrefix(const QString& prefix);

    void populateListWidgetWithTags(const QList<qulonglong>& tagIds);

    void pickColor();

private:
    void updateEnableStates();

    void updateUiElementsBasedOnSearchbar();

private:
    std::unique_ptr<Ui::ManageTagsDialog> ui_;
    std::shared_ptr<memo::TagCollection> tags_;
    std::unique_ptr<QColorDialog> colorDialog_;
};


