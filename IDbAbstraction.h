#ifndef IDBABSTRACTION_H_
#define IDBABSTRACTION_H_

#include <QStringList>

#include <map>

class IDbAbstraction
{
public:
    virtual ~IDbAbstraction() {};

    virtual QStringList getCollections(int level = 0, QString parent = "") const = 0;
    virtual void updateCollections(QString col) = 0;
};



#endif /* IDBABSTRACTION_H_ */
