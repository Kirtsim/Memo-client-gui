#include "remote/model/ListMemos.hpp"

namespace memo::remote {

ListMemosResponse ListMemosResponse::Construct(const ListMemosResponseData& data)
{
    ListMemosResponse response;
    response.memos_ = data.memos;
    response.requestUuid_ = data.requestUuid;
    response.prevPageToken_ = data.prevPageToken;
    response.nextPageToken_ = data.nextPageToken;
    return response;
}

const std::string& ListMemosResponse::requestUuid() const
{
    return requestUuid_;
}

const std::vector<std::shared_ptr<model::Memo>>& ListMemosResponse::memos() const
{
    return memos_;
}

const std::string& ListMemosResponse::prevPageToken() const
{
    return prevPageToken_;
}

const std::string& ListMemosResponse::nextPageToken() const
{
    return nextPageToken_;
}
} // namespace memo::remote
