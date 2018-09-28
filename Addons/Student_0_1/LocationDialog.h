#ifndef LOCATIONDIALOG_H
#define LOCATIONDIALOG_H

#include <QDialog>
#include <memory>
#include <QComboBox>
#include <QVector>

#include "StudentTemplate.h"
#include <QSignalMapper>

class LocationDialog : public QDialog
{
    Q_OBJECT

    std::shared_ptr<StudentTemplate> tmpStudent;

//ELEMENTS==================================================================

    QSignalMapper* mMapper;

    QVector<QLineEdit*> lstLineEdit;

    QComboBox* cmbBoxCountry;
    QLineEdit* lEditCountry;
    QPushButton* pBtnCountry;
    QPushButton* pBtnChangeCountry;
    QPushButton* pBtnDeleteCountry;

    QComboBox* cmbBoxRegion;
    QLineEdit* lEditRegion;
    QPushButton* pBtnRegion;
    QPushButton* pBtnChangeRegion;
    QPushButton* pBtnDeleteRegion;

    QComboBox* cmbBoxLocality;
    QComboBox* cmbBoxType;
    QLineEdit* lEditLocality;
    QPushButton* pBtnLocality;
    QPushButton* pBtnChangeLocality;
    QPushButton* pBtnDeleteLocality;

    QComboBox* cmbBoxDistrict;
    QLineEdit* lEditDistrict;
    QPushButton* pBtnDistrict;
    QPushButton* pBtnChangeDistrict;
    QPushButton* pBtnDeleteDistrict;

    QComboBox* cmbBoxStreet;
    QLineEdit* lEditStreet;
    QPushButton* pBtnStreet;
    QPushButton* pBtnChangeStreet;
    QPushButton* pBtnDeleteStreet;

    QPushButton* pBtnConfirm;

    void initElements();


//METHODS===============================================================

    void addCmbBoxItems(QComboBox *cmbBox, QList<QPair<QString, QString>> listPairs);

public:

    explicit LocationDialog(std::shared_ptr<StudentTemplate> _tmpStudent, QWidget *parent = 0);

signals:

    void countryAdded();
    void regionAdded();
    void localityAdded();
    void districtAdded();
    void streetAdded();

    void localityUpdated();

public slots:

    void updateObjectName(int addressID);

    void loadCmbBoxCountry();
    void loadCmbBoxRegion();
    void loadCmbBoxLocality();
    void loadCmbBoxDistrict();
    void loadCmbBoxStreet();

    void addCountry();
    void deleteRecCountry();

    void addRegion();
    void deleteRecRegion();

    void addLocality();
    void updateLocalityName();
    void deleteRecLocality();

    void addDistrict();
    void deleteRecDistrict();

    void addStreet();
    void deleteRecStreet();

    //clear all fields slot

    void clearFields();
};

#endif // ADDRESSDIALOG_H
