#pragma once
#include "MemoSvc.pb.h"
#include "TagSvc.pb.h"
#include <grpcpp/grpcpp.h>

#include <string>

namespace memo {

class IGrpcClient
{
public:
    virtual ~IGrpcClient() = default;

    virtual grpc::Status status() = 0;

    virtual proto::ListMemosRs listMemos(const proto::ListMemosRq& request) = 0;

    virtual proto::AddMemoRs addMemo(const proto::AddMemoRq& request) = 0;

    virtual proto::UpdateMemoRs updateMemo(const proto::UpdateMemoRq& request) = 0;

    virtual proto::RemoveMemoRs removeMemo(const proto::RemoveMemoRq& request) = 0;

    virtual proto::ListTagsRs listTags(const proto::ListTagsRq& request) = 0;

    virtual proto::AddTagRs addTag(const proto::AddTagRq& request) = 0;

    virtual proto::UpdateTagRs updateTag(const proto::UpdateTagRq& request) = 0;

    virtual proto::RemoveTagRs removeTag(const proto::RemoveTagRq& request) = 0;
};

} // namespace memo
