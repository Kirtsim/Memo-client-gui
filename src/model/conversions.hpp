#include "model/Memo.hpp"
#include "model/Tag.hpp"
#include "Memo.pb.h"
#include "Tag.pb.h"
#include <map>

namespace memo {

proto::Memo ModelToProto(const model::Memo& memo);

proto::Tag ModelToProto(const model::Tag& tag);

model::Memo ProtoToModel(const proto::Memo& memo, const std::map<unsigned long, std::shared_ptr<model::Tag>>& tags={});

model::Tag ProtoToModel(const proto::Tag& tag);

} // namespace memo
