//Source file for QSqlRelationalDataModel class

#include "Core/QSqlRelationalLib/Headers/QSqlRelationalDataModel.h"
#include <QMessageBox>
#include <QSqlIndex>
#include <QSqlRecord>
#include <QSqlField>
#include <QDebug>

QSqlRelationalDataModel::QSqlRelationalDataModel(QSqlDatabase db, QObject* parent) : QSqlRelationalTableModel(parent, db)
{
    //ctor
}

bool QSqlRelationalDataModel::queryExec(QString query)
{
    setQuery(QSqlQuery(query));

    if (lastError().isValid())
    {
        return false;
    }
    return true;
}



//METHODS RETURNS STRINGS LIST OF OBJECT NAMES==========================================



QStringList QSqlRelationalDataModel::getDatabases()
{
    sqlQueryModel.setQuery(QSqlQuery("show databases"));

    QStringList qstrDatabases;

    for (int i = 0; i < sqlQueryModel.rowCount(); i++)
    {
        //bad code need
        //needs to refactor
        if (sqlQueryModel.record(i).value(0).toString() == "lb_student_tmp")
        {
            continue;
        }

        qstrDatabases << sqlQueryModel.record(i).value(0).toString();
    }

    return qstrDatabases;
}

QStringList QSqlRelationalDataModel::getTables()
{
    QStringList qstrTables;


    sqlQueryModel.setQuery("show tables");

    for (int i = 0; i < sqlQueryModel.rowCount(); i++)
    {
        qstrTables << sqlQueryModel.record(i).value(0).toString();
    }


    return qstrTables;
}

QStringList QSqlRelationalDataModel::getColumns()
{
    QStringList columns;

    for (int i = 0; i < columnCount(); i++)
    {
        columns.push_back(record().fieldName(i));
    }

    return columns;
}

QStringList QSqlRelationalDataModel::getAgregateNames()
{
    QStringList strListAgregates;

    strListAgregates.push_back("COUNT");
    strListAgregates.push_back("SUM");
    strListAgregates.push_back("AVG");
    strListAgregates.push_back("MAX");
    strListAgregates.push_back("MIN");

    return strListAgregates;
}



//METHODS OF DATABASE MANAGMENT========================================================



bool QSqlRelationalDataModel::setDatabase(QString dbName)
{
    QString queryTmp = "use " + dbName;

    qDebug() << "USE QUERY: " << queryTmp;

    sqlQueryModel.setQuery(queryTmp);

    if (!sqlQueryModel.lastError().isValid())
    {
        return true;
    }

    return false;
}

bool QSqlRelationalDataModel::createDataBase(QString dataBaseName)
{
    QString queryTmp = "create database " + dataBaseName;

    sqlQueryModel.setQuery(queryTmp);

    if (!sqlQueryModel.lastError().isValid())
    {
        return true;
    }

    return false;
}

bool QSqlRelationalDataModel::dropDataBase(QString dbName)
{
    QString queryTmp = "drop database " + dbName;


    sqlQueryModel.setQuery(queryTmp);

    if (!sqlQueryModel.lastError().isValid())
    {
        return true;
    }

    return false;
}

void QSqlRelationalDataModel::readDump(QString fileName)
{
    DataDump dataDump(this);
    dataDump.readDump(fileName);
}

void QSqlRelationalDataModel::writeDump(QString fileName)
{
    DataDump dataDump(this);
    dataDump.writeDump(fileName);
}


//METHODS OF TABLE MANAGMENT=============================================================



