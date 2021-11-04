#include "model/conversions.hpp"

namespace memo {

proto::Memo ModelToProto(const model::Memo& memo)
{
    proto::Memo result;
    result.set_id(memo.id());
    result.set_title(memo.title());
    result.set_description(memo.description());
    result.set_timestamp(memo.timestamp());
    for (const auto& tag : memo.tags())
    {
        if (tag)
        {
            result.add_tag_ids(tag->id());
        }
    }
    return result;
}

proto::Tag ModelToProto(const model::Tag& tag)
{
    proto::Tag result;
    result.set_id(tag.id());
    result.set_name(tag.name());
    result.set_color(tag.color());
    result.set_timestamp(tag.timestamp());
    return result;
}

model::Memo ProtoToModel(const proto::Memo& memo, const std::map<unsigned long, std::shared_ptr<model::Tag>>& tags)
{
    model::Memo result;
    result.setId(memo.id());
    result.setTitle(memo.title());
    result.setDescription(memo.description());
    result.setTimestamp(memo.timestamp());
    for (const auto tagId : memo.tag_ids())
    {
        auto it = tags.find(tagId);
        auto tag = (it != std::end(tags)) ? it->second : nullptr;
        if (tag)
        {
            result.addTag(it->second);
        }
    }
    return result;
}

model::Tag ProtoToModel(const proto::Tag& tag)
{
    model::Tag result;
    result.setId(tag.id());
    result.setName(tag.name());
    result.setColor(tag.color());
    result.setTimestamp(tag.timestamp());
    return result;
}
} // namespace memo
