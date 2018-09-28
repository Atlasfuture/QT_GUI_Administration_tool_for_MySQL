#include "StudentDialog.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QVariant>
#include <QMessageBox>
#include <QDebug>

#include "Student.h"
#include "Address.h"
#include "LocationDialog.h"


StudentDialog::StudentDialog(std::shared_ptr<StudentTemplate> _tmpStudent, QString _studentId, workMode _currentMode, QWidget *parent)
{
    tmpStudent = _tmpStudent;
    studentId = _studentId;

    initElements();

    currentMode = _currentMode;

    initDepartments();
    initCourse();
    initGroups();

    if (currentMode == UPDATE_MODE)
    {
        fillFields(studentId);
    }



    connect(pBtnConfirm, SIGNAL(clicked(bool)), SLOT(confirm()));

    initText();
}

void StudentDialog::setCurrentGroup(QString departmentName, QString course, QString groupName)
{
    qDebug() << "CURRENT DEPARTMENT: " << departmentName;

    qDebug() << "CURRENT COURSE: " << course;

    pageMain->cmbBoxDepartment->setCurrentText(departmentName);

    pageMain->cmbBoxCourse->setCurrentText(course);


    pageMain->cmbBoxGroup->setCurrentText(groupName);


}


void StudentDialog::addCmbBoxItems(QComboBox *cmbBox, QList<QPair<QString, QString>> listPairs)
{
    for (int i = 0; i < listPairs.size(); i++)
    {
        cmbBox->addItem(listPairs.at(i).second, QVariant(listPairs.at(i).first));
    }

    cmbBox->addItem(tr("empty"), QVariant("NULL"));
}

void StudentDialog::initElements()
{
    vLayout = new QVBoxLayout;

    tabWidget = new QTabWidget;

    pageMain = new MainInfoPage(tmpStudent);
    pageContact = new ContactInfoPage(tmpStudent, this);
    pageAdditional = new AdditionalInfoPage;


    tabWidget->addTab(pageMain, tr("Main information"));
    tabWidget->addTab(pageContact, tr("Contact information"));
    tabWidget->addTab(pageAdditional, tr("Additional info"));

    vLayout->addWidget(tabWidget);

    pBtnConfirm = new QPushButton;
    vLayout->addWidget(pBtnConfirm);

    setLayout(vLayout);

    connect(pageMain->cmbBoxDepartment, SIGNAL(currentTextChanged(QString)), SLOT(initGroups()));
    connect(pageMain->cmbBoxCourse, SIGNAL(currentTextChanged(QString)), SLOT(initGroups()));
}


void StudentDialog::initText()
{
    tabWidget->setTabText(0, tr("Main information"));
    tabWidget->setTabText(1, tr("Contact information"));
    tabWidget->setTabText(2, tr("Additional info"));

    pBtnConfirm->setText(tr("confirm"));


}

