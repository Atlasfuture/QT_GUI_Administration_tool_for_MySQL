#include "StudentWidget.h"

#include <QDebug>

#include <QFileDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QMessageBox>

#include <QMenu>
#include <QAction>

#include "CourseDialog.h"
#include "GroupDialog.h"
#include "DepartmentDialog.h"

#include <QMdiSubWindow>

#include "CheckBoxGroup.h"

#include "Reports/AgeDistributionReport.h"
#include "LocationDialog.h"
#include "Core/QSqlRelationalLib/Headers/DataDump.h"



StudentWidget::StudentWidget(QSqlDatabase sqlDatabase, QMdiArea *_mdiArea, QWidget *parent)
{
    sqlDataModel = std::shared_ptr<QSqlRelationalDataModel> (new QSqlRelationalDataModel(sqlDatabase, this));

    mdiArea = _mdiArea;

    tmpStudent = std::shared_ptr<StudentTemplate> (new StudentTemplate(sqlDataModel));

    initElements();

    loadCmbBoxCountry();

    initText();

    wdgtStudentView = nullptr;
}


void StudentWidget::initElements()
{
    hLayoutMain = new QHBoxLayout;

    //installation of the base layout
    setLayout(hLayoutMain);




    QVBoxLayout *vMainLayout = new QVBoxLayout;
    hLayoutMain->addLayout(vMainLayout);




//TOOL MENU SECTION-------------------------------------------------------------------------------


    tBarTools = new QToolBar(this);

    vMainLayout->addWidget(tBarTools);

    tBtnMain = new QToolButton(this);

    menuTools = new QMenu;

    aLoadData = menuTools->addAction(tr("Load data"));
    connect(aLoadData, SIGNAL(triggered(bool)), SLOT(loadData()));

    aSaveData = menuTools->addAction(tr("Save data"));
    connect(aSaveData, SIGNAL(triggered(bool)), SLOT(saveData()));

    aGenerateReport = menuTools->addAction(tr("Generate report"));
    connect(aGenerateReport, SIGNAL(triggered(bool)), SLOT(generateReport()));

    tBtnMain->setMenu(menuTools);

    tBarTools->addWidget(tBtnMain);

    connect(tBtnMain, SIGNAL(clicked(bool)), tBtnMain, SLOT(showMenu()));


   // connect(menuTools, SIGNAL(triggered(QAction*)), SLOT(toolMenuSlot(QAction*)));



//DEPARTMENT SECTION------------------------------------------------------------------------------




    cmbBoxDepartment = new QComboBox;
    connect(cmbBoxDepartment, SIGNAL(currentIndexChanged(int)), this, SLOT(updateListGroups()));

    pBtnAddDepartment = new QPushButton;
    connect(pBtnAddDepartment, SIGNAL(clicked(bool)), SLOT(loadCreateDepartmentDialog()));

    pBtnUpdateDepartment = new QPushButton;
    connect(pBtnUpdateDepartment, SIGNAL(clicked(bool)), SLOT(loadUpdateDepartmentDialog()));

    pBtnDeleteDepartment = new QPushButton;
    connect(pBtnDeleteDepartment, SIGNAL(clicked(bool)), this, SLOT(deleteDepartment()));


    labelDepartments = new QLabel;
    vMainLayout->addWidget(labelDepartments);
    vMainLayout->addWidget(cmbBoxDepartment);
    vMainLayout->addWidget(pBtnAddDepartment);
    vMainLayout->addWidget(pBtnUpdateDepartment);
    vMainLayout->addWidget(pBtnDeleteDepartment);





//COURSE SECTION------------------------------------------------------------------------------------------


    cmbBoxCourse = new QComboBox;
    connect(cmbBoxCourse, SIGNAL(currentIndexChanged(int)), this, SLOT(updateListGroups()));

    pBtnAddCourse = new QPushButton;
    connect(pBtnAddCourse, SIGNAL(clicked(bool)), this, SLOT(loadCourseDialog()));

    pBtnDeleteCourse = new QPushButton;
    connect(pBtnDeleteCourse, SIGNAL(clicked(bool)), SLOT(deleteCourse()));

    labelCourse = new QLabel;
    vMainLayout->addWidget(labelCourse);
    vMainLayout->addWidget(cmbBoxCourse);
    vMainLayout->addWidget(pBtnAddCourse);
    vMainLayout->addWidget(pBtnDeleteCourse);





//GROUP SECTION--------------------------------------------------------------------------



    CheckBoxGroup* gChkBoxGroup = new CheckBoxGroup(this);

    cmbBoxGroup = new QComboBox;
    cmbBoxGroup->addItem(tr("empty"), QVariant("NULL"));
    connect(cmbBoxGroup, SIGNAL(currentIndexChanged(int)), SLOT(filter()));

    chkBoxAllGroups = new QCheckBox;
    connect(chkBoxAllGroups, SIGNAL(clicked(bool)), SLOT(updateListGroups()));
    connect(chkBoxAllGroups, SIGNAL(clicked(bool)), SLOT(setInactiveCmbBoxes(bool)));
    gChkBoxGroup->addButton(chkBoxAllGroups, 1);


    pBtnAddGroup = new QPushButton;
    connect(pBtnAddGroup, SIGNAL(clicked(bool)), this, SLOT(loadCreateGroupDialog()));

    pBtnUpdateGroup = new QPushButton;
    connect(pBtnUpdateGroup, SIGNAL(clicked(bool)), this, SLOT(loadUpdateGroupDialog()));

    pBtnDeleteGroup = new QPushButton;
    connect(pBtnDeleteGroup, SIGNAL(clicked(bool)), this, SLOT(deleteGroup()));

    QVBoxLayout *vLtGroup = new QVBoxLayout;

    labelGroup = new QLabel;
    vMainLayout->addWidget(labelGroup);
    vMainLayout->addWidget(cmbBoxGroup);
    vMainLayout->addWidget(chkBoxAllGroups);
    vMainLayout->addWidget(pBtnAddGroup);
    vMainLayout->addWidget(pBtnUpdateGroup);
    vMainLayout->addWidget(pBtnDeleteGroup);
    vMainLayout->addStretch(1);




//FILTER SECTION------------------------------------------------------------------------


    chkBoxShowAllStudents = new QCheckBox;
    connect(chkBoxShowAllStudents, SIGNAL(clicked(bool)), SLOT(filter()));
    connect(chkBoxShowAllStudents, SIGNAL(clicked(bool)), SLOT(setInactiveGroupCriterions(bool)));

    vLayoutFilter = new QVBoxLayout;
    gBoxFilter = new QGroupBox;
    gBoxFilter->setLayout(vLayoutFilter);

    hLayoutMain->addWidget(gBoxFilter);

    labelAllStudents = new QLabel;
    vLayoutFilter->addWidget(labelAllStudents);

    vLayoutFilter->addWidget(chkBoxShowAllStudents);



    //age subsection--------------------------------------------------


    labelAgeSubsection = new QLabel;
    vLayoutFilter->addWidget(labelAgeSubsection);



    //age equal
    CheckBoxGroup* groupChkBox = new CheckBoxGroup(this);


    hLayoutAge = new QHBoxLayout;
    vLayoutFilter->addLayout(hLayoutAge);

    chkBoxAgeEqual = new QCheckBox(this);
    connect(chkBoxAgeEqual, SIGNAL(clicked(bool)), SLOT(filter()));
    groupChkBox->addButton(chkBoxAgeEqual, 1);
    hLayoutAge->addWidget(chkBoxAgeEqual);

    labelAgeEqual = new QLabel;
    hLayoutAge->addWidget(labelAgeEqual);

    cmbBoxAgeEqual = new QComboBox;
    hLayoutAge->addWidget(cmbBoxAgeEqual);

    for (int i = 10; i < 30; i++)
    {
        cmbBoxAgeEqual->addItem(QString::number(i));
    }
    connect(cmbBoxAgeEqual, SIGNAL(currentIndexChanged(int)), SLOT(filter()));


    //age from

    hLayoutAgeFromTo = new QHBoxLayout;
    vLayoutFilter->addLayout(hLayoutAgeFromTo);

    chkBoxFromAge = new QCheckBox(this);
    groupChkBox->addButton(chkBoxFromAge, 2);
    connect(chkBoxFromAge, SIGNAL(clicked(bool)), SLOT(filter()));
    hLayoutAgeFromTo->addWidget(chkBoxFromAge);

    labelAgeFrom = new QLabel;
    hLayoutAgeFromTo->addWidget(labelAgeFrom);

    cmbBoxAgeFrom = new QComboBox;

    hLayoutAgeFromTo->addWidget(cmbBoxAgeFrom);

    for (int i = 10; i < 30; i++)
    {
        cmbBoxAgeFrom->addItem(QString::number(i));
    }
    connect(cmbBoxAgeFrom, SIGNAL(currentIndexChanged(int)), SLOT(filter()));

    //age to
    chkBoxToAge = new QCheckBox;
    connect(chkBoxToAge, SIGNAL(clicked(bool)), SLOT(filter()));
    groupChkBox->addButton(chkBoxToAge, 2);

    hLayoutAgeFromTo->addWidget(chkBoxToAge);

    cmbBoxAgeTo = new QComboBox;
    for (int i = 10; i < 30; i++)
    {
        cmbBoxAgeTo->addItem(QString::number(i));
    }
    connect(cmbBoxAgeTo, SIGNAL(currentIndexChanged(int)), SLOT(filter()));

    labelAgeTo = new QLabel;
    hLayoutAgeFromTo->addWidget(labelAgeTo);
    hLayoutAgeFromTo->addWidget(cmbBoxAgeTo);




    //gender subsection----------------------------------------------------------



    hLayoutGender = new QHBoxLayout;
    labelGenderSubsection = new QLabel;

    vLayoutFilter->addWidget(labelGenderSubsection);
    vLayoutFilter->addLayout(hLayoutGender);

    chkBoxGender = new QCheckBox;
    connect(chkBoxGender, SIGNAL(clicked(bool)), SLOT(filter()));
    hLayoutGender->addWidget(chkBoxGender);

    cmbBoxGender = new QComboBox;
    cmbBoxGender->addItem(strMale, QVariant("male"));
    cmbBoxGender->addItem(strFemale, QVariant("female"));
    connect(cmbBoxGender, SIGNAL(currentIndexChanged(int)), SLOT(filter()));

    hLayoutGender->addWidget(cmbBoxGender);




    //shcool_graduation subsection---------------------------------------



    hLayoutGraduation = new QHBoxLayout;

    labelSchoolGraduation = new QLabel;
    vLayoutFilter->addWidget(labelSchoolGraduation);
    vLayoutFilter->addLayout(hLayoutGraduation);

    chkBoxGraduation = new QCheckBox;
    connect(chkBoxGraduation, SIGNAL(clicked(bool)), SLOT(filter()));

    dEditGraduation = new QDateEdit;
    connect(dEditGraduation, SIGNAL(dateChanged(QDate)), SLOT(filter()));

    hLayoutGraduation->addWidget(chkBoxGraduation);
    hLayoutGraduation->addWidget(dEditGraduation);



    //status subsection------------------------------------------------------




    hLayoutStatus = new QHBoxLayout;

    statusSubSection = new QLabel;
    vLayoutFilter->addWidget(statusSubSection);
    vLayoutFilter->addLayout(hLayoutStatus);

    chkBoxStatus = new QCheckBox;
    connect(chkBoxStatus, SIGNAL(clicked(bool)), SLOT(filter()));

    cmbBoxStatus = new QComboBox;
    cmbBoxStatus->addItem(strOrphan, QVariant("orphan"));
    cmbBoxStatus->addItem(strIncompleteFamily, QVariant("incomplete_family"));
    connect(cmbBoxStatus, SIGNAL(currentIndexChanged(int)), SLOT(filter()));

    hLayoutStatus->addWidget(chkBoxStatus);
    hLayoutStatus->addWidget(cmbBoxStatus);


    //surname section
    hLayoutSurname = new QHBoxLayout;
    labelSurname = new QLabel;
    vLayoutFilter->addWidget(labelSurname);
    vLayoutFilter->addLayout(hLayoutSurname);

    chkBoxSurname = new QCheckBox;
    connect(chkBoxSurname, SIGNAL(clicked(bool)), SLOT(filter()));

    lEditSurname = new QLineEdit;
    connect(lEditSurname, SIGNAL(textChanged(QString)), SLOT(filter()));

    hLayoutSurname->addWidget(chkBoxSurname);
    hLayoutSurname->addWidget(lEditSurname);



    //name section
    hLayoutName = new QHBoxLayout;
    labelName = new QLabel;
    vLayoutFilter->addWidget(labelName);
    vLayoutFilter->addLayout(hLayoutName);

    chkBoxName = new QCheckBox;
    connect(chkBoxName, SIGNAL(clicked(bool)), SLOT(filter()));

    lEditName = new QLineEdit;
    connect(lEditName, SIGNAL(textChanged(QString)), SLOT(filter()));

    hLayoutName->addWidget(chkBoxName);
    hLayoutName->addWidget(lEditName);






    //middle name subsection-----------------------------------------------



    hLayoutMidname = new QHBoxLayout;
    labelMiddle = new QLabel;
    vLayoutFilter->addWidget(labelMiddle);
    vLayoutFilter->addLayout(hLayoutMidname);

    chkBoxMidname = new QCheckBox;
    connect(chkBoxMidname, SIGNAL(clicked(bool)), SLOT(filter()));

    lEditMidname = new QLineEdit;
    connect(lEditMidname, SIGNAL(textChanged(QString)), SLOT(filter()));

    hLayoutMidname->addWidget(chkBoxMidname);
    hLayoutMidname->addWidget(lEditMidname);

    vLayoutFilter->addStretch(1);




//ADDRESS FILTER SECTION------------------------------------------------




    vLayoutAddress = new QVBoxLayout;

    gBoxAddress = new QGroupBox;
    gBoxAddress->setLayout(vLayoutAddress);

    hLayoutMain->addWidget(gBoxAddress);



    //country section--------------------------------------------------



    hLayoutCountry = new QHBoxLayout;
    labelCountry = new QLabel;
    vLayoutAddress->addWidget(labelCountry);
    vLayoutAddress->addLayout(hLayoutCountry);

    chkBoxCountry = new QCheckBox;
    connect(chkBoxCountry, SIGNAL(clicked(bool)), SLOT(filter()));
    hLayoutCountry->addWidget(chkBoxCountry);

    cmbBoxCountry = new QComboBox;
    connect(cmbBoxCountry, SIGNAL(currentTextChanged(QString)), SLOT(loadCmbBoxRegion()));
    connect(cmbBoxCountry, SIGNAL(currentTextChanged(QString)), SLOT(filter()));
    hLayoutCountry->addWidget(cmbBoxCountry);

    //region section
    hLayoutRegion = new QHBoxLayout;
    labelRegion = new QLabel;
    vLayoutAddress->addWidget(labelRegion);
    vLayoutAddress->addLayout(hLayoutRegion);

    chkBoxRegion = new QCheckBox;
    connect(chkBoxRegion, SIGNAL(clicked(bool)), SLOT(filter()));
    hLayoutRegion->addWidget(chkBoxRegion);

    cmbBoxRegion = new QComboBox;
    connect(cmbBoxRegion, SIGNAL(currentTextChanged(QString)), SLOT(loadCmbBoxLocality()));
    connect(cmbBoxRegion, SIGNAL(currentTextChanged(QString)), SLOT(filter()));
    hLayoutRegion->addWidget(cmbBoxRegion);




    //locality type section------------------------------------------



    hLayoutLocalityType = new QHBoxLayout;
    labelLocalityType = new QLabel;
    vLayoutAddress->addWidget(labelLocalityType);
    vLayoutAddress->addLayout(hLayoutLocalityType);

    chkBoxType = new QCheckBox;
    connect(chkBoxType, SIGNAL(clicked(bool)), SLOT(filter()));
    hLayoutLocalityType->addWidget(chkBoxType);

    cmbBoxType = new QComboBox;
    connect(cmbBoxType, SIGNAL(currentIndexChanged(QString)), SLOT(filter()));
    hLayoutLocalityType->addWidget(cmbBoxType);



    //locality section---------------------------------------------------



    hLayoutLocality = new QHBoxLayout;
    labelLocality = new QLabel;
    vLayoutAddress->addWidget(labelLocality);
    vLayoutAddress->addLayout(hLayoutLocality);

    chkBoxLocality = new QCheckBox;
    connect(chkBoxLocality, SIGNAL(clicked(bool)), SLOT(filter()));
    hLayoutLocality->addWidget(chkBoxLocality);

    cmbBoxLocality = new QComboBox;
    connect(cmbBoxLocality, SIGNAL(currentTextChanged(QString)), SLOT(loadCmbBoxDistrict()));
    connect(cmbBoxLocality, SIGNAL(currentTextChanged(QString)), SLOT(filter()));
    hLayoutLocality->addWidget(cmbBoxLocality);



    //district section---------------------------------------------------



    hLayoutDistrict = new QHBoxLayout;
    labelDistrict = new QLabel;
    vLayoutAddress->addWidget(labelDistrict);
    vLayoutAddress->addLayout(hLayoutDistrict);

    chkBoxDistrict = new QCheckBox;
    connect(chkBoxDistrict, SIGNAL(clicked(bool)), SLOT(filter()));
    hLayoutDistrict->addWidget(chkBoxDistrict);

    cmbBoxDistrict = new QComboBox;
    connect(cmbBoxDistrict, SIGNAL(currentTextChanged(QString)), SLOT(loadCmbBoxStreet()));
    connect(cmbBoxDistrict, SIGNAL(currentTextChanged(QString)), SLOT(filter()));
    hLayoutDistrict->addWidget(cmbBoxDistrict);




    //street section------------------------------------------------------



    hLayoutStreet = new QHBoxLayout;
    labelStreet = new QLabel;
    vLayoutAddress->addWidget(labelStreet);
    vLayoutAddress->addLayout(hLayoutStreet);

    chkBoxStreet = new QCheckBox;
    connect(chkBoxStreet, SIGNAL(clicked(bool)), SLOT(filter()));
    hLayoutStreet->addWidget(chkBoxStreet);

    cmbBoxStreet = new QComboBox;
    connect(cmbBoxStreet, SIGNAL(currentTextChanged(QString)), SLOT(filter()));
    hLayoutStreet->addWidget(cmbBoxStreet);

    pBtnAddLocation = new QPushButton;
    connect(pBtnAddLocation, SIGNAL(clicked(bool)), SLOT(loadLocationDialog()));
    vLayoutAddress->addWidget(pBtnAddLocation);

    vLayoutAddress->addStretch(1);




    //student view initialization----------------------------------------



    wdgtStudentView = new StudentViewWidget(tmpStudent);

    connect(wdgtStudentView, SIGNAL(locationAdded()), SLOT(loadCmbBoxCountry()));

    connect(cmbBoxDepartment, SIGNAL(currentTextChanged(QString)), wdgtStudentView, SLOT(setDepartment(QString)));
    connect(cmbBoxCourse, SIGNAL(currentTextChanged(QString)), wdgtStudentView, SLOT(setCourse(QString)));
    connect(cmbBoxGroup, SIGNAL(currentTextChanged(QString)), wdgtStudentView, SLOT(setGroup(QString)));

    QMdiSubWindow* sub = mdiArea->addSubWindow(wdgtStudentView);
    sub->setAttribute(Qt::WA_DeleteOnClose);
    sub->setWindowIcon(QIcon(""));
    sub->setWindowFlags( Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint );
    sub->show();

    connect(this, SIGNAL(closed()), sub, SLOT(close()));

    connect(this, SIGNAL(filterActivated()), wdgtStudentView, SLOT(countRecords()));

    updateLists();
}

