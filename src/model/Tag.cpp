#include "Tag.hpp"

namespace model {

void Tag::setId(unsigned long id)
{
    id_ = id;
}

unsigned long Tag::id() const
{
    return id_;
}

void Tag::setName(const std::string& name)
{
    name_ = name;
}

const std::string& Tag::name() const
{
    return name_;
}

void Tag::setColor(int color)
{
    color_ = color;
}

int Tag::color() const
{
    return color_;
}

void Tag::setTimestamp(unsigned long timestamp)
{
    timestamp_ = timestamp;
}

unsigned long Tag::timestamp() const
{
    return timestamp_;
}

} // namespace model
