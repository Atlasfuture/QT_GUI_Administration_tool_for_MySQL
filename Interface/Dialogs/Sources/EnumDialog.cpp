#include "Interface/Dialogs/Headers/EnumDialog.h"

#include <QHBoxLayout>
#include <QLayoutItem>
#include <QMessageBox>


EnumDialog::EnumDialog()
{
    mMapperDelete = new QSignalMapper(this);

    initElements();
    addEdit();

    connect(mMapperDelete, SIGNAL(mapped(QWidget*)), SLOT(deleteEdit(QWidget*)));

    initText();
}



void EnumDialog::initElements()
{
     hMainLayout = new QHBoxLayout;

     vEditsLayout = new QVBoxLayout;
     vEditsLayout->addStretch(1);

     vButtonsDelete = new QVBoxLayout;
     vButtonsDelete->addStretch(1);

     vButtonsManage = new QVBoxLayout;

     hMainLayout->addLayout(vEditsLayout);
     hMainLayout->addLayout(vButtonsDelete);
     hMainLayout->addLayout(vButtonsManage);

     pBtnAddEdit = new QPushButton;
     connect(pBtnAddEdit, SIGNAL(clicked(bool)), SLOT(addEdit()));
     vButtonsManage->addWidget(pBtnAddEdit);

     pBtnConfirm = new QPushButton;
     connect(pBtnConfirm, SIGNAL(clicked(bool)), SLOT(confirm()));
     vButtonsManage->addWidget(pBtnConfirm);

     vButtonsManage->addStretch(1);

     setLayout(hMainLayout);
}

void EnumDialog::initText()
{
    pBtnAddEdit->setText(tr("Add"));
    pBtnConfirm->setText(tr("confirm"));
    lst_DeleteButtons.at(0)->setText(tr("Delete"));
}


//EVENTS==========================================================================



void EnumDialog::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        initText();
    }
}




//SLOTS=============================================================================



void EnumDialog::addEdit()
{
    QLineEdit* lEdit = new QLineEdit;
    lst_LineEdits.push_back(lEdit);

    QPushButton* pBtnDelete = new QPushButton(tr("Delete"));
    mMapperDelete->setMapping(pBtnDelete, pBtnDelete);
    lst_DeleteButtons.push_back(pBtnDelete);

    connect(pBtnDelete, SIGNAL(clicked(bool)), mMapperDelete, SLOT(map()));

    vEditsLayout->insertWidget(vEditsLayout->count()-1, lEdit);
    vButtonsDelete->insertWidget(vButtonsDelete->count()-1, pBtnDelete);
}

void EnumDialog::deleteEdit(QWidget *widget)
{
    QLayoutItem* child;

    int index = vButtonsDelete->indexOf(widget);
    child = vButtonsDelete->takeAt(index);
    delete child->widget();

    child = vEditsLayout->takeAt(index);
    delete child->widget();

    lst_LineEdits.erase(lst_LineEdits.begin() + index);
    lst_DeleteButtons.erase(lst_DeleteButtons.begin() + index);
}

void EnumDialog::confirm()
{
    if (!lst_LineEdits.isEmpty())
    {
        QString strEnum;

        strEnum = "'" + lst_LineEdits.at(0)->text() + "'";

        for (int i = 1; i < lst_LineEdits.size(); i++)
        {
            strEnum += ", '" + lst_LineEdits.at(i)->text() + "'";
        }

        emit enumCreated(strEnum);

        close();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText(tr("No elements!"));
        msgBox.exec();
    }
}
