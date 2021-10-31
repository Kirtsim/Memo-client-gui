#pragma once
#include "remote/IGrpcClientAdapter.hpp"
#include "remote/concurrency/visitor/WorkerVisitor.hpp"
#include <QThread>

namespace memo::remote {

class Worker : public QThread
{
    Q_OBJECT
public:
    virtual ~Worker() override = default;

    virtual const QString& id() const = 0;

    virtual void accept(WorkerVisitor& visitor) = 0;

signals:
    void workFinished(const QString& workId);
};

} // namespace memo::remote
