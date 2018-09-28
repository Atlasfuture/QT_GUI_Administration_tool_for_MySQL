#ifndef INSERTGROUPWIDGET_H
#define INSERTGROUPWIDGET_H

#include <memory>

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QVBoxLayout>

#include "StudentTemplate.h"

class GroupDialog : public QDialog
{
    Q_OBJECT

    std::shared_ptr<StudentTemplate> tmpStudent;

    //layout
    QVBoxLayout* vLayout;

    //elements
    QLineEdit* lEditName;
    QComboBox* cmbBoxCourse;
    QComboBox* cmbBoxDepartment;

    QPushButton* pBtnConfirm;

    void initElements();

    enum MODE
    {
        CREATE = 0,
        UPDATE = 1
    };

    MODE dialogMode;

    QString groupID;
public:



    GroupDialog(std::shared_ptr<StudentTemplate> _tmpStudent, QWidget *parent = 0);
    GroupDialog(std::shared_ptr<StudentTemplate> _tmpStudent, QString _groupId, QWidget *parent = 0);

    //sets current data in combo boxes of dialog
    void loadData(QString departmentName, QString course);
    void loadData(QString departmentName, QString course, QString group);

signals:

    void groupCreated();
    void groupUpdated();
    void error();
public slots:

    void confirm();
};

#endif // INSERTGROUPWIDGET_H
