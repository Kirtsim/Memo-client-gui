#include "remote/model/UpdateMemo.hpp"

namespace memo::remote {

UpdateMemoResponse UpdateMemoResponse::Construct(const UpdateMemoResponseData& data)
{
    UpdateMemoResponse response;
    response.requestUuid_ = data.requestUuid;
    response.operationStatus_ = data.operationStatus;
    return response;
}

const std::string& UpdateMemoResponse::requestUuid() const
{
    return requestUuid_;
}

const OperationStatus& UpdateMemoResponse::operationStatus() const
{
    return operationStatus_;
}
} // namespace memo::remote