#include "Interface/Widgets/Header/CreateTableWidget.h"
#include <QAction>
#include <memory>
#include <QMessageBox>
#include <QEvent>
#include <QDebug>




CreateTableWidget::CreateTableWidget(std::shared_ptr<QSqlRelationalDataModel> _sqlModel, QWidget *parent)
{
    sqlModel = _sqlModel;

    mMapperDelete = new QSignalMapper(this);
    mMapperChkBox = new QSignalMapper(this);
    mMapperCmbBox = new QSignalMapper(this);

    connect(mMapperDelete, SIGNAL(mapped(QWidget*)), SLOT(deleteRowEdit(QWidget*)));
    connect(mMapperChkBox, SIGNAL(mapped(QWidget*)), SLOT(uncheckOtherAI(QWidget*)));
    connect(mMapperCmbBox, SIGNAL(mapped(QWidget*)), SLOT(addEnumDialog(QWidget*)));

    initElements();

    addRowEdit();

    connect(pBtnAddColumn, SIGNAL(clicked(bool)), this, SLOT(addRowEdit()));
    connect(pBtnConfirm, SIGNAL(clicked(bool)), this, SLOT(createTable()));

    eDialog = nullptr;

    initText();
}



//INIT ELEMENTS METHODS==============================================================



void CreateTableWidget::initElements()
{
    labelTableName = new QLabel;
    lEditTableName = new QLineEdit;

    pBtnConfirm = new QPushButton(this);

    pBtnAddColumn = new QPushButton(this);


    hMainLayout = new QHBoxLayout;

    vEditLayout = new QVBoxLayout;
    vEditLayout->addStretch(1);

    vBtnLayout = new QVBoxLayout;

    vBtnLayout->addWidget(labelTableName);
    vBtnLayout->addWidget(lEditTableName);

    vBtnLayout->addWidget(pBtnAddColumn);
    vBtnLayout->addWidget(pBtnConfirm);
    vBtnLayout->addStretch(1);

    hMainLayout->addLayout(vEditLayout);
    hMainLayout->addLayout(vBtnLayout);

    setLayout(hMainLayout);

}

void CreateTableWidget::initText()
{
    labelTableName->setText(tr("Table name: "));

    pBtnConfirm->setText(tr("confirm"));

    pBtnAddColumn->setText(tr("Add column"));
}




void CreateTableWidget::addRowEdit()
{
    hLayoutRowEdit = new QHBoxLayout;
    vct_hBoxLayout.push_back(hLayoutRowEdit);

    QLineEdit *lEditRowName = new QLineEdit;
    vct_lEditColName.push_back(lEditRowName);
    hLayoutRowEdit->addWidget(new QLabel(tr("Name:")));
    hLayoutRowEdit->addWidget(lEditRowName);


    QCheckBox *chkBoxNull = new QCheckBox;
    vct_chkBoxNull.push_back(chkBoxNull);
    hLayoutRowEdit->addWidget(new QLabel(tr("Null:")));
    hLayoutRowEdit->addWidget(chkBoxNull);


    QComboBox *cmbBoxType = new QComboBox;
    setCmbBoxType(cmbBoxType);
    vct_cmbBoxType.push_back(cmbBoxType);
    hLayoutRowEdit->addWidget(new QLabel(tr("Type:")));
    hLayoutRowEdit->addWidget(cmbBoxType);


    QLineEdit *lEditPrecision = new QLineEdit;
    vct_lEditPrecision.push_back(lEditPrecision);
    hLayoutRowEdit->addWidget(new QLabel(tr("Precision:")));
    hLayoutRowEdit->addWidget(lEditPrecision);


    QCheckBox *chkBoxPrimary = new QCheckBox;
    vct_chkBoxPrimary.push_back(chkBoxPrimary);
    hLayoutRowEdit->addWidget(new QLabel(tr("Primary:")));
    hLayoutRowEdit->addWidget(chkBoxPrimary);


    QCheckBox *chkBoxAI = new QCheckBox;
    vct_chkBoxAI.push_back(chkBoxAI);
    mMapperChkBox->setMapping(chkBoxAI, chkBoxAI);
    hLayoutRowEdit->addWidget(new QLabel(tr("Auto increment:")));
    hLayoutRowEdit->addWidget(chkBoxAI);
    connect(chkBoxAI, SIGNAL(clicked(bool)), mMapperChkBox, SLOT(map()));


    QPushButton *pBtnDelete = new QPushButton(tr("Delete"));
    vct_pBtnDelete.push_back(pBtnDelete);
    mMapperDelete->setMapping(pBtnDelete, pBtnDelete);
    hLayoutRowEdit->addWidget(pBtnDelete);
    connect(pBtnDelete, SIGNAL(clicked(bool)), mMapperDelete, SLOT(map()));


    mMapperCmbBox->setMapping(cmbBoxType, cmbBoxType);
    connect(cmbBoxType, SIGNAL(currentIndexChanged(QString)), mMapperCmbBox, SLOT(map()));


    vEditLayout->insertLayout(vEditLayout->count()-1, hLayoutRowEdit);
}




