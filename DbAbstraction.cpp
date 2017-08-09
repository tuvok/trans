#include <DbAbstraction.h>

#include <iostream>

DbAbstraction::DbAbstraction() : conn("dbname = trans user = tuvok password = 123 hostaddr = 127.0.0.1 port = 5432")
{

}

DbAbstraction::~DbAbstraction()
{

}

QStringList DbAbstraction::getCollections(int level, QString parent) const
{
    QStringList res;

    QString select = "SELECT name FROM collection WHERE parent='%1';";
    pqxx::nontransaction n(conn);

    try
    {
        pqxx::result r = n.exec(select.arg(parent).toStdString());
        if (r.size() > 0)
            res.push_back("");

        for (const auto& row: r)
        {
            res.push_back(QString(row[0].as<std::string>().c_str()));
        }

    }
    catch (std::exception& e)
    {
        std::cout << "DbAbstraction::getCollections(level=" << level << ", parent='" << parent.toStdString() << "'): " << e.what() << std::endl;
    }

    return res;
}

void DbAbstraction::updateCollections(QString col)
{
    QStringList parts = col.split('.', QString::SkipEmptyParts);

    if (parts.size() <= 0)
        return;

    QString exists = "SELECT count(1) FROM collection WHERE name='%1' AND parent='%2';";
    QString insert = "INSERT INTO collection (name, parent) VALUES ('%1', '%2');";

    pqxx::nontransaction n(conn);

    try
    {
        for (unsigned int i = 0; i < parts.size(); ++i)
        {
            QString parent = "";
            if (i>0)
                parent = parts[i-1];

            pqxx::result r = n.exec(exists.arg(parts[i]).arg(parent).toStdString());

            if (r.begin()[0].as<int>() != 0)
                continue;

            n.exec(insert.arg(parts[i]).arg(parent).toStdString());
        }
    }
    catch (std::exception& e)
    {
        std::cout << "DbAbstraction::updateCollections(col='" << col.toStdString() << "'): " << e.what() << std::endl;
    }
}
