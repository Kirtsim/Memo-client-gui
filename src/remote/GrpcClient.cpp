#include "remote/GrpcClient.hpp"
#include "MemoSvc.grpc.pb.h"
#include "TagSvc.grpc.pb.h"

namespace memo {

class Grpc_
{
public:
    Grpc_(const std::string& fullAddress);

    std::unique_ptr<proto::MemoService::StubInterface> memoStub;
    std::unique_ptr<proto::TagService::StubInterface> tagStub;
    grpc::CompletionQueue completionQueue;
};

Grpc_::Grpc_(const std::string& fullAddress)
    : memoStub(proto::MemoService::NewStub(grpc::CreateChannel(fullAddress, grpc::InsecureChannelCredentials())))
    , tagStub(proto::TagService::NewStub(grpc::CreateChannel(fullAddress, grpc::InsecureChannelCredentials())))
{
}

GrpcClient::GrpcClient(const std::string& address, const std::string& port)
    : address_(address)
    , port_(port)
    , grpc_(std::make_unique<Grpc_>(address + ":" + port))
{
}

GrpcClient::~GrpcClient()
{
    // TODO: cleanup the completion queue.
}

grpc::Status GrpcClient::status()
{
    return grpc::Status();
}

proto::ListMemosRs GrpcClient::listMemos(const proto::ListMemosRq& request)
{
    return proto::ListMemosRs();
}

proto::AddMemoRs GrpcClient::addMemo(const proto::AddMemoRq& request)
{
    return proto::AddMemoRs();
}

proto::UpdateMemoRs GrpcClient::updateMemo(const proto::UpdateMemoRq& request)
{
    return proto::UpdateMemoRs();
}

proto::RemoveMemoRs GrpcClient::removeMemo(const proto::RemoveMemoRq& request)
{
    return proto::RemoveMemoRs();
}

proto::ListTagsRs GrpcClient::listTags(const proto::ListTagsRq& request)
{
    return proto::ListTagsRs();
}

proto::AddTagRs GrpcClient::addTag(const proto::AddTagRq& request)
{
    return proto::AddTagRs();
}

proto::UpdateTagRs GrpcClient::updateTag(const proto::UpdateTagRq& request)
{
    return proto::UpdateTagRs();
}

proto::RemoveTagRs GrpcClient::removeTag(const proto::RemoveTagRq& request)
{
    return proto::RemoveTagRs();
}

} // namespace memo
