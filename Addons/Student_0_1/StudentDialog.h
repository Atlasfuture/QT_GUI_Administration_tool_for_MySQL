#ifndef STUDENTDIALOG_H
#define STUDENTDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QDateEdit>
#include <QComboBox>
#include <QTabWidget>
#include <QGroupBox>
#include <QRadioButton>

#include "StudentTemplate.h"

class MainInfoPage;
class ContactInfoPage;
class AdditionalInfoPage;

class StudentDialog : public QDialog
{
    Q_OBJECT

    //student template descriptor
    std::shared_ptr<StudentTemplate> tmpStudent;

    QString studentId;

    //main elements

    QVBoxLayout* vLayout;
    QTabWidget* tabWidget;
    QPushButton* pBtnConfirm;

    //tab pages
    MainInfoPage* pageMain;
    ContactInfoPage* pageContact;
    AdditionalInfoPage* pageAdditional;


//METHODS===============================================================================

    //main methods----------------------------------------------------------------------


    //init elements
    void initElements();
    void initText();



    //fill fields in update mode by student id
    void fillFields(QString studentId);


    //additional data transform methods
    int getYear(QString date);
    int getMonth(QString date);
    int getDay(QString date);

    //check all fields on info fill
    bool checkFields();

public:

    static void addCmbBoxItems(QComboBox* cmbBox, QList<QPair<QString, QString>> listPairs);

    //work mode of student dialog
    //CREATE_MODE - creating new student
    //UPDATE_MODE - updating student data
    enum workMode
    {
        CREATE_MODE,
        UPDATE_MODE
    };

    workMode currentMode;   //current work mode

    //constructor
    StudentDialog(std::shared_ptr<StudentTemplate> _tmpStudent, QString studentId, workMode mode, QWidget *parent);

    void setCurrentGroup(QString departmentName, QString course, QString groupName);
signals:
    //signal that emited when all user confirm
    void confirmed();
public slots:

    //MAIN SLOT THAT CREATE AND SEND NEW STUDENT DATA IN DATABASE
    void confirm();

    void clearFields();


    void initDepartments();
    void initCourse();
    void initGroups();
};



//TAB PAGES=========================================================================================================



class MainInfoPage : public QWidget
{
    Q_OBJECT

    std::shared_ptr<StudentTemplate> tmpStudent;

    //layout
    QVBoxLayout* vLayout;

public:

    void initElements();

    //elements

    //personal
    QGroupBox* gBoxPersonal;
    QLineEdit* lEditSurname;
    QLineEdit* lEditName;
    QLineEdit* lEditMiddleName;
    QComboBox* cmbBoxGender;

    //date edits
    QDateEdit* dateEditBirthday;
    QDateEdit* dateEditSchool;

    //group
    QGroupBox* gBoxGroup;

    QComboBox* cmbBoxDepartment;
    QComboBox* cmbBoxCourse;
    QComboBox* cmbBoxGroup;

    MainInfoPage(std::shared_ptr<StudentTemplate> _tmpStudent, QWidget* parent = 0);

public slots:

};





class ContactInfoPage : public QWidget
{
    Q_OBJECT

    std::shared_ptr<StudentTemplate> tmpStudent;

    //layout

    QVBoxLayout* vLayout;

    QPushButton* pBtnAddLocation;

    void initElements();

public:

    //elements
    //adress
    QGroupBox* groupBoxAdress;

    QComboBox* cmbBoxCountry;
    QComboBox* cmbBoxRegion;
    QComboBox* cmbBoxLocality;
    QComboBox* cmbBoxDistrict;
    QComboBox* cmbBoxStreet;

    QLineEdit* lEditHouse;
    QLineEdit* lEditFlat;

    //number
    QGroupBox* groupBoxNumber;
    QComboBox* cmbBoxType;
    QLineEdit* lEditNumber;

    ContactInfoPage(std::shared_ptr<StudentTemplate> _tmpStudent, QWidget* parent = 0);

public slots:
    void loadCmbBoxCountry();
    void loadCmbBoxRegion();
    void loadCmbBoxLocality();
    void loadCmbBoxDistrict();
    void loadCmbBoxStreet();

    void loadLocationDialog();
};



class AdditionalInfoPage : public QWidget
{
    Q_OBJECT

    //layout
    QVBoxLayout* vLayout;

    void initElements();
public:

    AdditionalInfoPage(QWidget* parent = 0);

    //elements
    QComboBox* cmbBoxStatus;
};


#endif // STUDENTDIALOG_H
