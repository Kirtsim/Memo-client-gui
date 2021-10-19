#include "remote/model/AddMemo.hpp"

namespace memo::remote {


AddMemoResponse AddMemoResponse::Construct(const AddMemoResponseData& data)
{
    AddMemoResponse response;
    response.requestId_ = data.requestId;
    response.addedMemoId_ = data.addedMemoId;
    response.operationStatus_ = data.operationStatus;
    return response;
}

const std::string& AddMemoResponse::requestId() const
{
    return requestId_;
}

unsigned long AddMemoResponse::addedMemoId() const
{
    return addedMemoId_;
}

const OperationStatus& AddMemoResponse::operationStatus() const
{
    return operationStatus_
}
} // namespace memo::remote