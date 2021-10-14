#pragma once
#include <string>
#include <vector>
#include <memory>

namespace memo::model {

class Tag;

class Memo
{
public:
    void setId(unsigned long id);

    unsigned long id() const;

    void setTitle(const std::string& title);

    const std::string& title() const;

    void setDescription(const std::string& description);

    const std::string& description() const;

    void setTags(const std::vector<std::shared_ptr<Tag>>& tags);

    void addTag(const std::shared_ptr<Tag>& tag);

    const std::vector<std::shared_ptr<Tag>>& tags() const;

    void setTimestamp(unsigned long timestamp);

    unsigned long timestamp() const;

private:
    unsigned long id_ = 0;
    std::string title_;
    std::string description_;
    std::vector<std::shared_ptr<Tag>> tags_;
    unsigned long timestamp_ = 0;
};

} // namespace memo::model
