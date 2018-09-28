#include "Interface/Widgets/Header/CreateDBwidget.h"

#include <QMessageBox>

#include <QVBoxLayout>
#include <QIntValidator>
#include <QEvent>

CreateDBWidget::CreateDBWidget(std::shared_ptr<QSqlRelationalDataModel> _sqlModel, QWidget *parent)
{
    sqlModel = _sqlModel;

    initElements();

    connect(pBtnConfirm, SIGNAL(clicked(bool)), this, SLOT(createDataBase()));

    initText();
}

void CreateDBWidget::initElements()
{
    labelDbName = new QLabel(this);

    lEditDbName = new QLineEdit(this);

    pBtnConfirm = new QPushButton(this);

    QVBoxLayout *vLayout = new QVBoxLayout;

    vLayout->addWidget(labelDbName);
    vLayout->addWidget(lEditDbName);
    vLayout->addWidget(pBtnConfirm);

    setLayout(vLayout);
}

void CreateDBWidget::initText()
{
    labelDbName->setText(tr("Input database name"));
    pBtnConfirm->setText(tr("confirm"));
}



//EVENT====================================================================================================



void CreateDBWidget::closeEvent(QCloseEvent *event)
{
    emit closed();
}



void CreateDBWidget::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        initText();
    }
}


//CREATE DATABASE SLOT=======================================================================================

void CreateDBWidget::createDataBase()
{
    QString strDbName = lEditDbName->text();

    if (strDbName == "")
    {
        QMessageBox errorMsg;
        errorMsg.setText(tr("The database name is not entered"));
        errorMsg.exec();

        return;
    }


    QIntValidator intValidator;
    int pos = 0;

    if(intValidator.validate(strDbName, pos) == QIntValidator::Acceptable)
    {
        QMessageBox errorMsg;
        errorMsg.setText(tr("The database name must contain letters"));
        errorMsg.exec();

        return;
    }

    if (sqlModel->createDataBase(lEditDbName->text()))
    {
        QString strTmp = tr("Database ") + lEditDbName->text() + tr(" created");

        QMessageBox message;
        message.setText(strTmp);
        message.exec();

       // parent()->deleteLater();

        emit dbCreated();
    }
    else
    {
        QMessageBox errorMsg;
        errorMsg.setText(sqlModel->lastError().text());
        errorMsg.exec();
    }
}