void StudentWidget::initText()
{

//TOOL MENU SECTION---------------------------------------------------

    tBtnMain->setText(tr("Tools"));


//DEPARTMENT SECTION---------------------------------------------------

    pBtnAddDepartment->setText(tr("Add department"));
    pBtnUpdateDepartment->setText(tr("Update department"));
    pBtnDeleteDepartment->setText(tr("Delete department"));

    labelDepartments->setText(tr("Departments"));



//COURSE SECTION------------------------------------------------------------


    pBtnAddCourse->setText(tr("Add course"));
    pBtnDeleteCourse->setText(tr("Delete course"));

    labelCourse->setText(tr("Courses"));


//GROUPS SECTION-----------------------------------------------------------


    chkBoxAllGroups->setText(tr("Show all"));
    pBtnAddGroup->setText(tr("Add group"));
    pBtnUpdateGroup->setText(tr("Update group"));
    pBtnDeleteGroup->setText(tr("Delete group"));

    labelGroup->setText(tr("Groups"));



//MAIN FILTER SECTION-------------------------------------------------------

    labelAllStudents->setText(tr("Show all students"));

    gBoxFilter->setTitle(tr("Students filter"));
    labelAgeSubsection->setText(tr("Age"));
    labelAgeFrom->setText(tr("from"));
    labelAgeTo->setText(tr("to"));

    labelGenderSubsection->setText(tr("Gender"));

    labelSchoolGraduation->setText(tr("School graduation"));


    strMale = tr("male");
    strFemale = tr("female");

    cmbBoxGender->clear();
    cmbBoxGender->addItem(strMale, QVariant("male"));
    cmbBoxGender->addItem(strFemale, QVariant("female"));



    strOrphan = tr("orphan");
    strIncompleteFamily = tr("Incomplete family");

    cmbBoxStatus->clear();
    cmbBoxStatus->addItem(strOrphan, QVariant("orphan"));
    cmbBoxStatus->addItem(strIncompleteFamily, QVariant("incomplete_family"));

    statusSubSection->setText(tr("Status"));

    labelName->setText(tr("Name"));

    labelSurname->setText(tr("Surname"));

    labelMiddle->setText(tr("Middle name"));



//ADDRESS SECTION----------------------------------------------------------



    gBoxAddress->setTitle(tr("Address filter"));

    labelCountry->setText(tr("Country"));

    labelRegion->setText(tr("Region"));

    labelLocalityType->setText(tr("Locality type"));

    labelLocality->setText(tr("Locality"));

    labelDistrict->setText(tr("District"));

    labelStreet->setText(tr("Street"));

    strCity = tr("city");
    strVillage = tr("village");
    strTownship = tr("township");

    cmbBoxType->clear();
    cmbBoxType->addItem(strCity, QVariant("city"));
    cmbBoxType->addItem(strVillage, QVariant("village"));
    cmbBoxType->addItem(strTownship, QVariant("township"));

    pBtnAddLocation->setText(tr("Add location"));
}