bool QSqlRelationalDataModel::createTable(QSqlTableStructure tableStructure)
{
    QString queryTmp = "create table " + tableStructure.getName() + " (";

    int columnCount = tableStructure.columnCount();

    for (int i = 0; i < columnCount; i++)
    {
        sptr_qSqlColumn currentColumn = tableStructure.getColumn(i);

        queryTmp += currentColumn->getName() + " ";
        queryTmp += currentColumn->getTypeName() + " ";

        if (!currentColumn->getPrecision().isEmpty())
        {
            queryTmp += "(" + currentColumn->getPrecision() + ")";
        }

        if (!currentColumn->isNull())
        {
            queryTmp += " not null ";
        }

        if (currentColumn->isAutoIncrement())
        {
            queryTmp += " auto_increment ";
        }

        if (i != columnCount - 1)
        {
            queryTmp += ", ";
        }
    }

    if (tableStructure.getPrimaryKey().size())
    {
        queryTmp += ", primary key (";

        std::vector<sptr_qSqlColumn> prmKey = tableStructure.getPrimaryKey();

        for (int i = 0; i < prmKey.size(); i++)
        {
            queryTmp += prmKey[i]->getName();
            if (i != prmKey.size() - 1)
            {
                queryTmp += " , ";
            }
            else
            {
                queryTmp += ")";
            }
        }
    }

    queryTmp += ")";

    if (queryExec(queryTmp))
    {
        refreshTable();
        return true;
    }
    refreshTable();
    return false;
}

bool QSqlRelationalDataModel::dropTable(QString tableName)
{
    QString queryTmp = "drop table " + tableName;

    if (queryExec(queryTmp))
    {
        refreshTable();
        return true;
    }
    return false;
}

void QSqlRelationalDataModel::refreshTable()
{
    setTable(tableName());
    select();
}


bool QSqlRelationalDataModel::setPrimaryKey(QString columnName)
{
    QString queryTmp = "alter table " + tableName();

    if (!primaryKey().isEmpty())
    {
        queryTmp += " drop primary key, ";
    }

    queryTmp += " add primary key (" + columnName + ")";

    if (queryExec(queryTmp))
    {
        refreshTable();
        return true;
    }
    refreshTable();
    return false;
}


bool QSqlRelationalDataModel::dropPrimaryKey()
{
    QString queryTmp = "alter table " + tableName() + " drop primary key";

    if (queryExec(queryTmp))
    {
        return true;
        refreshTable();
    }
    refreshTable();
    return false;
}

bool QSqlRelationalDataModel::addColumn(QSqlColumn column)
{
    QString queryTmp = "alter table " + tableName() + " add column ";

    queryTmp += column.getName() + " ";
    queryTmp += column.getTypeName() + " ";

    if (!column.getPrecision().isEmpty())
    {
        queryTmp += "(" + column.getPrecision() + ")";
    }

    if (!column.isNull())
    {
        queryTmp += " not null ";
    }

    if (column.isAutoIncrement())
    {
        queryTmp += " auto_increment ";
    }

    qDebug() << queryTmp;

    if (queryExec(queryTmp))
    {
        refreshTable();
        return true;
    }
    refreshTable();
    return false;
}



bool QSqlRelationalDataModel::dropColumn(QString columnName)
{
    QString queryTmp = "alter table " + tableName() + " drop column " + columnName;

    if (queryExec(queryTmp))
    {
        refreshTable();
        return true;
    }
    refreshTable();
    return false;
}



bool QSqlRelationalDataModel::changeColumn(QString varColumn, QSqlColumn column)
{
    QString queryTmp = "alter table " + tableName() + " change " + varColumn + " ";

    queryTmp += column.getName() + " ";
    queryTmp += column.getTypeName() + " ";

    if (!column.getPrecision().isEmpty())
    {
        queryTmp += "(" + column.getPrecision() + ")";
    }

    if (!column.isNull())
    {
        queryTmp += " not null ";
    }

    if (column.isAutoIncrement())
    {
        queryTmp += " auto_increment ";
    }

    if (queryExec(queryTmp))
    {
        refreshTable();
        return true;
    }
    refreshTable();
    return false;
}



bool QSqlRelationalDataModel::addForeignKey(QString columnName, QString refTable, QString refColumn)
{
    QString queryTmp = "alter table " + tableName() + " add foreign key (" + columnName + ") references " + refTable + " (" + refColumn + ")";

    if (queryExec(queryTmp))
    {
        refreshTable();
        return true;
    }
    refreshTable();
    return false;
}



