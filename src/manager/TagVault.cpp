#include "manager/TagVault.hpp"
#include "model/Tag.hpp"

namespace memo {

using LockGuard = std::lock_guard<std::mutex>;

namespace {
    std::shared_ptr<model::Tag> CopyOf(const std::shared_ptr<model::Tag>& original);
} // namespace

TagVector TagVault::list() const
{
    const LockGuard lock(mutex_);

    TagVector result;
    result.reserve(idToTag_.size());
    for (const auto& [id, tag] : idToTag_)
    {
        result.emplace_back(CopyOf(tag));
    }
    return result;
}

std::shared_ptr<model::Tag> TagVault::find(unsigned long id) const
{
    const LockGuard lock(mutex_);
    auto tag = _find(id);
    return CopyOf(tag);
}

std::shared_ptr<model::Tag> TagVault::find(const std::string& name) const
{
    const LockGuard lock(mutex_);
    auto tag = _find(name);
    return CopyOf(tag);
}

bool TagVault::add(const std::shared_ptr<model::Tag>& tag)
{
    const LockGuard lock(mutex_);
    if (tag)
    {
        const auto tagById = _find(tag->id());
        const auto tagByName = _find(tag->name());
        if (tagById == tagByName)
        {
            auto copy = CopyOf(tag);
            idToTag_[tag->id()] = copy;
            nameToTag_[tag->name()] = copy;
            return true;
        }
    }
    return false;
}

bool TagVault::remove(unsigned long id)
{
    const LockGuard lock(mutex_);
    if (auto tag = _find(id))
    {
        _remove(*tag);
        return true;
    }
    return false;
}

bool TagVault::remove(const std::string& name)
{
    const LockGuard lock(mutex_);
    if (auto tag = _find(name))
    {
        _remove(*tag);
        return true;
    }
    return false;
}

void TagVault::clear()
{
    const LockGuard lock(mutex_);
    idToTag_.clear();
    nameToTag_.clear();
}

int TagVault::size() const
{
    const LockGuard lock(mutex_);
    return static_cast<int>(idToTag_.size());
}

bool TagVault::empty() const
{
    const LockGuard lock(mutex_);
    return idToTag_.empty();
}

void TagVault::_remove(const model::Tag& tag)
{
    idToTag_.erase(tag.id());
    nameToTag_.erase(tag.name());
}

std::shared_ptr<model::Tag> TagVault::_find(unsigned long id) const
{
    auto iter = idToTag_.find(id);
    return iter != std::end(idToTag_) ? iter->second : nullptr;
}

std::shared_ptr<model::Tag> TagVault::_find(const std::string& name) const
{
    auto iter = nameToTag_.find(name);
    return iter != std::end(nameToTag_) ? iter->second : nullptr;
}

namespace {
    std::shared_ptr<model::Tag> CopyOf(const std::shared_ptr<model::Tag>& original)
    {
        if (original)
            return std::make_shared<model::Tag>(*original);
        return nullptr;
    }
} // namespace

} // namespace memo
