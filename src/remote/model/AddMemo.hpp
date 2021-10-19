#pragma once
#include "remote/model/OperationStatus.hpp"
#include <string>
#include <memory>

namespace memo::model { class Memo; }
namespace memo::remote {

struct AddMemoResponseData;

class AddMemoResponse
{
public:
    static AddMemoResponse Construct(const AddMemoResponseData& data);

    const std::string& requestId() const;

    unsigned long addedMemoId() const;

    const OperationStatus& operationStatus() const;

private:
    std::string requestId_;
    unsigned long addedMemoId_ = -1ul;
    OperationStatus operationStatus_;
};

struct AddMemoRequest
{
    std::string uuid;
    std::shared_ptr<model::Memo> memo;
};

struct AddMemoResponseData
{
    std::string requestId;
    unsigned long addedMemoId = -1ul;
    OperationStatus operationStatus;
};

} // namespace memo::remote
