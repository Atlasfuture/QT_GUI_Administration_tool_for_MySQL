#include "LocationDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>


LocationDialog::LocationDialog(std::shared_ptr<StudentTemplate> _tmpStudent, QWidget *parent) : QDialog(parent)
{
    tmpStudent = _tmpStudent;

    mMapper = new QSignalMapper(this);

    connect(mMapper, SIGNAL(mapped(int)), SLOT(updateObjectName(int)));

    initElements();

    connect(cmbBoxCountry, SIGNAL(currentTextChanged(QString)), SLOT(loadCmbBoxRegion()));
    connect(this, SIGNAL(countryAdded()), this, SLOT(loadCmbBoxCountry()));

    connect(cmbBoxRegion, SIGNAL(currentTextChanged(QString)), SLOT(loadCmbBoxLocality()));
    connect(this, SIGNAL(regionAdded()), this, SLOT(loadCmbBoxRegion()));

    connect(cmbBoxLocality, SIGNAL(currentTextChanged(QString)), SLOT(loadCmbBoxDistrict()));
    connect(this, SIGNAL(localityAdded()), this, SLOT(loadCmbBoxLocality()));

    connect(cmbBoxDistrict, SIGNAL(currentTextChanged(QString)), SLOT(loadCmbBoxStreet()));
    connect(this, SIGNAL(districtAdded()), this, SLOT(loadCmbBoxDistrict()));

    connect(this, SIGNAL(streetAdded()), this, SLOT(loadCmbBoxStreet()));


    loadCmbBoxCountry();
}




//METHODS=======================================================================================================


void LocationDialog::addCmbBoxItems(QComboBox *cmbBox, QList<QPair<QString, QString>> listPairs)
{
    for (int i = 0; i < listPairs.size(); i++)
    {
        cmbBox->addItem(listPairs.at(i).second, QVariant(listPairs.at(i).first));
    }
}



