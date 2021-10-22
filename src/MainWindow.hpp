#pragma once
#include <QMainWindow>
#include <QMap>
#include <QItemSelection>
#include <memory>


namespace memo::model { class Memo; }

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
    ~MainWindow() override;

private slots:
    void fetchMemos();
    void processMemoSelection(const QItemSelection& selected, const QItemSelection& deselected);

private:
    void displayMemo(const memo::model::Memo& memo);

private:
    Ui::MainWindow* ui_;
    QMap<QString, std::shared_ptr<memo::model::Memo>> memos_;
};
