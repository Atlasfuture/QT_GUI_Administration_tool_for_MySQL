#ifndef STUDENTVIEWWIDGET_H
#define STUDENTVIEWWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QVBoxLayout>
#include <QLabel>

#include <memory>


#include "Core/QSqlRelationalLib/Headers/QSqlRelationalDataModel.h"
#include "StudentTemplate.h"
#include "StudentDialog.h"


class StudentViewWidget : public QWidget
{
    Q_OBJECT

    QString currentDepartment;
    QString currentCourse;
    QString currentGroup;

    std::shared_ptr<StudentTemplate> tmpStudent;
    std::shared_ptr<QSqlRelationalDataModel> sqlDataModel;



    //elements

    QLabel* labelText;
    QLabel* labelCount;
    QTableView* tableView;

    //layouts
    QVBoxLayout* vLayout;


    void initElements();
    void initText();

    QString studentId;
    QString studentSName;
    QString studentName;
    QString studentMName;
    QString studentGroup;
    QString studentCourse;

//EVENTS================================================

    void closeEvent(QCloseEvent *event);


public:
    explicit StudentViewWidget(std::shared_ptr<StudentTemplate> tmpStudent, QWidget *parent = 0);

signals:
    void locationAdded();
public slots:



    void initContextMenu(QPoint point);   //initialize context menu


    void deleteStudent();                 //delete selected student
    void loadStAddDialog();                    //calls student dialog in CREATE_MODE
    void loadStInfoDialog();               //calls student dialog in UPDATE_MODE
    void refreshSelection();                  //refresh current selection

    void countRecords();

    //set current data slots

    void setDepartment(QString department);
    void setCourse(QString course);
    void setGroup(QString group);
};

#endif // STUDENTVIEWWIDGET_H