//EVENTS==========================================================================================



void StudentWidget::closeEvent(QCloseEvent *event)
{
    emit closed();
}

void StudentWidget::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        initText();
    }
}



//METHODS=========================================================================================



void StudentWidget::addCmbBoxItems(QComboBox *cmbBox, QList<QPair<QString, QString> > listPairs)
{
    for (int i = 0; i < listPairs.size(); i++)
    {
        cmbBox->addItem(listPairs.at(i).second, QVariant(listPairs.at(i).first));
    }
}

//SLOTS=================================================================================


void StudentWidget::updateLists()
{
    loadCmbBoxDepartments();
    loadListCourses();
    updateListGroups();

    loadCmbBoxCountry();
    loadCmbBoxLocalityType();
}


//DEPARTMENT SECTION-------------------------------------------------------------------------------------------------


void StudentWidget::loadCmbBoxDepartments()
{
    cmbBoxDepartment->clear();

    QList<QPair<QString, QString>> listPairs = tmpStudent->getDepartments();

    if(!listPairs.isEmpty())
    {
        for (int i = 0; i < listPairs.size(); i++)
        {
            cmbBoxDepartment->addItem(listPairs.at(i).second, QVariant(listPairs.at(i).first));
        }
    }

    cmbBoxDepartment->addItem(tr("empty"), QVariant("NULL"));
}

