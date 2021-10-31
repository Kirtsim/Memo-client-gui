#pragma once
#include "remote/concurrency/BaseWorker.hpp"
#include "remote/model/AddMemo.hpp"
#include "remote/model/AddTag.hpp"
#include "remote/GrpcResponse.hpp"


namespace memo::remote {

class CreateMemoWorker : public BaseWorker<AddMemoRequest, AddMemoResponse>
{
public:
    CreateMemoWorker(const QString& id, IGrpcClientAdapter& client, AddMemoRequest request);

    ~CreateMemoWorker() override;

    void run() override;

    void accept(WorkerVisitor& visitor) override;
};

class CreateTagWorker : public BaseWorker<AddTagRequest, AddTagResponse>
{
public:
    CreateTagWorker(const QString& id, IGrpcClientAdapter& client, AddTagRequest request);

    ~CreateTagWorker() override;

    void run() override;

    void accept(WorkerVisitor& visitor) override;
};
} // namespace memo::remote
