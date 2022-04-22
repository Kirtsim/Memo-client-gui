#include "Tag.hpp"

namespace memo::model {

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

void Tag::setColor(const Color& color)
{
    color_ = color;
}

const Color& Tag::color() const
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

} // namespace memo::model
