#ifndef STUDENT_H
#define STUDENT_H

#include <memory>

#include "Core/QSqlRelationalLib/Headers/QSqlRelationalDataModel.h"
#include "StudentTemplate.h"
#include "StudentViewWidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QListWidget>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QTableView>
#include <QMdiArea>
#include <QToolButton>
#include <QButtonGroup>
#include <QMenu>
#include <QAction>
#include <QToolBar>

class StudentWidget : public QWidget
{
    Q_OBJECT

    std::shared_ptr<QSqlRelationalDataModel> sqlDataModel;
    std::shared_ptr<StudentTemplate> tmpStudent;


    QMdiArea* mdiArea;

    //student view
    StudentViewWidget* wdgtStudentView;

    //BASE LAYOUT
    QHBoxLayout* hLayoutMain;


//ELEMENTS================================================================


    //TOOL MENU SECTION
    QToolBar* tBarTools;
    QToolButton* tBtnMain;
    QMenu* menuTools;

    //tool menu actions
    QAction* aLoadData;
    QAction* aSaveData;
    QAction* aGenerateReport;

    //DEPARTMENT SECTION

    QLabel* labelDepartments;
    QComboBox* cmbBoxDepartment;
    QPushButton* pBtnAddDepartment;
    QPushButton* pBtnUpdateDepartment;
    QPushButton* pBtnDeleteDepartment;

    //COURSE SECTION
    QLabel* labelCourse;
    QComboBox* cmbBoxCourse;
    QPushButton* pBtnAddCourse;
    QPushButton* pBtnDeleteCourse;

    //GROUP SECTION
    QLabel* labelGroup;
    QComboBox* cmbBoxGroup;

    QCheckBox* chkBoxAllGroups;


    QPushButton* pBtnAddGroup;
    QPushButton* pBtnUpdateGroup;
    QPushButton* pBtnDeleteGroup;

    //MAIN FILTER SECTION
    QVBoxLayout* vLayoutFilter;
    QGroupBox* gBoxFilter;

    QLabel* labelAllStudents;
    QCheckBox* chkBoxShowAllStudents;

    //age section

    QLabel* labelAgeSubsection;

    //age equal
    QHBoxLayout* hLayoutAge;
    QCheckBox* chkBoxAgeEqual;

    QLabel* labelAgeEqual;
    QComboBox* cmbBoxAgeEqual;

    //age from

    QHBoxLayout* hLayoutAgeFromTo;

    QCheckBox* chkBoxFromAge;

    QLabel* labelAgeFrom;
    QComboBox* cmbBoxAgeFrom;

    //age to
    QCheckBox* chkBoxToAge;
    QLabel* labelAgeTo;
    QComboBox* cmbBoxAgeTo;

    //gender section
    QHBoxLayout* hLayoutGender;
    QLabel* labelGenderSubsection;

    QCheckBox* chkBoxGender;
    QComboBox* cmbBoxGender;

    QString strMale;
    QString strFemale;

    //school_graduation section
    QHBoxLayout* hLayoutGraduation;
    QLabel* labelSchoolGraduation;

    QCheckBox* chkBoxGraduation;
    QDateEdit* dEditGraduation;

    //status section
    QHBoxLayout* hLayoutStatus;
    QLabel* statusSubSection;

    QString strOrphan;
    QString strIncompleteFamily;

    QCheckBox* chkBoxStatus;
    QComboBox* cmbBoxStatus;


    //surname section
    QHBoxLayout* hLayoutSurname;
    QLabel* labelSurname;
    QCheckBox* chkBoxSurname;
    QLineEdit* lEditSurname;


    //name section
    QHBoxLayout* hLayoutName;
    QLabel* labelName;

    QCheckBox* chkBoxName;
    QLineEdit* lEditName;



    //middle name section
    QHBoxLayout* hLayoutMidname;
    QLabel* labelMiddle;
    QCheckBox* chkBoxMidname;
    QLineEdit* lEditMidname;

    //ADDRESS FILTER SECTION
    QVBoxLayout* vLayoutAddress;
    QGroupBox* gBoxAddress;


    //contry section
    QHBoxLayout* hLayoutCountry;
    QLabel* labelCountry;
    QCheckBox* chkBoxCountry;
    QComboBox* cmbBoxCountry;

    //region section
    QHBoxLayout* hLayoutRegion;
    QLabel* labelRegion;
    QCheckBox* chkBoxRegion;
    QComboBox* cmbBoxRegion;

    //locality section
    QHBoxLayout* hLayoutLocalityType;
    QLabel* labelLocalityType;
    QCheckBox* chkBoxType;
    QComboBox* cmbBoxType;

    QHBoxLayout* hLayoutLocality;
    QLabel* labelLocality;
    QCheckBox* chkBoxLocality;
    QComboBox* cmbBoxLocality;

    QString strCity;
    QString strVillage;
    QString strTownship;

    //district section
    QHBoxLayout* hLayoutDistrict;
    QLabel* labelDistrict;
    QCheckBox* chkBoxDistrict;
    QComboBox* cmbBoxDistrict;

    //street section
    QHBoxLayout* hLayoutStreet;
    QLabel* labelStreet;
    QCheckBox* chkBoxStreet;
    QComboBox* cmbBoxStreet;

    QPushButton* pBtnAddLocation;

    //main section
    QPushButton* pBtnShowStudents;


    void initElements();
    void initText();

//EVENTS===============================================

    void closeEvent(QCloseEvent *event);
    void changeEvent(QEvent *event);

//METHODS==============================================

    void addCmbBoxItems(QComboBox* cmbBox, QList<QPair<QString, QString>> listPairs);


public:
    explicit StudentWidget(QSqlDatabase sqlDatabase, QMdiArea* _mdiArea, QWidget *parent = 0);


signals:
    void closed();
    void filterActivated();

public slots:

    //void connectToDatabase();
    void updateLists();

    //group slots
    void updateListGroups();
    void loadListGroups();
    void loadCreateGroupDialog();
    void loadUpdateGroupDialog();
    void deleteGroup();

    //department slots
    void loadCreateDepartmentDialog();
    void loadUpdateDepartmentDialog();
    void deleteDepartment();
    void loadCmbBoxDepartments();

    //course
    void loadCourseDialog();
    void loadListCourses();
    void deleteCourse();

    //FILTER SLOTS
  //  void showStudents();

    //ADRESS FILTER SLOTS
    void loadCmbBoxCountry();
    void loadCmbBoxRegion();
    void loadCmbBoxLocalityType();
    void loadCmbBoxLocality();
    void loadCmbBoxDistrict();
    void loadCmbBoxStreet();

    void loadLocationDialog();


    //FILTER SLOT
    void filter();


    //TOOL MENU SLOTS

    void loadData();
    void saveData();
    void generateReport();


    //ADDITIONAL SLOTS

    //sets some elements of the widget inactive for user
    void setInactiveGroupCriterions(bool inactive);
    void setInactiveCmbBoxes(bool inactive);


};

#endif // STUDENT_H
