#pragma once
#include "remote/concurrency/visitor/WorkerVisitor.hpp"

#include <memory>

#include <QObject>
#include <QString>
#include <QVector>
#include <QMap>

namespace memo {
namespace model { class Tag; }
namespace remote { class Worker; }

    class IGrpcClientAdapter;
    class TagVault;

class TagCollection : public QObject, remote::TagWorkerVisitor
{
    Q_OBJECT

public:
    explicit TagCollection(std::unique_ptr<IGrpcClientAdapter> client);

    ~TagCollection() override;

    bool add(const std::shared_ptr<model::Tag>& tag);

    std::shared_ptr<model::Tag> find(unsigned long tagId);

    std::shared_ptr<model::Tag> find(const std::string& name);

    void visit(const remote::QueryTagWorker& worker) override;

    void visit(const remote::CreateTagWorker& worker) override;

    void visit(const remote::UpdateTagWorker& worker) override;

    void visit(const remote::RemoveTagWorker& worker) override;

signals:
    void tagCacheCleared();

    void tagAdded(unsigned long tagId);

    void tagsAdded(const QVector<qulonglong>& tagIds);

    void tagUpdated(unsigned long tagId);

    void tagRemoved(unsigned long tagId);

    void networkOperationFailed(int operation);

public slots:
    std::vector<std::shared_ptr<model::Tag>> listAll();

    bool remove(unsigned long tagId);

private slots:
    void onWorkerFinished(const QString& workId);

private:
    std::unique_ptr<IGrpcClientAdapter> client_;
    std::unique_ptr<TagVault> tagVault_;
    QMap<QString, remote::Worker*> workers_;
};

} // namespace memo
