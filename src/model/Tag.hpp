#pragma once
#include <string>


namespace model {

class Tag
{
public:

    void setId(unsigned long id);

    unsigned long id() const;

    void setName(const std::string& name);

    const std::string& name() const;

    void setColor(int color);

    int color() const;

    void setTimestamp(unsigned long timestamp);

    unsigned long timestamp() const;

private:
    unsigned long id_ = 0;
    std::string name_;
    int color_ = 0;
    unsigned long timestamp_ = 0;
};

} // namespace model