#include "Interface/Widgets/Header/FindSortWidget.h"

#include <QMessageBox>
#include <QEvent>


FindSortWidget::FindSortWidget(std::shared_ptr<QSqlRelationalDataModel> _sqlModel, QWidget *parent) : QWidget(parent)
{
    sqlModel = _sqlModel;


    mMapperDelete = new QSignalMapper(this);
    connect(mMapperDelete, SIGNAL(mapped(QWidget*)), SLOT(deleteRowFilter(QWidget*)));


    initElements();

    initText();
}



//INIT METHODS===========================================================================================================



void FindSortWidget::initElements()
{
    pBtnAddFilter = new QPushButton;
    pBtnFind = new QPushButton;
    pBtnSort = new QPushButton;

    //layouts
    vLayoutFind = new QVBoxLayout;
    vLayoutFind->addStretch(1);

    vLayoutSort = new QVBoxLayout;
    vLayoutBtns = new QVBoxLayout;

    cmbBoxSort = new QComboBox(this);
    initCmbBoxFields(cmbBoxSort);

    rBtnAsc = new QRadioButton;
    rBtnDesc = new QRadioButton;

    btnGroup = new QButtonGroup;

    vLayoutSort->addWidget(cmbBoxSort);
    vLayoutSort->addWidget(rBtnAsc);
    vLayoutSort->addWidget(rBtnDesc);
    vLayoutSort->addWidget(pBtnSort);
    vLayoutSort->addStretch(1);

    btnGroup->addButton(rBtnAsc);
    btnGroup->addButton(rBtnDesc);

    vLayoutBtns->addWidget(pBtnAddFilter);
    vLayoutBtns->addWidget(pBtnFind);
    vLayoutBtns->addStretch(1);

    hLayoutMain = new QHBoxLayout;

    hLayoutMain->addLayout(vLayoutFind);
    hLayoutMain->addLayout(vLayoutSort);
    hLayoutMain->addLayout(vLayoutBtns);

    setLayout(hLayoutMain);

    connect(pBtnSort, SIGNAL(clicked(bool)), this, SLOT(sortRows()));
    connect(pBtnAddFilter, SIGNAL(clicked(bool)), this, SLOT(addFilter()));
    connect(pBtnFind, SIGNAL(clicked(bool)), this, SLOT(findRows()));

    addFilter();

}


void FindSortWidget::initCmbBoxFields(QComboBox* cmbBox)
{
    cmbBox->clear();
    cmbBox->addItems(sqlModel->getColumns());
}

void FindSortWidget::initText()
{
    pBtnAddFilter->setText(tr("Add filter"));
    pBtnFind->setText(tr("Find"));
    pBtnSort->setText(tr("Sort"));

    rBtnAsc->setText(tr("ASC"));
    rBtnDesc->setText(tr("DESC"));

    vct_pBtnDelete[0]->setText(tr("Delete"));
}


//EVENTS======================================================================================



void FindSortWidget::closeEvent(QCloseEvent *event)
{
    emit closed();
}

void FindSortWidget::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        initText();
    }
}



//SLOTS=================================================================================



void FindSortWidget::findRows()
{
    QString tmpString;

    for (int i = 0; i < vct_CmbBox.size(); i++)
    {
        tmpString += vct_CmbBox[i]->currentText() + " = " + vct_lEdit[i]->text();
        if (i != vct_CmbBox.size() - 1)
        {
            tmpString += " and ";
        }
    }

    sqlModel->setFilter(tmpString);
    sqlModel->filter();
}



void FindSortWidget::sortRows()
{
    int index = cmbBoxSort->currentIndex();
    if (rBtnAsc->isChecked())
    {
        sqlModel->sort(index, Qt::AscendingOrder);
    }
    else if (rBtnDesc->isChecked())
    {
        sqlModel->sort(index, Qt::DescendingOrder);
    }
}

void FindSortWidget::clearWidget()
{
    QLayoutItem* childItem;

    vct_CmbBox.clear();
    vct_lEdit.clear();

    for (int i = 0; i < vct_hBoxFilterRow.size(); i++)
    {
        while ((childItem = vct_hBoxFilterRow[i]->takeAt(0)) != 0)
        {
            delete childItem->widget();
        }
    }

    vct_hBoxFilterRow.clear();

    initCmbBoxFields(cmbBoxSort);
    addFilter();
}



void FindSortWidget::addFilter()
{
    QComboBox* cmbBox = new QComboBox;
    initCmbBoxFields(cmbBox);
    vct_CmbBox.push_back(cmbBox);

    QLineEdit* lEdit = new QLineEdit;
    vct_lEdit.push_back(lEdit);

    QHBoxLayout* hLayout = new QHBoxLayout;
    vct_hBoxFilterRow.push_back(hLayout);
    hLayout->addWidget(cmbBox);
    hLayout->addWidget(lEdit);

    QPushButton* pBtnDelete = new QPushButton(tr("Delete"));
    vct_pBtnDelete.push_back(pBtnDelete);
    mMapperDelete->setMapping(pBtnDelete, pBtnDelete);
    connect(pBtnDelete, SIGNAL(clicked(bool)), mMapperDelete, SLOT(map()));
    hLayout->addWidget(pBtnDelete);

    vLayoutFind->insertLayout(vLayoutFind->count() - 1, hLayout);
}



void FindSortWidget::deleteRowFilter(QWidget *widget)
{
    if (vct_pBtnDelete.size() != 1)
    {
        QLayoutItem* childItem;

        int index = std::find(vct_pBtnDelete.begin(), vct_pBtnDelete.end(), widget) - vct_pBtnDelete.begin();

        vct_CmbBox.erase(vct_CmbBox.begin() + index);
        vct_lEdit.erase(vct_lEdit.begin() + index);
        vct_pBtnDelete.erase(vct_pBtnDelete.begin() + index);


        QHBoxLayout* hBoxTmp = vct_hBoxFilterRow[index];

        while ((childItem = hBoxTmp->takeAt(0)) != 0)
        {
            delete childItem->widget();
        }

        vct_hBoxFilterRow.erase(vct_hBoxFilterRow.begin() + index);
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Table must have at least one column for filter!"));
        msgBox.exec();
    }
}

