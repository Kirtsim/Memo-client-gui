#pragma once
#include "remote/model/ListMemos.hpp"
#include "remote/model/ModelView.hpp"
#include "MemoSvc.pb.h"

namespace memo {

namespace ListMemos {
    proto::ListMemosRq CreateProtoRequest(const remote::ListMemosRequest& input);

    remote::ListMemosResponseData ExtractResponseData(const proto::ListMemosRs& response);
}

proto::ModelView ToProtoModelView(remote::ModelView modelView);

remote::ModelView ToModelView(proto::ModelView modelView);

} // namespace memo
