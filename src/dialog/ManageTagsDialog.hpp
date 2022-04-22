#pragma once
#include <QDialog>
#include <QWidget>
#include <QColor>

#include <memory>

namespace Ui { class ManageTagsDialog; }

namespace memo { class TagCollection; }

class QColorDialog;

class ManageTagsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ManageTagsDialog(const std::shared_ptr<memo::TagCollection>& tags, QWidget* parent=nullptr);

    ~ManageTagsDialog() override;

private slots:
    void onAddButtonClicked();

    void onUpdateButtonClicked();

    void displayTagsWithPrefix(const QString& prefix);

    void populateListWidgetWithTags(const QList<qulonglong>& tagIds);

    void onTagSelectionChanged();

    void pickColor();

private:
    void updateEnableStates();

    void updateColorButtonIcon(const QColor& color);

    void updateDisplayedColor(const QColor& color);

private:
    std::unique_ptr<Ui::ManageTagsDialog> ui_;
    std::shared_ptr<memo::TagCollection> tags_;
    std::unique_ptr<QColorDialog> colorDialog_;
    QColor displayedColor_;
};
