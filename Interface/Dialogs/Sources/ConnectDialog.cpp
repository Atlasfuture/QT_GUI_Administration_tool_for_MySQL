#include "Interface/Dialogs/Headers/ConnectDialog.h"
#include <QSqlError>
#include <QErrorMessage>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <fstream>
#include <QApplication>

ConnectDialog::ConnectDialog(std::shared_ptr<QSqlDatabase> _sqlDatabase, QApplication *_app, QTranslator *_translator, QWidget *parent)
{
    sqlDatabase = _sqlDatabase;

    app = _app;
    translator = _translator;

    initElements();
    initText();
}



//METHODS=========================================================================




void ConnectDialog::initElements()
{
    pBtnConfirm = new QPushButton(this);

    labels = new QLabel[5];
    lineEdits = new QLineEdit[4];


    //create layouts
    vLayout = new QVBoxLayout;
    hLayout = new QHBoxLayout[5];

    //add elements to the layouts
    for (int i = 0; i < 4; i++)
    {
        hLayout[i].addWidget(labels + i);
        hLayout[i].addWidget(lineEdits + i);
        vLayout->addLayout(hLayout + i);
    }

    labelSaveData = new QLabel(tr("Save data"));


    QHBoxLayout* hBoxLanguage = new QHBoxLayout;

    labelLanguage = new QLabel;
    cmbBoxLanguage = new QComboBox;

    cmbBoxLanguage->addItem("English");
    cmbBoxLanguage->addItem("Ukrainian");

    connect(cmbBoxLanguage, SIGNAL(currentIndexChanged(int)), SLOT(changeLanguage()));

    hBoxLanguage->addWidget(labelLanguage);
    hBoxLanguage->addWidget(cmbBoxLanguage);
    hBoxLanguage->addStretch(1);

    vLayout->addLayout(hBoxLanguage);

    QHBoxLayout* hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(labelSaveData);

    chkBoxSave = new QCheckBox;
    chkBoxSave->setChecked(true);
    hBoxLayout->addWidget(chkBoxSave);

    hBoxLayout->addStretch(1);

    vLayout->addLayout(hBoxLayout);

    vLayout->addStretch(1);

    //adds confirm button in layout
    vLayout->addWidget(pBtnConfirm);

    setLayout(vLayout);

    connect(pBtnConfirm, SIGNAL(clicked(bool)), this, SLOT(confirmData()));
}

void ConnectDialog::initText()
{

    labels[0].setText(tr("host"));
    labels[1].setText(tr("user"));
    labels[2].setText(tr("port"));
    labels[3].setText(tr("password"));

    labelSaveData->setText(tr("Save data"));
    labelLanguage->setText(tr("Language"));


    pBtnConfirm->setText(tr("confirm"));
}

void ConnectDialog::showEvent(QShowEvent *event)
{
    QString strPath = QApplication::applicationDirPath() + "/connect.txt";

    QFile file(strPath);

    file.open(QIODevice::ReadOnly | QIODevice::Text);

    int i = 0;

    file.atEnd();
    QString strTmp = file.readLine();

    cmbBoxLanguage->setCurrentText(strTmp.remove("\n"));

    while(!file.atEnd())
    {
        QString strTmp = file.readLine();
        strTmp.remove("\n");
        qDebug() << strTmp;

        lineEdits[i].setText(strTmp);
        i++;
    }

    file.close();
}



//EVENTS=========================================================================



void ConnectDialog::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        initText();
    }
}



//SLOTS==========================================================================




void ConnectDialog::confirmData()
{
    sqlDatabase->setHostName(lineEdits[0].text());
    sqlDatabase->setUserName(lineEdits[1].text());
    sqlDatabase->setPort(lineEdits[2].text().toInt());
    sqlDatabase->setPassword(lineEdits[3].text());

    if(sqlDatabase->open())
    {
        QMessageBox message;
        message.setWindowTitle(tr("connection"));
        message.setText(tr("connection is established"));
        message.exec();

        emit confirmed();

        saveData();

        close();
    }
    else
    {
        QErrorMessage error(this);
        error.setWindowTitle(tr("error"));
        error.showMessage(sqlDatabase->lastError().text());
        error.exec();
    }
}




void ConnectDialog::saveData()
{
    if (chkBoxSave->isChecked())
    {
        QString strPath = QApplication::applicationDirPath() + "/connect.txt";

        QFile file(strPath);

        file.open(QIODevice::WriteOnly | QIODevice::Text);

        QString strTmp;

        int i = 0;

        QTextStream out(&file);

        out << cmbBoxLanguage->currentText() + "\n";

        for (int i = 0; i < 4; i++)
        {
            out << lineEdits[i].text() + "\n";
        }

        file.close();
    }
}

void ConnectDialog::changeLanguage()
{
    QString strLangPath = app->applicationDirPath() + "/Languages";


    if (cmbBoxLanguage->currentText() == "English")
    {
        app->removeTranslator(translator);
    }
    else if (cmbBoxLanguage->currentText() == "Ukrainian")
    {
        app->removeTranslator(translator);
        translator->load("LightBase_ua", strLangPath);
        app->installTranslator(translator);
    }
    else if (cmbBoxLanguage->currentText() == "Russian")
    {
        app->removeTranslator(translator);
        translator->load("LightBase_ru", strLangPath);
        app->installTranslator(translator);
    }

}

