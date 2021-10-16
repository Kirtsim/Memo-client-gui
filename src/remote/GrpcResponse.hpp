#pragma once
#include <string>


namespace memo {

template<class ResponseType>
class GrpcResponse
{
public:
    GrpcResponse(bool success, ResponseType body);

    bool ok() const;

    const ResponseType& body() const;

private:
    bool success_;
    ResponseType body_;
};

template<class ResponseType>
GrpcResponse<ResponseType>::GrpcResponse(bool success, ResponseType body)
    : success_(success)
    , body_(std::move(body))
{
}

template<class ResponseType>
bool GrpcResponse<ResponseType>::ok() const
{
    return success_;
}

template<class ResponseType>
const ResponseType& GrpcResponse<ResponseType>::body() const
{
    return body_;
}

} // namespace memo