void StudentDialog::fillFields(QString studentId)
{

    Student stData = tmpStudent->getStudentData(studentId);

    pageMain->lEditSurname->setText(stData.surname);
    pageMain->lEditName->setText(stData.name);
    pageMain->lEditMiddleName->setText(stData.middleName);

    if (stData.gender == "male")
    {
        pageMain->cmbBoxGender->setCurrentIndex(0);
    }
    else
    {
        pageMain->cmbBoxGender->setCurrentIndex(1);
    }



    QString departmentName = tmpStudent->getDepartmentByGroup(stData.group);

    QString course = tmpStudent->getCourseByGroup(stData.group);


    if (departmentName == "")
    {
       departmentName = tr("empty");
    }

    if (course == "0")
    {
       course = tr("empty");
    }

    pageMain->cmbBoxDepartment->setCurrentText(departmentName);
    pageMain->cmbBoxCourse->setCurrentText(course);
    pageMain->cmbBoxGroup->setCurrentText(tmpStudent->getGroupNameById(stData.group));

    QString dateStr = stData.birthdate;
    QDate date(getYear(dateStr), getMonth(dateStr), getDay(dateStr));

    pageMain->dateEditBirthday->setDate(date);

    dateStr = stData.schoolGraduation;
    date.setDate(getYear(dateStr), getMonth(dateStr), getDay(dateStr));

    pageMain->dateEditSchool->setDate(date);


    if (stData.status == "orphan")
    {
        pageAdditional->cmbBoxStatus->setCurrentText(tr("Orphan"));
    }
    else if(stData.status == "incomplete_family")
    {
        pageAdditional->cmbBoxStatus->setCurrentText(tr("Incomplete family"));
    }


    //address----------------------------------------------------------------------------


    Address address = tmpStudent->getAddressByStudent(studentId);

    pageContact->cmbBoxCountry->setCurrentText(address.country);
    pageContact->cmbBoxRegion->setCurrentText(address.region);
    pageContact->cmbBoxLocality->setCurrentText(address.locality);
    pageContact->cmbBoxDistrict->setCurrentText(address.district);
    pageContact->cmbBoxStreet->setCurrentText(address.street);

    QString strTmp = address.house;

    if (strTmp.contains("."))
    {
        strTmp.replace(".", "/");
    }

    pageContact->lEditHouse->setText(strTmp);


    strTmp = address.flat;

    if (strTmp.contains("."))
    {
        strTmp.replace(".", "/");
    }

    pageContact->lEditFlat->setText(strTmp);


    Phone phone = tmpStudent->getPhoneByStudent(studentId);


    if (phone.phoneType == tr("mobile"))
    {
        pageContact->cmbBoxType->setCurrentText(tr("Mobile"));
    }
    else if(phone.phoneType == tr("home"))
    {
        pageContact->cmbBoxType->setCurrentText(tr("Home"));
    }

    strTmp = phone.phoneNumber;

    if (strTmp.contains("."))
    {
        strTmp.replace(".", "-");
    }

    pageContact->lEditNumber->setText(strTmp);
}

int StudentDialog::getYear(QString date)
{
    return date.section("-", 0, 0).toInt();
}

int StudentDialog::getMonth(QString date)
{
    return date.section("-", 1, 1).toInt();
}

int StudentDialog::getDay(QString date)
{
    return date.section("-", 2, 2).toInt();
}

bool StudentDialog::checkFields()
{
    if (currentMode == CREATE_MODE)
    {
        if (pageMain->cmbBoxGroup->currentData() == "NULL")
        {
            QMessageBox msgBox;
            msgBox.setText(tr("No group selected"));
            msgBox.exec();

            return false;
        }
    }

    if (pageContact->cmbBoxCountry->currentData() == "NULL"
        || pageContact->cmbBoxRegion->currentData() == "NULL"
        || pageContact->cmbBoxLocality->currentData() == "NULL"
        || pageContact->cmbBoxDistrict->currentData() == "NULL"
        || pageContact->cmbBoxStreet->currentData() == "NULL"
        || pageContact->lEditHouse->text().isEmpty()
        || pageContact->lEditFlat->text().isEmpty()
        || pageContact->lEditNumber->text().isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Incomplete contact information"));
        msgBox.exec();

        return false;
    }

    return true;
}




//SLOTS

