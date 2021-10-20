#include "remote/ClientAdapterUtils.hpp"
#include "model/Tag.hpp"
#include "model/conversions.hpp"

namespace memo {
namespace {

    void populateMemoFilter(proto::MemoFilter& filter, const remote::MemoFilter& source);

    void populateTagFilter(proto::TagFilter& filter, const remote::TagFilter& source);

    std::map<unsigned long, std::shared_ptr<model::Tag>> extractTags(const proto::ListMemosRs& response);

    remote::OperationStatus ReadOperationStatus(const proto::OperationStatus& protoStatus);

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
        auto& memos = responseData.memos;
        for (const auto& protoMemo : response.memos())
        {
            auto memo = ProtoToModel(protoMemo, tags);
            memos.emplace_back(std::make_shared<model::Memo>(std::move(memo)));
        }
        responseData.requestUuid = response.request_uuid();
        responseData.nextPageToken = response.next_page_token();
        responseData.prevPageToken = response.prev_page_token();
        return responseData;
    }
} // namespace ListMemos

namespace AddMemo {
    proto::AddMemoRq CreateProtoRequest(const remote::AddMemoRequest& input)
    {
        proto::AddMemoRq request;
        request.set_uuid(input.uuid);
        (*request.mutable_memo()) = ModelToProto(*input.memo);
        return request;
    }

    remote::AddMemoResponseData ExtractResponseData(const proto::AddMemoRs& response)
    {
        remote::AddMemoResponseData responseData;
        responseData.requestId = response.request_uuid();
        responseData.addedMemoId = response.added_memo().id();
        responseData.operationStatus = ReadOperationStatus(response.operation_status());
        return responseData;
    }
} // namespace AddMemo

namespace UpdateMemo {
    proto::UpdateMemoRq CreateProtoRequest(const remote::UpdateMemoRequest& input)
    {
        proto::UpdateMemoRq request;
        request.set_request_uuid(input.uuid);
        (*request.mutable_memo()) = ModelToProto(*input.memo);
        return request;
    }

    remote::UpdateMemoResponseData ExtractResponseData(const proto::UpdateMemoRs& response)
    {
        remote::UpdateMemoResponseData responseData;
        responseData.requestUuid = response.request_uuid();
        responseData.operationStatus = ReadOperationStatus(response.operation_status());
        return responseData;
    }
} // namespace UpdateMemo

namespace RemoveMemo {
    proto::RemoveMemoRq CreateProtoRequest(const remote::RemoveMemoRequest& input)
    {
        proto::RemoveMemoRq request;
        request.set_request_uuid(input.uuid);
        for (const auto memoId : input.memoIds)
            request.add_ids(memoId);

        return request;
    }

    remote::RemoveMemoResponseData ExtractResponseData(const proto::RemoveMemoRs& response)
    {
        remote::RemoveMemoResponseData responseData;
        responseData.requestUuid = response.request_uuid();
        for (const auto& memo : response.removed_memos())
            responseData.removedMemoIds.emplace_back(memo.id());
        responseData.operationStatus = ReadOperationStatus(response.operation_status());
        return responseData;
    }
} // namespace RemoveMemo



namespace ListTags {
    proto::ListTagsRq CreateProtoRequest(const remote::ListTagsRequest& input)
    {
        proto::ListTagsRq protoRequest;
        protoRequest.set_uuid(input.uuid);
        protoRequest.set_page_token(input.pageToken);
        protoRequest.set_result_page_size(input.resultPageSize);
        protoRequest.set_view(ToProtoModelView(input.modelView));
        populateTagFilter(*protoRequest.mutable_filter(), input.searchFilter);

        return protoRequest;
    }

    remote::ListTagsResponseData ExtractResponseData(const proto::ListTagsRs& response)
    {
        remote::ListTagsResponseData responseData;
        responseData.requestUuid = response.request_uuid();
        responseData.nextPageToken = response.next_page_token();
        responseData.prevPageToken = response.prev_page_token();
        auto& tags = responseData.tags;
        for (const auto& protoTag : response.tags())
        {
            auto tag = ProtoToModel(protoTag);
            tags.emplace_back(std::make_shared<model::Tag>(std::move(tag)));
        }

        return responseData;
    }
} // namespace ListTags

namespace AddTag {
    proto::AddTagRq CreateProtoRequest(const remote::AddTagRequest& input)
    {
        proto::AddTagRq request;
        request.set_uuid(input.uuid);
        (*request.mutable_tag()) = ModelToProto(*input.tag);
        return request;
    }

    remote::AddTagResponseData ExtractResponseData(const proto::AddTagRs& response)
    {
        remote::AddTagResponseData responseData;
        responseData.requestUuid = response.request_uuid();
        responseData.addedTagId = response.tag().id();
        responseData.operationStatus = ReadOperationStatus(response.operation_status());
        return responseData;
    }
} // namespace AddTag

namespace UpdateTag {
    proto::UpdateTagRq CreateProtoRequest(const remote::UpdateTagRequest& input)
    {
        proto::UpdateTagRq request;
        request.set_uuid(input.uuid);
        (*request.mutable_tag()) = ModelToProto(*input.tag);
        return request;
    }

    remote::UpdateTagResponseData ExtractResponseData(const proto::UpdateTagRs& response)
    {
        remote::UpdateTagResponseData responseData;
        responseData.requestUuid = response.request_uuid();
        responseData.operationStatus = ReadOperationStatus(response.operation_status());
        return responseData;
    }
} // namespace UpdateTag

namespace RemoveTag {
    proto::RemoveTagRq CreateProtoRequest(const remote::RemoveTagRequest& input)
    {
        proto::RemoveTagRq request;
        request.set_uuid(input.uuid);
        for (const auto tagId : input.tagIds)
            request.add_ids(tagId);

        return request;
    }

    remote::RemoveTagResponseData ExtractResponseData(const proto::RemoveTagRs& response)
    {
        remote::RemoveTagResponseData responseData;
        responseData.requestUuid = response.request_uuid();
        for (const auto& tag : response.removed_tags())
            responseData.removedTagIds.emplace_back(tag.id());
        responseData.operationStatus = ReadOperationStatus(response.operation_status());
        return responseData;
    }
} // namespace RemoveTag

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

    void populateTagFilter(proto::TagFilter& filter, const remote::TagFilter& source)
    {
        filter.set_name_starts_with(source.namePrefix);
        filter.set_contains(source.nameContains);
        if (source.timestampFrom != -1ul)
            filter.mutable_creation_time()->set_start(source.timestampFrom);
        if (source.timestampUntil != -1ul)
            filter.mutable_creation_time()->set_end(source.timestampUntil);
        for (const int color : source.colors)
            filter.add_colours(color);
        for (const auto memoId : source.memoIds)
            filter.add_assigned_to_memos(memoId);
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

    remote::OperationStatus ReadOperationStatus(const proto::OperationStatus& protoStatus)
    {
        remote::OperationStatus status;
        status.code = protoStatus.code();
        status.comment = protoStatus.comment();
        switch (protoStatus.type())
        {
            case proto::OperationStatus_Type_SUCCESS: status.type = remote::OperationStatus::kSuccess; break;
            case proto::OperationStatus_Type_WARNING: status.type = remote::OperationStatus::kWarning; break;
            default: status.type = remote::OperationStatus::kWarning;
        }
        return status;
    }
} // namespace
} // namespace memo