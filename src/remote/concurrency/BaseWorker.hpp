#pragma once
#include "remote/concurrency/Worker.hpp"
#include "remote/GrpcResponse.hpp"

namespace memo::remote {

template <class Request, class Response>
class BaseWorker : public Worker
{
public:
    BaseWorker(const QString& id, IGrpcClientAdapter& client, Request request);

    virtual ~BaseWorker() override = default;

    const QString& id() const override;

    const Request& request() const;

    const GrpcResponse<Response>& response() const;

protected:
    IGrpcClientAdapter& client();

    void workComplete(GrpcResponse<Response> response);

private:
    QString id_;
    IGrpcClientAdapter& client_;
    Request request_;
    GrpcResponse<Response> response_;
};

template<class Request, class Response>
BaseWorker<Request, Response>::BaseWorker(const QString& id, IGrpcClientAdapter& client, Request request)
    : Worker()
    , id_(id)
    , client_(client)
    , request_(std::move(request))
    , response_(false, {})
{
}

template<class Request, class Response>
const QString& BaseWorker<Request, Response>::id() const
{
    return id_;
}

template<class Request, class Response>
const Request& BaseWorker<Request, Response>::request() const
{
    return request_;
}

template<class Request, class Response>
const GrpcResponse<Response>& BaseWorker<Request, Response>::response() const
{
    return response_;
}

template<class Request, class Response>
IGrpcClientAdapter& BaseWorker<Request, Response>::client()
{
    return client_;
}

template<class Request, class Response>
void BaseWorker<Request, Response>::workComplete(GrpcResponse<Response> response)
{
    response_ = std::move(response);
    emit workFinished(id_);
}
} // namespace memo::remote
