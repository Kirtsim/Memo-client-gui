#include "remote/GrpcClient.hpp"
#include "MemoSvc.grpc.pb.h"
#include "TagSvc.grpc.pb.h"
#include <grpcpp/grpcpp.h>

namespace memo {

class Grpc_
{
public:
    explicit Grpc_(const std::string& fullAddress);

    std::unique_ptr<proto::MemoService::StubInterface> memoStub;
    std::unique_ptr<proto::TagService::StubInterface> tagStub;
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

bool GrpcClient::ping()
{
    return false;
}

namespace {
    void wait(grpc::CompletionQueue& queue);
} // namespace

GrpcResponse<proto::ListMemosRs> GrpcClient::listMemos(const proto::ListMemosRq& request)
{
    proto::ListMemosRs response;
    grpc::ClientContext context;
    grpc::CompletionQueue queue;

    auto reader = grpc_->memoStub->PrepareAsyncListMemos(&context, request, &queue);
    if (!reader)
    {
        // TODO: Log
        return { false, std::move(response) };
    }

    grpc::Status status;
    reader->StartCall();
    reader->Finish(&response, &status, (void*)this);
    wait(queue);
    if (!status.ok())
    {
        // TODO: Log
    }

    return { status.ok(), std::move(response) };
}

GrpcResponse<proto::AddMemoRs> GrpcClient::addMemo(const proto::AddMemoRq& request)
{
    return { false, proto::AddMemoRs() };
}

GrpcResponse<proto::UpdateMemoRs> GrpcClient::updateMemo(const proto::UpdateMemoRq& request)
{
    return { false, proto::UpdateMemoRs() };
}

GrpcResponse<proto::RemoveMemoRs> GrpcClient::removeMemo(const proto::RemoveMemoRq& request)
{
    return { false, proto::RemoveMemoRs() };
}

GrpcResponse<proto::ListTagsRs> GrpcClient::listTags(const proto::ListTagsRq& request)
{
    return { false, proto::ListTagsRs() };
}

GrpcResponse<proto::AddTagRs> GrpcClient::addTag(const proto::AddTagRq& request)
{
    return { false, proto::AddTagRs() };
}

GrpcResponse<proto::UpdateTagRs> GrpcClient::updateTag(const proto::UpdateTagRq& request)
{
    return { false, proto::UpdateTagRs() };
}

GrpcResponse<proto::RemoveTagRs> GrpcClient::removeTag(const proto::RemoveTagRq& request)
{
    return { false, proto::RemoveTagRs() };
}

namespace {
    void wait(grpc::CompletionQueue& queue)
    {
        void* tag;
        bool flag = false;

        // block the main thread until a response arrives
        queue.Next(&tag, &flag); // flag always true according to the documentation
    }
} // namespace
} // namespace memo
