#ifndef DBABSTRACTION_H_
#define DBABSTRACTION_H_

#include <IDbAbstraction.h>

#include <pqxx/pqxx>

class DbAbstraction: public IDbAbstraction
{
public:
    DbAbstraction();
    virtual ~DbAbstraction();

    QStringList getCollections(int level = 0, QString parent = "") const;

    void updateCollections(QString col);

private:
    mutable pqxx::connection conn;
};

#endif /* DBABSTRACTION_H_ */
