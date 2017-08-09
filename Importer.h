#ifndef IMPORTER_H_
#define IMPORTER_H_

#include <IDbAbstraction.h>
#include <Paragraph.h>

#include <QObject>
#include <QTextDocument>
#include <QUrl>

#include <mutex>

class Importer : public QObject
{
    Q_OBJECT

public:
    Importer(IDbAbstraction& idb_);
    virtual ~Importer();

signals:
    void showTexImportPreview(int id);
    void setCollectionCombos(int level, QStringList collections);

public slots:
    void importTex(QUrl path, QObject* doc);
    void importAccepted(int id, QString collection);
    void importDeclined(int id);
    void provideCollections(int level, QString parent);

private:
    QTextDocument* prepareDocument(const std::vector<Paragraph>& par, QTextDocument* doc);
    void prepareAndSave(const std::vector<Paragraph>& par, QString collection);

    IDbAbstraction& idb;
    int counter = 0;
    std::map<int, std::vector<Paragraph>> inProgress;
    std::mutex inProgress_mtx;
};

#endif /* IMPORTER_H_ */