void LocationDialog::initElements()
{
    QVBoxLayout* vLayoutMain = new QVBoxLayout;

    //country subsection
    cmbBoxCountry = new QComboBox;
    lEditCountry = new QLineEdit;
    lstLineEdit.push_back(lEditCountry);

    pBtnCountry = new QPushButton(tr("Add"));
    connect(pBtnCountry, SIGNAL(clicked(bool)), SLOT(addCountry()));

    pBtnChangeCountry = new QPushButton(tr("Change"));
    mMapper->setMapping(pBtnChangeCountry, StudentTemplate::COUNTRY);
    connect(pBtnChangeCountry, SIGNAL(clicked(bool)), mMapper, SLOT(map()));

    pBtnDeleteCountry = new QPushButton(tr("Delete"));
    connect(pBtnDeleteCountry, SIGNAL(clicked(bool)), SLOT(deleteRecCountry()));

    //region subsection
    cmbBoxRegion = new QComboBox;
    lEditRegion = new QLineEdit;
    lstLineEdit.push_back(lEditRegion);

    pBtnRegion = new QPushButton(tr("Add"));
    connect(pBtnRegion, SIGNAL(clicked(bool)), SLOT(addRegion()));

    pBtnChangeRegion = new QPushButton(tr("Change"));
    mMapper->setMapping(pBtnChangeRegion, StudentTemplate::REGION);
    connect(pBtnChangeRegion, SIGNAL(clicked(bool)), mMapper, SLOT(map()));

    pBtnDeleteRegion = new QPushButton(tr("Delete"));
    connect(pBtnDeleteRegion, SIGNAL(clicked(bool)), SLOT(deleteRecRegion()));

    //locality subsection
    cmbBoxType = new QComboBox;
    cmbBoxType->addItem(tr("city"), QVariant("city"));
    cmbBoxType->addItem(tr("village"), QVariant("village"));
    cmbBoxType->addItem(tr("township"), QVariant("township"));


    cmbBoxLocality = new QComboBox;
    lEditLocality = new QLineEdit;
    lstLineEdit.push_back(lEditLocality);

    pBtnLocality = new QPushButton(tr("Add"));
    connect(pBtnLocality, SIGNAL(clicked(bool)), SLOT(addLocality()));

    pBtnChangeLocality = new QPushButton(tr("Change"));
    connect(pBtnChangeLocality, SIGNAL(clicked(bool)), SLOT(updateLocalityName()));

    pBtnDeleteLocality = new QPushButton(tr("Delete"));
    connect(pBtnDeleteLocality, SIGNAL(clicked(bool)), SLOT(deleteRecLocality()));

    //district subsection
    cmbBoxDistrict = new QComboBox;
    lEditDistrict = new QLineEdit;
    lstLineEdit.push_back(lEditDistrict);

    pBtnDistrict = new QPushButton(tr("Add"));
    connect(pBtnDistrict, SIGNAL(clicked(bool)), SLOT(addDistrict()));

    pBtnChangeDistrict = new QPushButton(tr("Change"));
    mMapper->setMapping(pBtnChangeDistrict, StudentTemplate::DISTRICT);
    connect(pBtnChangeDistrict, SIGNAL(clicked(bool)), mMapper, SLOT(map()));

    pBtnDeleteDistrict = new QPushButton(tr("Delete"));
    connect(pBtnDeleteDistrict, SIGNAL(clicked(bool)), SLOT(deleteRecDistrict()));

    //street subsection
    cmbBoxStreet = new QComboBox;
    lEditStreet = new QLineEdit;
    lstLineEdit.push_back(lEditStreet);

    pBtnStreet = new QPushButton(tr("Add"));
    connect(pBtnStreet, SIGNAL(clicked(bool)), SLOT(addStreet()));

    pBtnChangeStreet = new QPushButton(tr("Change"));
    mMapper->setMapping(pBtnChangeStreet, StudentTemplate::STREET);
    connect(pBtnChangeStreet, SIGNAL(clicked(bool)), mMapper, SLOT(map()));

    pBtnDeleteStreet = new QPushButton(tr("Delete"));
    connect(pBtnDeleteStreet, SIGNAL(clicked(bool)), SLOT(deleteRecStreet()));

    QHBoxLayout* hBoxCountry = new QHBoxLayout;
    hBoxCountry->addWidget(new QLabel(tr("Country")));
    hBoxCountry->addWidget(cmbBoxCountry);
    hBoxCountry->addWidget(lEditCountry);
    hBoxCountry->addWidget(pBtnCountry);
    hBoxCountry->addWidget(pBtnChangeCountry);
    hBoxCountry->addWidget(pBtnDeleteCountry);

    vLayoutMain->addLayout(hBoxCountry);

    QHBoxLayout* hBoxRegion = new QHBoxLayout;
    hBoxRegion->addWidget(new QLabel(tr("Region")));
    hBoxRegion->addWidget(cmbBoxRegion);
    hBoxRegion->addWidget(lEditRegion);
    hBoxRegion->addWidget(pBtnRegion);
    hBoxRegion->addWidget(pBtnChangeRegion);
    hBoxRegion->addWidget(pBtnDeleteRegion);

    vLayoutMain->addLayout(hBoxRegion);

    QHBoxLayout* hBoxCity = new QHBoxLayout;
    hBoxCity->addWidget(new QLabel(tr("Locality")));
    hBoxCity->addWidget(cmbBoxLocality);
    hBoxCity->addWidget(new QLabel(tr("Type")));
    hBoxCity->addWidget(cmbBoxType);
    hBoxCity->addWidget(lEditLocality);
    hBoxCity->addWidget(pBtnLocality);
    hBoxCity->addWidget(pBtnChangeLocality);
    hBoxCity->addWidget(pBtnDeleteLocality);

    vLayoutMain->addLayout(hBoxCity);

    QHBoxLayout* hBoxDistrict = new QHBoxLayout;
    hBoxDistrict->addWidget(new QLabel(tr("District")));
    hBoxDistrict->addWidget(cmbBoxDistrict);
    hBoxDistrict->addWidget(lEditDistrict);
    hBoxDistrict->addWidget(pBtnDistrict);
    hBoxDistrict->addWidget(pBtnChangeDistrict);
    hBoxDistrict->addWidget(pBtnDeleteDistrict);

    vLayoutMain->addLayout(hBoxDistrict);

    QHBoxLayout* hBoxStreet = new QHBoxLayout;
    hBoxStreet->addWidget(new QLabel(tr("Street")));
    hBoxStreet->addWidget(cmbBoxStreet);
    hBoxStreet->addWidget(lEditStreet);
    hBoxStreet->addWidget(pBtnStreet);
    hBoxStreet->addWidget(pBtnChangeStreet);
    hBoxStreet->addWidget(pBtnDeleteStreet);

    vLayoutMain->addLayout(hBoxStreet);

    vLayoutMain->addStretch(1);


    setLayout(vLayoutMain);
}



//SLOTS==================================================================================================



void LocationDialog::loadCmbBoxCountry()
{
    cmbBoxCountry->clear();

    addCmbBoxItems(cmbBoxCountry, tmpStudent->getCountries());
}

void LocationDialog::loadCmbBoxRegion()
{
    cmbBoxRegion->clear();

    addCmbBoxItems(cmbBoxRegion, tmpStudent->getRegions(cmbBoxCountry->currentData().toString()));
}

void LocationDialog::loadCmbBoxLocality()
{
    cmbBoxLocality->clear();

    addCmbBoxItems(cmbBoxLocality, tmpStudent->getLocalities(cmbBoxRegion->currentData().toString()));
}

