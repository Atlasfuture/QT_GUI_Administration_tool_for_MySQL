#include "AgeDistributionReport.h"

#include <QSqlRecord>

#include <QFileDialog>

#include <QDebug>
#include <QDate>

AgeDistributionReport::AgeDistributionReport(QSqlDatabase database, bool _considerMonth)
{
    considerMonth = _considerMonth;

    tmpStudent = std::shared_ptr<StudentTemplate>(
                 new StudentTemplate(std::shared_ptr<QSqlRelationalDataModel> (
                 new QSqlRelationalDataModel(database))));

    designTable();
    fillTable();
}

void AgeDistributionReport::designTable()
{
    QStringList strList;

    strList.push_back(QObject::tr("Найменування показника"));
    strList.push_back(QObject::tr("№ рядка"));
    strList.push_back(QObject::tr("Усього навчається (сума гр. 2 -19)"));
    strList.push_back(QObject::tr("14 років і менше"));

    for (int i = 15; i < 23; i++)
    {
        QString tmpStr = QString::number(i) + " років";
        strList.push_back(tmpStr);
    }

    writer = std::shared_ptr<TableWriter>(new TableWriter(strList));

    strList.clear();

    strList.push_back(QObject::tr("A"));
    strList.push_back(QObject::tr("Б"));

    for (int i = 1; i < 11; i++)
    {
        strList.push_back(QString::number(i));
    }

    writer->insertRow(strList);
}

void AgeDistributionReport::fillTable()
{
    QStringList strFirst;
    QStringList strSecond;
    QStringList strThird;

    //FIRST LINE FILL

    strFirst.push_back(QObject::tr("Усього"));
    strFirst.push_back("01");

    strFirst.push_back(QString::number(tmpStudent->countAllStudents()));

    if (considerMonth)
    {
        tmpStudent->activateFilter(StudentTemplate::AGE_TO_FILTER, "14");
        strFirst.push_back(QString::number(tmpStudent->runTableFilter()));
        tmpStudent->deactivateFilter(StudentTemplate::AGE_TO_FILTER);
    }
    else
    {
        tmpStudent->activateFilter(StudentTemplate::AGE_TO_FILTER_C, "14");
        strFirst.push_back(QString::number(tmpStudent->runTableFilter()));
        tmpStudent->deactivateFilter(StudentTemplate::AGE_TO_FILTER_C);
    }



    //SECOND LINE FILL

    strSecond.push_back(QObject::tr("У т.ч. студентів з числа дітей сиріт"));
    strSecond.push_back("02");


    tmpStudent->activateFilter(StudentTemplate::STATUS_FILTER, "orphan");
    tmpStudent->activateFilter(StudentTemplate::STATUS_FILTER, StudentTemplate::OR_MODE, "guardians");

    strSecond.push_back(QString::number(tmpStudent->runTableFilter()));

    if (considerMonth)
    {
        tmpStudent->activateFilter(StudentTemplate::AGE_TO_FILTER, "14");
        strSecond.push_back(QString::number(tmpStudent->runTableFilter()));
        tmpStudent->deactivateFilter(StudentTemplate::AGE_TO_FILTER);
    }
    else
    {
        tmpStudent->activateFilter(StudentTemplate::AGE_TO_FILTER_C, "14");
        strSecond.push_back(QString::number(tmpStudent->runTableFilter()));
        tmpStudent->deactivateFilter(StudentTemplate::AGE_TO_FILTER_C);
    }

    //THIRD LINE FILL

    strThird.push_back(QObject::tr("з них підлягають випуску у поточному році"));
    strThird.push_back(QObject::tr("03"));

    if (QDate::currentDate().month() < 7)
    {
        tmpStudent->activateFilter(StudentTemplate::COURSE_FILTER, "4");
        strThird.push_back(QString::number(tmpStudent->runTableFilter()));

        if (considerMonth)
        {
            tmpStudent->activateFilter(StudentTemplate::AGE_TO_FILTER, "14");
            strThird.push_back(QString::number(tmpStudent->runTableFilter()));
        }
        else
        {
            tmpStudent->activateFilter(StudentTemplate::AGE_TO_FILTER_C, "14");
            strThird.push_back(QString::number(tmpStudent->runTableFilter()));
        }
    }

    //LOOP

    tmpStudent->deactivateFilters();

    for (int i = 15; i < 23; i++)
    {
        //FIRST LINE FILL
        if (considerMonth)
        {
            tmpStudent->activateFilter(StudentTemplate::AGE_FILTER, QString::number(i));
            strFirst.push_back(QString::number(tmpStudent->runTableFilter()));
        }
        else
        {
            tmpStudent->activateFilter(StudentTemplate::AGE_FILTER_C, QString::number(i));
            strFirst.push_back(QString::number(tmpStudent->runTableFilter()));
        }

        //SECOND LINE FILL
        tmpStudent->activateFilter(StudentTemplate::STATUS_FILTER, "orphan");
        tmpStudent->activateFilter(StudentTemplate::STATUS_FILTER, StudentTemplate::OR_MODE, "guardians");

        strSecond.push_back(QString::number(tmpStudent->runTableFilter()));

        //THIRD LINE FILL
        if (QDate::currentDate().month() < 7)
        {
            tmpStudent->activateFilter(StudentTemplate::COURSE_FILTER, "4");
            strThird.push_back(QString::number(tmpStudent->runTableFilter()));
        }

        tmpStudent->deactivateFilters();
    }

    writer->insertRow(strFirst);
    writer->insertRow(strSecond);
    writer->insertRow(strThird);
}

void AgeDistributionReport::writeReport(QString fileName)
{
    writer->writeTableFile(fileName);
}
