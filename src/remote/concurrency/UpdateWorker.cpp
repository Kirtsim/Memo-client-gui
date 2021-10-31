#include "remote/concurrency/UpdateWorker.hpp"

namespace memo::remote {

UpdateMemoWorker::UpdateMemoWorker(const QString& id, IGrpcClientAdapter& client, UpdateMemoRequest request)
    : BaseWorker(id, client, std::move(request))
{
}

UpdateMemoWorker::~UpdateMemoWorker() = default;

void UpdateMemoWorker::run()
{
    auto response = client().updateMemo(request());
    workComplete(std::move(response));
}

void UpdateMemoWorker::accept(WorkerVisitor& visitor)
{
    if (auto vis = dynamic_cast<MemoWorkerVisitor*>(&visitor))
        vis->visit(*this);
}


UpdateTagWorker::UpdateTagWorker(const QString& id, IGrpcClientAdapter& client, UpdateTagRequest request)
    : BaseWorker(id, client, std::move(request))
{
}

UpdateTagWorker::~UpdateTagWorker() = default;

void UpdateTagWorker::run()
{
    auto response = client().updateTag(request());
    workComplete(std::move(response));
}

void UpdateTagWorker::accept(WorkerVisitor& visitor)
{
    if (auto vis = dynamic_cast<TagWorkerVisitor*>(&visitor))
        vis->visit(*this);
}
} // namespace memo::remote