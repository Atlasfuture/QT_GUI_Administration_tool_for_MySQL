#ifndef DEPARTMENTDIALOG_H
#define DEPARTMENTDIALOG_H

#include <memory>

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>

#include "StudentTemplate.h"

class DepartmentDialog : public QDialog
{
    Q_OBJECT

    std::shared_ptr<StudentTemplate> tmpStudent;



//ELEMENTS===============================================================



    QLineEdit* lEditFullName;
    QLineEdit* lEditShortName;
    QLineEdit* lEditCode;
    QPushButton* pBtnConfirm;

    enum MODE
    {
        CREATE = 0,
        UPDATE = 1
    };

    MODE dialogMode;

    QString departmentID;

    void loadData();
public:
    DepartmentDialog(std::shared_ptr<StudentTemplate> _tmpStudent, QWidget *parent = 0);
    DepartmentDialog(std::shared_ptr<StudentTemplate> _tmpStudent, QString _departmentID, QWidget *parent = 0);



    //elements
    void initElements();
signals:
    void created();
    void updated();

    void error();
public slots:
    void confirm();
};

#endif // DEPARTMENTDIALOG_H
