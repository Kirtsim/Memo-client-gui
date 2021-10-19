#pragma once
#include "remote/model/OperationStatus.hpp"
#include <string>
#include <memory>


namespace memo::model { class Tag; }
namespace memo::remote {

struct AddTagResponseData;

class AddTagResponse
{
public:
    static AddTagResponse Construct(const AddTagResponseData& data);

    const std::string& requestUuid() const;

    unsigned long addedTagId() const;

    const OperationStatus& operationStatus() const;

private:
    std::string requestUuid_;
    unsigned long addedTagId_ = -1ul;
    OperationStatus operationStatus_;
};

struct AddTagResponseData
{
    std::string requestUuid;
    unsigned long addedTagId = -1ul;
    OperationStatus operationStatus;
};

struct AddTagRequest
{
    std::string uuid;
    std::shared_ptr<model::Tag> tag;
};
} // namespace memo::remote