bool QSqlRelationalDataModel::agregateExec(QString columnName, int funcType)
{
    QString tmpString = "SELECT ";

    switch (funcType)
    {
    case COUNT:
        tmpString += "COUNT(";
        break;
    case SUM:
        tmpString += "SUM(";
        break;
    case AVG:
        tmpString += "AVG(";
        break;
    case MAX:
        tmpString += "MAX(";
        break;
    case MIN:
        tmpString += "MIN(";
        break;
    }

    tmpString += columnName + ") FROM " + tableName();

    QMessageBox msgBox;
    msgBox.setText(tmpString);
    msgBox.exec();

    return queryExec(tmpString);
}

QSqlTableStructure QSqlRelationalDataModel::getTableStructure()
{
    QSqlTableStructure tableStructure(tableName());

    QString queryTmp = "describe " + tableName();

    queryExec(queryTmp);

    for (int i = 0; i < rowCount(); i++)
    {
        std::shared_ptr<QSqlColumn> column(new QSqlColumn(record(i).value(0).toString()));

        select();
        column->setPrecision(QString::number(record().field(i).length()));

        QString queryTmp = "describe " + tableName();
        queryExec(queryTmp);

        QString typeName = record(i).value(1).toString();

        if (typeName.contains("tinyint"))
        {
            column->setType(QSqlColumn::TINYINT);
        }
        else if (typeName.contains("smallint"))
        {
            column->setType(QSqlColumn::SMALLINT);
        }
        else if (typeName.contains("mediumint"))
        {
            column->setType(QSqlColumn::MEDIUMINT);
        }
        else if (typeName.contains("int"))
        {
            column->setType(QSqlColumn::INT);
        }
        else if (typeName.contains("bigint"))
        {
            column->setType(QSqlColumn::BIGINT);
        }
        else if (typeName.contains("float"))
        {
            column->setType(QSqlColumn::FLOAT);
        }
        else if (typeName.contains("double"))
        {
            column->setType(QSqlColumn::DOUBLE);
        }
        else if (typeName.contains("double precision"))
        {
            column->setType(QSqlColumn::DOUBLE_PRECISION);
        }
        else if (typeName.contains("real"))
        {
            column->setType(QSqlColumn::REAL);
        }
        else if (typeName.contains("decimal"))
        {
            column->setType(QSqlColumn::DECIMAL);
        }
        else if (typeName.contains("date"))
        {
            column->setType(QSqlColumn::DATE);
        }
        else if (typeName.contains("datetime"))
        {
            column->setType(QSqlColumn::DATETIME);
        }
        else if (typeName.contains("timestamp"))
        {
            column->setType(QSqlColumn::TIMESTAMP);
        }
        else if (typeName.contains("time"))
        {
            column->setType(QSqlColumn::TIME);
        }
        else if (typeName.contains("year"))
        {
            column->setType(QSqlColumn::YEAR);
        }
        else if (typeName.contains("varchar"))
        {
            column->setType(QSqlColumn::VARCHAR);
        }
        else if (typeName.contains("char"))
        {
            column->setType(QSqlColumn::CHAR);
        }
        else if (typeName.contains("tinyblob"))
        {
            column->setType(QSqlColumn::TINYBLOB);
        }
        else if (typeName.contains("blob"))
        {
            column->setType(QSqlColumn::BLOB);
        }
        else if (typeName.contains("text"))
        {
            column->setType(QSqlColumn::TEXT);
        }
        else if (typeName.contains("enum"))
        {
            column->setType(QSqlColumn::ENUM);
        }
        else if (typeName.contains("set"))
        {
            column->setType(QSqlColumn::SET);
        }

        if (record(i).value(2).toString() == "NO")
        {
            column->setNull(false);
        }

        if (record(i).value(3).toString() == "PRI")
        {
            column->setPrimaryKey(true);
        }

        if (record(i).value(5).toString() == "auto_increment")
        {
            column->autoIncrement = true;
        }

        tableStructure.addColumn(column);
    }
    refreshTable();

    return tableStructure;
}
