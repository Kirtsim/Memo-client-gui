#pragma once
#include "remote/IGrpcClientAdapter.hpp"
#include <QThread>

namespace memo::remote {

class Worker : public QThread
{
    Q_OBJECT
public:
    virtual ~Worker() override = default;

    virtual const QString& id() const = 0;

signals:
    void workFinished(const QString& workId);
};

} // namespace memo::remote