void CreateTableWidget::setCmbBoxType(QComboBox *cmbBox)
{
    QSqlColumn tmpCol("tmp");

    cmbBox->addItems(tmpCol.getTypesList());
}

void CreateTableWidget::closeEvent(QCloseEvent *event)
{
    emit closed();
}

void CreateTableWidget::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        initText();
    }
}



void CreateTableWidget::deleteRowEdit(QWidget *widget)
{
    if (vct_pBtnDelete.size() != 1)
    {
        QLayoutItem* childItem;

        int index = std::find(vct_pBtnDelete.begin(), vct_pBtnDelete.end(), widget) - vct_pBtnDelete.begin();


        vct_lEditColName.erase(vct_lEditColName.begin() + index);
        vct_chkBoxNull.erase(vct_chkBoxNull.begin() + index);
        vct_cmbBoxType.erase(vct_cmbBoxType.begin() + index);
        vct_lEditPrecision.erase(vct_lEditPrecision.begin() + index);
        vct_chkBoxPrimary.erase(vct_chkBoxPrimary.begin() + index);
        vct_chkBoxAI.erase(vct_chkBoxAI.begin() + index);
        vct_pBtnDelete.erase(vct_pBtnDelete.begin() + index);

        QHBoxLayout* hBoxTmp = vct_hBoxLayout[index];

        while ((childItem = hBoxTmp->takeAt(0)) != 0)
        {
            delete childItem->widget();
        }

        vct_hBoxLayout.erase(vct_hBoxLayout.begin() + index);
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Table must have at least one column!"));
        msgBox.exec();
    }
}



void CreateTableWidget::uncheckOtherAI(QWidget* widget)
{
    for (int i = 0; i < vct_chkBoxAI.size(); i++)
    {
        if (vct_chkBoxAI.at(i) != widget)
        {
            vct_chkBoxAI.at(i)->setChecked(false);
        }
        else
        {
            if (!vct_chkBoxPrimary.at(i)->isChecked())
            {
                vct_chkBoxAI.at(i)->setChecked(false);

                QMessageBox msgBox;
                msgBox.setText(tr("Auto increment field must be a key"));
                msgBox.exec();
            }
        }
    }
}



void CreateTableWidget::addEnumDialog(QWidget *widget)
{
    int index = std::find(vct_cmbBoxType.begin(), vct_cmbBoxType.end(), widget) - vct_cmbBoxType.begin();
    QLineEdit* lEditTmp;
    lEditTmp = vct_lEditPrecision.at(index);

    if (vct_cmbBoxType.at(index)->currentText() == "ENUM" || vct_cmbBoxType.at(index)->currentText() == "SET")
    {
        eDialog = new EnumDialog;
        connect(eDialog, SIGNAL(enumCreated(QString)), lEditTmp, SLOT(setText(QString)));
        connect(lEditTmp, SIGNAL(returnPressed()), eDialog, SLOT(exec()));
    }
    else
    {
        if (eDialog != 0)
        {
            lEditTmp->disconnect();
            delete eDialog;
            eDialog = 0;
        }
    }
}




void CreateTableWidget::createTable()
{
    QIntValidator intValidator;

    QString tableName = lEditTableName->text();
    int pos = 0;

    if (intValidator.validate(tableName, pos) == QIntValidator::Acceptable)
    {
        QMessageBox errorMsg;
        errorMsg.setText(tr("Table name must contain letters"));
        errorMsg.exec();
        return;
    }

    QSqlTableStructure tableStructure;

    tableStructure.setName(tableName);

    for (int i = 0; i < vct_lEditColName.size(); i++)
    {
        std::shared_ptr<QSqlColumn> column(new QSqlColumn(vct_lEditColName[i]->text()));

        if (vct_chkBoxNull[i]->checkState() == Qt::Checked)
        {
            column->setNull(true);
        }

        column->setType(column->nameToId(vct_cmbBoxType[i]->currentText()));
        column->setPrecision(vct_lEditPrecision[i]->text());

        if (vct_chkBoxPrimary[i]->checkState() == Qt::Checked)
        {
            column->setPrimaryKey(true);
        }

        tableStructure.addColumn(column);

        if (vct_chkBoxAI[i]->isChecked())
        {
            tableStructure.setAutoIncrement(i);
        }
    }


    if (sqlModel->createTable(tableStructure))
    {
        QString tmpStr = tr("Table ") + lEditTableName->text() + tr(" created");

        QMessageBox msgBox;
        msgBox.setText(tmpStr);
        msgBox.exec();

        emit closed();
        emit tableCreated();

        parent()->deleteLater();
    }
    else
    {
        QMessageBox errorMsg;
        errorMsg.setText(tr("Creating table error"));
        errorMsg.exec();
    }
}
