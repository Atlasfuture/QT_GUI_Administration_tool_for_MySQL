#include "CourseDialog.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>



CourseDialog::CourseDialog(std::shared_ptr<StudentTemplate> _tmpStudent, QWidget *parent) : QDialog(parent)
{
    tmpStudent = _tmpStudent;

    initElements();

    connect(pBtnConfirm, SIGNAL(clicked(bool)), SLOT(confirm()));
}

void CourseDialog::initElements()
{
    QVBoxLayout* vMainLayout = new QVBoxLayout;

    QHBoxLayout* hLayoutCourse = new QHBoxLayout;
    vMainLayout->addLayout(hLayoutCourse);

    cmbBoxCourse = new QComboBox;

    for (int i = 1; i < 10; i++)
    {
        cmbBoxCourse->addItem(QString::number(i));
    }

    hLayoutCourse->addWidget(new QLabel(tr("Course")));
    hLayoutCourse->addWidget(cmbBoxCourse);

    QHBoxLayout* hLayoutCode = new QHBoxLayout;
    vMainLayout->addLayout(hLayoutCode);

    lEditCode = new QLineEdit;

    hLayoutCode->addWidget(new QLabel(tr("Code")));
    hLayoutCode->addWidget(lEditCode);

    pBtnConfirm = new QPushButton(tr("Confirm"));

    vMainLayout->addWidget(pBtnConfirm);

    setLayout(vMainLayout);
}




//SLOTS================================================================================================




void CourseDialog::confirm()
{
    tmpStudent->createCourse(cmbBoxCourse->currentText(), lEditCode->text());

    emit confirmed();

    close();
}
