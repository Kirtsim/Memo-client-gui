#include "manager/MemoCollection.hpp"
#include "remote/IGrpcClientAdapter.hpp"
#include "remote/model/ListMemos.hpp"
#include "remote/model/AddMemo.hpp"
#include "model/Memo.hpp"

namespace memo {
using MemoVector = std::vector<std::shared_ptr<model::Memo>>;

namespace {
    MemoVector toVector(const std::map<std::string, std::shared_ptr<model::Memo>>& memoMap);
} // namespace

MemoCollection::MemoCollection(std::unique_ptr<IGrpcClientAdapter> client)
    : QObject()
    , client_(std::move(client))
{
}

MemoCollection::~MemoCollection() = default;

bool MemoCollection::add(const std::shared_ptr<model::Memo>& memo)
{
    remote::AddMemoRequest request;
    request.uuid = "aaaa-bbbb-1111-2222-9999";
    request.memo = memo;

    auto response = client_->addMemo(request);
    if (response.ok())
    {
        auto copy = std::make_shared<model::Memo>(*memo);
        copy->setId(response.body().addedMemoId());
        idToMemo_[copy->id()] = copy;
        titleToMemo_[copy->title()] = copy;

        emit memoAdded(copy->id());
        return true;
    }

    emit networkOperationFailed(static_cast<int>(MemoOperation::kAdd));
    return false;
}

std::shared_ptr<model::Memo> MemoCollection::find(unsigned long memoId)
{
    auto it = idToMemo_.find(memoId);
    return it != std::end(idToMemo_) ? it->second : nullptr;
}

std::shared_ptr<model::Memo> MemoCollection::find(const std::string& title)
{
    auto it = titleToMemo_.find(title);
    return it != std::end(titleToMemo_) ? it->second : nullptr;
}

MemoVector MemoCollection::listAll()
{
    remote::ListMemosRequest request;
    request.uuid = "123-123-123-123-123";
    auto response = client_->listMemos(request);

    if (response.ok())
    {
        idToMemo_.clear();
        titleToMemo_.clear();
        emit memoCacheCleared();

        const auto& responseData = response.body();
        for (const auto& memo : responseData.memos())
        {
            titleToMemo_[memo->title()] = memo;
            idToMemo_[memo->id()] = memo;
            emit memoAdded(memo->id());
        }
        return responseData.memos();
    }
    return toVector(titleToMemo_);
}

bool MemoCollection::remove(unsigned long memoId)
{
    return false;
}

MemoOperation toMemoOperation(int value)
{
    switch (value)
    {
        case 1: return MemoOperation::kListAll;
        case 1 << 1: return MemoOperation::kAdd;
        case 1 << 2: return MemoOperation::kRemove;
        default: return MemoOperation::kUnknown;
    }
}

namespace {

    MemoVector toVector(const std::map<std::string, std::shared_ptr<model::Memo>>& memoMap)
    {
        MemoVector result;
        for (const auto& [title, memo] : memoMap)
            result.emplace_back(memo);
        return result;
    }
} // namespace
} // namespace memo
