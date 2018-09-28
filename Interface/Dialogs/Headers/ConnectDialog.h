#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <memory>
#include <QSqlDatabase>
#include "Core/QSqlRelationalLib/Headers/QSqlRelationalDataModel.h"

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QEvent>
#include <QLineEdit>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QComboBox>

#include <QApplication>
#include <QTranslator>

class ConnectDialog : public QDialog
{
    Q_OBJECT

    QApplication* app;
    QTranslator* translator;

    std::shared_ptr<QSqlDatabase> sqlDatabase;

    int currentIndex;

public:
    enum Languages
    {
        ENGLISH,
        UKRAINIAN
    };

    ConnectDialog(std::shared_ptr<QSqlDatabase> _sqlDatabase, QApplication* _app, QTranslator* _translator,  QWidget *parent = 0);


//elements-----------------------------------------------------------------------



    QPushButton* pBtnConfirm;
    QLineEdit* lineEdits;

    QLabel* labelSaveData;
    QCheckBox* chkBoxSave;

    QLabel* labelLanguage;
    QComboBox* cmbBoxLanguage;

    QString strEng;
    QString strUa;
    QString strRu;

    QLabel* labels;


//layouts-------------------------------------------------------------------------


    QVBoxLayout *vLayout;
    QHBoxLayout *hLayout;

//INIT METHODS==================================


    void initElements();
    void initText();

//EVENTS====================================


    void showEvent(QShowEvent* event);
    void changeEvent(QEvent* event);

signals:
    void widgetClosed();
    void confirmed();
public slots:
    void confirmData();
    void changeLanguage();
    void saveData();
};

#endif // CONNECTDIALOG_H
