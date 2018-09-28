#include "Core/QSqlRelationalLib/Headers/DataDump.h"
#include <QSqlRecord>
#include <QDebug>
#include <QFile>
#include <fstream>

DataDump::DataDump(QSqlRelationalDataModel* sqlModel)
{
    sqlDataModel = sqlModel;

    sqlQueryModel = new QSqlQueryModel;
}

DataDump::~DataDump()
{
    delete sqlQueryModel;
}



bool DataDump::checkFileId(QString fileName, QString fileID)
{

    QFile file(fileName);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        if (!file.atEnd())
        {
            QString strFile = file.readLine();

            if (!strFile.contains(fileID))
            {
                return false;
            }
        }
    }
    else
    {
        return false;
    }

    file.close();

    return true;
}




void DataDump::parse()
{

    QStringList lstrTables = sqlDataModel->getTables();


    foreach(QString name, lstrTables)
    {
        QString query = "SHOW CREATE TABLE " + name;

        sqlQueryModel->setQuery(query);

        query = sqlQueryModel->record(0).value(1).toString();

        query.remove("\n");

        QString tblStructureQuery;

        QString strEngine = "ENGINE " + query.split("ENGINE").at(1);

        QStringList lstrTableStructure = query.split(",");

        for (int i = 0; i < lstrTableStructure.count(); i++)
        {
            QString tmpStr = lstrTableStructure.at(i);
            if (tmpStr.contains("CONSTRAINT"))
            {
                QString alterTable = "ALTER TABLE " + name + " ADD ";

                if (tmpStr.contains("ENGINE"))
                {
                    QStringList tmpStrList = tmpStr.split(") ENGINE");
                    alterTable += tmpStrList.at(0) + ";";
                }
                else
                {
                    alterTable += tmpStr + ";";
                }

                lstrTableReference.push_back(alterTable);
            }
            else
            {
                if (tmpStr.contains("ENGINE"))
                {
                    QStringList tmpStrList = tmpStr.split(") ENGINE");
                    tmpStr = tmpStrList.at(0);
                }

                if (tblStructureQuery.isEmpty())
                {
                    tblStructureQuery += tmpStr;
                }
                else
                {
                    tblStructureQuery += ", " + tmpStr;
                }
            }
        }

        tblStructureQuery += ") " + strEngine + ";";

        lstrTableQuery.push_back(tblStructureQuery);
    }
}

void DataDump::dump()
{
    QStringList lstrTables = sqlDataModel->getTables();

    foreach (QString tblName, lstrTables)
    {
        QString strSelectQuery = "SELECT * FROM " + tblName;



        sqlQueryModel->setQuery(strSelectQuery);

        qDebug() << "COUNT: " << sqlQueryModel->rowCount();

        if (sqlQueryModel->rowCount())
        {

            QString strDump = "INSERT INTO " + tblName + " (";


            //get a list of fields--------------------------------------------------

            QStringList lstrFields;

            for (int i = 0; i < sqlQueryModel->columnCount(); i++)
            {
                lstrFields.push_back(sqlQueryModel->record().fieldName(i));
            }


            strDump += lstrFields.at(0);


            for (int i = 1; i < lstrFields.count(); i++)
            {
                strDump += ", " + lstrFields.at(i);
            }

            strDump += ") VALUES (";


            if (sqlQueryModel->record(0).value(0).isNull())
            {
                strDump += ", NULL";
            }
            else
            {
                strDump += "'" + sqlQueryModel->record(0).value(0).toString() + "'";
            }

            for (int j = 1; j < lstrFields.count(); j++)
            {
                if (sqlQueryModel->record(0).value(j).isNull())
                {
                    strDump += ", NULL";
                }
                else
                {
                    strDump += ", '" + sqlQueryModel->record(0).value(j).toString() + "'";
                }
            }

            strDump += ")";

            for (int i = 1; i < sqlQueryModel->rowCount(); i++)
            {
                if (sqlQueryModel->record(i).value(0).isNull())
                {
                    strDump += ", NULL";
                }
                else
                {
                    strDump += ", ('" + sqlQueryModel->record(i).value(0).toString() + "'";
                }

                for (int j = 1; j < lstrFields.count(); j++)
                {
                    if (sqlQueryModel->record(i).value(j).isNull())
                    {
                        strDump += ", NULL";
                    }
                    else
                    {
                        strDump += ", '" + sqlQueryModel->record(i).value(j).toString() + "'";
                    }
                }

                strDump += ")";
            }

            strDump += ";";

            lstrTableDump.push_back(strDump);
        }
    }
}

void DataDump::writeDump(QString fileName)
{
    parse();
    dump();

    QString fName = fileName;

    if (!fileName.contains(".sql"))
    {
        fName += ".sql";
        qDebug() << "FILE NAME: " << fName;
    }


    std::ofstream fout(fName.toStdString());

    fout << "/* ------------------- TABLE STRUCTURES ---------------------*/" << std::endl << std::endl;

    foreach (QString str, lstrTableQuery)
    {
        fout << str.toStdString() << std::endl << std::endl;
    }

    fout << "/* ------------------- TABLE DUMPS ---------------------*/" << std::endl << std::endl;

    foreach (QString str, lstrTableDump)
    {
        fout << str.toStdString() << std::endl << std::endl;
    }

    fout << "/* ------------------- TABLE REFERENCES ---------------------*/" << std::endl << std::endl;

    foreach (QString str, lstrTableReference)
    {
        fout << str.toStdString() << std::endl << std::endl;
    }

    fout.close();
}

void DataDump::writeDump(QString fileName, QString fileID)
{
    parse();
    dump();

    QString fName = fileName;

    if (!fileName.contains(".sql"))
    {
        fName += ".sql";
    }
    else
    {
        fName = fileName;
    }

    std::ofstream fout(fName.toStdString());

    fout << "/* FILE ID: " << fileID.toStdString() << "*/" << std::endl << std::endl;

    fout << "/* ------------------- TABLE STRUCTURES ---------------------*/" << std::endl << std::endl;

    foreach (QString str, lstrTableQuery)
    {
        fout << str.toStdString() << std::endl << std::endl;
    }

    fout << "/* ------------------- TABLE DUMPS ---------------------*/" << std::endl << std::endl;

    foreach (QString str, lstrTableDump)
    {
        fout << str.toStdString() << std::endl << std::endl;
    }

    fout << "/* ------------------- TABLE REFERENCES ---------------------*/" << std::endl << std::endl;

    foreach (QString str, lstrTableReference)
    {
        fout << str.toStdString() << std::endl << std::endl;
    }

    fout.close();
}

void DataDump::readDump(QString fileName)
{
    QFile file(fileName);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString query;

        while (!file.atEnd())
        {
            query.remove("\n");

            qDebug() << query;

            if (query.contains(";"))
            {
                sqlQueryModel->setQuery(query);
                query = "";
            }

            query += file.readLine();
        }
    }

    file.close();
}

bool DataDump::readDump(QString fileName, QString fileID)
{
    QFile file(fileName);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString query;

        if (!file.atEnd())
        {
            QString fileStr = file.readLine();

            if (!fileStr.contains(fileID))
            {
                return false;
            }
        }
        else
        {
            return false;
        }

        while (!file.atEnd())
        {
            if (query.contains(";"))
            {
                sqlQueryModel->setQuery(query);
                query = "";
            }

            query += file.readLine();
        }
    }
    else
    {
        return false;
    }

    file.close();

    return true;
}




