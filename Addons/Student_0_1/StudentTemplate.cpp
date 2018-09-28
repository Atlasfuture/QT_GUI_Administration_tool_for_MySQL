#include "StudentTemplate.h"
#include <QApplication>
#include <QSqlRecord>
#include <QMessageBox>
#include <QDebug>
#include "Core/QSqlRelationalLib/Headers/DataDump.h"




StudentTemplate::StudentTemplate(std::shared_ptr<QSqlRelationalDataModel> _sqlDataModel)
{
    allStudentsShow = false;

    sqlDataModel = _sqlDataModel;

    installBaseTemplate();

    //init table query

    filterQuery = "";

    selectQuery = "SELECT id_student, surname, student.name, middle_name FROM student";



    //main filters map
    mapValueFilter[GROUP_FILTER] = "";
    mapJoin[GROUP_FILTER] = "";


    mapValueFilter[COURSE_FILTER] = "";
    mapJoin[COURSE] = "";

    mapValueFilter[GENDER_FILTER] = "";
    mapValueFilter[AGE_FILTER] = "";
    mapValueFilter[AGE_FROM_FILTER] = "";
    mapValueFilter[AGE_TO_FILTER] = "";
    mapValueFilter[GRADUATION_FILTER] = "";
    mapValueFilter[STATUS_FILTER] = "";
    mapValueFilter[NAME_FILTER] = "";
    mapValueFilter[SURNAME_FILTER] = "";
    mapValueFilter[MIDNAME_FILTER] = "";

    //address filters map
    mapValueFilter[COUNTRY_FILTER] = "";
    mapJoin[COUNTRY_FILTER] = "";

    mapValueFilter[REGION_FILTER] = "";
    mapJoin[REGION_FILTER] = "";

    mapValueFilter[LOCALITY_FILTER] = "";
    mapJoin[LOCALITY_FILTER] = "";

    mapValueFilter[LOCTYPE_FILTER] = "";
    mapJoin[LOCTYPE_FILTER] = "";

    mapValueFilter[DISTRICT_FILTER] = "";
    mapJoin[DISTRICT_FILTER] = "";

    mapValueFilter[STREET_FILTER] = "";
    mapJoin[STREET_FILTER] = "";


    //activate mode map
    mapMode[AND_MODE] = " and ";
    mapMode[OR_MODE] = " or ";

    //activate all students
    mapValueFilter[ALL_STUDENTS] = "";

    mapValueFilter[AGE_FILTER_C] = "";
    mapValueFilter[AGE_FROM_FILTER_C] = "";
    mapValueFilter[AGE_TO_FILTER_C] = "";
}



QList<QPair<QString, QString>> StudentTemplate::makePairList(int first, int second)
{
    QList<QPair<QString, QString>> listPairs;


    for (int i = 0; i < sqlQueryModel.rowCount(); i++)
    {
        QString id = sqlQueryModel.record(i).value(first).toString();
        QString name = sqlQueryModel.record(i).value(second).toString();

        auto tupleCountry = qMakePair(id, name);


        listPairs.push_back(tupleCountry);
    }

    return listPairs;
}


void StudentTemplate::installBaseTemplate()
{
    sqlQueryModel.setQuery("show databases");

    QStringList strListDatabases;

    for (int i = 0; i < sqlQueryModel.rowCount(); i++)
    {
        strListDatabases << sqlQueryModel.record(i).value(0).toString();
    }


    if (!strListDatabases.contains("lb_student_tmp"))
    {
        sqlDataModel->createDataBase("lb_student_tmp");

        sqlDataModel->setDatabase("lb_student_tmp");

        QString strDirPath = QApplication::applicationDirPath() + "/TemplateDumps/lb_student_tmp.sql";

        qDebug() << strDirPath;

        sqlDataModel->readDump(strDirPath);
    }

    sqlDataModel->setDatabase("lb_student_tmp");
}

bool StudentTemplate::checkOperationResult()
{
    if(!sqlQueryModel.lastError().isValid())
    {
        return true;
    }
    else
    {
        return false;
    }
}


std::shared_ptr<QSqlRelationalDataModel> StudentTemplate::getModel()
{
    return sqlDataModel;
}

void StudentTemplate::refreshCurrentSelection()
{
    sqlDataModel->queryExec(selectQuery);
}



//DEPARTMENT---------------------------------------------------------------------------------------------

bool StudentTemplate::createDepartment(QString fullName, QString shortName, QString code)
{
    QString tmpStr = "INSERT INTO department (short_name, full_name, code) values";
    tmpStr += " ('" + shortName + "', '" + fullName + "', '" + code + "')";



    sqlQueryModel.setQuery(tmpStr, sqlDataModel->database());

    return checkOperationResult();
}

bool StudentTemplate::updateDepartment(QString departmentID, QString fullName, QString shortName, QString code)
{
    QString tmpStr = "UPDATE department SET short_name = '" + shortName + "', full_name = '" + fullName + "', code = '" + code + "'";
    tmpStr += " WHERE id_department = " + departmentID;


    sqlQueryModel.setQuery(tmpStr, sqlDataModel->database());

    return checkOperationResult();
}

QList<QPair<QString, QString>> StudentTemplate::getDepartments()
{
    sqlQueryModel.setQuery("SELECT * FROM department", sqlDataModel->database());


    return makePairList(0, 1);
}


