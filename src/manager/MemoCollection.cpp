#include "manager/MemoCollection.hpp"
#include "manager/MemoVault.hpp"
#include "remote/IGrpcClientAdapter.hpp"
#include "remote/model/ListMemos.hpp"
#include "remote/model/AddMemo.hpp"
#include "remote/model/RemoveMemo.hpp"
#include "remote/model/UpdateMemo.hpp"
#include "remote/concurrency/QueryWorker.hpp"
#include "remote/concurrency/CreationWorker.hpp"
#include "remote/concurrency/UpdateWorker.hpp"
#include "remote/concurrency/RemovalWorker.hpp"
#include "model/Memo.hpp"

#include <QtConcurrent/QtConcurrent>

namespace memo {

MemoCollection::MemoCollection(std::unique_ptr<IGrpcClientAdapter> client)
    : QObject()
    , client_(std::move(client))
    , memoVault_(std::make_unique<MemoVault>())
{
}

MemoCollection::~MemoCollection() = default;

bool MemoCollection::add(const std::shared_ptr<model::Memo>& memo)
{
    QString workId;

    std::shared_ptr<remote::Worker> worker;
    if (memoVault_->find(memo->id()))
    {
        workId = "UPDATE_" + QString::fromStdString(memo->title());
        remote::UpdateMemoRequest request;
        request.uuid = "000100-190939-390239";
        request.memo = memo;
        worker = std::make_shared<remote::UpdateMemoWorker>(workId, *client_, request);
    }
    else
    {
        workId = "Add_" + QString::fromStdString(memo->title());
        remote::AddMemoRequest request;
        request.uuid = "123123-123123-123123-123123";
        request.memo = std::make_shared<model::Memo>(*memo);
        worker = std::make_shared<remote::CreateMemoWorker>(workId, *client_, request);
    }
    if (workers_.contains(workId))
        return true;
    connect(worker.get(), &remote::CreateMemoWorker::workFinished, this, &MemoCollection::onWorkerFinished);

    workers_[workId] = worker;
    worker->start();
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
    const QString workId = "LIST_ALL_MEMOS";
    remote::ListMemosRequest request;
    request.uuid = "5555-5555-5555-5555";

    auto worker = std::make_shared<remote::QueryMemoWorker>(workId, *client_, request);
    if (workers_.contains(workId))
        workers_[workId]->quit();
    workers_[workId] = worker;

    connect(worker.get(), &remote::QueryMemoWorker::workFinished, this, &MemoCollection::onWorkerFinished);
    worker->start();
    return memoVault_->list();
}

bool MemoCollection::remove(unsigned long memoId)
{
    remote::RemoveMemoRequest request;
    request.uuid = "123123-123123-12-3123";
    request.memoIds.emplace_back(memoId);

    auto worker = std::make_shared<remote::RemoveMemoWorker>("REMOVE_", *client_, request);
    connect(worker.get(), &remote::RemoveMemoWorker::workFinished, this, &MemoCollection::onWorkerFinished);
    workers_["REMOVE_"] = worker;

    worker->start();
    return true;
}

void MemoCollection::onWorkerFinished(const QString& workId)
{
    auto iter = workers_.find(workId);
    if (iter == std::end(workers_) || !iter.value())
        return;
    auto workerThread = iter.value();
    workers_.remove(workId);

    if (workId == "LIST_ALL_MEMOS")
    {
        if (auto worker = std::dynamic_pointer_cast<remote::QueryMemoWorker>(workerThread))
            processResponse(worker->response());
    }
    else if (workId.startsWith("Add_"))
    {
        if (auto worker = std::dynamic_pointer_cast<remote::CreateMemoWorker>(workerThread))
        {
            if (worker->request().memo)
                processResponse(worker->response(), worker->request().memo);
        }
    }
    else if (workId.startsWith("REMOVE_"))
    {
        if (auto worker = std::dynamic_pointer_cast<remote::RemoveMemoWorker>(workerThread))
            processResponse(worker->response());

    }
    else if (workId.startsWith("UPDATE_"))
    {
        if (auto worker = std::dynamic_pointer_cast<remote::UpdateMemoWorker>(workerThread))
            processResponse(worker->response(), worker->request().memo);
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

void MemoCollection::processResponse(const GrpcResponse<remote::AddMemoResponse>& response,
                                     const std::shared_ptr<model::Memo>& memo)
{
    if (response.ok() && memo)
    {
        auto copy = std::make_shared<model::Memo>(*memo);
        copy->setId(response.body().addedMemoId());
        memoVault_->add(copy);
        emit memoAdded(copy->id());
    }
}

void MemoCollection::processResponse(const GrpcResponse<remote::RemoveMemoResponse>& response)
{
    if (response.ok())
    {
        for (const auto memoId : response.body().removedMemoIds())
        {
            memoVault_->remove(memoId);
            emit memoRemoved(memoId);
        }
    }
}

void MemoCollection::processResponse(const GrpcResponse<remote::UpdateMemoResponse>& response,
                                     const std::shared_ptr<model::Memo>& memo)
{
    if (response.ok() && response.body().operationStatus().type == remote::OperationStatus::kSuccess)
    {
        memoVault_->add(memo);
        emit memoUpdated(memo->id());
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
