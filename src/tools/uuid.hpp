#pragma once
#include <string>
#include <QString>

namespace memo {

/// @brief Generates a new uuid string in the format xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx.
/// @return Uuid std::string.
std::string GenerateUuid();

/// @brief Generates a new uuid string in the format xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx.
/// @return Uuid QString.
QString GenerateQUuid();

/// @brief Returns a null uuid as std::string. Null uuid consists of zeros only: 00000000-0000-0000-0000-000000000000.
std::string NullUuid();

/// @brief Returns a null uuid as QString. Null uuid consists of zeros only: 00000000-0000-0000-0000-000000000000.
QString NullQUuid();

} // namespace memo