#include "manager/MemoCollection.hpp"
#include "manager/MemoVault.hpp"
#include "remote/IGrpcClientAdapter.hpp"
#include "remote/model/ListMemos.hpp"
#include "remote/model/AddMemo.hpp"
#include "model/Memo.hpp"

#include <QtConcurrent/QtConcurrent>

namespace memo {

class WorkerThread : public QThread
{
   Q_OBJECT
public:
    WorkerThread(IGrpcClientAdapter& client, const QString& id)
        : client_(client)
        , id_(id)
    {
    }

    virtual ~WorkerThread() override = default;

    const QString& id() const
    {
       return id_;
    }

signals:
    void resultReady(const QString& id);

protected:
    IGrpcClientAdapter& client()
    {
        return client_;
    }

private:
    IGrpcClientAdapter& client_;
    QString id_;
};

class ListAllTask : public WorkerThread
{
    Q_OBJECT
public:
    ListAllTask(IGrpcClientAdapter& client, const QString& id)
        : WorkerThread(client, id)
        , result_(false, {})
    {
    }

    void run() override
    {
        remote::ListMemosRequest request;
        request.uuid = "123-123-123-123-123";
        result_ = client().listMemos(request);
        emit resultReady(id());
    }

    const GrpcResponse<remote::ListMemosResponse>& result() const
    {
        return result_;
    }

private:
    GrpcResponse<remote::ListMemosResponse> result_;
};

class AddMemoTask : public WorkerThread
{
Q_OBJECT
public:
    AddMemoTask(IGrpcClientAdapter& client, const QString& id, const std::shared_ptr<model::Memo>& memo)
            : WorkerThread(client, id)
            , result_(false, {})
            , memo_(memo)
    {
    }

    void run() override
    {
        remote::AddMemoRequest request;
        request.uuid = "123-123-123-123-123";
        request.memo = memo_;
        result_ = client().addMemo(request);
        emit resultReady(id());
    }

    const std::shared_ptr<model::Memo>& memo() const
    {
        return memo_;
    }

    const GrpcResponse<remote::AddMemoResponse>& result() const
    {
        return result_;
    }

private:
    GrpcResponse<remote::AddMemoResponse> result_;
    std::shared_ptr<model::Memo> memo_;
};

MemoCollection::MemoCollection(std::unique_ptr<IGrpcClientAdapter> client)
    : QObject()
    , client_(std::move(client))
    , memoVault_(std::make_unique<MemoVault>())
{
}

MemoCollection::~MemoCollection() = default;

bool MemoCollection::add(const std::shared_ptr<model::Memo>& memo)
{
    QString workId = "Add_" + QString::fromStdString(memo->title());
    if (workers_.contains(workId))
        return true;

    auto worker = std::make_shared<AddMemoTask>(*client_, workId, memo);
    connect(worker.get(), &WorkerThread::resultReady, this, &MemoCollection::onWorkerFinished);

    workers_[workId] = worker;
    worker->run();
    return true;
}

std::shared_ptr<model::Memo> MemoCollection::find(unsigned long memoId)
{
    return memoVault_->find(memoId);
}

std::shared_ptr<model::Memo> MemoCollection::find(const std::string& title)
{
    return memoVault_->find(title);
}

MemoVector MemoCollection::listAll()
{
    const QString workId = "LIST_ALL";
    auto worker = std::make_shared<ListAllTask>(*client_, workId);
    workers_[workId] = worker;

    connect(worker.get(), &ListAllTask::resultReady, this, &MemoCollection::onWorkerFinished);
    worker->start();
    return memoVault_->list();
}

bool MemoCollection::remove(unsigned long memoId)
{
    memoVault_->remove(memoId);
    return true;
}

void MemoCollection::onWorkerFinished(const QString& workId)
{
    auto iter = workers_.find(workId);
    if (iter == std::end(workers_) || !iter.value())
        return;
    auto workerThread = iter.value();
    workers_.remove(workId);

    if (workId == "LIST_ALL")
    {
        if (auto worker = std::dynamic_pointer_cast<ListAllTask>(workerThread))
            processResponse(worker->result());
    }
    else if (workId.startsWith("Add_"))
    {
        if (auto worker = std::dynamic_pointer_cast<AddMemoTask>(workerThread))
            processResponse(worker->result(), *worker->memo());
    }
}

void MemoCollection::processResponse(const GrpcResponse<remote::ListMemosResponse>& response)
{
    if (response.ok())
    {
        memoVault_->clear();
        emit memoCacheCleared();

        const auto& responseData = response.body();
        for (const auto& memo : responseData.memos())
        {
            memoVault_->add(memo);
            emit memoAdded(memo->id());
        }
    }
}

void MemoCollection::processResponse(const GrpcResponse<remote::AddMemoResponse>& response, const model::Memo& memo)
{
    if (response.ok())
    {
        auto copy = std::make_shared<model::Memo>(memo);
        copy->setId(response.body().addedMemoId());
        memoVault_->add(copy);
        emit memoAdded(copy->id());
    }
}

MemoOperation toMemoOperation(int value)
{
    switch (value)
    {
        case 1: return MemoOperation::kListAll;
        case 1 << 1: return MemoOperation::kAdd;
        case 1 << 2: return MemoOperation::kRemove;
        default: return MemoOperation::kUnknown;
    }
}

} // namespace memo

#include "MemoCollection.moc"