void StudentDialog::confirm()
{
     if (!checkFields())
     {
        return;
     }

     Student stTmp;

     stTmp.surname = pageMain->lEditSurname->text().replace(QRegularExpression("'"), "`");
     stTmp.name = pageMain->lEditName->text().replace(QRegularExpression("'"), "`");
     stTmp.middleName = pageMain->lEditMiddleName->text().replace(QRegularExpression("'"), "`");
     stTmp.gender = pageMain->cmbBoxGender->currentData().toString();
     stTmp.status = pageAdditional->cmbBoxStatus->currentData().toString();

     //birthdate----------------------------------------------------------------------------
     QDate date = pageMain->dateEditBirthday->date();

     stTmp.birthdate = QString::number(date.year()) + "-";
     stTmp.birthdate += QString::number(date.month()) + "-";
     stTmp.birthdate += QString::number(date.day());

     //school graduation---------------------------------------------------------------------


     date = pageMain->dateEditSchool->date();
     stTmp.schoolGraduation = QString::number(date.year()) + "-";
     stTmp.schoolGraduation += QString::number(date.month()) + "-";
     stTmp.schoolGraduation += QString::number(date.day());


     stTmp.group = pageMain->cmbBoxGroup->currentData().toString();



     //address------------------------------------------------------------------------------


     Address adr;
     adr.country = pageContact->cmbBoxCountry->currentText();
     adr.region = pageContact->cmbBoxRegion->currentText();
     adr.locality = pageContact->cmbBoxLocality->currentText();
     adr.district = pageContact->cmbBoxDistrict->currentText();
     adr.street = pageContact->cmbBoxStreet->currentText();

     QString strTmp = pageContact->lEditHouse->text();


     QRegExp re("[*/+-]");

     if (strTmp.contains(re))
     {
        strTmp.replace(re, ".");
     }

     adr.house = strTmp;

     strTmp = pageContact->lEditFlat->text();

     if (strTmp.contains(re))
     {
        strTmp.replace(re, ".");
     }

     adr.flat = strTmp;



     //phone---------------------------------------------------------------------------------------


     Phone phone;

     strTmp = pageContact->lEditNumber->text();

     if (strTmp.contains(re))
     {
        strTmp.remove(QRegExp("-"));
     }

     phone.phoneNumber = strTmp;
     phone.phoneType = pageContact->cmbBoxType->currentData().toString();

     QString resultMsgText;
     bool result = false;


     switch(currentMode)
     {
        case CREATE_MODE:
            result = tmpStudent->createStudent(stTmp, adr);
            tmpStudent->createPhone(phone, stTmp);
            resultMsgText = tr("Student added");
        break;

        case UPDATE_MODE:
            int res = QMessageBox::question(this, tr("Student update?"), tr("Update student information?"), QMessageBox::Yes | QMessageBox::No);

            if (res == QMessageBox::Yes)
            {
                result = tmpStudent->updateStudent(studentId, stTmp);
                tmpStudent->updateAddress(studentId, adr);
                tmpStudent->updatePhone(phone, studentId);
                resultMsgText = tr("Student information updated");
            }
        break;
     }

     emit confirmed();

     if (result)
     {
         QMessageBox msgBox;
         msgBox.setText(resultMsgText);
         msgBox.exec();

         switch(currentMode)
         {
            case CREATE_MODE:
                clearFields();
            break;

            case UPDATE_MODE:
                close();
            break;
         }
     }
     else
     {
         QMessageBox msgBox;
         msgBox.setText(tr("Incorrect data or not all fields are filled"));
         msgBox.exec();
     }

}

void StudentDialog::clearFields()
{
    pageMain->lEditSurname->setText("");
    pageMain->lEditName->setText("");
    pageMain->lEditMiddleName->setText("");

    pageContact->lEditHouse->setText("");
    pageContact->lEditFlat->setText("");
    pageContact->lEditNumber->setText("");
}





void StudentDialog::initDepartments()
{
    addCmbBoxItems(pageMain->cmbBoxDepartment, tmpStudent->getDepartments());
}


void StudentDialog::initCourse()
{
    pageMain->cmbBoxCourse->addItems(tmpStudent->getCourses());
    pageMain->cmbBoxCourse->addItem(tr("empty"), QVariant("NULL"));
}


