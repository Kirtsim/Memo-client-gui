#pragma once
namespace memo::remote {

class QueryMemoWorker;
class CreateMemoWorker;
class UpdateMemoWorker;
class RemoveMemoWorker;

class WorkerVisitor
{
public:
    virtual ~WorkerVisitor() = 0;
};

inline WorkerVisitor::~WorkerVisitor()
{
}

class MemoWorkerVisitor : public WorkerVisitor
{
public:
    virtual ~MemoWorkerVisitor() = default;

    virtual void visit(const QueryMemoWorker& worker) = 0;

    virtual void visit(const CreateMemoWorker& worker) = 0;

    virtual void visit(const UpdateMemoWorker& worker) = 0;

    virtual void visit(const RemoveMemoWorker& worker) = 0;

};

class QueryTagWorker;
class CreateTagWorker;
class UpdateTagWorker;
class RemoveTagWorker;

class TagWorkerVisitor : public WorkerVisitor
{
public:
    virtual ~TagWorkerVisitor() = default;

    virtual void visit(const QueryTagWorker& worker) = 0;

    virtual void visit(const CreateTagWorker& worker) = 0;

    virtual void visit(const UpdateTagWorker& worker) = 0;

    virtual void visit(const RemoveTagWorker& worker) = 0;
};

} // namespace memo::remote