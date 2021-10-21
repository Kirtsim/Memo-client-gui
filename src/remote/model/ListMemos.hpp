#pragma once
#include "remote/model/ModelView.hpp"
#include <string>
#include <vector>
#include <map>
#include <memory>

namespace memo::model { class Memo; }

namespace memo::remote {

struct ListMemosResponseData;

class ListMemosResponse
{
public:
    static ListMemosResponse Construct(const ListMemosResponseData& data);

    const std::string& requestUuid() const;

    const std::vector<std::shared_ptr<model::Memo>>& memos() const;

    const std::string& prevPageToken() const;

    const std::string& nextPageToken() const;

private:
    std::string requestUuid_;
    std::vector<std::shared_ptr<model::Memo>> memos_;
    std::string prevPageToken_;
    std::string nextPageToken_;
};

struct MemoFilter
{
    std::string titlePrefix;
    std::vector<std::string> titleKeywords;
    std::vector<std::string> descriptionKeywords;
    std::vector<unsigned long> tagIds;
    unsigned long timestampFrom = -1ul;
    unsigned long timestampUntil = -1ul;
};

struct ListMemosRequest
{
    std::string uuid;
    MemoFilter searchFilter;
    int resultPageSize = 20;
    ModelView memoView;
    std::string pageToken;
};

struct ListMemosResponseData
{
    std::string requestUuid;
    std::vector<std::shared_ptr<model::Memo>> memos;
    std::string prevPageToken;
    std::string nextPageToken;
};


} // namespace memo::remote
