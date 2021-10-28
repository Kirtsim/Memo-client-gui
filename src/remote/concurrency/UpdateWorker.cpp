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

} // namespace memo::remote