void StudentWidget::deleteDepartment()
{
    int res = QMessageBox::question(this, tr("Delete department?"), tr("Delete groups with this department?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    int subRes;

    switch(res)
    {
    case QMessageBox::Yes:
        subRes = QMessageBox::question(this, tr("Delete department?"), tr("Delete students whith groups?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

            switch(subRes)
            {
            case QMessageBox::Yes:
                tmpStudent->deleteDepartment(cmbBoxDepartment->currentData().toString(), true, true);
            break;

            case QMessageBox::No:
                tmpStudent->deleteDepartment(cmbBoxDepartment->currentData().toString(), true, false);
            break;

            case QMessageBox::Cancel:
                return;
            }
    break;

    case QMessageBox::No:
        tmpStudent->deleteDepartment(cmbBoxDepartment->currentData().toString(), false, false);
    break;

    case QMessageBox::Cancel:
        return;
    }

    updateLists();
}




void StudentWidget::loadCreateDepartmentDialog()
{
    DepartmentDialog dialogDepartment(tmpStudent, this);
    connect(&dialogDepartment, SIGNAL(created()), SLOT(loadCmbBoxDepartments()));

    QMessageBox msgBox;
    msgBox.setText(tr("Department added"));

    connect(&dialogDepartment, SIGNAL(created()), &msgBox, SLOT(exec()));

    QMessageBox msgBoxError;
    msgBoxError.setText(tr("Error department creating"));

    connect(&dialogDepartment, SIGNAL(error()), &msgBoxError, SLOT(exec()));

    dialogDepartment.exec();
}



void StudentWidget::loadUpdateDepartmentDialog()
{
    DepartmentDialog dialogDepartment(tmpStudent, cmbBoxDepartment->currentData().toString(), this);
    connect(&dialogDepartment, SIGNAL(updated()), SLOT(loadCmbBoxDepartments()));

    QMessageBox msgBox;
    msgBox.setText(tr("Department updated"));

    connect(&dialogDepartment, SIGNAL(updated()), &msgBox, SLOT(exec()));

    QMessageBox msgBoxError;
    msgBoxError.setText(tr("Error department updating"));

    connect(&dialogDepartment, SIGNAL(error()), &msgBoxError, SLOT(exec()));


    dialogDepartment.exec();
}



//COURSE SECTION-------------------------------------------------------------------------------------------------------

void StudentWidget::loadListCourses()
{
    cmbBoxCourse->clear();

    QStringList strList = tmpStudent->getCourses();

    if (!strList.isEmpty())
    {
        cmbBoxCourse->addItems(strList);
    }

    cmbBoxCourse->addItem(tr("empty"), QVariant("NULL"));
}

void StudentWidget::loadCourseDialog()
{
    CourseDialog dialogCourse(tmpStudent, this);
    connect(&dialogCourse, SIGNAL(confirmed()), SLOT(updateLists()));

    QMessageBox msgBox;
    msgBox.setText(tr("Course added"));

    connect(&dialogCourse, SIGNAL(confirmed()), &msgBox, SLOT(exec()));

    dialogCourse.exec();
}

void StudentWidget::deleteCourse()
{
    int res = QMessageBox::question(this, tr("Delete course?"), tr("Delete groups with this course?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    int subRes;

    switch(res)
    {
    case QMessageBox::Yes:
        subRes = QMessageBox::question(this, tr("Delete course?"), tr("Delete students whith groups?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

            switch(subRes)
            {
            case QMessageBox::Yes:
                tmpStudent->deleteCourses(cmbBoxCourse->currentText(), true, true);
            break;

            case QMessageBox::No:
                tmpStudent->deleteCourses(cmbBoxCourse->currentText(), true, false);
            break;

            case QMessageBox::Cancel:
                return;
            }
    break;

    case QMessageBox::No:
        tmpStudent->deleteCourses(cmbBoxCourse->currentText(), false, false);
    break;

    case QMessageBox::Cancel:
        return;
    }

    updateLists();
}



//GROUP SECTION================================================================================



void StudentWidget::loadListGroups()
{
    QList<QPair<QString, QString>> listPairs = tmpStudent->getGroups();

    if (!listPairs.isEmpty())
    {

        for (int i = 0; i < listPairs.size(); i++)
        {         
            cmbBoxGroup->addItem(listPairs.at(i).second, QVariant(listPairs.at(i).first));
        }

    }
}

void StudentWidget::updateListGroups()
{
    cmbBoxGroup->clear();

    QList<QPair<QString, QString>> listPairs;

    if (chkBoxAllGroups->isChecked())
    {
        loadListGroups();
    }
    else
    {
        if (cmbBoxDepartment->count() && cmbBoxCourse->count())
        {
            QString course = cmbBoxCourse->currentText();

            if (course == tr("empty"))
            {
                course = "NULL";
            }

            listPairs = tmpStudent->getGroups(cmbBoxDepartment->currentData().toString(), course);
        }
    }


    if (!listPairs.isEmpty())
    {
        for (int i = 0; i < listPairs.size(); i++)
        {

            cmbBoxGroup->addItem(listPairs.at(i).second, QVariant(listPairs.at(i).first));
        }

        cmbBoxGroup->addItem(tr("empty"), QVariant("NULL"));
    }
    else
    {
        cmbBoxGroup->addItem(tr("empty"), QVariant("NULL"));
    }
}

void StudentWidget::loadCreateGroupDialog()
{
    GroupDialog groupDialog(tmpStudent, this);
    groupDialog.loadData(cmbBoxDepartment->currentText(), cmbBoxCourse->currentText());
    connect(&groupDialog, SIGNAL(groupCreated()), this, SLOT(updateListGroups()));

    QMessageBox msgBox;
    msgBox.setText(tr("Group created"));

    connect(&groupDialog, SIGNAL(groupCreated()), &msgBox, SLOT(exec()));

    QMessageBox errorMsgBox;
    errorMsgBox.setText(tr("Error group creating"));

    connect(&groupDialog, SIGNAL(error()), &errorMsgBox, SLOT(exec()));

    groupDialog.exec();
}

void StudentWidget::loadUpdateGroupDialog()
{
    if (cmbBoxGroup->count())
    {
        GroupDialog groupDialog(tmpStudent, cmbBoxGroup->currentData().toString(), this);
        groupDialog.loadData(cmbBoxDepartment->currentText(), cmbBoxCourse->currentText(), cmbBoxGroup->currentText());
        connect(&groupDialog, SIGNAL(groupUpdated()), this, SLOT(updateListGroups()));

        QMessageBox msgBox;
        msgBox.setText(tr("Group updated"));

        connect(&groupDialog, SIGNAL(groupUpdated()), &msgBox, SLOT(exec()));

        QMessageBox errorMsgBox;
        errorMsgBox.setText(tr("Error group updating"));

        connect(&groupDialog, SIGNAL(error()), &errorMsgBox, SLOT(exec()));

        groupDialog.exec();
    }
    else
    {
        QMessageBox errorMsgBox;
        errorMsgBox.setText(tr("No group selected"));
        errorMsgBox.exec();
    }
}

void StudentWidget::deleteGroup()
{
    int res = QMessageBox::question(this, tr("Delete group?"), tr("Delete students with this group?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

    bool deleteSt;

    switch(res)
    {
        case QMessageBox::Yes:
            deleteSt = true;
        break;

        case QMessageBox::No:
            deleteSt = false;
        break;

        case QMessageBox::Cancel:
            return;
    }

    if (cmbBoxGroup->count())
    {
        tmpStudent->deleteGroup(cmbBoxGroup->currentData().toString(), deleteSt);
    }

    updateListGroups();
}



//FILTER SECTION============================================================================================



void StudentWidget::filter()
{
    qDebug() << "StudentWidget::filter";

    if (chkBoxShowAllStudents->isChecked())
    {
        tmpStudent->deactivateFilter(StudentTemplate::GROUP_FILTER);
    }
    else
    {
        if (cmbBoxGroup->count())
        {
            tmpStudent->activateFilter(StudentTemplate::GROUP_FILTER, cmbBoxGroup->currentData().toString());
        }
    }

    if (chkBoxAgeEqual->isChecked())
    {
        tmpStudent->activateFilter(StudentTemplate::AGE_FILTER, cmbBoxAgeEqual->currentText());
    }
    else
    {
        tmpStudent->deactivateFilter(StudentTemplate::AGE_FILTER);
    }

    if (chkBoxFromAge->isChecked())
    {
        tmpStudent->activateFilter(StudentTemplate::AGE_FROM_FILTER, cmbBoxAgeFrom->currentText());
    }
    else
    {
        tmpStudent->deactivateFilter(StudentTemplate::AGE_FROM_FILTER);
    }

    if (chkBoxToAge->isChecked())
    {
        tmpStudent->activateFilter(StudentTemplate::AGE_TO_FILTER, cmbBoxAgeTo->currentText());
    }
    else
    {
        tmpStudent->deactivateFilter(StudentTemplate::AGE_TO_FILTER);
    }

    if (chkBoxGender->isChecked())
    {
        tmpStudent->activateFilter(StudentTemplate::GENDER_FILTER, cmbBoxGender->currentData().toString());
    }
    else
    {
        tmpStudent->deactivateFilter(StudentTemplate::GENDER_FILTER);
    }

    if (chkBoxGraduation->isChecked())
    {
        QString dateStr =QString::number(dEditGraduation->date().year());
        dateStr += "-" + QString::number(dEditGraduation->date().month());
        dateStr += "-" + QString::number(dEditGraduation->date().day());

        tmpStudent->activateFilter(StudentTemplate::GRADUATION_FILTER, dateStr);
    }
    else
    {
        tmpStudent->deactivateFilter(StudentTemplate::GRADUATION_FILTER);
    }

    if (chkBoxStatus->isChecked())
    {
        tmpStudent->activateFilter(StudentTemplate::STATUS_FILTER, cmbBoxStatus->currentData().toString());
    }
    else
    {
        tmpStudent->deactivateFilter(StudentTemplate::STATUS_FILTER);
    }

    if (chkBoxName->isChecked())
    {
        tmpStudent->activateFilter(StudentTemplate::NAME_FILTER, lEditName->text());
    }
    else
    {
        tmpStudent->deactivateFilter(StudentTemplate::NAME_FILTER);
    }

    if (chkBoxSurname->isChecked())
    {
        tmpStudent->activateFilter(StudentTemplate::SURNAME_FILTER, lEditSurname->text());
    }
    else
    {
        tmpStudent->deactivateFilter(StudentTemplate::SURNAME_FILTER);
    }

    if (chkBoxMidname->isChecked())
    {
        tmpStudent->activateFilter(StudentTemplate::MIDNAME_FILTER, lEditMidname->text());
    }
    else
    {
        tmpStudent->deactivateFilter(StudentTemplate::MIDNAME_FILTER);
    }

    //address processing
    if (chkBoxCountry->isChecked())
    {
        tmpStudent->activateFilter(StudentTemplate::COUNTRY_FILTER, cmbBoxCountry->currentData().toString());
    }
    else
    {
        tmpStudent->deactivateFilter(StudentTemplate::COUNTRY_FILTER);
    }

    if (chkBoxRegion->isChecked())
    {
        tmpStudent->activateFilter(StudentTemplate::REGION_FILTER, cmbBoxRegion->currentData().toString());
    }
    else
    {
        tmpStudent->deactivateFilter(StudentTemplate::REGION_FILTER);
    }

    if (chkBoxType->isChecked())
    {
        tmpStudent->activateFilter(StudentTemplate::LOCTYPE_FILTER, cmbBoxType->currentData().toString());
    }
    else
    {
        tmpStudent->deactivateFilter(StudentTemplate::LOCTYPE_FILTER);
    }

    if (chkBoxLocality->isChecked())
    {
        tmpStudent->activateFilter(StudentTemplate::LOCALITY_FILTER, cmbBoxLocality->currentData().toString());
    }
    else
    {
        tmpStudent->deactivateFilter(StudentTemplate::LOCALITY_FILTER);
    }

    if (chkBoxDistrict->isChecked())
    {
        tmpStudent->activateFilter(StudentTemplate::DISTRICT_FILTER, cmbBoxDistrict->currentData().toString());
    }
    else
    {
        tmpStudent->deactivateFilter(StudentTemplate::DISTRICT_FILTER);
    }

    if (chkBoxStreet->isChecked())
    {
        tmpStudent->activateFilter(StudentTemplate::STREET_FILTER, cmbBoxStreet->currentData().toString());
    }
    else
    {
        tmpStudent->deactivateFilter(StudentTemplate::STREET_FILTER);
    }

    tmpStudent->runTableFilter();
    emit filterActivated();
}



//ADDRESS FILTER SLOTS===========================================================================================

void StudentWidget::loadCmbBoxCountry()
{
    cmbBoxCountry->clear();

    addCmbBoxItems(cmbBoxCountry, tmpStudent->getCountries());
}

void StudentWidget::loadCmbBoxRegion()
{
    cmbBoxRegion->clear();

    addCmbBoxItems(cmbBoxRegion, tmpStudent->getRegions(cmbBoxCountry->currentData().toString()));
}

void StudentWidget::loadCmbBoxLocalityType()
{
    cmbBoxType->clear();
    cmbBoxType->addItem(strCity, tr("city"));
    cmbBoxType->addItem(strVillage, tr("village"));
    cmbBoxType->addItem(strTownship, tr("township"));
}

void StudentWidget::loadCmbBoxLocality()
{
    cmbBoxLocality->clear();

    addCmbBoxItems(cmbBoxLocality, tmpStudent->getLocalities(cmbBoxRegion->currentData().toString()));
}

void StudentWidget::loadCmbBoxDistrict()
{
    cmbBoxDistrict->clear();

    addCmbBoxItems(cmbBoxDistrict, tmpStudent->getDistricts(cmbBoxLocality->currentData().toString()));
}

void StudentWidget::loadCmbBoxStreet()
{
    cmbBoxStreet->clear();

    addCmbBoxItems(cmbBoxStreet, tmpStudent->getStreets(cmbBoxDistrict->currentData().toString()));
}

void StudentWidget::loadLocationDialog()
{
    LocationDialog dialogLocation(tmpStudent);


    connect(&dialogLocation, SIGNAL(countryAdded()), this, SLOT(loadCmbBoxCountry()));
    connect(&dialogLocation, SIGNAL(regionAdded()), this, SLOT(loadCmbBoxRegion()));
    connect(&dialogLocation, SIGNAL(localityAdded()), this, SLOT(loadCmbBoxLocality()));
    connect(&dialogLocation, SIGNAL(districtAdded()), this, SLOT(loadCmbBoxDistrict()));
    connect(&dialogLocation, SIGNAL(streetAdded()), this, SLOT(loadCmbBoxStreet()));

    dialogLocation.exec();
}



//TOOL MENU SLOTS=====================================================================================================


void StudentWidget::loadData()
{
    int res = QMessageBox::question(this, tr("Load data?"), tr("Are you sure? All old data will be deleted"), QMessageBox::Yes | QMessageBox::No);

    if (res == QMessageBox::Yes)
    {
        sqlDataModel->dropDataBase("lb_student_tmp");

        sqlDataModel->createDataBase("lb_student_tmp");

        sqlDataModel->setDatabase("lb_student_tmp");

        QString strFilePath = QFileDialog::getOpenFileName(this, tr("Load data save"), QDir::currentPath(), tr("Document (*.sql)"));

        if (strFilePath != "")
        {
            if (tmpStudent->loadData(strFilePath))
            {

                updateLists();

                QMessageBox msgBox;
                msgBox.setText(tr("New data loaded"));
                msgBox.exec();
            }
            else
            {
                QMessageBox msgBox;
                msgBox.setText(tr("File is not student database template!"));
                msgBox.exec();
            }
        }
    }
}



void StudentWidget::saveData()
{
    QString strFilePath = QFileDialog::getSaveFileName(this, tr("Save data"), QDir::currentPath(), tr("Document (*.sql)"));

    if (strFilePath != "")
    {
        tmpStudent->writeData(strFilePath);

        QMessageBox msgBox;
        msgBox.setText(tr("Data saved"));
        msgBox.exec();
    }
}


void StudentWidget::generateReport()
{
    int res = QMessageBox::question(this, tr("Generate report?"), tr("Generate report on student age distribution??"), QMessageBox::Yes | QMessageBox::No);

    if (res == QMessageBox::Yes)
    {
        bool consider;

        res = QMessageBox::question(this, tr("Generate report?"), tr("Consider current date?"), QMessageBox::Yes | QMessageBox::No);

        if (res == QMessageBox::Yes)
        {
            consider = true;
        }
        else
        {
            consider = false;
        }

        QString strFilePath = QFileDialog::getSaveFileName(this, tr("Save report dialog"), QDir::currentPath(), tr("Documents (*.odt)"));

        if (strFilePath != "")
        {
            AgeDistributionReport report(sqlDataModel->database(), consider);
            report.writeReport(strFilePath);
        }
    }
}



void StudentWidget::setInactiveGroupCriterions(bool inactive)
{
    bool active = true;

    if (inactive)
    {
        active = false;
    }

    cmbBoxDepartment->setEnabled(active);
    cmbBoxCourse->setEnabled(active);
    cmbBoxGroup->setEnabled(active);
    chkBoxAllGroups->setEnabled(active);
}

void StudentWidget::setInactiveCmbBoxes(bool inactive)
{
    bool active = true;

    if (inactive)
    {
        active = false;
    }

    cmbBoxDepartment->setEnabled(active);
    cmbBoxCourse->setEnabled(active);
}


