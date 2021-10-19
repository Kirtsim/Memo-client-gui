#include "remote/model/RemoveTag.hpp"

namespace memo::remote {

RemoveTagResponse RemoveTagResponse::Construct(const RemoveTagResponseData& data)
{
    RemoveTagResponse response;
    response.requestUuid_ = data.requestUuid;
    response.removedTagIds_ = data.removedTagIds;
    response.operationStatus_ = data.operationStatus;
    return response;
}

const std::string& RemoveTagResponse::requestUuid() const
{
    return requestUuid_;
}

const std::vector<unsigned long>& RemoveTagResponse::removedTagIds() const
{
    return removedTagIds_;
}

const OperationStatus& RemoveTagResponse::operationStatus() const
{
    return operationStatus_;
}
} // namespace memo::remote
