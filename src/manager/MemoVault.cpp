#include "manager/MemoVault.hpp"
#include "model/Memo.hpp"


namespace memo {

using LockGuard = std::lock_guard<std::mutex>;

MemoVector MemoVault::list() const
{
    const LockGuard guard(mutex_);
    MemoVector result;
    result.reserve(idToMemo_.size());

    for (const auto& [id, memo]: idToMemo_)
    {
        auto copy = std::make_shared<model::Memo>(*memo);
        result.emplace_back(copy);
    }
    return result;
}

std::shared_ptr<model::Memo> MemoVault::find(unsigned long id) const
{
    const LockGuard guard(mutex_);
    return _find(id);
}

std::shared_ptr<model::Memo> MemoVault::find(const std::string& title) const
{
    const LockGuard guard(mutex_);
    return _find(title);
}

bool MemoVault::add(const std::shared_ptr<model::Memo>& memo)
{
    const LockGuard guard(mutex_);
    if (memo)
    {
        auto memoById = _find(memo->id());
        auto memoByTitle = _find(memo->title());

        if (memoById == memoByTitle)
        {
            auto copy = std::make_shared<model::Memo>(*memo);
            titleToMemo_[memo->title()] = copy;
            idToMemo_[memo->id()] = copy;
            return true;
        }
    }
    return false;
}

bool MemoVault::remove(unsigned long id)
{
    const LockGuard guard(mutex_);
    if (auto memo = _find(id))
    {
        _remove(*memo);
        return true;
    }
    return false;
}

bool MemoVault::remove(const std::string& title)
{
    const LockGuard guard(mutex_);
    if (auto memo = _find(title))
    {
        _remove(*memo);
        return true;
    }
    return false;
}

void MemoVault::clear()
{
    const LockGuard guard(mutex_);
    idToMemo_.clear();
    titleToMemo_.clear();
}

void MemoVault::_remove(const model::Memo& memo)
{
    idToMemo_.erase(memo.id());
    titleToMemo_.erase(memo.title());
}

std::shared_ptr<model::Memo> MemoVault::_find(unsigned long id) const
{
    auto iter = idToMemo_.find(id);
    return iter != std::end(idToMemo_) ? iter->second : nullptr;
}

std::shared_ptr<model::Memo> MemoVault::_find(const std::string& title) const
{
    auto iter = titleToMemo_.find(title);
    return iter != std::end(titleToMemo_) ? iter->second : nullptr;
}

} // namespace memo