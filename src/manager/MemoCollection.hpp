#pragma once
#include "remote/concurrency/visitor/WorkerVisitor.hpp"

#include <QObject>
#include <QMap>
#include <QString>

#include <string>
#include <map>
#include <memory>

namespace memo {
namespace model { class Memo; }
namespace remote { class Worker; }

    class IGrpcClientAdapter;
    class MemoVault;

enum class MemoOperation
{
    kUnknown = 0,
    kListAll = 1,
    kAdd = 1 << 1,
    kRemove = 1 << 2,
};

MemoOperation toMemoOperation(int value);

class MemoCollection : public QObject, remote::MemoWorkerVisitor
{
    Q_OBJECT
public:
    explicit MemoCollection(std::unique_ptr<IGrpcClientAdapter> client);

    ~MemoCollection() override;

    bool add(const std::shared_ptr<model::Memo>& memo);

    std::shared_ptr<model::Memo> find(unsigned long memoId) const;

    std::shared_ptr<model::Memo> find(const std::string& title) const;

    int count() const;

    bool empty() const;

    void visit(const remote::QueryMemoWorker& worker) override;

    void visit(const remote::CreateMemoWorker& worker) override;

    void visit(const remote::UpdateMemoWorker& worker) override;

    void visit(const remote::RemoveMemoWorker& worker) override;

signals:
    void memoCacheCleared();

    void memoAdded(unsigned long memoId);

    void memoUpdated(unsigned long memoId);

    void memoRemoved(unsigned long memoId);

    void networkOperationFailed(int operation);

public slots:
    std::vector<std::shared_ptr<model::Memo>> listAll();

    bool remove(unsigned long memoId);

private slots:
    void onWorkerFinished(const QString& workId);

private:
    std::unique_ptr<IGrpcClientAdapter> client_;
    std::unique_ptr<MemoVault> memoVault_;
    QMap<QString, remote::Worker*> workers_;
};

} // namespace memo
