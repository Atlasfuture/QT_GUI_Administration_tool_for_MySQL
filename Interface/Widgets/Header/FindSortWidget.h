#ifndef FINDSORTWIDGET_H
#define FINDSORTWIDGET_H

#include <memory>
#include <QWidget>


#include "Core/QSqlRelationalLib/Headers/QSqlRelationalDataModel.h"
#include "Core/QSqlRelationalLib/Headers/QSqlTableStructure.h"

#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>

#include <QRadioButton>
#include <QButtonGroup>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QVector>
#include <QSignalMapper>


class FindSortWidget : public QWidget
{
    Q_OBJECT


//DATA FIELDS========================================================



    //relational data model smart pointer
    std::shared_ptr<QSqlRelationalDataModel> sqlModel;

    QSignalMapper* mMapperDelete;

//ELEMENTS===========================================================



    QPushButton* pBtnFind;
    QPushButton* pBtnAddFilter;
    QPushButton* pBtnSort;

    QComboBox* cmbBoxSort;
    QRadioButton* rBtnAsc;
    QRadioButton* rBtnDesc;

    QButtonGroup* btnGroup;

    //vectors of elements pointers
    std::vector<QComboBox*> vct_CmbBox;
    std::vector<QLineEdit*> vct_lEdit;
    std::vector<QPushButton*> vct_pBtnDelete;


//LAYOUTS============================================================



    QVBoxLayout* vLayoutFind;
    QVBoxLayout* vLayoutBtns;
    QVBoxLayout* vLayoutSort;
    QHBoxLayout* hLayoutMain;

    //vector of horizontal filter layout pointers
    std::vector<QHBoxLayout*> vct_hBoxFilterRow;


//INIT METHODS========================================================


    void initElements();
    void initCmbBoxFields(QComboBox* cmbBox);
    void initText();

//EVENTS===============================================================
    void closeEvent(QCloseEvent* event);
    void changeEvent(QEvent* event);

signals:
    void closed();

public:
    explicit FindSortWidget(std::shared_ptr<QSqlRelationalDataModel> _sqlModel, QWidget* parent = 0);

public slots:
    void addFilter();
    void deleteRowFilter(QWidget* widget);
    void findRows();
    void sortRows();
    void clearWidget();
};









#endif
