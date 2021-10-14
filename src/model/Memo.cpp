#include "model/Memo.hpp"

namespace memo::model {

void Memo::setId(unsigned long id)
{
    id_ = id;
}

unsigned long Memo::id() const
{
    return id_;
}

void Memo::setTitle(const std::string& title)
{
    title_ = title;
}

const std::string& Memo::title() const
{
    return title_;
}

void Memo::setDescription(const std::string& description)
{
    description_ = description;
}

const std::string& Memo::description() const
{
    return description_;
}

void Memo::setTags(const std::vector<std::shared_ptr<Tag>>& tags)
{
    tags_ = tags;
}

void Memo::addTag(const std::shared_ptr<Tag>& tag)
{
    if (tag)
    {
        tags_.emplace_back(tag);
    }
}

const std::vector<std::shared_ptr<Tag>>& Memo::tags() const
{
    return tags_;
}

void Memo::setTimestamp(unsigned long timestamp)
{
    timestamp_ = timestamp;
}

unsigned long Memo::timestamp() const
{
    return timestamp_;
}

} // namespace memo::model