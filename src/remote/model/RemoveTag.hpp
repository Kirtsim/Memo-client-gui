#pragma once
#include "remote/model/OperationStatus.hpp"
#include <string>
#include <vector>

namespace memo::remote {

struct RemoveTagResponseData;

class RemoveTagResponse
{
public:
    static RemoveTagResponse Construct(const RemoveTagResponseData& data);

    const std::string& requestUuid() const;

    const std::vector<unsigned long>& removedTagIds() const;

    const OperationStatus& operationStatus() const;

private:
    std::string requestUuid_;
    std::vector<unsigned long> removedTagIds_;
    OperationStatus operationStatus_;
};

struct RemoveTagResponseData
{
    std::string requestUuid;
    std::vector<unsigned long> removedTagIds;
    OperationStatus operationStatus;
};

struct RemoveTagRequest
{
    std::string uuid;
    std::vector<unsigned long> tagIds;
};

} // namespace memo::remote
