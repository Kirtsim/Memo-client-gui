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
#include "tools/uuid.hpp"
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
    remote::Worker* worker;
    const auto requestUuid = GenerateQUuid();

    if (memoVault_->find(memo->id()))
    {
        remote::UpdateMemoRequest request;
        request.uuid = requestUuid.toStdString();
        request.memo = memo;
        worker = new remote::UpdateMemoWorker(requestUuid, *client_, request);
    }
    else
    {
        remote::AddMemoRequest request;
        request.uuid = requestUuid.toStdString();
        request.memo = std::make_shared<model::Memo>(*memo);
        worker = new remote::CreateMemoWorker(requestUuid, *client_, request);
    }
    connect(worker, &remote::CreateMemoWorker::workFinished, this, &MemoCollection::onWorkerFinished);
    connect(worker, &remote::Worker::finished, worker, &remote::Worker::deleteLater);

    workers_[requestUuid] = worker;
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
    const auto requestUuid = GenerateQUuid();
    remote::ListMemosRequest request;
    request.uuid = requestUuid.toStdString();

    auto worker = new remote::QueryMemoWorker(requestUuid, *client_, request);
    workers_[requestUuid] = worker;

    connect(worker, &remote::QueryMemoWorker::workFinished, this, &MemoCollection::onWorkerFinished);
    connect(worker, &remote::Worker::finished, worker, &remote::Worker::deleteLater);
    worker->start();
    return memoVault_->list();
}

bool MemoCollection::remove(unsigned long memoId)
{
    const auto requestUuid = GenerateQUuid();
    remote::RemoveMemoRequest request;
    request.uuid = requestUuid.toStdString();
    request.memoIds.emplace_back(memoId);

    auto worker = new remote::RemoveMemoWorker(requestUuid, *client_, request);
    connect(worker, &remote::RemoveMemoWorker::workFinished, this, &MemoCollection::onWorkerFinished);
    connect(worker, &remote::Worker::finished, worker, &remote::Worker::deleteLater);
    workers_[requestUuid] = worker;
    worker->start();
    return true;
}

void MemoCollection::onWorkerFinished(const QString& workId)
{
    auto iter = workers_.find(workId);
    if (iter == std::end(workers_) || !iter.value())
        return;
    auto worker = iter.value();
    workers_.remove(workId);
    worker->accept(*this);
}

void MemoCollection::visit(const remote::QueryMemoWorker& worker)
{
    const auto& response = worker.response();
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

void MemoCollection::visit(const remote::CreateMemoWorker& worker)
{
    const auto& response = worker.response();
    const auto memo = worker.request().memo;
    if (response.ok() && memo)
    {
        auto copy = std::make_shared<model::Memo>(*memo);
        copy->setId(response.body().addedMemoId());
        memoVault_->add(copy);
        emit memoAdded(copy->id());
    }
}

void MemoCollection::visit(const remote::UpdateMemoWorker& worker)
{
    const auto& response = worker.response();
    const auto responseStatus = response.body().operationStatus().type;
    if (response.ok() && responseStatus == remote::OperationStatus::kSuccess)
    {
        const auto memo = worker.request().memo;
        const bool titleChanged = (nullptr == memoVault_->find(memo->title()));
        if (titleChanged)
            memoVault_->remove(memo->id());
        memoVault_->add(memo);
        emit memoUpdated(memo->id());
    }
}

void MemoCollection::visit(const remote::RemoveMemoWorker& worker)
{
    const auto& response = worker.response();
    if (response.ok())
    {
        const auto& removedMemoIds = response.body().removedMemoIds();
        for (const auto memoId : removedMemoIds)
        {
            memoVault_->remove(memoId);
            emit memoRemoved(memoId);
        }
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
