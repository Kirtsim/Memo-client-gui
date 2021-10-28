#pragma once
#include "remote/concurrency/BaseWorker.hpp"
#include "remote/model/RemoveMemo.hpp"
#include "remote/model/RemoveTag.hpp"
#include "remote/GrpcResponse.hpp"


namespace memo::remote {

class RemoveMemoWorker : public BaseWorker<RemoveMemoRequest, RemoveMemoResponse>
{
public:
    RemoveMemoWorker(const QString& id, IGrpcClientAdapter& client, RemoveMemoRequest request);

    ~RemoveMemoWorker() override;

    void run() override;
};

class RemoveTagWorker : public BaseWorker<RemoveTagRequest, RemoveTagResponse>
{
public:
    RemoveTagWorker(const QString& id, IGrpcClientAdapter& client, RemoveTagRequest request);

    ~RemoveTagWorker() override;

    void run() override;
};
} // namespace memo::remote