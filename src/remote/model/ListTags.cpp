#include "remote/model/ListTags.hpp"

namespace memo::remote {

ListTagsResponse ListTagsResponse::Construct(const ListTagsResponseData& data)
{
    ListTagsResponse response;
    response.requestUuid_ = data.requestUuid;
    response.tags_ = data.tags;
    response.prevPageToken_ = data.prevPageToken;
    response.nextPageToken_ = data.nextPageToken;
    return response;
}

const std::string& ListTagsResponse::requestUuid() const
{
    return requestUuid_;
}

const std::vector<std::shared_ptr<model::Tag>>& ListTagsResponse::tags() const
{
    return tags_;
}

const std::string& ListTagsResponse::prevPageToken() const
{
    return prevPageToken_;
}

const std::string& ListTagsResponse::nextPageToken() const
{
    return nextPageToken_;
}
} // namespace memo::remote
