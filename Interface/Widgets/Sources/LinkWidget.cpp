#include "Interface/Widgets/Header/LinkWidget.h"

#include <QMessageBox>
#include <QErrorMessage>
#include <QEvent>

LinkWidget::LinkWidget(std::shared_ptr<QSqlRelationalDataModel> _sqlModel, QWidget *parent) : QWidget(parent)
{
    sqlModel = _sqlModel;

    initElements();


    loadCmbBoxFields();
    loadCmbBoxTables();


    connect(pBtnConfirm, SIGNAL(clicked(bool)), this, SLOT(confirm()));

    initText();
}

void LinkWidget::initElements()
{
    pBtnConfirm = new QPushButton;


    cmbBoxFields = new QComboBox;
    cmbBoxRefTables = new QComboBox;
    cmbBoxRefPriKeys = new QComboBox;

    hLayout = new QHBoxLayout;

    labelForeignKey = new QLabel;
    hLayout->addWidget(labelForeignKey);

    hLayout->addWidget(cmbBoxFields);

    labelReferenceTable = new QLabel;
    hLayout->addWidget(labelReferenceTable);

    hLayout->addWidget(cmbBoxRefTables);

    labelPrimaryKey = new QLabel;
    hLayout->addWidget(labelPrimaryKey);

    hLayout->addWidget(cmbBoxRefPriKeys);

    hLayout->addWidget(pBtnConfirm);

    setLayout(hLayout);
}

void LinkWidget::initText()
{
    labelForeignKey->setText(tr("Foreign key:"));
    labelReferenceTable->setText(tr("Reference table:"));
    labelPrimaryKey->setText(tr("Primary key:"));

    pBtnConfirm->setText(tr("confirm"));
}


//EVENTS================================================================================


void LinkWidget::closeEvent(QCloseEvent *event)
{
    closed();
}

void LinkWidget::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        initText();
    }
}


//SLOTS==================================================================================



void LinkWidget::loadCmbBoxFields()
{
    cmbBoxFields->clear();
    QSqlTableStructure tblStructure = sqlModel->getTableStructure();

    for (int i = 0; i < tblStructure.columnCount(); i++)
    {
        cmbBoxFields->addItem(tblStructure.getColumn(i)->getName());
    }
}

void LinkWidget::loadCmbBoxTables()
{
    cmbBoxRefTables->clear();
    cmbBoxRefTables->addItems(sqlModel->getTables());

    connect(cmbBoxRefTables, SIGNAL(activated(QString)), this, SLOT(loadCmbBoxRefPriKeys(QString)));

    loadCmbBoxRefPriKeys(sqlModel->getTables().at(0));
}

void LinkWidget::loadCmbBoxRefPriKeys(QString RefTableName)
{
    cmbBoxRefPriKeys->clear();

    currentTable = sqlModel->tableName();

    sqlModel->setTable(RefTableName);

    QSqlTableStructure tblStructure = sqlModel->getTableStructure();

    sqlModel->setTable(currentTable);
    sqlModel->select();

    std::vector<std::shared_ptr<QSqlColumn>> sqlClmnPriKey = tblStructure.getPrimaryKey();

    for (int i = 0; i < sqlClmnPriKey.size(); i++)
    {
        cmbBoxRefPriKeys->addItem(sqlClmnPriKey[i]->getName());
    }
}

void LinkWidget::confirm()
{
    if (sqlModel->addForeignKey(cmbBoxFields->currentText(), cmbBoxRefTables->currentText(), cmbBoxRefPriKeys->currentText()))
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Link created"));
        msgBox.exec();
    }
    else
    {
        QMessageBox errorMsg;
        errorMsg.setText(sqlModel->lastError().text());
        errorMsg.exec();
    }
}












