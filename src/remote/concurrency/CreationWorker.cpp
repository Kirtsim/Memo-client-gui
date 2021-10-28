#include "CreationWorker.hpp"

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

} // namespace memo::remote
