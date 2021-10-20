#include "remote/ClientAdapterUtils.hpp"
#include "model/Tag.hpp"
#include "model/conversions.hpp"

namespace memo {
namespace {
    void populateMemoFilter(proto::MemoFilter& filter, const remote::MemoFilter& source);
    std::map<unsigned long, std::shared_ptr<model::Tag>> extractTags(const proto::ListMemosRs& response);
} // namespace

namespace ListMemos {
    proto::ListMemosRq CreateProtoRequest(const remote::ListMemosRequest& input)
    {
        proto::ListMemosRq protoRequest;
        protoRequest.set_uuid(input.uuid);
        protoRequest.set_page_token(input.pageToken);
        protoRequest.set_result_page_size(input.resultPageSize);
        protoRequest.set_view(ToProtoModelView(input.memoView));
        populateMemoFilter(*protoRequest.mutable_filter(), input.searchFilter);

        return protoRequest;
    }

    remote::ListMemosResponseData ExtractResponseData(const proto::ListMemosRs& response)
    {
        remote::ListMemosResponseData responseData;
        const auto tags = extractTags(response);
        const auto& memos = responseData.memos;
        for (const auto& protoMemo : response.memos())
        {
            auto memo = ProtoToModel(protoMemo, tags);
            responseData.memos.emplace_back(std::make_shared<model::Memo>(std::move(memo)));
        }
        responseData.requestUuid = response.request_uuid();
        responseData.nextPageToken = response.next_page_token();
        responseData.prevPageToken = response.prev_page_token();
        return responseData;
    }
} // namespace ListMemos

proto::ModelView ToProtoModelView(remote::ModelView modelView)
{
    return modelView == remote::kMinimal ? proto::MINIMAL : proto::COMPLETE;
}

remote::ModelView ToModelView(proto::ModelView modelView)
{
    return modelView == proto::MINIMAL ? remote::kMinimal : remote::kComplete;
}

namespace {

    void populateMemoFilter(proto::MemoFilter& filter, const remote::MemoFilter& source)
    {
        filter.set_title_starts_with(source.titlePrefix);
        for (const auto& keyword: source.titleKeywords)
            filter.add_title_contains(keyword);
        for (const auto& keyword: source.descriptionKeywords)
            filter.add_contains(keyword);
        for (const auto tagId: source.tagIds)
            filter.add_tagged_by(tagId);
        if (source.timestampFrom != -1ul)
            filter.mutable_creation_time()->set_start(source.timestampFrom);
        if (source.timestampUntil != -1ul)
            filter.mutable_creation_time()->set_end(source.timestampUntil);
    }


    std::map<unsigned long, std::shared_ptr<model::Tag>> extractTags(const proto::ListMemosRs& response)
    {
        std::map<unsigned long, std::shared_ptr<model::Tag>> tags;
        auto iter = response.tags().begin();
        while (iter != response.tags().end())
        {
            auto tag = ProtoToModel(iter->second);
            tags[iter->first] = std::make_shared<model::Tag>(std::move(tag));
        }
        return tags;
    }

} // namespace
} // namespace memo