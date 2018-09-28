#include "Core/QSqlRelationalLib/Headers/QSqlTableStructure.h"

QSqlTableStructure::QSqlTableStructure()
{
    autoIncrementColumn = 0;
}

QSqlTableStructure::QSqlTableStructure(QString _name)
{
    name = _name;
    autoIncrementColumn = 0;
}

void QSqlTableStructure::addColumn(sptr_qSqlColumn column)
{
    columns.push_back(column);

    if (column->isPrimaryKey())
    {
        primaryKeyColumns.push_back(column);
    }
}

void QSqlTableStructure::setName(QString tableName)
{
    name = tableName;
}

bool QSqlTableStructure::setAutoIncrement(QString columnName)
{
    for (int i = 0; i < columns.size(); i++)
    {
        if (columns[i]->getName() == columnName)
        {
            if (columns[i]->isPrimaryKey())
            {
                if (autoIncrementColumn)
                {
                    autoIncrementColumn->autoIncrement = false;
                }

                columns[i]->autoIncrement = true;

                autoIncrementColumn = columns[i];

                return true;
            }
        }
    }
    return false;
}

bool QSqlTableStructure::setAutoIncrement(int column)
{
    if (columns[column]->isPrimaryKey())
    {
        if (autoIncrementColumn)
        {
            autoIncrementColumn->autoIncrement = false;
        }

        columns[column]->autoIncrement = true;

        autoIncrementColumn = columns[column];

        return true;
    }

    return false;
}

QString QSqlTableStructure::getName()
{
    return name;
}

sptr_qSqlColumn QSqlTableStructure::getColumn(int column)
{
    return columns[column];
}

sptr_qSqlColumn QSqlTableStructure::getColumn(QString columnName)
{
    for (int i = 0; i < columns.size(); i++)
    {
        if (columns[i]->getName() == columnName)
        {
            return columns[i];
        }
    }
    return 0;
}

std::vector<sptr_qSqlColumn> QSqlTableStructure::getPrimaryKey()
{
    return primaryKeyColumns;
}

sptr_qSqlColumn QSqlTableStructure::getAutoIncrementColumn()
{
    return autoIncrementColumn;
}

int QSqlTableStructure::columnCount()
{
    return columns.size();
}


