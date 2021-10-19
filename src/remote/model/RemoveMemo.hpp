#pragma once
#include "remote/model/OperationStatus.hpp"
#include <string>
#include <vector>

namespace memo::remote {

struct RemoveMemoResponseData;

class RemoveMemoResponse
{
public:
    static RemoveMemoResponse Construct(const RemoveMemoResponseData& data);

    const std::string& requestUuid() const;

    const std::vector<unsigned long>& removedMemoIds() const;

    const OperationStatus& operationStatus() const;

private:
    std::string requestUuid_;
    std::vector<unsigned long> removedMemoIds_;
    OperationStatus operationStatus_;
};

struct RemoveMemoResponseData
{
    std::string requestUuid;
    std::vector<unsigned long> removedMemoIds;
    OperationStatus operationStatus;
};

struct RemoveMemoRequest
{
    std::string uuid;
    std::vector<unsigned long> memoIds;
};

} // namespace memo::remote

