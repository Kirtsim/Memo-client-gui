#pragma once
#include "remote/IGrpcClient.hpp"

#include <memory>
#include <string>

namespace memo {

class Grpc_;

class GrpcClient : public IGrpcClient
{
public:
    GrpcClient(const std::string& address, const std::string& port);

    ~GrpcClient() override;

    bool ping() override;

    GrpcResponse<proto::ListMemosRs> listMemos(const proto::ListMemosRq& request) override;

    GrpcResponse<proto::AddMemoRs> addMemo(const proto::AddMemoRq& request) override;

    GrpcResponse<proto::UpdateMemoRs> updateMemo(const proto::UpdateMemoRq& request) override;

    GrpcResponse<proto::RemoveMemoRs> removeMemo(const proto::RemoveMemoRq& request) override;

    GrpcResponse<proto::ListTagsRs> listTags(const proto::ListTagsRq& request) override;

    GrpcResponse<proto::AddTagRs> addTag(const proto::AddTagRq& request) override;

    GrpcResponse<proto::UpdateTagRs> updateTag(const proto::UpdateTagRq& request) override;

    GrpcResponse<proto::RemoveTagRs> removeTag(const proto::RemoveTagRq& request) override;

private:
    std::string address_;
    std::string port_;
    std::unique_ptr<Grpc_> grpc_;
};

} // namespace memo
