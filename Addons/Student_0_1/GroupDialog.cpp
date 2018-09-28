#include "GroupDialog.h"
#include <QLabel>
#include <QMessageBox>


GroupDialog::GroupDialog(std::shared_ptr<StudentTemplate> _tmpStudent, QWidget *parent) : QDialog(parent)
{ 
    dialogMode = CREATE;

    tmpStudent = _tmpStudent;

    initElements();
}

GroupDialog::GroupDialog(std::shared_ptr<StudentTemplate> _tmpStudent, QString _groupId, QWidget *parent)
{
    dialogMode = UPDATE;

    tmpStudent = _tmpStudent;

    groupID = _groupId;

    initElements();
}

void GroupDialog::loadData(QString departmentName, QString course)
{
    cmbBoxDepartment->setCurrentText(departmentName);
    cmbBoxCourse->setCurrentText(course);
}

void GroupDialog::loadData(QString departmentName, QString course, QString group)
{
    lEditName->setText(group);
    cmbBoxDepartment->setCurrentText(departmentName);
    cmbBoxCourse->setCurrentText(course);
}

void GroupDialog::initElements()
{
    lEditName = new QLineEdit;

    cmbBoxCourse = new QComboBox;
    cmbBoxCourse->addItems(tmpStudent->getCourses());



    cmbBoxDepartment = new QComboBox;

    QList<QPair<QString, QString>> listPairs = tmpStudent->getDepartments();

    for (int i = 0; i < listPairs.size(); i++)
    {
        cmbBoxDepartment->addItem(listPairs.at(i).second, listPairs.at(i).first);
    }

    pBtnConfirm = new QPushButton(tr("Confirm"));

    vLayout = new QVBoxLayout;

    vLayout->addWidget(new QLabel(tr("Group name")));
    vLayout->addWidget(lEditName);

    vLayout->addWidget(new QLabel(tr("Course")));
    vLayout->addWidget(cmbBoxCourse);

    vLayout->addWidget(new QLabel(tr("Department")));
    vLayout->addWidget(cmbBoxDepartment);

    vLayout->addWidget(pBtnConfirm);

    setLayout(vLayout);

    connect(pBtnConfirm, SIGNAL(clicked(bool)), this, SLOT(confirm()));
}

void GroupDialog::confirm()
{
    if (lEditName->text() != "")
    {
        if (dialogMode == CREATE)
        {
             if (tmpStudent->createGroup(lEditName->text(), cmbBoxCourse->currentText(), cmbBoxDepartment->currentData().toString()))
             {
                emit groupCreated();
                close();
             }
             else
             {
                emit error();
             }
        }
        else
        {
             if (tmpStudent->updateGroup(groupID, lEditName->text(), cmbBoxCourse->currentText(), cmbBoxDepartment->currentData().toString()))
             {
                 emit groupUpdated();
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
