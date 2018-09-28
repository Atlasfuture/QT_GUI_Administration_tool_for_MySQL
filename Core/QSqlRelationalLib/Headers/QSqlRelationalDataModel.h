/*=================================================
Class QSqlRelationalDataModel
Inherits class QSqlRelationalTableModel
Class contains methods of forming queries, getting
and processing data from server. Class is a model and
can be used by QTableView.
===================================================*/



#ifndef QSQLRELATIONALDATAMODEL_H
#define QSQLRELATIONALDATAMODEL_H




#include <QSqlRelationalTableModel>
#include "QSqlTableStructure.h"
#include "DataDump.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QStringList>
#include <QString>


class QSqlRelationalDataModel : public QSqlRelationalTableModel
{
public:

//MAIN DATA============================================================================


    //agreagate functions enumeration
    enum Agregate
    {
        COUNT = 0,
        SUM = 1,
        AVG = 2,
        MAX = 3,
        MIN = 4
    };

    QSqlRelationalDataModel(QSqlDatabase db = QSqlDatabase(), QObject *parent = 0);
    QSqlQueryModel sqlQueryModel;

    //execute query
    bool queryExec(QString query);


//METHODS RETURNS STRINGS LIST OF OBJECT NAMES==========================================



    //returns names of all database on the host
    QStringList getDatabases();

    //returns namses of all tables in the database
    QStringList getTables();

    //returns names of all columns in the current table
    QStringList getColumns();

    //returns names of all available agregate functions
    QStringList getAgregateNames();




//METHODS OF DATABASE MANAGMENT========================================================



    //set current database
    bool setDatabase(QString dbName);

    //create new database
    bool createDataBase(QString dbName);

    //drop database from the host
    bool dropDataBase(QString dbName);

    //database dump methods
    void readDump(QString fileName);
    void writeDump(QString fileName);

//METHODS OF TABLE MANAGMENT=============================================================


    //create new table in the current database
    bool createTable(QSqlTableStructure tableStructure);

    //drop current table from the current database
    bool dropTable(QString tableName);

    //refresh current table
    void refreshTable();

    //set primary key in the current table
    bool setPrimaryKey(QString columnName);

    //drop primary key from the current
    bool dropPrimaryKey();

    //add new column in the current table
    bool addColumn(QSqlColumn column);

    //drop column from the current table
    bool dropColumn(QString columnName);

    //'varColumn' is a name of modifiable column
    //'column' is a QSqlColumn with a new structure
    bool changeColumn(QString varColumn, QSqlColumn column);

    //add foreign key in the current table
    //'columnName' is column that must be foreign key
    //'refTable' is a table with 'refColumn' - column that is references by a foreign key
    bool addForeignKey(QString columnName, QString refTable, QString refColumn);


    bool agregateExec(QString columnName, int funcType);

    //returns QSqlTableStructure object that contains information about fields of current table
    QSqlTableStructure getTableStructure();
};

#endif // QSQLRELATIONALDATAMODEL_H
