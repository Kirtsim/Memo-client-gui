#pragma once
#include "remote/concurrency/BaseWorker.hpp"
#include "remote/GrpcResponse.hpp"
#include "remote/model/ListMemos.hpp"
#include "remote/model/ListTags.hpp"


namespace memo::remote {

class QueryMemoWorker : public BaseWorker<ListMemosRequest, ListMemosResponse>
{
public:
    QueryMemoWorker(const QString& id, IGrpcClientAdapter& client, ListMemosRequest request);

    ~QueryMemoWorker() override;

    void run() override;

    void accept(WorkerVisitor& visitor) override;
};

class QueryTagWorker :  public BaseWorker<ListTagsRequest, ListTagsResponse>
{
public:
    QueryTagWorker(const QString& id, IGrpcClientAdapter& client, ListTagsRequest request);

    ~QueryTagWorker() override;

    void run() override;

    void accept(WorkerVisitor& visitor) override;
};
} // namespace memo::remote
