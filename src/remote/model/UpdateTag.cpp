#include "remote/model/UpdateTag.hpp"

namespace memo::remote {

UpdateTagResponse UpdateTagResponse::Construct(const UpdateTagResponseData& data)
{
    UpdateTagResponse response;
    response.requestUuid_ = data.requestUuid;
    response.operationStatus_ = data.operationStatus;
    return response;
}

const std::string& UpdateTagResponse::requestUuid() const
{
    return requestUuid_;
}

const OperationStatus& UpdateTagResponse::operationStatus() const
{
    return operationStatus_;
}

} // namespace memo::remote
