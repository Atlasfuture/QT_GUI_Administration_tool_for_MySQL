#ifndef STUDENTTEMPLATE_H
#define STUDENTTEMPLATE_H

#include <memory>
#include <QStringList>
#include <map>

#include "Core/QSqlRelationalLib/Headers/QSqlRelationalDataModel.h"
#include <QSqlQueryModel>

#include "Student.h"
#include "Address.h"
#include "Phone.h"





class Student;
class Address;






class StudentTemplate
{
    bool allStudentsShow;

    //SQL models
    std::shared_ptr<QSqlRelationalDataModel> sqlDataModel;
    QSqlQueryModel sqlQueryModel;

    //last selection query
    QString selectQuery;

    QString filterQuery;

    std::map<int, QString> mapValueFilter;
    std::map<int, QString> mapJoin;
    std::map<int, QString> mapMode;

    //current student data
    QString currentDepartment;
    QString currentCourse;
    QString currentGroup;

    //makes list of pairs - key - name
    QList<QPair<QString, QString>> makePairList(int first, int second);

    //install base template if exist, or load it on server

    void installBaseTemplate();

    bool checkOperationResult();
public:
    StudentTemplate(std::shared_ptr<QSqlRelationalDataModel> _sqlDataModel);


    std::shared_ptr<QSqlRelationalDataModel> getModel();        //get current QSqlRelationalDataModel

    void refreshCurrentSelection();


    enum STUDENT_FILTER
    {
        //main filters
        GROUP_FILTER = 0,
        COURSE_FILTER = 1,
        GENDER_FILTER = 2,
        AGE_FILTER = 3,
        AGE_FROM_FILTER = 4,
        AGE_TO_FILTER = 5,
        GRADUATION_FILTER = 6,
        STATUS_FILTER = 7,
        NAME_FILTER = 8,
        SURNAME_FILTER = 9,
        MIDNAME_FILTER = 10,

        //address filters
        COUNTRY_FILTER = 11,
        REGION_FILTER = 12,
        LOCALITY_FILTER = 13,
        LOCTYPE_FILTER = 14,
        DISTRICT_FILTER = 15,
        STREET_FILTER = 16,

        //mode shows students from all departments and courses
        ALL_STUDENTS = 17,

        //age filters than dont consider current month
        AGE_FILTER_C = 18,
        AGE_FROM_FILTER_C = 19,
        AGE_TO_FILTER_C = 20
    };


    //DEPARTMENT METHODS


    bool createDepartment(QString fullName, QString shortName, QString code);
    bool updateDepartment(QString departmentID, QString fullName, QString shortName, QString code);
    bool deleteDepartment(QString departmentID, bool deleteGroups, bool deleteStudents);


    QString getDepartmentCode(QString departmentID);
    QString getDepartmentFullName(QString departmentID);
    QString getDepartmentShortName(QString departmentID);
    QList<QPair<QString, QString>> getDepartments();
    QString getDepartmentByGroup(QString groupId);



    //COURSE METHODS


    QStringList getCourses();
    bool createCourse(QString courseId, QString code);
    bool deleteCourses(QString courseId, bool deleteGroups, bool deleteStudents);

    QString getCourseByGroup(QString groupId);



    //GROUP METHODS


    enum GroupFilter
    {
        DEPARTMENT = 0,
        COURSE = 1,
        ALL = 2
    };

    QString getGroupNameById(QString groupId);
    QList<QPair<QString, QString>> getGroups();
    QList<QPair<QString, QString>> getGroups(QString id, GroupFilter filter);
    QList<QPair<QString, QString>> getGroups(QString departmentId, QString course);
    QList<QPair<QString, QString>> getAloneGroups(bool emptyDepartment, bool emptyCourse);
    bool createGroup(QString groupName, QString course, QString departmentID);
    bool updateGroup(QString groupID, QString groupName, QString course, QString departmentID);
    bool deleteGroup(QString groupID, bool deleteStudents);



    //ADDRESS METHODS

    enum ADDRESS
    {
        COUNTRY = 0,
        REGION = 1,
        DISTRICT = 2,
        STREET = 3
    };

    bool updateAdressObject(ADDRESS address, QString objectID, QString newName);

    QList<QPair<QString, QString>> getCountries();
    bool addCountry(QString name);
    bool deleteRecCountry(QString countryId);

    QList<QPair<QString, QString>> getRegions(QString countryId);
    bool addRegion(QString name, QString countryId);
    bool deleteRecRegions(QString regionId);

    QList<QPair<QString, QString>> getLocalities(QString regionId);
    bool addLocality(QString name, QString type, QString regionId);

    enum LOCALITY_TYPE
    {
        CITY = 0,
        VILLAGE = 1,
        TOWNSHIP = 2
    };

    bool updateLocalityName(QString localityID, QString newName, LOCALITY_TYPE type);
    bool deleteRecLocality(QString localityId);

    QList<QPair<QString, QString>> getDistricts(QString localityId);
    bool addDistrict(QString name, QString localityId);
    bool deleteRecDistrict(QString districtId);

    QList<QPair<QString, QString>> getStreets(QString districtId);
    bool addStreet(QString name, QString districtId);
    bool deleteRecStreet(QString streetId);

    bool createAddress(Address adr);
    bool updateAddress(QString studentId, Address adr);
    Address getAddressByStudent(QString studentId);

    //PHONE METHODS
    void createPhone(Phone phone, Student stTmp);
    bool updatePhone(Phone phone, QString studentId);
    Phone getPhoneByStudent(QString studentId);

    //STUDENT METHODS
    bool createStudent(Student st);
    bool createStudent(Student st, Address adr);
    void deleteStudent(QString idStudent);
    bool updateStudent(QString studentId, Student st);

    Student getStudentData(QString studentId);
    QString getAdressId(Address adr);
    QString getStudentId(Student st);
    int countAllStudents();


    //FILTER METHODS

    //type of filter

    void activateAllStudents();
    void deactivateAllStudents();



    enum ACTIVATE_MODE
    {
        AND_MODE = 0,
        OR_MODE = 1
    };

    void activateFilter(STUDENT_FILTER filter, QString value);


    void activateFilter(STUDENT_FILTER filter, ACTIVATE_MODE mode, QString value);
    void deactivateFilter(STUDENT_FILTER filter);
    void deactivateFilters();

    int runTableFilter();

    //DUMP METHODS


    //save student template database from sever on file
    void writeData(QString fileName);

    //load student template database from file on server
    bool loadData(QString fileName);

};



#endif // STUDENTTEMPLATE_H
