#include "StudentViewWidget.h"

#include <QMessageBox>
#include <QMenu>
#include <QAction>

#include <QPushButton>
#include <QSqlRecord>
#include <QSortFilterProxyModel>

#include <QTableWidget>


#include <QDebug>

StudentViewWidget::StudentViewWidget(std::shared_ptr<StudentTemplate> _tmpStudent, QWidget *parent) : QWidget(parent)
{
    tmpStudent = _tmpStudent;
    sqlDataModel = tmpStudent->getModel();

    tableView = new QTableView;


    initElements();



    tableView->setModel(sqlDataModel.get());
    tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(initContextMenu(QPoint)));

    initText();

    refreshSelection();
}

void StudentViewWidget::initElements()
{
    vLayout = new QVBoxLayout;


    labelText = new QLabel;

    labelCount = new QLabel;

    QHBoxLayout* hBoxLayoutCount = new QHBoxLayout;
    hBoxLayoutCount->addWidget(labelText);
    hBoxLayoutCount->addWidget(labelCount);
    hBoxLayoutCount->addStretch(1);

    vLayout->addLayout(hBoxLayoutCount);
    vLayout->addWidget(tableView);

    setLayout(vLayout);
}

void StudentViewWidget::initText()
{
    studentId = tr("Student id");
    studentSName = tr("Surname");
    studentName = tr("Name");
    studentMName = tr("Middle name");
    studentGroup = tr("Group name");
    studentCourse = tr("Course");

    labelText->setText(tr("Record count:"));
}

void StudentViewWidget::closeEvent(QCloseEvent *event)
{
    parent()->deleteLater();
}


//SLOTS

void StudentViewWidget::initContextMenu(QPoint point)
{
    QMenu* menu = new QMenu(this);

    QAction* aAddStudent = new QAction(tr("Add student"), this);
    connect(aAddStudent, SIGNAL(triggered(bool)), this, SLOT(loadStAddDialog()));
    menu->addAction(aAddStudent);

    QAction* aDeleteStudent = new QAction(tr("Delete student"), this);
    connect(aDeleteStudent, SIGNAL(triggered(bool)), this, SLOT(deleteStudent()));
    menu->addAction(aDeleteStudent);

    QAction* aShowInfo = new QAction(tr("Full information"), this);
    connect(aShowInfo, SIGNAL(triggered(bool)), this, SLOT(loadStInfoDialog()));
    menu->addAction(aShowInfo);

    menu->popup(this->mapToGlobal(point));
}

void StudentViewWidget::deleteStudent()
{
    int res = QMessageBox::question(this, tr("Student deleting"), tr("Are you sure?"), QMessageBox::Yes | QMessageBox::No);

    if (res == QMessageBox::Yes)
    {
        QString studentId = sqlDataModel->record(tableView->currentIndex().row()).value(0).toString();

        tmpStudent->deleteStudent(studentId);

        refreshSelection();
    }

    countRecords();
}

void StudentViewWidget::loadStAddDialog()
{
    if (tmpStudent->getGroups().isEmpty())
    {
        QMessageBox msgBoxError;
        msgBoxError.setText(tr("Add at least one group"));
        msgBoxError.exec();
        return;
    }

    QString stId = sqlDataModel->record(tableView->currentIndex().row()).value(0).toString();

    StudentDialog dialogAddStudent(tmpStudent, stId, StudentDialog::CREATE_MODE, this);


    connect(&dialogAddStudent, SIGNAL(confirmed()), SLOT(refreshSelection()));
    connect(&dialogAddStudent, SIGNAL(confirmed()), SLOT(countRecords()));

    dialogAddStudent.setCurrentGroup(currentDepartment, currentCourse, currentGroup);

    dialogAddStudent.exec();


    emit locationAdded();
}

void StudentViewWidget::loadStInfoDialog()
{
    QString stId = sqlDataModel->record(tableView->currentIndex().row()).value(0).toString();

    StudentDialog dialogUpdateStudent(tmpStudent, stId, StudentDialog::UPDATE_MODE, this);
    connect(&dialogUpdateStudent, SIGNAL(confirmed()), SLOT(refreshSelection()));


    dialogUpdateStudent.exec();

    emit locationAdded();
}

void StudentViewWidget::refreshSelection()
{
    tmpStudent->runTableFilter();



    tableView->hideColumn(4);
    tableView->hideColumn(5);
    tableView->hideColumn(6);
    tableView->hideColumn(7);
    tableView->hideColumn(8);
    tableView->hideColumn(9);
    tableView->hideColumn(10);

    tableView->hideColumn(12);

    for (int i = 14; i < 40; i++)
    {
        tableView->hideColumn(i);
    }

    sqlDataModel->setHeaderData(0, Qt::Horizontal, studentId);
    sqlDataModel->setHeaderData(1, Qt::Horizontal, studentSName);
    sqlDataModel->setHeaderData(2, Qt::Horizontal, studentName);
    sqlDataModel->setHeaderData(3, Qt::Horizontal, studentMName);
    sqlDataModel->setHeaderData(11, Qt::Horizontal, studentGroup);
    sqlDataModel->setHeaderData(13, Qt::Horizontal, studentCourse);
}

void StudentViewWidget::countRecords()
{
    labelCount->setText(QString::number(sqlDataModel->rowCount()));
}

void StudentViewWidget::setDepartment(QString department)
{
    currentDepartment = department;

    qDebug() << "SET CURRENT DEPARTMENT" << department;
}

void StudentViewWidget::setCourse(QString course)
{
    currentCourse = course;

    qDebug() << "SET CURRENT COURSE" << course;
}

void StudentViewWidget::setGroup(QString group)
{
    currentGroup = group;

    qDebug() << "SET CURRENT GROUP" << group;
}


