#pragma once
#include <string>
#include <map>
#include <mutex>
#include <vector>
#include <memory>

namespace memo {
namespace model { class Tag; }

using TagVector = std::vector<std::shared_ptr<model::Tag>>;

class TagVault
{
public:
    /// @brief Lists all containing tags in a vector. This function returns copies of the tags it holds.
    /// @return A vector of Memos.
    TagVector list() const;

    /// @brief Finds a tag with the given id.
    /// @param id The tag id.
    /// @return A copy of the found tag or nullptr if tag with the given id was not found.
    std::shared_ptr<model::Tag> find(unsigned long id) const;

    /// @brief Finds a tag with the given name.
    /// @param name The tag name.
    /// @return A copy of the found tag or nullptr if tag with the given name was not found.
    std::shared_ptr<model::Tag> find(const std::string& name) const;

    /// @brief Adds a copy of the given tag. Succeeds only if the Tag's id and name are unique across all tags.
    /// The method may as well be used to update existing Tags, provided that the id and name remain unchanged.
    /// Do not use this method to update Tag's id or name as it won't work. In such cases, remove the old Tag first
    /// and only then use this method to add it's updated version.
    /// @param tag The tag to be added to the collection.
    /// @return True if the tag was added/updated, false otherwise.
    bool add(const std::shared_ptr<model::Tag>& tag);

    /// @brief Removes tag that has the given id if such tag exists.
    /// @param id Tag id.
    /// @return True if the tag was removed, false if not.
    bool remove(unsigned long id);

    /// @brief Removes tag that has the given name if such tag exists.
    /// @param name Tag name.
    /// @return True if the tag was removed, false if not.
    bool remove(const std::string& name);

    /// @brief Removes all tags. After this operation TagVault will be empty.
    void clear();

    /// @brief Returns the number of tags in the vault.
    int size() const;

    /// @brief Returns true if the vault is empty, false if it contains at least one Tag.
    bool empty() const;

private:
    void _remove(const model::Tag& tag);

    std::shared_ptr<model::Tag> _find(unsigned long id) const;

    std::shared_ptr<model::Tag> _find(const std::string& name) const;

private:
    std::map<unsigned long, std::shared_ptr<model::Tag>> idToTag_;
    std::map<std::string, std::shared_ptr<model::Tag>> nameToTag_;
    mutable std::mutex mutex_;
};

} // namespace memo
