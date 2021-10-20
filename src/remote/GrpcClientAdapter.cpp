#include "remote/GrpcClientAdapter.hpp"
#include "remote/IGrpcClient.hpp"
#include "remote/model/ListMemos.hpp"
#include "remote/model/ListTags.hpp"
#include "remote/model/AddMemo.hpp"
#include "remote/model/AddTag.hpp"
#include "remote/model/UpdateMemo.hpp"
#include "remote/model/UpdateTag.hpp"
#include "remote/model/RemoveMemo.hpp"
#include "remote/model/RemoveTag.hpp"
#include "remote/ClientAdapterUtils.hpp"
#include "model/conversions.hpp"
#include "MemoSvc.pb.h"
#include "TagSvc.pb.h"


namespace memo {

GrpcClientAdapter::GrpcClientAdapter(std::unique_ptr<IGrpcClient> client)
    : client_(std::move(client))
{
}

GrpcClientAdapter::~GrpcClientAdapter()
{
}

bool GrpcClientAdapter::ping()
{
    return client_->ping();
}

GrpcResponse<remote::ListMemosResponse> GrpcClientAdapter::listMemos(const remote::ListMemosRequest& request)
{
    const auto protoRequest = ListMemos::CreateProtoRequest(request);

    const auto grpcResponse = client_->listMemos(protoRequest);
    const auto& protoResponse = grpcResponse.body();

    const auto responseData = ListMemos::ExtractResponseData(protoResponse);
    const auto response = remote::ListMemosResponse::Construct(responseData);
    return { grpcResponse.ok(), response };
}

GrpcResponse<remote::AddMemoResponse> GrpcClientAdapter::addMemo(const remote::AddMemoRequest& request)
{
    return GrpcResponse<remote::AddMemoResponse>(false, remote::AddMemoResponse());
}

GrpcResponse<remote::UpdateMemoResponse> GrpcClientAdapter::updateMemo(const remote::UpdateMemoRequest& request)
{
    return GrpcResponse<remote::UpdateMemoResponse>(false, remote::UpdateMemoResponse());
}

GrpcResponse<remote::RemoveMemoResponse> GrpcClientAdapter::removeMemo(const remote::RemoveMemoRequest& request)
{
    return GrpcResponse<remote::RemoveMemoResponse>(false, remote::RemoveMemoResponse());
}

GrpcResponse<remote::ListTagsResponse> GrpcClientAdapter::listTags(const remote::ListTagsRequest& request)
{
    return GrpcResponse<remote::ListTagsResponse>(false, remote::ListTagsResponse());
}

GrpcResponse<remote::AddTagResponse> GrpcClientAdapter::addTag(const remote::AddTagRequest& request)
{
    return GrpcResponse<remote::AddTagResponse>(false, remote::AddTagResponse());
}

GrpcResponse<remote::UpdateTagResponse> GrpcClientAdapter::updateTag(const remote::UpdateTagRequest& request)
{
    return GrpcResponse<remote::UpdateTagResponse>(false, remote::UpdateTagResponse());
}

GrpcResponse<remote::RemoveTagResponse> GrpcClientAdapter::removeTag(const remote::RemoveTagRequest& request)
{
    return GrpcResponse<remote::RemoveTagResponse>(false, remote::RemoveTagResponse());
}

} // namespace memo