bool StudentTemplate::deleteDepartment(QString departmentID, bool deleteGroups, bool deleteStudents)
{
    QString strTmp;

    if (deleteGroups)
    {
        if (deleteStudents)
        {
            strTmp = "DELETE number FROM number JOIN student on (number.id_student = student.id_student)";
            strTmp += " JOIN groups on (student.id_group = groups.id_group) join department on ";
            strTmp += "(department.id_department = groups.id_department) WHERE department.id_department = '" + departmentID + "'";

            sqlQueryModel.setQuery(strTmp, sqlDataModel->database());

            strTmp = "DELETE student FROM student JOIN groups on";
            strTmp += " (student.id_group = groups.id_group) join department on";
            strTmp += " (department.id_department = groups.id_department) WHERE department.id_department = '" + departmentID + "'";

            sqlQueryModel.setQuery(strTmp, sqlDataModel->database());


            strTmp = "DELETE groups FROM groups JOIN department on ";
            strTmp += "(department.id_department = groups.id_department) WHERE department.id_department = '" + departmentID + "'";

            sqlQueryModel.setQuery(strTmp, sqlDataModel->database());
        }
        else
        {
            strTmp = "UPDATE student, groups, department SET student.id_group = NULL";
            strTmp += " WHERE groups.id_group = department.id_group and student.id_group = groups.id_group";
            strTmp += " and department.id_department = '" + departmentID + "'";


            sqlQueryModel.setQuery(strTmp, sqlDataModel->database());
        }

        strTmp = "DELETE FROM groups USING groups, department WHERE";
        strTmp += " groups.id_department = department.id_department";
        strTmp += " and department.id_department = '"+ departmentID + "'";

        sqlQueryModel.setQuery(strTmp, sqlDataModel->database());
    }
    else
    {
        strTmp += "UPDATE groups, department SET groups.id_department = NULL";
        strTmp += " WHERE groups.id_department = department.id_department and department.id_department = '" + departmentID + "'";

        sqlQueryModel.setQuery(strTmp, sqlDataModel->database());
    }


    strTmp = "DELETE FROM department WHERE id_department = '" + departmentID + "'";


    sqlQueryModel.setQuery(strTmp, sqlDataModel->database());

    return checkOperationResult();
}

QString StudentTemplate::getDepartmentCode(QString departmentID)
{
    QString strTmp = "SELECT code FROM department WHERE id_department = " + departmentID;

    sqlQueryModel.setQuery(strTmp);

    return sqlQueryModel.record(0).value(0).toString();
}

QString StudentTemplate::getDepartmentFullName(QString departmentID)
{
    QString strTmp = "SELECT full_name FROM department WHERE id_department = " + departmentID;

    sqlQueryModel.setQuery(strTmp);

    return sqlQueryModel.record(0).value(0).toString();
}

QString StudentTemplate::getDepartmentShortName(QString departmentID)
{
    QString strTmp = "SELECT short_name FROM department WHERE id_department = " + departmentID;

    sqlQueryModel.setQuery(strTmp);

    return sqlQueryModel.record(0).value(0).toString();
}



QString StudentTemplate::getDepartmentByGroup(QString groupId)
{
    QString strTmp = "SELECT department.short_name FROM department JOIN";
    strTmp += " groups on(groups.id_department = department.id_department)";
    strTmp += "  WHERE groups.id_group = " + groupId;

    sqlQueryModel.setQuery(strTmp, sqlDataModel->database());


    return sqlQueryModel.record(0).value(0).toString();
}

//COURSES------------------------------------------------------------------------------------------------

QStringList StudentTemplate::getCourses()
{
    sqlQueryModel.setQuery("SELECT id_course FROM course", sqlDataModel->database());

    QStringList strList;

    for (int i = 0; i < sqlQueryModel.rowCount(); i++)
    {
        strList.push_back(sqlQueryModel.record(i).value(0).toString());
    }

    return strList;
}

bool StudentTemplate::createCourse(QString courseId, QString code)
{
    QString strTmp = "INSERT INTO course (id_course, code) VALUES";
    strTmp += " (" + courseId + ", '" + code + "')";

    sqlQueryModel.setQuery(strTmp, sqlDataModel->database());

    return checkOperationResult();
}

bool StudentTemplate::deleteCourses(QString courseId, bool deleteGroups, bool deleteStudents)
{
    QString strTmp;

    if (deleteGroups)
    {
        if (deleteStudents)
        {
            strTmp = "DELETE FROM number, student USING number, groups, student, course WHERE";
            strTmp += " number.id_student = student.id_student and";
            strTmp += " student.id_group = groups.id_group ";
            strTmp += " and groups.id_course = " + courseId;


            sqlQueryModel.setQuery(strTmp, sqlDataModel->database());
        }
        else
        {
            strTmp = "UPDATE student, groups, department SET student.id_group = NULL";
            strTmp += " WHERE student.id_group = groups.id_group";
            strTmp += " and groups.id_course = " + courseId;

            sqlQueryModel.setQuery(strTmp, sqlDataModel->database());
        }

        strTmp = "DELETE FROM groups WHERE id_course = " + courseId;

        sqlQueryModel.setQuery(strTmp, sqlDataModel->database());
    }
    else
    {
        strTmp += "UPDATE groups SET groups.id_course = NULL WHERE id_course = " + courseId;

        sqlQueryModel.setQuery(strTmp, sqlDataModel->database());
    }


    strTmp = "DELETE FROM course WHERE id_course = '" +  courseId + "'";


    sqlQueryModel.setQuery(strTmp, sqlDataModel->database());

    return checkOperationResult();
}


QString StudentTemplate::getCourseByGroup(QString groupId)
{
    QString strTmp = "SELECT id_course FROM groups WHERE groups.id_group = " + groupId;

    sqlQueryModel.setQuery(strTmp, sqlDataModel->database());


    return QString::number(sqlQueryModel.record(0).value(0).toInt());
}


//GROUPS---------------------------------------------------------------------------------------------------

QString StudentTemplate::getGroupNameById(QString groupId)
{
    QString strTmp = "SELECT name FROM groups WHERE groups.id_group = " + groupId;

    sqlQueryModel.setQuery(strTmp, sqlDataModel->database());

    return sqlQueryModel.record(0).value(0).toString();
}

QList<QPair<QString, QString> > StudentTemplate::getGroups()
{ 
    sqlQueryModel.setQuery("SELECT * FROM groups", sqlDataModel->database());

    return makePairList(0, 1);
}

QList<QPair<QString, QString> > StudentTemplate::getGroups(QString id, StudentTemplate::GroupFilter filter)
{
    QString tmpStr;

    switch(filter)
    {
    case DEPARTMENT:
        tmpStr = "SELECT * FROM groups WHERE id_department = '" + id + "'";
    break;

    case COURSE:
        tmpStr = "SELECT * FROM groups WHERE id_course = '" + id + "'";
    break;
    }

    sqlQueryModel.setQuery(tmpStr, sqlDataModel->database());


    return makePairList(0, 1);
}

