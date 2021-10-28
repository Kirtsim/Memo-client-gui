#pragma once
#include <QObject>
#include <QMap>
#include <QString>
#include "remote/model/AddMemo.hpp"
#include "remote/model/ListMemos.hpp"
#include "remote/IGrpcClientAdapter.hpp"

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

class MemoCollection : public QObject
{
    Q_OBJECT
public:
    explicit MemoCollection(std::unique_ptr<IGrpcClientAdapter> client);

    ~MemoCollection();

    bool add(const std::shared_ptr<model::Memo>& memo);

    std::shared_ptr<model::Memo> find(unsigned long memoId);

    std::shared_ptr<model::Memo> find(const std::string& title);

signals:
    void memoCacheCleared();

    void memoAdded(unsigned long memoId);

    void memoRemoved(unsigned long memoId);

    void networkOperationFailed(int operation);

public slots:
    std::vector<std::shared_ptr<model::Memo>> listAll();

    bool remove(unsigned long memoId);

private slots:
    void onWorkerFinished(const QString& workId);

private:
    void processResponse(const GrpcResponse<remote::ListMemosResponse>& response);

    void processResponse(const GrpcResponse<remote::AddMemoResponse>& response,
                         const std::shared_ptr<model::Memo>& memo);

private:
    std::unique_ptr<IGrpcClientAdapter> client_;
    std::unique_ptr<MemoVault> memoVault_;
    QMap<QString, std::shared_ptr<remote::Worker>> workers_;
};

} // namespace memo
