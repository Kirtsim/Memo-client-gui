#include "remote/GrpcClient.hpp"
#include "MemoSvc.grpc.pb.h"
#include "TagSvc.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include <functional>

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

GrpcClient::~GrpcClient() = default;


bool GrpcClient::ping()
{
    return false;
}

namespace {
    template<class Response>
    using ClientReaderPtr = std::unique_ptr<grpc::ClientAsyncResponseReaderInterface<Response>>;

    template<class Response>
    using ProvideReader = std::function<ClientReaderPtr<Response>(grpc::ClientContext*, grpc::CompletionQueue*)>;

    template<class Response>
    GrpcResponse<Response> performCall(const ProvideReader<Response>& provideReader);
} // namespace

GrpcResponse<proto::ListMemosRs> GrpcClient::listMemos(const proto::ListMemosRq& request)
{
    auto provideReader = [&](grpc::ClientContext* context, grpc::CompletionQueue* queue)
    {
        return grpc_->memoStub->PrepareAsyncListMemos(context, request, queue);
    };
    return performCall<proto::ListMemosRs>(provideReader);
}

GrpcResponse<proto::AddMemoRs> GrpcClient::addMemo(const proto::AddMemoRq& request)
{
    auto provideReader = [&](grpc::ClientContext* context, grpc::CompletionQueue* queue)
    {
        return grpc_->memoStub->PrepareAsyncAddMemo(context, request, queue);
    };
    return performCall<proto::AddMemoRs>(provideReader);
}

GrpcResponse<proto::UpdateMemoRs> GrpcClient::updateMemo(const proto::UpdateMemoRq& request)
{
    auto provideReader = [&](grpc::ClientContext* context, grpc::CompletionQueue* queue)
    {
        return grpc_->memoStub->PrepareAsyncUpdateMemo(context, request, queue);
    };
    return performCall<proto::UpdateMemoRs>(provideReader);
}

GrpcResponse<proto::RemoveMemoRs> GrpcClient::removeMemo(const proto::RemoveMemoRq& request)
{
    auto provideReader = [&](grpc::ClientContext* context, grpc::CompletionQueue* queue)
    {
        return grpc_->memoStub->PrepareAsyncRemoveMemo(context, request, queue);
    };
    return performCall<proto::RemoveMemoRs>(provideReader);
}

GrpcResponse<proto::ListTagsRs> GrpcClient::listTags(const proto::ListTagsRq& request)
{
    auto provideReader = [&](grpc::ClientContext* context, grpc::CompletionQueue* queue)
    {
        return grpc_->tagStub->PrepareAsyncListTags(context, request, queue);
    };
    return performCall<proto::ListTagsRs>(provideReader);
}

GrpcResponse<proto::AddTagRs> GrpcClient::addTag(const proto::AddTagRq& request)
{
    auto provideReader = [&](grpc::ClientContext* context, grpc::CompletionQueue* queue)
    {
        return grpc_->tagStub->PrepareAsyncAddTag(context, request, queue);
    };
    return performCall<proto::AddTagRs>(provideReader);
}

GrpcResponse<proto::UpdateTagRs> GrpcClient::updateTag(const proto::UpdateTagRq& request)
{
    auto provideReader = [&](grpc::ClientContext* context, grpc::CompletionQueue* queue)
    {
        return grpc_->tagStub->PrepareAsyncUpdateTag(context, request, queue);
    };
    return performCall<proto::UpdateTagRs>(provideReader);
}

GrpcResponse<proto::RemoveTagRs> GrpcClient::removeTag(const proto::RemoveTagRq& request)
{
    auto provideReader = [&](grpc::ClientContext* context, grpc::CompletionQueue* queue)
    {
        return grpc_->tagStub->PrepareAsyncRemoveTag(context, request, queue);
    };
    return performCall<proto::RemoveTagRs>(provideReader);
}

namespace {

    void wait(grpc::CompletionQueue& queue);

    template<class Response>
    GrpcResponse<Response> performCall(const ProvideReader<Response>& provideReader)
    {
        Response response;
        grpc::ClientContext context;
        grpc::CompletionQueue queue;

        auto reader = provideReader(&context, &queue);
        if (!reader)
        {
            // TODO: Log
            return { false, std::move(response) };
        }

        grpc::Status status;
        reader->StartCall();
        reader->Finish(&response, &status, nullptr);
        wait(queue);
        if (!status.ok())
        {
            // TODO: Log
        }

        return { status.ok(), std::move(response) };
    }

    void wait(grpc::CompletionQueue& queue)
    {
        void* tag;
        bool flag = false;

        // block the main thread until a response arrives
        queue.Next(&tag, &flag); // flag always true according to the documentation
    }
} // namespace
} // namespace memo
