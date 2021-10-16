#pragma once
#include "remote/GrpcResponse.hpp"
#include "MemoSvc.pb.h"
#include "TagSvc.pb.h"

#include <string>

namespace memo {

class IGrpcClient
{
public:
    virtual ~IGrpcClient() = default;

    virtual bool ping() = 0;

    virtual GrpcResponse<proto::ListMemosRs> listMemos(const proto::ListMemosRq& request) = 0;

    virtual GrpcResponse<proto::AddMemoRs> addMemo(const proto::AddMemoRq& request) = 0;

    virtual GrpcResponse<proto::UpdateMemoRs> updateMemo(const proto::UpdateMemoRq& request) = 0;

    virtual GrpcResponse<proto::RemoveMemoRs> removeMemo(const proto::RemoveMemoRq& request) = 0;

    virtual GrpcResponse<proto::ListTagsRs> listTags(const proto::ListTagsRq& request) = 0;

    virtual GrpcResponse<proto::AddTagRs> addTag(const proto::AddTagRq& request) = 0;

    virtual GrpcResponse<proto::UpdateTagRs> updateTag(const proto::UpdateTagRq& request) = 0;

    virtual GrpcResponse<proto::RemoveTagRs> removeTag(const proto::RemoveTagRq& request) = 0;
};

} // namespace memo