QList<QPair<QString, QString>> StudentTemplate::getGroups(QString departmentId, QString course)
{



    QString tmpStr = "SELECT * FROM groups WHERE id_department";

    if (departmentId == "NULL")
    {
        tmpStr += " is NULL ";
    }
    else
    {
        tmpStr += " = " + departmentId;
    }

    tmpStr += " and id_course";

    if (course == "NULL")
    {
        tmpStr += " is NULL";
    }
    else
    {
        tmpStr += " = " + course;
    }



    sqlQueryModel.setQuery(tmpStr, sqlDataModel->database());

    return makePairList(0, 1);
}

QList<QPair<QString, QString>> StudentTemplate::getAloneGroups(bool emptyDepartment, bool emptyCourse)
{
    QString tmpStr = "SELECT * FROM groups WHERE";

    if(emptyCourse && emptyDepartment)
    {
        tmpStr += " id_department is NULL and id_course is NULL";
    }
    else
    if (emptyDepartment)
    {
        tmpStr += " id_department is NULL";
    }
    else
    if (emptyCourse)
    {
        tmpStr += " id_course is NULL";
    }

    sqlQueryModel.setQuery(tmpStr);

    return makePairList(0, 1);
}

bool StudentTemplate::createGroup(QString groupName, QString course, QString departmentId)
{
    QString tmpStr = "INSERT INTO groups (name, id_department, id_course) values ('";
    tmpStr += groupName + "', '" + departmentId + "', '" + course + "')";

    sqlQueryModel.setQuery(tmpStr, sqlDataModel->database());

    return !sqlQueryModel.lastError().isValid();
}

bool StudentTemplate::updateGroup(QString groupID, QString groupName, QString course, QString departmentID)
{
    QString tmpStr = "UPDATE groups SET name = '" + groupName + "'";
    tmpStr += ", id_department = '" + departmentID + "'";
    tmpStr += ", id_course = '" + course + "'";
    tmpStr += " WHERE id_group = '" + groupID + "'";



    sqlQueryModel.setQuery(tmpStr, sqlDataModel->database());

    return !sqlQueryModel.lastError().isValid();
}

bool StudentTemplate::deleteGroup(QString groupID, bool deleteStudents)
{
    QString tmpStr;

    if (deleteStudents)
    {
        tmpStr = "DELETE FROM number, student USING number, student, groups WHERE number.id_student = student.id_student and student.id_group = groups.id_group and groups.id_group = '" + groupID + "'";
    }
    else
    {
        tmpStr = "UPDATE student, groups SET student.id_group = NULL WHERE student.id_group = groups.id_group and groups.id_group = '" + groupID + "'";
    }

    sqlQueryModel.setQuery(tmpStr, sqlDataModel->database());

    tmpStr = "DELETE FROM groups WHERE id_group = '" + groupID + "'";

    sqlQueryModel.setQuery(tmpStr, sqlDataModel->database());

    return checkOperationResult();
}



//STUDENT--------------------------------------------------------------------------------------------------------

bool StudentTemplate::createStudent(Student st)
{
    QString strTmp = "INSERT INTO student (surname, name, middle_name, gender, status, birth_date, school_graduation, id_address, id_group) VALUES (";

    strTmp += "'" + st.surname + "', ";
    strTmp += "'" + st.name + "', ";
    strTmp += "'" + st.middleName + "', ";
    strTmp += "'" + st.gender + "', ";
    strTmp += "'" + st.status + "', ";
    strTmp += "'" + st.birthdate + "', ";
    strTmp += "'" + st.schoolGraduation + "', ";
    strTmp += "NULL, ";

    QString strGroupId = "SELECT id_group FROM groups WHERE name = '" + st.group + "'";

    sqlQueryModel.setQuery(strGroupId, sqlDataModel->database());

    strGroupId = sqlQueryModel.record(0).value(0).toString();

    strTmp += "'" + strGroupId + "')";


    sqlQueryModel.setQuery(strTmp, sqlDataModel->database());

    return checkOperationResult();
}

bool StudentTemplate::createStudent(Student st, Address adr)
{
    createAddress(adr);

    QString strTmp = "INSERT INTO student (surname, name, middle_name, gender, family_status, birth_date, school_graduation, id_address, id_group) VALUES (";

    strTmp += "'" + st.surname + "', ";
    strTmp += "'" + st.name + "', ";
    strTmp += "'" + st.middleName + "', ";
    strTmp += "'" + st.gender + "', ";

    if (st.status == "NULL")
    {
        strTmp += "NULL, ";
    }
    else
    {
        strTmp += "'" + st.status + "', ";
    }

    strTmp += "'" + st.birthdate + "', ";
    strTmp += "'" + st.schoolGraduation + "', ";
    strTmp += "'" + getAdressId(adr) + "', ";
    strTmp += "'" + st.group + "')";


    qDebug() << "INSERT STUDENT QUERY: " << strTmp;

    sqlQueryModel.setQuery(strTmp, sqlDataModel->database());

    return checkOperationResult();
}

bool StudentTemplate::updateStudent(QString studentId, Student st)
{
    QString strTmp = "UPDATE student SET surname = '" + st.surname;
    strTmp += "', name = '" + st.name;
    strTmp += "', middle_name = '" + st.middleName;
    strTmp += "', gender = '" + st.gender;

    if (st.status == "NULL")
    {
        strTmp += "', family_status = NULL";

    }
    else
    {
        strTmp += "', family_status = '" + st.status + "'";
    }

    strTmp += ", birth_date = '" + st.birthdate;
    strTmp += "', school_graduation = '" + st.schoolGraduation;

    QString strGroup = "', id_group ";

    if (st.group == "NULL")
    {
        strTmp += "', id_group = " + st.group;
    }
    else
    {
        strTmp += "', id_group = '" + st.group + "'";
    }

    strTmp += " WHERE id_student = " + studentId;



    sqlQueryModel.setQuery(strTmp, sqlDataModel->database());

    return checkOperationResult();
}

void StudentTemplate::deleteStudent(QString idStudent)
{
    QString tmpStr = "DELETE FROM number WHERE id_student = " + idStudent;

    sqlQueryModel.setQuery(tmpStr, sqlDataModel->database());

    tmpStr = "DELETE FROM student WHERE id_student = '" + idStudent + "'";

    sqlQueryModel.setQuery(tmpStr, sqlDataModel->database());

    refreshCurrentSelection();
}

