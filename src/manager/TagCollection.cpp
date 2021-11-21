#include "manager/TagCollection.hpp"
#include "manager/TagVault.hpp"
#include "remote/IGrpcClientAdapter.hpp"
#include "remote/model/ListTags.hpp"
#include "remote/model/AddTag.hpp"
#include "remote/model/UpdateTag.hpp"
#include "remote/model/RemoveTag.hpp"
#include "remote/concurrency/QueryWorker.hpp"
#include "remote/concurrency/CreationWorker.hpp"
#include "remote/concurrency/UpdateWorker.hpp"
#include "remote/concurrency/RemovalWorker.hpp"
#include "model/Tag.hpp"
#include "tools/uuid.hpp"


namespace memo {

TagCollection::TagCollection(std::unique_ptr<IGrpcClientAdapter> client)
    : QObject()
    , client_(std::move(client))
    , tagVault_(std::make_unique<TagVault>())
{
}

TagCollection::~TagCollection()
{
    for (auto worker : workers_)
    {
        if (worker)
        {
            disconnect(worker, &remote::Worker::workFinished, this, &TagCollection::onWorkerFinished);
            worker->quit();
        }
    }
}

std::vector<std::shared_ptr<model::Tag>> TagCollection::listAll()
{
    const auto requestUuid = GenerateQUuid();
    remote::ListTagsRequest request;
    request.uuid = requestUuid.toStdString();
    auto worker = new remote::QueryTagWorker(requestUuid, *client_, std::move(request));
    connect(worker, &remote::Worker::workFinished, this, &TagCollection::onWorkerFinished);
    connect(worker, &remote::Worker::finished, worker, &remote::Worker::deleteLater);

    workers_[requestUuid] = worker;
    worker->start();

    return tagVault_->list();
}

std::shared_ptr<model::Tag> TagCollection::find(unsigned long tagId) const
{
    return tagVault_->find(tagId);
}

std::shared_ptr<model::Tag> TagCollection::find(const std::string& name) const
{
    return tagVault_->find(name);
}

std::vector<std::shared_ptr<model::Tag>> TagCollection::filterByPrefix(const std::string& prefix)
{
    using TagPtr = std::shared_ptr<model::Tag>;
    const auto tags = tagVault_->list();
    std::vector<TagPtr> filtered;
    std::copy_if(tags.begin(), tags.end(), std::back_inserter(filtered),
                 [&prefix](const TagPtr& tag) { return tag->name().starts_with(prefix); });
    return filtered;
}

int TagCollection::count() const
{
    return tagVault_->size();
}

bool TagCollection::empty() const
{
    return tagVault_->empty();
}

bool TagCollection::add(const std::shared_ptr<model::Tag>& tag)
{
    if (!tag)
        return false;

    const auto requestUuid = GenerateQUuid();
    remote::Worker* worker;

    const bool tagIsNew = (nullptr == tagVault_->find(tag->id()));
    if (tagIsNew)
    {
        remote::AddTagRequest request;
        request.uuid = requestUuid.toStdString();
        request.tag = std::make_shared<model::Tag>(*tag);
        worker = new remote::CreateTagWorker(requestUuid, *client_, std::move(request));
    }
    else
    {
        remote::UpdateTagRequest request;
        request.uuid = requestUuid.toStdString();
        request.tag = std::make_shared<model::Tag>(*tag);
        worker = new remote::UpdateTagWorker(requestUuid, *client_, std::move(request));
    }
    connect(worker, &remote::Worker::workFinished, this, &TagCollection::onWorkerFinished);
    connect(worker, &remote::Worker::finished, worker, &remote::Worker::deleteLater);

    workers_[requestUuid] = worker;
    worker->start();
    return true;
}

bool TagCollection::remove(unsigned long tagId)
{
    const auto requestUuid = GenerateQUuid();
    remote::RemoveTagRequest request;
    request.uuid = requestUuid.toStdString();
    request.tagIds.emplace_back(tagId);
    auto worker = new remote::RemoveTagWorker(requestUuid, *client_, std::move(request));
    connect(worker, &remote::Worker::workFinished, this, &TagCollection::onWorkerFinished);
    connect(worker, &remote::Worker::finished, worker, &remote::Worker::deleteLater);

    workers_[requestUuid] = worker;
    worker->start();

    return false;
}

void TagCollection::visit(const remote::QueryTagWorker& worker)
{
    const auto& response = worker.response();
    if (response.ok())
    {
        tagVault_->clear();
        emit tagCacheCleared();

        QVector<qulonglong> ids;
        for (const auto& tag : response.body().tags())
        {
            tagVault_->add(tag);
            ids << tag->id();
            emit tagAdded(tag->id());
        }
        emit tagsAdded(ids);
    }
}

void TagCollection::visit(const remote::CreateTagWorker& worker)
{
    const auto& response = worker.response();
    const auto status = worker.response().body().operationStatus().type;
    if (response.ok() && status == remote::OperationStatus::kSuccess)
    {
        auto tag = worker.request().tag;
        tag->setId(response.body().addedTagId());

        tagVault_->add(tag);
        emit tagAdded(tag->id());
    }
}

void TagCollection::visit(const remote::UpdateTagWorker& worker)
{
    const auto& response = worker.response();
    const auto status = worker.response().body().operationStatus().type;
    if (response.ok() && status == remote::OperationStatus::kSuccess)
    {
        const auto tag = worker.request().tag;
        const bool nameChanged = (nullptr != tagVault_->find(tag->name()));
        if (nameChanged)
            tagVault_->remove(tag->id());
        tagVault_->add(tag);
        emit tagUpdated(tag->id());
    }
}

void TagCollection::visit(const remote::RemoveTagWorker& worker)
{
    const auto& response = worker.response();
    const auto status = worker.response().body().operationStatus().type;
    if (response.ok() && status == remote::OperationStatus::kSuccess)
    {
        const auto& removedTagIds = response.body().removedTagIds();
        for (const auto tagId : removedTagIds)
        {
            tagVault_->remove(tagId);
            emit tagRemoved(tagId);
        }
    }
}

void TagCollection::onWorkerFinished(const QString& workId)
{
    const auto iter = workers_.find(workId);
    if (iter == std::end(workers_))
        return;
    auto worker = iter.value();
    workers_.remove(workId);

    if (worker)
        worker->accept(*this);
}

} // namespace memo
