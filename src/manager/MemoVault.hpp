#pragma once
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <mutex>


namespace memo {
namespace model { class Memo; }

using MemoVector = std::vector<std::shared_ptr<model::Memo>>;

/// @brief A collection class that stores Memos. Provides basic functionality such as add, find, remove, etc. This
/// class applies uniqueness constraint on the id and title of the stored Memos such that there can only be one Memo
/// with a given id or title.
/// This class is made thread safe.
class MemoVault
{
public:
    /// @brief Lists all containing memos in a vector. This function returns copies of the memos it holds.
    /// @return A vector of Memos.
    MemoVector list() const;

    /// @brief Finds a memo with the given id.
    /// @param id The memo id.
    /// @return A copy of the found memo or nullptr if memo with the given id was not found.
    std::shared_ptr<model::Memo> find(unsigned long id) const;

    /// @brief Finds a memo with the given title.
    /// @param title The memo title.
    /// @return A copy of the found memo or nullptr if memo with the given title was not found.
    std::shared_ptr<model::Memo> find(const std::string& title) const;

    /// @brief Adds a copy of the given memo. Succeeds only if the Memo's id and title are unique across all memos.
    /// The method may as well be used to update existing Memos, provided that the id and title remain unchanged.
    /// Do not use this method to update Memo's id or title as it won't work. In such cases, remove the old Memo first
    /// and only then use this method to add it's updated version.
    /// @param memo The memo to be added to the collection.
    bool add(const std::shared_ptr<model::Memo>& memo);

    /// @brief Removes memo that has the given id if such memo exists.
    /// @param id Memo id.
    bool remove(unsigned long id);

    /// @brief Removes memo that has the given title if such memo exists.
    /// @param title Memo title.
    bool remove(const std::string& title);

    /// @brief Removes all memos. After this operation MemoVault will be empty.
    void clear();

private:
    void _remove(const model::Memo& memo);

    std::shared_ptr<model::Memo> _find(unsigned long id) const;

    std::shared_ptr<model::Memo> _find(const std::string& title) const;

private:
    std::map<unsigned long, std::shared_ptr<model::Memo>> idToMemo_;
    std::map<std::string, std::shared_ptr<model::Memo>> titleToMemo_;
    mutable std::mutex mutex_;
};

} // namespace memo
