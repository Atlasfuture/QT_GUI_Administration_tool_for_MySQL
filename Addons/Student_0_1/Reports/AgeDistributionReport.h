#ifndef AGEDISTRIBUTIONREPORT_H

#define AGEDISTRIBUTIONREPORT_H

#include <memory>

#include "Addons/Student_0_1/StudentTemplate.h"

#include "Core/Writer/TableWriter.h"

#include <QSqlDatabase>





class AgeDistributionReport
{
    std::shared_ptr<StudentTemplate> tmpStudent;

    std::shared_ptr<TableWriter> writer;

    void designTable();

    void fillTable();

    bool considerMonth;
public:
    AgeDistributionReport(QSqlDatabase database, bool _considerMonth);


    void writeReport(QString fileName);
};










#endif
