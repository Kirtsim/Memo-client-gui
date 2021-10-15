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

    grpc::Status status() override;

    proto::ListMemosRs listMemos(const proto::ListMemosRq& request) override;

    proto::AddMemoRs addMemo(const proto::AddMemoRq& request) override;

    proto::UpdateMemoRs updateMemo(const proto::UpdateMemoRq& request) override;

    proto::RemoveMemoRs removeMemo(const proto::RemoveMemoRq& request) override;

    proto::ListTagsRs listTags(const proto::ListTagsRq& request) override;

    proto::AddTagRs addTag(const proto::AddTagRq& request) override;

    proto::UpdateTagRs updateTag(const proto::UpdateTagRq& request) override;

    proto::RemoveTagRs removeTag(const proto::RemoveTagRq& request) override;

private:
    std::string address_;
    std::string port_;
    std::unique_ptr<Grpc_> grpc_;
};

} // namespace memo
