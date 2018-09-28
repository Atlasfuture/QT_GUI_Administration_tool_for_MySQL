#include "Interface/Widgets/Header/InsertRowWidget.h"
#include <QSqlRecord>
#include <QErrorMessage>
#include <QValidator>
#include <QMessageBox>
#include <QDebug>


InsertRowWidget::InsertRowWidget(std::shared_ptr<QSqlRelationalDataModel> _sqlModel, QWidget *parent) : QWidget(parent)
{
    sqlModel = _sqlModel;


    vBoxLayout = new QVBoxLayout;
    setLayout(vBoxLayout);

    pBtnConfirm = new QPushButton(this);
    connect(pBtnConfirm, SIGNAL(clicked(bool)), this, SLOT(insertRow()));

    vBoxLayout->addWidget(pBtnConfirm);

    vBoxLayout->insertStretch(vBoxLayout->count() - 1);


    initElements();

    initText();
}



//INIT METHODS=================================================================



void InsertRowWidget::initElements()
{
    tableStructure = sqlModel->getTableStructure();

    int columnCount = tableStructure.columnCount();

    for (int i = 0; i < columnCount; i++)
    {
        if (tableStructure.getColumn(i)->isAutoIncrement())
        {
            autoIncrement = i;
        }


        QLabel* lField = new QLabel(tableStructure.getColumn(i)->getName());

        QLabel* lType = new QLabel(tableStructure.getColumn(i)->getTypeName());

        QHBoxLayout* hBoxLayout = new QHBoxLayout;

        hBoxLayout->addWidget(lField);
        hBoxLayout->addWidget(lType);

        QLineEdit* lEditField = new QLineEdit;
        setFieldMask(lEditField, tableStructure.getColumn(i)->getCurrentType());

        vct_lEditField.push_back(lEditField);

        hBoxLayout->addWidget(lEditField);
        vct_hBoxEditField.push_back(hBoxLayout);


        vBoxLayout->insertLayout(vBoxLayout->count() - 2, hBoxLayout);
    }
}



void InsertRowWidget::initText()
{
    pBtnConfirm->setText(tr("confirm"));
}



void InsertRowWidget::setFieldMask(QLineEdit *lEdit, int typeId)
{
    if (typeId > 0 && typeId < 6)
    {
        lEdit->setValidator(new QIntValidator());
    }
    else if (typeId > 5 && typeId < 10)
    {
        lEdit->setValidator(new QDoubleValidator());
    }
    else if (typeId == 11)
    {
        lEdit->setInputMask("0000-00-00");
    }
    else if (typeId == 12)
    {
        lEdit->setInputMask("0000-00-00 00:00:00");
    }
    else if (typeId == 13)
    {
        lEdit->setInputMask("0000-00-00 00:00:00");
    }
    else if (typeId == 14)
    {
        lEdit->setInputMask("00:00:00");
    }
    else if (typeId == 15)
    {
        lEdit->setInputMask("0000");
    }
}



//EVENTS=====================================================================



void InsertRowWidget::closeEvent(QCloseEvent *event)
{
    emit closed();
}

void InsertRowWidget::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        initText();
    }
}



//SLOTS=======================================================================



void InsertRowWidget::clearWidget()
{
    QLayoutItem* childItem;

    vct_lEditField.clear();

    for (int i = 0; i < vct_hBoxEditField.size(); i++)
    {
        while ((childItem = vct_hBoxEditField[i]->takeAt(0)) != 0)
        {
            delete childItem->widget();
        }
    }

    vct_hBoxEditField.clear();

    initElements();
}



void InsertRowWidget::insertRow()
{
    QSqlRecord rec = sqlModel->record();

    for (int i = 0; i < vct_lEditField.size(); i++)
    {
        std::shared_ptr<QSqlColumn> sqlColumn = tableStructure.getColumn(i);

        QString strInsValue = vct_lEditField[i]->text();

        if (sqlColumn->getCurrentType() > 6 && sqlColumn->getCurrentType() < 10)
        {
            strInsValue.replace(",", ".");
        }

        if (autoIncrement != i)
        {
            rec.setValue(i, QVariant(strInsValue));
        }
        else
        {
            if (vct_lEditField[i]->text() != "")
            {
                rec.setValue(i, QVariant(strInsValue));
            }
        }
    }

    if (!sqlModel->insertRecord(-1, rec))
    {
        QErrorMessage errorMsg;
        errorMsg.showMessage(sqlModel->lastError().text());
        errorMsg.exec();
    }
    else
    {
        sqlModel->refreshTable();
    }
}

