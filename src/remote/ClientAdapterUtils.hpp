#pragma once
#include "remote/model/ListMemos.hpp"
#include "remote/model/AddMemo.hpp"
#include "remote/model/UpdateMemo.hpp"
#include "remote/model/RemoveMemo.hpp"
#include "remote/model/ListTags.hpp"
#include "remote/model/AddTag.hpp"
#include "remote/model/UpdateTag.hpp"
#include "remote/model/RemoveTag.hpp"
#include "remote/model/ModelView.hpp"
#include "MemoSvc.pb.h"
#include "TagSvc.pb.h"

namespace memo {

namespace ListMemos {
    proto::ListMemosRq CreateProtoRequest(const remote::ListMemosRequest& input);

    remote::ListMemosResponseData ExtractResponseData(const proto::ListMemosRs& response);
} // namespace ListMemos

namespace AddMemo {
    proto::AddMemoRq CreateProtoRequest(const remote::AddMemoRequest& input);

    remote::AddMemoResponseData ExtractResponseData(const proto::AddMemoRs& response);
} // namespace AddMemo

namespace UpdateMemo {
    proto::UpdateMemoRq CreateProtoRequest(const remote::UpdateMemoRequest& input);

    remote::UpdateMemoResponseData ExtractResponseData(const proto::UpdateMemoRs& response);
} // namespace UpdateMemo

namespace RemoveMemo {
    proto::RemoveMemoRq CreateProtoRequest(const remote::RemoveMemoRequest& input);

    remote::RemoveMemoResponseData ExtractResponseData(const proto::RemoveMemoRs& response);
} // namespace RemoveMemo

namespace ListTags {
    proto::ListTagsRq CreateProtoRequest(const remote::ListTagsRequest& input);

    remote::ListTagsResponseData ExtractResponseData(const proto::ListTagsRs& response);
} // namespace ListTags

namespace AddTag {
    proto::AddTagRq CreateProtoRequest(const remote::AddTagRequest& input);

    remote::AddTagResponseData ExtractResponseData(const proto::AddTagRs& response);
} // namespace AddTag

namespace UpdateTag {
    proto::UpdateTagRq CreateProtoRequest(const remote::UpdateTagRequest& input);

    remote::UpdateTagResponseData ExtractResponseData(const proto::UpdateTagRs& response);
} // namespace UpdateTag

namespace RemoveTag {
    proto::RemoveTagRq CreateProtoRequest(const remote::RemoveTagRequest& input);

    remote::RemoveTagResponseData ExtractResponseData(const proto::RemoveTagRs& response);
} // namespace RemoveTag

proto::ModelView ToProtoModelView(remote::ModelView modelView);

remote::ModelView ToModelView(proto::ModelView modelView);

} // namespace memo
