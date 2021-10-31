#include "tools/uuid.hpp"
#include <QUuid>

namespace memo {

std::string GenerateUuid()
{
    return GenerateQUuid().toStdString();
}

QString GenerateQUuid()
{
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}

std::string NullUuid()
{
    return NullQUuid().toStdString();
}

QString NullQUuid()
{
    return QUuid().toString(QUuid::WithoutBraces);
}

} // namespace memo