void StudentDialog::initGroups()
{
    pageMain->cmbBoxGroup->clear();

    if (pageMain->cmbBoxDepartment->count())
    {
        QString departmentData = pageMain->cmbBoxDepartment->currentData().toString();



        QString courseData = pageMain->cmbBoxCourse->currentText();


        if (courseData == tr("empty"))
        {
            courseData = "NULL";
        }

        addCmbBoxItems(pageMain->cmbBoxGroup, tmpStudent->getGroups(departmentData, courseData));

    }
    else
    {
        addCmbBoxItems(pageMain->cmbBoxGroup, tmpStudent->getGroups());
    }
}


//PAGES=========================================================================================================

//MainInfoPage==================================================================================================

MainInfoPage::MainInfoPage(std::shared_ptr<StudentTemplate> _tmpStudent, QWidget *parent) : QWidget(parent)
{
    tmpStudent = _tmpStudent;

    initElements();

}



void MainInfoPage::initElements()
{
    vLayout = new QVBoxLayout;


    //personal
    QVBoxLayout* vLayoutPersonal = new QVBoxLayout;
    gBoxPersonal = new QGroupBox(tr("Personal"));


    lEditSurname = new QLineEdit;
    lEditName = new QLineEdit;
    lEditMiddleName = new QLineEdit;

    cmbBoxGender = new QComboBox;
    cmbBoxGender->addItem(tr("male"), QVariant("male"));
    cmbBoxGender->addItem(tr("female"), QVariant("female"));

    //date-time edit format
    dateEditBirthday = new QDateEdit;
    dateEditBirthday->setDisplayFormat("dd.MM.yyyy");
    dateEditSchool = new QDateEdit;
    dateEditSchool->setDisplayFormat("dd.MM.yyyy");

    QHBoxLayout* hLayoutSurname = new QHBoxLayout;
    hLayoutSurname->addWidget(new QLabel(tr("Surname")));
    hLayoutSurname->addWidget(lEditSurname);

    vLayoutPersonal->addLayout(hLayoutSurname);

    QHBoxLayout* hLayoutName = new QHBoxLayout;
    hLayoutName->addWidget(new QLabel(tr("Name")));
    hLayoutName->addWidget(lEditName);

    vLayoutPersonal->addLayout(hLayoutName);

    QHBoxLayout* hLayoutMiddleName = new QHBoxLayout;
    hLayoutMiddleName->addWidget(new QLabel(tr("Middle name")));
    hLayoutMiddleName->addWidget(lEditMiddleName);

    vLayoutPersonal->addLayout(hLayoutMiddleName);

    QHBoxLayout* hLayoutGender = new QHBoxLayout;
    hLayoutGender->addWidget(new QLabel(tr("Gender")));
    hLayoutGender->addWidget(cmbBoxGender);

    vLayoutPersonal->addLayout(hLayoutGender);

    QHBoxLayout* hLayoutDateBirthdat = new QHBoxLayout;
    hLayoutDateBirthdat->addWidget(new QLabel(tr("Birthdate")));
    hLayoutDateBirthdat->addWidget(dateEditBirthday);

    vLayoutPersonal->addLayout(hLayoutDateBirthdat);

    QHBoxLayout* hLayoutDateSchool = new QHBoxLayout;
    hLayoutDateSchool->addWidget(new QLabel(tr("School graduation")));
    hLayoutDateSchool->addWidget(dateEditSchool);

    vLayoutPersonal->addLayout(hLayoutDateSchool);

    gBoxPersonal->setLayout(vLayoutPersonal);

    vLayout->addWidget(gBoxPersonal);

    //group
    QVBoxLayout* vLayoutGroup = new QVBoxLayout;
    gBoxGroup = new QGroupBox(tr("Group"));

    cmbBoxDepartment = new QComboBox;
    cmbBoxCourse = new QComboBox;
    cmbBoxGroup = new QComboBox;

    QHBoxLayout *hLayoutDepartment = new QHBoxLayout;

    hLayoutDepartment->addWidget(new QLabel(tr("Department")));
    hLayoutDepartment->addWidget(cmbBoxDepartment);

    vLayoutGroup->addLayout(hLayoutDepartment);

    QHBoxLayout *hLayoutCourse = new QHBoxLayout;
    hLayoutCourse->addWidget(new QLabel(tr("Course")));
    hLayoutCourse->addWidget(cmbBoxCourse);

    vLayoutGroup->addLayout(hLayoutCourse);

    QHBoxLayout *hLayoutGroup = new QHBoxLayout;

    hLayoutGroup->addWidget(new QLabel(tr("Group")));
    hLayoutGroup->addWidget(cmbBoxGroup);

    vLayoutGroup->addLayout(hLayoutGroup);

    gBoxGroup->setLayout(vLayoutGroup);

    vLayout->addWidget(gBoxGroup);

    setLayout(vLayout);
}





