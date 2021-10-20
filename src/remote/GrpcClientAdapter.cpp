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

    const auto responseData = ListMemos::ExtractResponseData(grpcResponse.body());
    const auto response = remote::ListMemosResponse::Construct(responseData);
    return { grpcResponse.ok(), response };
}

GrpcResponse<remote::AddMemoResponse> GrpcClientAdapter::addMemo(const remote::AddMemoRequest& request)
{
    const auto protoRequest = AddMemo::CreateProtoRequest(request);

    const auto grpcResponse = client_->addMemo(protoRequest);

    const auto responseData = AddMemo::ExtractResponseData(grpcResponse.body());
    const auto response = remote::AddMemoResponse::Construct(responseData);
    return { grpcResponse.ok(), response };
}

GrpcResponse<remote::UpdateMemoResponse> GrpcClientAdapter::updateMemo(const remote::UpdateMemoRequest& request)
{
    const auto protoRequest = UpdateMemo::CreateProtoRequest(request);

    const auto grpcResponse = client_->updateMemo(protoRequest);

    const auto responseData = UpdateMemo::ExtractResponseData(grpcResponse.body());
    const auto response = remote::UpdateMemoResponse::Construct(responseData);
    return { grpcResponse.ok(), response };
}

GrpcResponse<remote::RemoveMemoResponse> GrpcClientAdapter::removeMemo(const remote::RemoveMemoRequest& request)
{
    const auto protoRequest = RemoveMemo::CreateProtoRequest(request);

    const auto grpcResponse = client_->removeMemo(protoRequest);

    const auto responseData = RemoveMemo::ExtractResponseData(grpcResponse.body());
    const auto response = remote::RemoveMemoResponse::Construct(responseData);
    return { grpcResponse.ok(), response };
}

GrpcResponse<remote::ListTagsResponse> GrpcClientAdapter::listTags(const remote::ListTagsRequest& request)
{
    const auto protoRequest = ListTags::CreateProtoRequest(request);

    const auto grpcResponse = client_->listTags(protoRequest);

    const auto responseData = ListTags::ExtractResponseData(grpcResponse.body());
    const auto response = remote::ListTagsResponse::Construct(responseData);
    return { grpcResponse.ok(), response };
}

GrpcResponse<remote::AddTagResponse> GrpcClientAdapter::addTag(const remote::AddTagRequest& request)
{
    const auto protoRequest = AddTag::CreateProtoRequest(request);

    const auto grpcResponse = client_->addTag(protoRequest);

    const auto responseData = AddTag::ExtractResponseData(grpcResponse.body());
    const auto response = remote::AddTagResponse::Construct(responseData);
    return { grpcResponse.ok(), response };
}

GrpcResponse<remote::UpdateTagResponse> GrpcClientAdapter::updateTag(const remote::UpdateTagRequest& request)
{
    const auto protoRequest = UpdateTag::CreateProtoRequest(request);

    const auto grpcResponse = client_->updateTag(protoRequest);

    const auto responseData = UpdateTag::ExtractResponseData(grpcResponse.body());
    const auto response = remote::UpdateTagResponse::Construct(responseData);
    return { grpcResponse.ok(), response };
}

GrpcResponse<remote::RemoveTagResponse> GrpcClientAdapter::removeTag(const remote::RemoveTagRequest& request)
{
    const auto protoRequest = RemoveTag::CreateProtoRequest(request);

    const auto grpcResponse = client_->removeTag(protoRequest);

    const auto responseData = RemoveTag::ExtractResponseData(grpcResponse.body());
    const auto response = remote::RemoveTagResponse::Construct(responseData);
    return { grpcResponse.ok(), response };
}

} // namespace memo
