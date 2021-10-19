#pragma once
#include "remote/model/OperationStatus.hpp"
#include <string>
#include <memory>

namespace memo::model { class Tag; }
namespace memo::remote {

struct UpdateTagResponseData;

class UpdateTagResponse
{
public:
    static UpdateTagResponse Construct(const UpdateTagResponseData& data);

    const std::string& requestUuid() const;

    const OperationStatus& operationStatus() const;

private:
    std::string requestUuid_;
    OperationStatus operationStatus_;
};

struct UpdateTagResponseData
{
    std::string requestUuid;
    OperationStatus operationStatus;
};

struct UpdateTagRequest
{
    std::string uuid;
    std::shared_ptr<model::Tag> tag;
};

} // namespace memo::remote
