#pragma once
#include "remote/IGrpcClientAdapter.hpp"
#include <memory>

namespace memo {

class IGrpcClient;

class GrpcClientAdapter : public IGrpcClientAdapter
{
public:
    GrpcClientAdapter(std::unique_ptr<IGrpcClient> client);

    ~GrpcClientAdapter() override;

    bool ping() override;

    GrpcResponse<remote::ListMemosResponse> listMemos(const remote::ListMemosRequest& request) override;

    GrpcResponse<remote::AddMemoResponse> addMemo(const remote::AddMemoRequest& request) override;

    GrpcResponse<remote::UpdateMemoResponse> updateMemo(const remote::UpdateMemoRequest& request) override;

    GrpcResponse<remote::RemoveMemoResponse> removeMemo(const remote::RemoveMemoRequest& request) override;

    GrpcResponse<remote::ListTagsResponse> listTags(const remote::ListTagsRequest& request) override;

    GrpcResponse<remote::AddTagResponse> addTag(const remote::AddTagRequest& request) override;

    GrpcResponse<remote::UpdateTagResponse> updateTag(const remote::UpdateTagRequest& request) override;

    GrpcResponse<remote::RemoveTagResponse> removeTag(const remote::RemoveTagRequest& request) override;

private:
    std::unique_ptr<IGrpcClient> client_;
};

} // namespace memo
