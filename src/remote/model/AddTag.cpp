#include "remote/model/AddTag.hpp"

namespace memo::remote {

AddTagResponse AddTagResponse::Construct(const AddTagResponseData& data)
{
    AddTagResponse response;
    response.requestUuid_ = data.requestUuid;
    response.addedTagId_ = data.addedTagId;
    response.operationStatus_ = data.operationStatus;
    return response;
}

const std::string& AddTagResponse::requestUuid() const
{
    return requestUuid_;
}

unsigned long AddTagResponse::addedTagId() const
{
    return addedTagId_;
}

const OperationStatus& AddTagResponse::operationStatus() const
{
    return operationStatus_;
}
} // namespace memo::remote