Student StudentTemplate::getStudentData(QString studentId)
{
    QString strTmp = "SELECT * FROM student WHERE id_student = " + studentId;

    sqlQueryModel.setQuery(strTmp, sqlDataModel->database());

    QSqlRecord sqlRec = sqlQueryModel.record(0);

    Student tmpStudent;
    tmpStudent.surname = sqlRec.value(1).toString();
    tmpStudent.name = sqlRec.value(2).toString();
    tmpStudent.middleName = sqlRec.value(3).toString();
    tmpStudent.gender = sqlRec.value(4).toString();
    tmpStudent.status = sqlRec.value(5).toString();
    tmpStudent.birthdate = sqlRec.value(6).toString();
    tmpStudent.schoolGraduation = sqlRec.value(7).toString();
    tmpStudent.address = sqlRec.value(8).toString();
    tmpStudent.group = sqlRec.value(9).toString();

    return tmpStudent;
}


QString StudentTemplate::getStudentId(Student st)
{
    QString strTmp = "SELECT id_student FROM student WHERE";
    strTmp += " surname = '" + st.surname + "' and";
    strTmp += " name = '" + st.name + "' and";
    strTmp += " middle_name = '" + st.middleName + "' and";
    strTmp += " gender = '" + st.gender + "' and";

    if (st.status == "NULL")
    {
        strTmp += " family_status is NULL and ";
    }
    else
    {
        strTmp += " family_status = '" + st.status + "' and";
    }

    strTmp += " birth_date = '" + st.birthdate + "' and";
    strTmp += " school_graduation = '" + st.schoolGraduation + "'";

    sqlQueryModel.setQuery(strTmp, sqlDataModel->database());


    return sqlQueryModel.record(0).value(0).toString();
}

int StudentTemplate::countAllStudents()
{
    QString strTmp = "SELECT count(*) FROM student";

    sqlDataModel->queryExec(strTmp);

    return sqlDataModel->record(0).value(0).toInt();
}




void StudentTemplate::activateAllStudents()
{
    allStudentsShow = true;
}

void StudentTemplate::deactivateAllStudents()
{
    allStudentsShow = false;
}




//ADDRESS---------------------------------------------------------------------------------------------------------

bool StudentTemplate::updateAdressObject(StudentTemplate::ADDRESS address, QString objectID, QString newName)
{
    QString strTmp = "UPDATE ";
    QString strTmpWhere = " WHERE ";

    switch (address)
    {
        case COUNTRY:
            strTmp += "country ";
            strTmpWhere += "id_country = ";
        break;

        case REGION:
            strTmp += "region ";
            strTmpWhere += "id_region = ";
        break;

        case DISTRICT:
            strTmp += "district ";
            strTmpWhere += "id_district = ";
        break;

        case STREET:
            strTmp += "street ";
            strTmpWhere += "id_street = ";
        break;
    }

    strTmp += "SET name = '" + newName + "'" + strTmpWhere + objectID;

    qDebug() << "UPDATE ADDRESS OBJECT: " << strTmp;

    sqlQueryModel.setQuery(strTmp, sqlDataModel->database());

    return checkOperationResult();
}

QList<QPair<QString, QString>> StudentTemplate::getCountries()
{
    QString tmpStr = "SELECT * FROM country";

    sqlQueryModel.setQuery(tmpStr, sqlDataModel->database());

    return makePairList(0, 1);
}

bool StudentTemplate::addCountry(QString name)
{
    QString tmpStr = "INSERT INTO country (name) VALUES ('" + name + "')";

    qDebug() << tmpStr;

    sqlQueryModel.setQuery(tmpStr, sqlDataModel->database());

    return checkOperationResult();
}


bool StudentTemplate::deleteRecCountry(QString countryId)
{
    QString tmpStr = "DELETE FROM country WHERE id_country = '" + countryId + "'";

    sqlQueryModel.setQuery(tmpStr, sqlDataModel->database());

    return checkOperationResult();
}

QList<QPair<QString, QString>> StudentTemplate::getRegions(QString countryId)
{
    QString strTmp = "SELECT * FROM region WHERE id_country = " + countryId;

    sqlQueryModel.setQuery(strTmp, sqlDataModel->database());

    return makePairList(0, 1);
}

bool StudentTemplate::addRegion(QString name, QString countryId)
{
    QString strTmp = "INSERT INTO region (name, id_country) VALUES ('" + name + "', '" + countryId + "')";

    sqlQueryModel.setQuery(strTmp, sqlDataModel->database());

    if (!sqlQueryModel.lastError().isValid())
    {
        return true;
    }
    else
    {
        return false;
    }
}




bool StudentTemplate::deleteRecRegions(QString regionId)
{
    QString tmpStr = "DELETE FROM region WHERE id_region = '" + regionId + "'";

    sqlQueryModel.setQuery(tmpStr, sqlDataModel->database());

    if (!sqlQueryModel.lastError().isValid())
    {
        return true;
    }
    else
    {
        return false;
    }
}

QList<QPair<QString, QString> > StudentTemplate::getLocalities(QString regionId)
{
    QString strTmp = "SELECT * FROM locality WHERE id_region = " + regionId;

    sqlQueryModel.setQuery(strTmp, sqlDataModel->database());

    return makePairList(0, 2);
}

