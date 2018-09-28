#include "Interface/Widgets/Header/InsertColumnWidget.h"
#include "Core/QSqlRelationalLib/Headers/QSqlColumn.h"
#include "Core/QSqlRelationalLib/Headers/QSqlTableStructure.h"
#include <QLabel>
#include <QDebug>
#include <QMessageBox>
#include <QEvent>


InsertColumnWidget::InsertColumnWidget(std::shared_ptr<QSqlRelationalDataModel> _sqlModel, QWidget *_parent, ColumnMode _mode)
{
    mode = _mode;

    sqlModel = _sqlModel;

    initElements();

    eDialog = nullptr;

    initText();
}


void InsertColumnWidget::initCmbBoxType(QComboBox *cmbBox)
{
    QSqlColumn tmpCol("tmp");
    cmbBox->addItems(tmpCol.getTypesList());
}

void InsertColumnWidget::initElements()
{
    hBoxLayout = new QHBoxLayout;

    labelName = new QLabel;
    hBoxLayout->addWidget(labelName);

    lEditName = new QLineEdit;
    hBoxLayout->addWidget(lEditName);

    labelNull = new QLabel;
    hBoxLayout->addWidget(labelNull);

    chkBoxNull = new QCheckBox;
    hBoxLayout->addWidget(chkBoxNull);

    labelType = new QLabel;
    hBoxLayout->addWidget(labelType);


    cmbBoxType = new QComboBox;
    initCmbBoxType(cmbBoxType);
    connect(cmbBoxType, SIGNAL(currentIndexChanged(int)), SLOT(addEnumDialog()));
    hBoxLayout->addWidget(cmbBoxType);

    labelPrecision = new QLabel;
    hBoxLayout->addWidget(labelPrecision);


    lEditPrecision = new QLineEdit;
    lEditPrecision->setValidator(new QIntValidator());
    hBoxLayout->addWidget(lEditPrecision);

    pBtnConfirm = new QPushButton;

    hBoxLayout->addWidget(pBtnConfirm);
    setLayout(hBoxLayout);

    connect(pBtnConfirm, SIGNAL(clicked(bool)), this, SLOT(insertColumn()));
}

void InsertColumnWidget::initText()
{
    labelName->setText(tr("Name:"));
    labelNull->setText(tr("Null:"));
    labelType->setText(tr("Type:"));
    labelPrecision->setText(tr("Precision:"));

    pBtnConfirm->setText(tr("confirm"));
}


//EVENTS====================================================================================


void InsertColumnWidget::closeEvent(QCloseEvent *event)
{
    emit closed();
}

void InsertColumnWidget::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        initText();
    }
}


//SLOTS====================================================================================

void InsertColumnWidget::insertColumn()
{
    QSqlColumn *column = new QSqlColumn(lEditName->text());

    if (chkBoxNull->checkState() == Qt::Checked)
    {
        column->setNull(true);
    }

    column->setType(column->nameToId(cmbBoxType->currentText()));
    column->setPrecision(lEditPrecision->text());

    if (mode == INSERT_MODE)
    {
        if (sqlModel->addColumn(*column))
        {
            QString tmpStr = tr("New column inserted into table ") + sqlModel->tableName();
            QMessageBox msgBox;
            msgBox.setText(tmpStr);
            msgBox.exec();

            emit closed();
            parent()->deleteLater();
        }
        else
        {
            QMessageBox errorMsg;
            errorMsg.setText(sqlModel->lastError().text());
            errorMsg.exec();
        }
    }
    else
    {
        if (sqlModel->changeColumn(columnName, *column))
        {
            QString tmpStr = tr("Column has been changed");
            QMessageBox msgBox;
            msgBox.setText(tmpStr);
            msgBox.exec();

            emit closed();
            parent()->deleteLater();
        }
        else
        {
            QMessageBox errorMsg;
            errorMsg.setText(sqlModel->lastError().text());
            errorMsg.exec();
        }
    }
}

void InsertColumnWidget::addEnumDialog()
{
    if (cmbBoxType->currentText() == "ENUM" || cmbBoxType->currentText() == "SET")
    {
        lEditPrecision->setValidator(nullptr);
        eDialog = new EnumDialog;
        connect(eDialog, SIGNAL(enumCreated(QString)),lEditPrecision, SLOT(setText(QString)));
        connect(lEditPrecision, SIGNAL(returnPressed()), eDialog, SLOT(exec()));
    }
    else
    {
        lEditPrecision->setValidator(new QIntValidator());
        if (eDialog != nullptr)
        {
            lEditPrecision->disconnect();
            eDialog = nullptr;
        }
    }
}

void InsertColumnWidget::fillFields(QString _columnName)
{
    columnName = _columnName;

    std::shared_ptr<QSqlColumn> sqlColumn = sqlModel->getTableStructure().getColumn(columnName);

    lEditName->setText(sqlColumn->getName());
    chkBoxNull->setChecked(sqlColumn->isNull());
    cmbBoxType->setCurrentText(sqlColumn->getTypeName());
    lEditPrecision->setText(sqlColumn->getPrecision());

}