void LocationDialog::loadCmbBoxDistrict()
{
    cmbBoxDistrict->clear();

    addCmbBoxItems(cmbBoxDistrict, tmpStudent->getDistricts(cmbBoxLocality->currentData().toString()));
}

void LocationDialog::loadCmbBoxStreet()
{
    cmbBoxStreet->clear();

    addCmbBoxItems(cmbBoxStreet, tmpStudent->getStreets(cmbBoxDistrict->currentData().toString()));
}

void LocationDialog::addCountry()
{
    if (tmpStudent->addCountry(lEditCountry->text()))
    {
        emit countryAdded();
        lEditCountry->clear();
    }
}


void LocationDialog::deleteRecCountry()
{
    tmpStudent->deleteRecCountry(cmbBoxCountry->currentData().toString());

    emit countryAdded();
}



void LocationDialog::addRegion()
{
    if(tmpStudent->addRegion(lEditRegion->text(), cmbBoxCountry->currentData().toString()))
    {
        emit regionAdded();
        lEditRegion->clear();
    }
}

void LocationDialog::deleteRecRegion()
{
    tmpStudent->deleteRecRegions(cmbBoxRegion->currentData().toString());
    emit regionAdded();
}



void LocationDialog::addLocality()
{
    if(tmpStudent->addLocality(lEditLocality->text(), cmbBoxType->currentText(), cmbBoxRegion->currentData().toString()))
    {
        emit localityAdded();
        lEditLocality->clear();
    }
}



void LocationDialog::updateLocalityName()
{
    StudentTemplate::LOCALITY_TYPE locType;

    if (cmbBoxType->currentData().toString() == "city")
    {
        locType = StudentTemplate::CITY;
    }
    else if (cmbBoxType->currentData().toString() == "village")
    {
        locType = StudentTemplate::VILLAGE;
    }
    else
    {
        locType = StudentTemplate::TOWNSHIP;
    }

    if(tmpStudent->updateLocalityName(cmbBoxLocality->currentData().toString(), lEditLocality->text(), locType))
    {
        emit localityAdded();
        lEditLocality->clear();
    }
}

void LocationDialog::deleteRecLocality()
{
    tmpStudent->deleteRecLocality(cmbBoxLocality->currentData().toString());
    emit localityAdded();
}



void LocationDialog::addDistrict()
{
    if(tmpStudent->addDistrict(lEditDistrict->text(), cmbBoxLocality->currentData().toString()))
    {
        emit districtAdded();
        lEditDistrict->clear();
    }
}

void LocationDialog::deleteRecDistrict()
{
    tmpStudent->deleteRecDistrict(cmbBoxDistrict->currentData().toString());
    emit districtAdded();
}



void LocationDialog::addStreet()
{
    if(tmpStudent->addStreet(lEditStreet->text(), cmbBoxDistrict->currentData().toString()))
    {
        emit streetAdded();
        lEditStreet->clear();
    }
}

void LocationDialog::deleteRecStreet()
{
    tmpStudent->deleteRecStreet(cmbBoxStreet->currentData().toString());
    emit streetAdded();
}

void LocationDialog::clearFields()
{
    foreach(QLineEdit* lineEdit, lstLineEdit)
    {
        lineEdit->setText("");
    }
}

void LocationDialog::updateObjectName(int address)
{
    QComboBox* cmbBox;
    QLineEdit* lEdit;

    StudentTemplate::ADDRESS addressID;

    switch (address)
    {
        case StudentTemplate::COUNTRY:
            cmbBox = cmbBoxCountry;
            lEdit = lEditCountry;
            addressID = StudentTemplate::COUNTRY;
        break;

        case StudentTemplate::REGION:
            cmbBox = cmbBoxRegion;
            lEdit = lEditRegion;
            addressID = StudentTemplate::REGION;
        break;

        case StudentTemplate::DISTRICT:
            cmbBox = cmbBoxDistrict;
            lEdit = lEditDistrict;
            addressID = StudentTemplate::DISTRICT;
        break;

        case StudentTemplate::STREET:
            cmbBox = cmbBoxStreet;
            lEdit = lEditStreet;
            addressID = StudentTemplate::STREET;
        break;
    }

    if(tmpStudent->updateAdressObject(addressID, cmbBox->currentData().toString(), lEdit->text()))
    {
        switch (address)
        {
            case StudentTemplate::COUNTRY:
                emit countryAdded();
            break;

            case StudentTemplate::REGION:
                emit regionAdded();
            break;

            case StudentTemplate::DISTRICT:
                emit districtAdded();
            break;

            case StudentTemplate::STREET:
                emit streetAdded();
            break;
        }

        cmbBox->setCurrentText(lEdit->text());
    }
}


