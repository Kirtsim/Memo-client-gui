#pragma once
#include "remote/concurrency/BaseWorker.hpp"
#include "remote/model/UpdateMemo.hpp"
#include "remote/model/UpdateTag.hpp"
#include "remote/GrpcResponse.hpp"


namespace memo::remote {

class UpdateMemoWorker : public BaseWorker<UpdateMemoRequest, UpdateMemoResponse>
{
public:
    UpdateMemoWorker(const QString& id, IGrpcClientAdapter& client, UpdateMemoRequest request);

    ~UpdateMemoWorker() override;

    void run() override;

    void accept(WorkerVisitor& visitor) override;
};

class UpdateTagWorker : public BaseWorker<UpdateTagRequest, UpdateTagResponse>
{
public:
    UpdateTagWorker(const QString& id, IGrpcClientAdapter& client, UpdateTagRequest request);

    ~UpdateTagWorker() override;

    void run() override;

    void accept(WorkerVisitor& visitor) override;
};
} // namespace memo::remote