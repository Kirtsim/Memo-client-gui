#pragma once
#include <string>

namespace memo::remote {

struct OperationStatus
{
    enum Type {
        FAILURE;
        WARNING;
        SUCCESS;
    } type;
    int code;
    std::string comment;
};

} // namespace memo::remote