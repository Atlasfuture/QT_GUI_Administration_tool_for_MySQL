#include "DepartmentDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QErrorMessage>
#include <QLabel>

DepartmentDialog::DepartmentDialog(std::shared_ptr<StudentTemplate> _tmpStudent, QWidget *parent) : QDialog(parent)
{
    dialogMode = CREATE;

    tmpStudent = _tmpStudent;

    initElements();

    connect(pBtnConfirm, SIGNAL(clicked(bool)), SLOT(confirm()));
}

DepartmentDialog::DepartmentDialog(std::shared_ptr<StudentTemplate> _tmpStudent, QString _departmentID, QWidget *parent)
{
    dialogMode = UPDATE;

    departmentID = _departmentID;

    tmpStudent = _tmpStudent;


    initElements();

    loadData();

    connect(pBtnConfirm, SIGNAL(clicked(bool)), SLOT(confirm()));
}

void DepartmentDialog::loadData()
{
    lEditFullName->setText(tmpStudent->getDepartmentFullName(departmentID));
    lEditShortName->setText(tmpStudent->getDepartmentShortName(departmentID));
    lEditCode->setText(tmpStudent->getDepartmentCode(departmentID));
}


void DepartmentDialog::initElements()
{
    QVBoxLayout *vMainLayout = new QVBoxLayout;

    QHBoxLayout *hLayoutFullName = new QHBoxLayout;
    vMainLayout->addLayout(hLayoutFullName);

    hLayoutFullName->addWidget(new QLabel(tr("Full name")));
    lEditFullName = new QLineEdit;
    hLayoutFullName->addWidget(lEditFullName);

    QHBoxLayout* hLayoutShortName = new QHBoxLayout;
    vMainLayout->addLayout(hLayoutShortName);

    hLayoutShortName->addWidget(new QLabel(tr("Short name")));
    lEditShortName = new QLineEdit;
    hLayoutShortName->addWidget(lEditShortName);

    QHBoxLayout* hLayoutCode = new QHBoxLayout;
    vMainLayout->addLayout(hLayoutCode);

    hLayoutCode->addWidget(new QLabel(tr("Code")));
    lEditCode = new QLineEdit;
    hLayoutCode->addWidget(lEditCode);

    pBtnConfirm = new QPushButton(tr("Confirm"));
    vMainLayout->addWidget(pBtnConfirm);

    setLayout(vMainLayout);
}


//SLOTS

void DepartmentDialog::confirm()
{
    if (lEditFullName->text() != "" && lEditShortName->text() != "" && lEditCode->text() != "")
    {
        if (dialogMode == CREATE)
        {
            if(tmpStudent->createDepartment(lEditFullName->text(), lEditShortName->text(), lEditCode->text()))
            {
                emit created();
                close();
            }
            else
            {
                emit error();
            }
        }
        else
        {
            if(tmpStudent->updateDepartment(departmentID, lEditFullName->text(), lEditShortName->text(), lEditCode->text()))
            {
                emit updated();
                close();
            }
            else
            {
                emit error();
            }
        }
    }
    else
    {
        emit error();
    }
}
