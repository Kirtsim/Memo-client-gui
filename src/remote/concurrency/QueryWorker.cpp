#include "remote/concurrency/QueryWorker.hpp"


namespace memo::remote {

QueryMemoWorker::QueryMemoWorker(const QString& id, IGrpcClientAdapter& client, ListMemosRequest request)
    : BaseWorker(id, client, std::move(request))
{
}

QueryMemoWorker::~QueryMemoWorker() = default;

void QueryMemoWorker::run()
{
    auto response = client().listMemos(request());
    workComplete(std::move(response));
}


QueryTagWorker::QueryTagWorker(const QString& id, IGrpcClientAdapter& client, ListTagsRequest request)
    : BaseWorker(id, client, std::move(request))
{
}

QueryTagWorker::~QueryTagWorker() = default;

void QueryTagWorker::run()
{
    auto response = client().listTags(request());
    workComplete(std::move(response));
}

} // namespace memo::remote
