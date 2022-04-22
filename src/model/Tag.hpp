#pragma once
#include "model/Color.hpp"
#include <string>


namespace memo::model {

class Tag
{
public:
    void setId(unsigned long id);

    unsigned long id() const;

    void setName(const std::string& name);

    const std::string& name() const;

    void setColor(const Color& color);

    const Color& color() const;

    void setTimestamp(unsigned long timestamp);

    unsigned long timestamp() const;

private:
    unsigned long id_ = 0;
    std::string name_;
    Color color_;
    unsigned long timestamp_ = 0;
};

} // namespace memo::model