#include "remote/model/RemoveMemo.hpp"

namespace memo::remote {

RemoveMemoResponse RemoveMemoResponse::Construct(const RemoveMemoResponseData& data)
{
    RemoveMemoResponse response;
    response.requestUuid_ = data.requestUuid;
    response.removedMemoIds_ = data.removedMemoIds;
    return response;
}

const std::string& RemoveMemoResponse::requestUuid() const
{
    return requestUuid_;
}

const std::vector<unsigned long>& RemoveMemoResponse::removedMemoIds() const
{
    return removedMemoIds_;
}

const OperationStatus& RemoveMemoResponse::operationStatus() const
{
    return operationStatus_;
}
} // namespace memo::remote
