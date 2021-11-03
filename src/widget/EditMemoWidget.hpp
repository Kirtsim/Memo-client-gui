#pragma once
#include <QWidget>

namespace Ui {
    class EditMemoWidget;
}

class EditMemoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EditMemoWidget(QWidget* parent=nullptr);

    ~EditMemoWidget() override;

    QString title() const;

    void setTitle(const QString& title);

    QString description() const;

    void setDescription(const QString& description);

    void setAvailableTags(const QStringList& tags);

    QStringList selectedTags() const;

    void setSelectedTags(const QStringList& tags);

signals:
    void titleChanged(const QString& newTitle);

    void userSelectedTagsAdded(const QStringList& tags);

    void userSelectedTagsRemoved(const QStringList& tags);

public slots:
    void clearAvailableTags();

    void clearSelectedTags();

private slots:
    void addTags();

    void removeTags();

    void updateButtonStates();

private:
    Ui::EditMemoWidget* ui_;
};


