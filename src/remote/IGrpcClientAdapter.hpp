#pragma once
#include "remote/GrpcResponse.hpp"
#include <vector>

namespace memo::remote {
    class ListMemosResponse;  class ListMemosRequest;
    class AddMemoResponse;    class AddMemoRequest;
    class UpdateMemoResponse; class UpdateMemoRequest;
    class RemoveMemoResponse; class RemoveMemoRequest;
    class ListTagsResponse;   class ListTagsRequest;
    class AddTagResponse;     class AddTagRequest;
    class UpdateTagResponse;  class UpdateTagRequest;
    class RemoveTagResponse;  class RemoveTagRequest;
}

namespace memo {


class IGrpcClientAdapter
{
public:
    virtual ~IGrpcClientAdapter();

    virtual ~IGrpcClient() = default;

    virtual bool ping() = 0;

    virtual GrpcResponse<remote::ListMemosResponse> listMemos(const remote::ListMemosRequest& request) = 0;

    virtual GrpcResponse<remote::AddMemoResponse> addMemo(const remote::AddMemoRequest& request) = 0;

    virtual GrpcResponse<remote::UpdateMemoResponse> updateMemo(const remote::UpdateMemoRequest& request) = 0;

    virtual GrpcResponse<remote::RemoveMemoResponse> removeMemo(const remote::RemoveMemoRequest& request) = 0;

    virtual GrpcResponse<remote::ListTagsResponse> listTags(const remote::ListTagsRequest& request) = 0;

    virtual GrpcResponse<remote::AddTagResponse> addTag(const remote::AddTagRequest& request) = 0;

    virtual GrpcResponse<remote::UpdateTagResponse> updateTag(const remote::UpdateTagRequest& request) = 0;

    virtual GrpcResponse<remote::RemoveTagResponse> removeTag(const remote::RemoveTagRequest& request) = 0;
};

} // namespace memo
