#pragma once
#include "manager/MemoCollection.hpp"
#include "manager/TagCollection.hpp"
#include <QMainWindow>
#include <QMap>
#include <QItemSelection>
#include <memory>


namespace memo { class IGrpcClientAdapter; }

namespace memo::model { class Memo; }
namespace memo::model { class Tag; }

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
    ~MainWindow() override;

private slots:
    void processMemoSelection(const QItemSelection& selected, const QItemSelection& deselected);
    void newMemo();

private:
    void displayMemo(const memo::model::Memo& memo);

private:
    Ui::MainWindow* ui_;
    std::shared_ptr<memo::MemoCollection> memos_;
    std::shared_ptr<memo::TagCollection> tags_;
};
