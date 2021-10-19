#pragma once
#include "remote/model/OperationStatus.hpp"
#include <string>
#include <memory>

namespace memo::model { class Memo; }
namespace memo::remote {

struct UpdateMemoResponseData;

class UpdateMemoResponse
{
public:
    static UpdateMemoResponse Construct(const UpdateMemoResponseData& data);

    const std::string& requestUuid() const;

    const OperationStatus& operationStatus() const;

private:
    std::string requestUuid_;
    OperationStatus operationStatus_;
};

struct UpdateMemoResponseData
{
    std::string requestUuid;
    OperationStatus operationStatus;
};

struct UpdateMemoRequest
{
    std::string uuid;
    std::shared_ptr<model::Memo> memo;
};

} // namespace memo::remote
