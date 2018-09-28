#ifndef DATADUMP_H
#define DATADUMP_H



#include <QStringList>
#include <QSqlQueryModel>
#include <list>
#include "QSqlRelationalDataModel.h"




class DataDump
{
    QStringList lstrTableReference;
    QStringList lstrTableQuery;
    QStringList lstrTableDump;

    QSqlRelationalDataModel* sqlDataModel;
    QSqlQueryModel* sqlQueryModel;

    void parse();
    void dump();

public:
    DataDump(QSqlRelationalDataModel* sqlModel);
    ~DataDump();


    bool checkFileId(QString fileName, QString fileID);
    void writeDump(QString fileName);
    void writeDump(QString fileName, QString fileID);

    void readDump(QString fileName);
    bool readDump(QString fileName, QString fileID);
};




#endif // DBEXPORTER_H
