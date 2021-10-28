#include "remote/concurrency/RemovalWorker.hpp"


namespace memo::remote {

RemoveMemoWorker::RemoveMemoWorker(const QString& id, IGrpcClientAdapter& client, RemoveMemoRequest request)
    : BaseWorker(id, client, std::move(request))
{
}

RemoveMemoWorker::~RemoveMemoWorker() = default;

void RemoveMemoWorker::run()
{
    auto response = client().removeMemo(request());
    workComplete(std::move(response));
}


RemoveTagWorker::RemoveTagWorker(const QString& id, IGrpcClientAdapter& client, RemoveTagRequest request)
    : BaseWorker(id, client, std::move(request))
{
}

RemoveTagWorker::~RemoveTagWorker() = default;

void RemoveTagWorker::run()
{
    auto response = client().removeTag(request());
    workComplete(std::move(response));
}

} // namespace memo::remote