//ContactInfoPage


ContactInfoPage::ContactInfoPage(std::shared_ptr<StudentTemplate> _tmpStudent, QWidget *parent)
{
    tmpStudent = _tmpStudent;

    initElements();

    connect(cmbBoxCountry, SIGNAL(currentTextChanged(QString)), SLOT(loadCmbBoxRegion()));
    connect(cmbBoxRegion, SIGNAL(currentTextChanged(QString)), SLOT(loadCmbBoxLocality()));
    connect(cmbBoxLocality, SIGNAL(currentTextChanged(QString)), SLOT(loadCmbBoxDistrict()));
    connect(cmbBoxDistrict, SIGNAL(currentTextChanged(QString)), SLOT(loadCmbBoxStreet()));

    loadCmbBoxCountry();
}

void ContactInfoPage::initElements()
{
    vLayout = new QVBoxLayout;

    //adress
    groupBoxAdress = new QGroupBox(tr("Adress"));

    QVBoxLayout* vLayoutAdress = new QVBoxLayout;

    cmbBoxCountry = new QComboBox;
    cmbBoxRegion = new QComboBox;
    cmbBoxLocality = new QComboBox;
    cmbBoxDistrict = new QComboBox;
    cmbBoxStreet = new QComboBox;

    pBtnAddLocation = new QPushButton(tr("Add location"));
    connect(pBtnAddLocation, SIGNAL(clicked(bool)), SLOT(loadLocationDialog()));

    lEditHouse = new QLineEdit;
    lEditFlat = new QLineEdit;

    QHBoxLayout* hBoxCountry = new QHBoxLayout;
    hBoxCountry->addWidget(new QLabel(tr("Country")));
    hBoxCountry->addWidget(cmbBoxCountry);

    vLayoutAdress->addLayout(hBoxCountry);

    QHBoxLayout* hBoxRegion = new QHBoxLayout;
    hBoxRegion->addWidget(new QLabel(tr("Region")));
    hBoxRegion->addWidget(cmbBoxRegion);

    vLayoutAdress->addLayout(hBoxRegion);

    QHBoxLayout* hBoxCity = new QHBoxLayout;
    hBoxCity->addWidget(new QLabel(tr("Locality")));
    hBoxCity->addWidget(cmbBoxLocality);

    vLayoutAdress->addLayout(hBoxCity);

    QHBoxLayout* hBoxDistrict = new QHBoxLayout;
    hBoxDistrict->addWidget(new QLabel(tr("District")));
    hBoxDistrict->addWidget(cmbBoxDistrict);

    vLayoutAdress->addLayout(hBoxDistrict);

    QHBoxLayout* hBoxStreet = new QHBoxLayout;
    hBoxStreet->addWidget(new QLabel(tr("Street")));
    hBoxStreet->addWidget(cmbBoxStreet);

    vLayoutAdress->addLayout(hBoxStreet);

    vLayoutAdress->addWidget(pBtnAddLocation);

    QHBoxLayout* hBoxHouse = new QHBoxLayout;
    hBoxHouse->addWidget(new QLabel(tr("House")));
    hBoxHouse->addWidget(lEditHouse);

    vLayoutAdress->addLayout(hBoxHouse);

    QHBoxLayout* hBoxFlat = new QHBoxLayout;
    hBoxFlat->addWidget(new QLabel(tr("Flat")));
    hBoxFlat->addWidget(lEditFlat);

    vLayoutAdress->addLayout(hBoxFlat);


    groupBoxAdress->setLayout(vLayoutAdress);

    vLayout->addWidget(groupBoxAdress);

    //number
    groupBoxNumber = new QGroupBox(tr("Number"));

    QHBoxLayout* hLayoutNumber = new QHBoxLayout;


    lEditNumber = new QLineEdit;

    cmbBoxType = new QComboBox;
    cmbBoxType->addItem(tr("Mobile"), QVariant("mobile"));
    cmbBoxType->addItem(tr("Home"), QVariant("home"));

    hLayoutNumber->addWidget(cmbBoxType);
    hLayoutNumber->addWidget(lEditNumber);

    groupBoxNumber->setLayout(hLayoutNumber);

    vLayout->addWidget(groupBoxNumber);

    setLayout(vLayout);
}

