#pragma once
#include "remote/model/ModelView.hpp"
#include <string>
#include <vector>
#include <memory>

namespace memo::model { class Tag; }
namespace memo::remote {

struct TagFilter
{
    std::string namePrefix;
    std::string nameContains;
    std::vector<int> colors;
    std::vector<unsigned long> memoIds;
    unsigned long timestampFrom = -1ul;
    unsigned long timestampUntil = -1ul;
};

struct ListTagsResponseData;

class ListTagsResponse
{
public:
    static ListTagsResponse Construct(const ListTagsResponseData& data);

    const std::string& requestUuid() const;

    const std::vector<std::shared_ptr<model::Tag>>& tags() const;

    const std::string& prevPageToken() const;

    const std::string& nextPageToken() const;

private:
    std::string requestUuid_;
    std::vector<std::shared_ptr<model::Tag>> tags_;
    std::string prevPageToken_;
    std::string nextPageToken_;
};

struct ListTagsResponseData
{
    std::string requestUuid;
    std::vector<std::shared_ptr<model::Tag>> tags;
    std::string prevPageToken;
    std::string nextPageToken;
};

struct ListTagsRequest
{
    std::string uuid;
    TagFilter searchFilter;
    int resultPageSize = 20;
    ModelView modelView;         // MINIMAL will return id & name of the tags
    std::string pageToken;
};



} // namespace memo::remote

