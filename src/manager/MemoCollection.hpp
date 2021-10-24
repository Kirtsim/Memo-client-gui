#pragma once
#include <QObject>

#include <string>
#include <map>
#include <memory>

namespace memo {
    class IGrpcClientAdapter;

namespace model { class Memo; }

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

private:
    std::unique_ptr<IGrpcClientAdapter> client_;
    std::map<unsigned long, std::shared_ptr<model::Memo>> idToMemo_;
    std::map<std::string, std::shared_ptr<model::Memo>> titleToMemo_;
};

} // namespace memo
