#ifndef QSQLTABLESTRUCTURE_H
#define QSQLTABLESTRUCTURE_H
#include "QSqlColumn.h"
#include <memory>
#include <vector>


typedef std::shared_ptr<QSqlColumn> sptr_qSqlColumn;

class QSqlTableStructure
{
    QString name;
    std::vector<sptr_qSqlColumn> columns;               //vector of shared_pointers on columns

    std::vector<sptr_qSqlColumn> primaryKeyColumns;     //vector of shared_pointers on primary key columns

    sptr_qSqlColumn autoIncrementColumn;                //shared_pointer on auto increment column
public:
    QSqlTableStructure();
    QSqlTableStructure(QString _name);

    void addColumn(sptr_qSqlColumn column);

    //set methods
    void setName(QString tableName);
    bool setAutoIncrement(QString columnName);
    bool setAutoIncrement(int column);

    //get methods
    QString getName();

    sptr_qSqlColumn getColumn(int column);
    sptr_qSqlColumn getColumn(QString columnName);

    std::vector<sptr_qSqlColumn> getPrimaryKey();
    sptr_qSqlColumn getAutoIncrementColumn();

    int columnCount();                                  //return number of columns in the current table structure
};

#endif // QSQLTABLESTRUCTURE_H
