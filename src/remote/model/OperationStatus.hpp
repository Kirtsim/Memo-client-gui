#pragma once
#include <string>

namespace memo::remote {

struct OperationStatus
{
    enum Type {
        kFailure,
        kWarning,
        kSuccess,
    } type = kSuccess;
    int code;
    std::string comment;
};

} // namespace memo::remote