#include "remote/concurrency/CreationWorker.hpp"

namespace memo::remote {

CreateMemoWorker::CreateMemoWorker(const QString& id, IGrpcClientAdapter& client, AddMemoRequest request)
    : BaseWorker(id, client, std::move(request))
{
}

CreateMemoWorker::~CreateMemoWorker() = default;

void CreateMemoWorker::run()
{
    auto response = client().addMemo(request());
    workComplete(std::move(response));
}

void CreateMemoWorker::accept(WorkerVisitor& visitor)
{
    if (auto vis = dynamic_cast<MemoWorkerVisitor*>(&visitor))
        vis->visit(*this);
}

CreateTagWorker::CreateTagWorker(const QString& id, IGrpcClientAdapter& client, AddTagRequest request)
    : BaseWorker(id, client, std::move(request))
{
}

CreateTagWorker::~CreateTagWorker() = default;

void CreateTagWorker::run()
{
    auto response = client().addTag(request());
    workComplete(std::move(response));
}

void CreateTagWorker::accept(WorkerVisitor& visitor)
{
    if (auto vis = dynamic_cast<TagWorkerVisitor*>(&visitor))
        vis->visit(*this);
}

} // namespace memo::remote