bool StudentTemplate::addLocality(QString name, QString type, QString regionId)
{
    QString strTmp = "INSERT INTO locality (name, type, id_region) VALUES ('" + name + "', '" + type + "', '" + regionId + "')";

    sqlQueryModel.setQuery(strTmp, sqlDataModel->database());

    if (!sqlQueryModel.lastError().isValid())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool StudentTemplate::updateLocalityName(QString localityID, QString newName, StudentTemplate::LOCALITY_TYPE type)
{
    QString strTmp = "UPDATE locality SET name = '" + newName + "', type = ";

    switch (type)
    {
        case CITY:
            strTmp += "'city' ";
        break;

        case VILLAGE:
            strTmp += "'village' ";
        break;

        case TOWNSHIP:
            strTmp += "'township' ";
        break;
    }

    strTmp += "WHERE id_locality = " + localityID;

    qDebug() << "UPDATE LOCALITY QUERY: " << strTmp;

    sqlQueryModel.setQuery(strTmp, sqlDataModel->database());

    return checkOperationResult();
}


bool StudentTemplate::deleteRecLocality(QString localityId)
{
    QString tmpStr = "DELETE FROM locality WHERE id_locality = '" + localityId + "'";

    sqlQueryModel.setQuery(tmpStr, sqlDataModel->database());

    if (!sqlQueryModel.lastError().isValid())
    {
        return true;
    }
    else
    {
        return false;
    }
}

QList<QPair<QString, QString> > StudentTemplate::getDistricts(QString localityId)
{
    QString strTmp = "SELECT * FROM district WHERE id_locality = " + localityId;

    sqlQueryModel.setQuery(strTmp, sqlDataModel->database());

    return makePairList(0, 1);
}

bool StudentTemplate::addDistrict(QString name, QString localityId)
{
    QString strTmp = "INSERT INTO district (name, id_locality) VALUES ('" + name + "', '" + localityId + "')";

    sqlQueryModel.setQuery(strTmp, sqlDataModel->database());

    if (!sqlQueryModel.lastError().isValid())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool StudentTemplate::deleteRecDistrict(QString districtId)
{
    QString tmpStr = "DELETE FROM district WHERE id_district = '" + districtId + "'";

    sqlQueryModel.setQuery(tmpStr, sqlDataModel->database());

    if (!sqlQueryModel.lastError().isValid())
    {
        return true;
    }
    else
    {
        return false;
    }
}

QList<QPair<QString, QString> > StudentTemplate::getStreets(QString districtId)
{
    QString strTmp = "SELECT * FROM street WHERE id_district = " + districtId;

    sqlQueryModel.setQuery(strTmp, sqlDataModel->database());

    return makePairList(0, 1);
}

bool StudentTemplate::addStreet(QString name, QString districtId)
{
    QString strTmp = "INSERT INTO street (name, id_district) VALUES ('" + name + "', '" + districtId + "')";

    sqlQueryModel.setQuery(strTmp, sqlDataModel->database());

    if (!sqlQueryModel.lastError().isValid())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool StudentTemplate::deleteRecStreet(QString streetId)
{
    QString tmpStr = "DELETE FROM street WHERE id_street = '" + streetId + "'";

    sqlQueryModel.setQuery(tmpStr, sqlDataModel->database());

    if (!sqlQueryModel.lastError().isValid())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool StudentTemplate::createAddress(Address adr)
{
   QString strTmp = "INSERT INTO address (street, house, flat) VALUES (";

   QString strStreetId = "SELECT id_street FROM street JOIN district on (street.id_district = district.id_district)";
   strStreetId += " JOIN locality on (district.id_locality = locality.id_locality)";
   strStreetId += " JOIN region on (locality.id_region = region.id_region)";
   strStreetId += " JOIN country on (region.id_country = country.id_country)";
   strStreetId += " WHERE country.name = '" + adr.country + "' and ";
   strStreetId += " region.name = '" + adr.region + "' and";
   strStreetId += " locality.name = '" + adr.locality + "' and";
   strStreetId += " district.name = '" + adr.district + "' and";
   strStreetId += " street.name = '" + adr.street + "'";


   sqlQueryModel.setQuery(strStreetId, sqlDataModel->database());

   strStreetId = sqlQueryModel.record(0).value(0).toString();

   strTmp += "'" + strStreetId + "', ";
   strTmp += "'" + adr.house + "', ";
   strTmp += "'" + adr.flat + "')";


   sqlQueryModel.setQuery(strTmp, sqlDataModel->database());

   if (!sqlQueryModel.lastError().isValid())
   {
       return true;
   }
   else
   {
       return false;
   }
}

bool StudentTemplate::updateAddress(QString studentId, Address adr)
{
    QString strAddressId = "SELECT id_address FROM student WHERE id_student = " + studentId;

    sqlQueryModel.setQuery(strAddressId, sqlDataModel->database());

    strAddressId = sqlQueryModel.record(0).value(0).toString();

    QString strStreetId = "SELECT id_street FROM street JOIN district on (street.id_district = district.id_district)";
    strStreetId += " JOIN locality on (district.id_locality = locality.id_locality)";
    strStreetId += " JOIN region on (locality.id_region = region.id_region)";
    strStreetId += " JOIN country on (region.id_country = country.id_country)";
    strStreetId += " WHERE country.name = '" + adr.country + "'";
    strStreetId += " and region.name = '" + adr.region + "'";
    strStreetId += " and locality.name = '" + adr.locality + "'";
    strStreetId += " and district.name = '" + adr.district + "'";
    strStreetId += " and street.name = '" + adr.street + "'";


    sqlQueryModel.setQuery(strStreetId, sqlDataModel->database());

    strStreetId = sqlQueryModel.record(0).value(0).toString();


    QString strTmp = "UPDATE address SET";
    strTmp += " house = " + adr.house;
    strTmp += " ,flat = " + adr.flat;
    strTmp += " ,street = " + strStreetId;
    strTmp += " WHERE id_address = " + strAddressId;


    sqlQueryModel.setQuery(strTmp, sqlDataModel->database());

    if (!sqlQueryModel.lastError().isValid())
    {
        return true;
    }
    else
    {
        return false;
    }
}

Address StudentTemplate::getAddressByStudent(QString studentId)
{
    QString strTmp = "SELECT country.name, region.name, locality.name, district.name, street.name, address.house, address.flat";
    strTmp += " FROM student JOIN address on(student.id_address = address.id_address)";
    strTmp += " JOIN street on(address.street = street.id_street)";
    strTmp += " JOIN district on(street.id_district = district.id_district)";
    strTmp += " JOIN locality on(district.id_locality = locality.id_locality)";
    strTmp += " JOIN region on(locality.id_region = region.id_region)";
    strTmp += " JOIN country on(region.id_country = country.id_country)";
    strTmp += " WHERE student.id_student = " + studentId;


    qDebug() << "GET ADDRESS QUERY: " << strTmp;

    sqlQueryModel.setQuery(strTmp, sqlDataModel->database());

    QSqlRecord sqlRec = sqlQueryModel.record(0);

    Address address;
    address.country = sqlRec.value(0).toString();
    address.region = sqlRec.value(1).toString();
    address.locality = sqlRec.value(2).toString();
    address.district = sqlRec.value(3).toString();
    address.street = sqlRec.value(4).toString();
    address.house = sqlRec.value(5).toString();
    address.flat = sqlRec.value(6).toString();

    return address;
}

QString StudentTemplate::getAdressId(Address adr)
{
    QString strTmp = "SELECT id_address FROM address JOIN street on (address.street = street.id_street)";
    strTmp += " JOIN district on (street.id_district = district.id_district)";
    strTmp += " JOIN locality on (district.id_locality = locality.id_locality)";
    strTmp += " JOIN region on (locality.id_region = region.id_region)";
    strTmp += " JOIN country on (region.id_country = country.id_country)";
    strTmp += " WHERE country.name = '" + adr.country + "' and ";
    strTmp += " region.name = '" + adr.region + "' and";
    strTmp += " locality.name = '" + adr.locality + "' and";
    strTmp += " district.name = '" + adr.district + "' and";
    strTmp += " street.name = '" + adr.street + "' and";
    strTmp += " address.house = '" + adr.house + "' and";
    strTmp += " address.flat = '" + adr.flat + "'";

    sqlQueryModel.setQuery(strTmp, sqlDataModel->database());

    return sqlQueryModel.record(0).value(0).toString();
}

//PHONE METHODS
void StudentTemplate::createPhone(Phone phone, Student stTmp)
{
    QString strTmp = "INSERT INTO number (number, type, id_student) VALUES ('" + phone.phoneNumber + "', '";
    strTmp +=  phone.phoneType + "', '";
    strTmp +=  getStudentId(stTmp) + "')";


    sqlQueryModel.setQuery(strTmp, sqlDataModel->database());
}

bool StudentTemplate::updatePhone(Phone phone, QString studentId)
{
    QString strTmp = "UPDATE number SET number = " + phone.phoneNumber + ", type = '" + phone.phoneType;
    strTmp += "' WHERE id_student = " + studentId;

    sqlQueryModel.setQuery(strTmp, sqlDataModel->database());

    if (!sqlQueryModel.lastError().isValid())
    {
        return true;
    }
    else
    {
        return false;
    }
}


Phone StudentTemplate::getPhoneByStudent(QString studentId)
{
    QString strTmp = "SELECT * FROM number JOIN student on(number.id_student = student.id_student) WHERE student.id_student = " + studentId;

    sqlQueryModel.setQuery(strTmp, sqlDataModel->database());

    QSqlRecord sqlRec = sqlQueryModel.record(0);

    Phone phone;

    phone.phoneNumber = sqlRec.value(1).toString();
    phone.phoneType = sqlRec.value(2).toString();

    return phone;
}


//FILTERS-----------------------------------------------------------------------------------------------------

int StudentTemplate::runTableFilter()
{
    QString tmpFilter = selectQuery;

    if (!allStudentsShow)
    {
        bool filterUsed = false;

        for (int i = 0; i < mapJoin.size(); i++)
        {
            if (!tmpFilter.contains(mapJoin[i]))
            {
                tmpFilter += mapJoin[i];
            }
        }

        for (int i = 0; i < mapValueFilter.size(); i++)
        {
            if (filterUsed)
            {
                if (mapValueFilter[i] != "")
                {
                    tmpFilter += " and (" + mapValueFilter[i] + ") ";
                }
            }
            else
            {
                if (mapValueFilter[i] != "")
                {
                    tmpFilter += " WHERE (" + mapValueFilter[i] + ") ";
                    filterUsed = true;
                }
            }
        }

    }


    qDebug() << "FILTER: " << tmpFilter;

    sqlDataModel->queryExec(tmpFilter);

    return sqlDataModel->rowCount();
}




void StudentTemplate::activateFilter(StudentTemplate::STUDENT_FILTER filter, QString value)
{
    QString strTmp;

    int intValue;




    switch (filter)
    {
        case GROUP_FILTER:

            if (value != "NULL")
            {
                value = " = '" + value + "'";
            }
            else
            {
                value = " is NULL";
            }

            mapValueFilter[GROUP_FILTER] = " student.id_group " + value;

            currentGroup = value;

        break;

        case COURSE:
            mapValueFilter[COURSE] = " groups.id_course = " + value;

            mapJoin[COURSE] = " JOIN groups on (student.id_group = groups.id_group)";

        break;

        case GENDER_FILTER:

            mapValueFilter[GENDER_FILTER] = " gender = '" + value + "'";

        break;

        case AGE_FILTER:
            mapValueFilter[AGE_FILTER] = " ((YEAR( CURRENT_DATE ) - YEAR( birth_date )) - ";
            mapValueFilter[AGE_FILTER] += " ( RIGHT( CURRENT_DATE, 5 ) < RIGHT( birth_date, 5 ))) = " + value;
        break;

        case AGE_FILTER_C:
            mapValueFilter[AGE_FILTER_C] = " (YEAR( CURRENT_DATE ) - YEAR( birth_date ))  = " + value;
        break;

        case AGE_FROM_FILTER:

            intValue = value.toInt();
            intValue--;

            value = QString::number(intValue);


        mapValueFilter[AGE_FROM_FILTER] = " ((YEAR( CURRENT_DATE ) - YEAR( birth_date )) - ";
        mapValueFilter[AGE_FROM_FILTER] += "( RIGHT( CURRENT_DATE, 5 ) < RIGHT( birth_date, 5 ))) > " + value;

        break;

        case AGE_FROM_FILTER_C:
            intValue = value.toInt();
            intValue--;

            value = QString::number(intValue);
            mapValueFilter[AGE_FROM_FILTER_C] = " (YEAR( CURRENT_DATE ) - YEAR( birth_date )) > " + value;

        break;


        case AGE_TO_FILTER:

            intValue = value.toInt();
            intValue++;

            value = QString::number(intValue);


            mapValueFilter[AGE_TO_FILTER] = " ((YEAR( CURRENT_DATE ) - YEAR( birth_date )) - ";
            mapValueFilter[AGE_TO_FILTER] += "( RIGHT( CURRENT_DATE, 5 ) < RIGHT( birth_date, 5 ))) < " + value;

        break;


        case AGE_TO_FILTER_C:

            intValue = value.toInt();
            intValue++;

            value = QString::number(intValue);


            mapValueFilter[AGE_TO_FILTER_C] = " (YEAR( CURRENT_DATE ) - YEAR( birth_date ))  < " + value;
        break;

        case GRADUATION_FILTER:
            mapValueFilter[GRADUATION_FILTER] = " school_graduation = '" + value + "'";
        break;

        case STATUS_FILTER:
            mapValueFilter[STATUS_FILTER] = " family_status = '" + value + "'";
        break;

        case NAME_FILTER:
            mapValueFilter[NAME_FILTER] = " student.name like '%" + value + "%'";
        break;

        case SURNAME_FILTER:
            mapValueFilter[SURNAME_FILTER] = " student.surname like '%" + value + "%'";
        break;

        case MIDNAME_FILTER:
            mapValueFilter[MIDNAME_FILTER] = " student.middle_name like '%" + value + "%'";
        break;

        case COUNTRY_FILTER:
            mapValueFilter[COUNTRY_FILTER] = " country.id_country = '" + value + "'";

            mapJoin[COUNTRY_FILTER] = " JOIN address on (student.id_address = address.id_address) ";
            mapJoin[COUNTRY_FILTER] += " JOIN street on (address.street = street.id_street) ";
            mapJoin[COUNTRY_FILTER] += " JOIN district on (street.id_district = district.id_district) ";
            mapJoin[COUNTRY_FILTER] += " JOIN locality on (district.id_locality = locality.id_locality) ";
            mapJoin[COUNTRY_FILTER] += " JOIN region on (locality.id_region = region.id_region) ";
            mapJoin[COUNTRY_FILTER] += " JOIN country on (region.id_country = country.id_country) ";
        break;

        case REGION_FILTER:
            mapValueFilter[REGION_FILTER] = " region.id_region = '" + value + "'";

            mapJoin[REGION_FILTER] = " JOIN address on (student.id_address = address.id_address) ";
            mapJoin[REGION_FILTER] += " JOIN street on (address.street = street.id_street) ";
            mapJoin[REGION_FILTER] += " JOIN district on (street.id_district = district.id_district) ";
            mapJoin[REGION_FILTER] += " JOIN locality on (district.id_locality = locality.id_locality) ";
            mapJoin[REGION_FILTER] += " JOIN region on (locality.id_region = region.id_region) ";
        break;

        case LOCALITY_FILTER:
            mapValueFilter[LOCALITY_FILTER] = " locality.id_locality = '" + value + "'";

            mapJoin[LOCALITY_FILTER] = " JOIN address on (student.id_address = address.id_address) ";
            mapJoin[LOCALITY_FILTER] += " JOIN street on (address.street = street.id_street) ";
            mapJoin[LOCALITY_FILTER] += " JOIN district on (street.id_district = district.id_district) ";
            mapJoin[LOCALITY_FILTER] += " JOIN locality on (district.id_locality = locality.id_locality) ";
        break;

        case LOCTYPE_FILTER:
            mapValueFilter[LOCTYPE_FILTER] = " locality.type = '" + value + "'";

            mapJoin[LOCTYPE_FILTER] = " JOIN address on (student.id_address = address.id_address) ";
            mapJoin[LOCTYPE_FILTER] += " JOIN street on (address.street = street.id_street) ";
            mapJoin[LOCTYPE_FILTER] += " JOIN district on (street.id_district = district.id_district) ";
            mapJoin[LOCTYPE_FILTER] += " JOIN locality on (district.id_locality = locality.id_locality) ";
        break;

        case DISTRICT_FILTER:
            mapValueFilter[DISTRICT_FILTER] = " district.id_district = '" + value + "'";

            mapJoin[DISTRICT_FILTER] = " JOIN address on (student.id_address = address.id_address) ";
            mapJoin[DISTRICT_FILTER] += " JOIN street on (address.street = street.id_street) ";
            mapJoin[DISTRICT_FILTER] += " JOIN district on (street.id_district = district.id_district) ";

        break;

        case STREET_FILTER:
            mapValueFilter[STREET_FILTER] = " street.id_street = '" + value + "'";

            mapJoin[STREET_FILTER] = " JOIN address on (student.id_address = address.id_address) ";
            mapJoin[STREET_FILTER] += " JOIN street on (address.street = street.id_street) ";
        break;
    }

    if (!selectQuery.contains(strTmp))
    {
        selectQuery += strTmp;
    }
}

void StudentTemplate::activateFilter(StudentTemplate::STUDENT_FILTER filter, StudentTemplate::ACTIVATE_MODE mode, QString value)
{
    if (mapValueFilter[filter] != "")
    {
        mapValueFilter[filter] += mapMode[mode];
    }

    QString strTmp;


    int intValue;

    switch (filter)
    {
        case GROUP_FILTER:

            if (value != "NULL")
            {
                value = "'" + value + "'";
            }

            mapValueFilter[GROUP_FILTER] += " student.id_group = " + value;

            currentGroup = value;

        break;

        case COURSE_FILTER:
            mapValueFilter[COURSE] += " groups.id_course = " + value;

            mapJoin[COURSE] = " JOIN groups on (student.id_group = groups.id_group)";
        break;

        case GENDER_FILTER:

            mapValueFilter[GENDER_FILTER] += " gender = '" + value + "'";
        break;

        case AGE_FILTER:
            mapValueFilter[AGE_FILTER] += " ((YEAR( CURRENT_DATE ) - YEAR( birth_date )) - ";
            mapValueFilter[AGE_FILTER] += " ( RIGHT( CURRENT_DATE, 5 ) < RIGHT( birth_date, 5 ))) = " + value;
        break;

        case AGE_FILTER_C:
            mapValueFilter[AGE_FILTER_C] += " (YEAR( CURRENT_DATE ) - YEAR( birth_date ))  = " + value;
        break;

        case AGE_FROM_FILTER:

            intValue = value.toInt();
            intValue--;

            value = QString::number(intValue);


            mapValueFilter[AGE_FROM_FILTER] += " ((YEAR( CURRENT_DATE ) - YEAR( birth_date )) - ";
            mapValueFilter[AGE_FROM_FILTER] += "( RIGHT( CURRENT_DATE, 5 ) < RIGHT( birth_date, 5 ))) > " + value;
        break;

        case AGE_FROM_FILTER_C:
            intValue = value.toInt();
            intValue--;

            value = QString::number(intValue);
            mapValueFilter[AGE_FROM_FILTER_C] += " (YEAR( CURRENT_DATE ) - YEAR( birth_date )) > " + value;

        break;


        case AGE_TO_FILTER:

            intValue = value.toInt();
            intValue++;

            value = QString::number(intValue);


            mapValueFilter[AGE_TO_FILTER] = " ((YEAR( CURRENT_DATE ) - YEAR( birth_date )) - ";
            mapValueFilter[AGE_TO_FILTER] += "( RIGHT( CURRENT_DATE, 5 ) < RIGHT( birth_date, 5 ))) < " + value;
        break;


        case AGE_TO_FILTER_C:

            intValue = value.toInt();
            intValue++;

            value = QString::number(intValue);


            mapValueFilter[AGE_TO_FILTER_C] = " (YEAR( CURRENT_DATE ) - YEAR( birth_date ))  < " + value;
        break;

        case GRADUATION_FILTER:
            mapValueFilter[GRADUATION_FILTER] += " school_graduation = '" + value + "'";
        break;
        case STATUS_FILTER:
            mapValueFilter[STATUS_FILTER] += " family_status = '" + value + "'";
        break;

        case NAME_FILTER:
            mapValueFilter[NAME_FILTER] += " student.name like '%" + value + "%'";
        break;

        case SURNAME_FILTER:
            mapValueFilter[SURNAME_FILTER] += " student.surname like '%" + value + "%'";
        break;

        case MIDNAME_FILTER:
            mapValueFilter[MIDNAME_FILTER] += " student.middle_name like '%" + value + "%'";
        break;

        case COUNTRY_FILTER:
            mapValueFilter[COUNTRY_FILTER] += " country.id_country = '" + value + "'";

            mapJoin[COUNTRY_FILTER] = " JOIN address on (student.id_address = address.id_address) ";
            mapJoin[COUNTRY_FILTER] += " JOIN street on (address.street = street.id_street) ";
            mapJoin[COUNTRY_FILTER] += " JOIN district on (street.id_district = district.id_district) ";
            mapJoin[COUNTRY_FILTER] += " JOIN locality on (district.id_locality = locality.id_locality) ";
            mapJoin[COUNTRY_FILTER] += " JOIN region on (locality.id_region = region.id_region) ";
            mapJoin[COUNTRY_FILTER] += " JOIN country on (region.id_country = country.id_country) ";
        break;

        case REGION_FILTER:
            mapValueFilter[REGION_FILTER] += " region.id_region = '" + value + "'";

            mapJoin[REGION_FILTER] = " JOIN address on (student.id_address = address.id_address) ";
            mapJoin[REGION_FILTER] += " JOIN street on (address.street = street.id_street) ";
            mapJoin[REGION_FILTER] += " JOIN district on (street.id_district = district.id_district) ";
            mapJoin[REGION_FILTER] += " JOIN locality on (district.id_locality = locality.id_locality) ";
            mapJoin[REGION_FILTER] += " JOIN region on (locality.id_region = region.id_region) ";
        break;

        case LOCALITY_FILTER:
            mapValueFilter[LOCALITY_FILTER] += " locality.id_locality = '" + value + "'";

            mapJoin[LOCALITY_FILTER] = " JOIN address on (student.id_address = address.id_address) ";
            mapJoin[LOCALITY_FILTER] += " JOIN street on (address.id_street = street.id_street) ";
            mapJoin[LOCALITY_FILTER] += " JOIN district on (street.id_district = district.id_district) ";
            mapJoin[LOCALITY_FILTER] += " JOIN locality on (district.id_locality = locality.id_locality) ";
        break;

        case LOCTYPE_FILTER:
            mapValueFilter[LOCTYPE_FILTER] += " locality.type = '" + value + "'";

            mapJoin[LOCTYPE_FILTER] = " JOIN address on (student.id_address = address.id_address) ";
            mapJoin[LOCTYPE_FILTER] += " JOIN street on (address.street = street.id_street) ";
            mapJoin[LOCTYPE_FILTER] += " JOIN district on (street.id_district = district.id_district) ";
            mapJoin[LOCTYPE_FILTER] += " JOIN locality on (district.id_locality = locality.id_locality) ";
        break;

        case DISTRICT_FILTER:
            mapValueFilter[DISTRICT_FILTER] += " district.id_district = '" + value + "'";

            mapJoin[DISTRICT_FILTER] = " JOIN address on (student.id_address = address.id_address) ";
            mapJoin[DISTRICT_FILTER] += " JOIN street on (address.street = street.id_street) ";
            mapJoin[DISTRICT_FILTER] += " JOIN district on (street.id_district = district.id_district) ";
        break;

        case STREET_FILTER:
            mapValueFilter[STREET_FILTER] += " street.id_street = '" + value + "'";

            mapJoin[STREET_FILTER] = " JOIN address on (student.id_address = address.id_address) ";
            mapJoin[STREET_FILTER] += " JOIN street on (address.street = street.id_street) ";
        break;
    }

    if (!selectQuery.contains(strTmp))
    {
        selectQuery += strTmp;
    }
}

void StudentTemplate::deactivateFilter(StudentTemplate::STUDENT_FILTER filter)
{
    mapValueFilter[filter] = "";

    mapJoin[filter] = "";

}


void StudentTemplate::deactivateFilters()
{
    for (int i = 0; i < mapValueFilter.size(); i++)
    {
        mapValueFilter[i] = "";
    }
}


void StudentTemplate::writeData(QString fileName)
{
    DataDump dump(sqlDataModel.get());

    dump.writeDump(fileName, "lb_student_tmp");
}


bool StudentTemplate::loadData(QString fileName)
{
    DataDump dump(sqlDataModel.get());

    if (!dump.checkFileId(fileName, "lb_student_tmp"))
    {
        return false;
    }

    sqlDataModel->dropDataBase("lb_student_tmp");

    sqlDataModel->createDataBase("lb_student_tmp");

    sqlDataModel->setDatabase("lb_student_tmp");


    dump.readDump(fileName);

    return true;
}
