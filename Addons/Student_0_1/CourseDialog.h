#ifndef COURSE_H
#define COURSE_H

#include <memory>

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>

#include "StudentTemplate.h"

class CourseDialog : public QDialog
{
    Q_OBJECT


//VARIABLES========================================================================================


    std::shared_ptr<StudentTemplate> tmpStudent;


//ELEMENTS======================================================================================

    QComboBox* cmbBoxCourse;
    QLineEdit* lEditCode;

    QPushButton* pBtnConfirm;
public:
    explicit CourseDialog(std::shared_ptr<StudentTemplate> _tmpStudent, QWidget *parent = 0);



    void initElements();

signals:

    void confirmed();

public slots:

    void confirm();
};

#endif // COURSE_H