void ContactInfoPage::loadCmbBoxCountry()
{
    cmbBoxCountry->clear();

    StudentDialog::addCmbBoxItems(cmbBoxCountry, tmpStudent->getCountries());
}

void ContactInfoPage::loadCmbBoxRegion()
{
    cmbBoxRegion->clear();

    StudentDialog::addCmbBoxItems(cmbBoxRegion, tmpStudent->getRegions(cmbBoxCountry->currentData().toString()));
}

void ContactInfoPage::loadCmbBoxLocality()
{
    cmbBoxLocality->clear();

    StudentDialog::addCmbBoxItems(cmbBoxLocality, tmpStudent->getLocalities(cmbBoxRegion->currentData().toString()));
}

void ContactInfoPage::loadCmbBoxDistrict()
{
    cmbBoxDistrict->clear();

    StudentDialog::addCmbBoxItems(cmbBoxDistrict, tmpStudent->getDistricts(cmbBoxLocality->currentData().toString()));
}

void ContactInfoPage::loadCmbBoxStreet()
{
    cmbBoxStreet->clear();

    StudentDialog::addCmbBoxItems(cmbBoxStreet, tmpStudent->getStreets(cmbBoxDistrict->currentData().toString()));
}

void ContactInfoPage::loadLocationDialog()
{
    LocationDialog dialogLocation(tmpStudent);

    connect(&dialogLocation, SIGNAL(countryAdded()), this, SLOT(loadCmbBoxCountry()));
    connect(&dialogLocation, SIGNAL(regionAdded()), this, SLOT(loadCmbBoxRegion()));
    connect(&dialogLocation, SIGNAL(localityAdded()), this, SLOT(loadCmbBoxLocality()));
    connect(&dialogLocation, SIGNAL(districtAdded()), this, SLOT(loadCmbBoxDistrict()));
    connect(&dialogLocation, SIGNAL(streetAdded()), this, SLOT(loadCmbBoxStreet()));

    dialogLocation.exec();
}



//Additional info page


AdditionalInfoPage::AdditionalInfoPage(QWidget *parent)
{
    initElements();
}

void AdditionalInfoPage::initElements()
{
    vLayout = new QVBoxLayout;

    cmbBoxStatus = new QComboBox;
    cmbBoxStatus->addItem(tr("No status"), QVariant("NULL"));
    cmbBoxStatus->addItem(tr("Orphan"), QVariant("orphan"));
    cmbBoxStatus->addItem(tr("Incomplete family"), QVariant("incomplete_family"));

    QHBoxLayout *hLayoutStatus = new QHBoxLayout;
    hLayoutStatus->addWidget(new QLabel(tr("Status")));
    hLayoutStatus->addWidget(cmbBoxStatus);

    vLayout->addLayout(hLayoutStatus);
    vLayout->addStretch(1);

    setLayout(vLayout);
}

