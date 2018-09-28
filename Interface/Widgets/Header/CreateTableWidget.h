#ifndef CREATETABLEWIDGET_H
#define CREATETABLEWIDGET_H

#include "Core/QSqlRelationalLib/Headers/QSqlRelationalDataModel.h"
#include <Interface/Dialogs/Headers/EnumDialog.h>

#include <vector>
#include <map>

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QComboBox>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QSignalMapper>



class CreateTableWidget : public QWidget
{
    Q_OBJECT



//DATA FIELDS=============================================



    std::shared_ptr<QSqlRelationalDataModel> sqlModel;

    QSignalMapper* mMapperDelete;
    QSignalMapper* mMapperChkBox;
    QSignalMapper* mMapperCmbBox;




//ELEMENTS==============================================



    EnumDialog* eDialog;
    QLabel* labelTableName;
    QLineEdit *lEditTableName;

    QPushButton *pBtnConfirm;
    QPushButton *pBtnAddColumn;
    QPushButton *pBtnRemoveColumn;


//LAYOUTS================================================



    QHBoxLayout *hMainLayout;

    QVBoxLayout *vEditLayout;

    QVBoxLayout *vBtnLayout;
    QHBoxLayout *hLayoutRowEdit;



//ELEMENTS LISTS===========================================



    std::vector<QLineEdit*> vct_lEditColName;
    std::vector<QCheckBox*> vct_chkBoxNull;
    std::vector<QComboBox*> vct_cmbBoxType;
    std::vector<QLineEdit*> vct_lEditPrecision;
    std::vector<QCheckBox*> vct_chkBoxPrimary;
    std::vector<QCheckBox*> vct_chkBoxAI;
    std::vector<QPushButton*> vct_pBtnDelete;

    std::vector<QHBoxLayout*> vct_hBoxLayout;



 //INIT ELEMENTS METHODS====================================

    void initElements();
    void initText();
    void setCmbBoxType(QComboBox* cmbBox);



//EVENTS



    void closeEvent(QCloseEvent* event);
    void changeEvent(QEvent* event);

public:

    explicit CreateTableWidget(std::shared_ptr<QSqlRelationalDataModel> _sqlModel, QWidget *parent = 0);


signals:

    void tableCreated();
    void closed();

public slots:

    //ROW EDITS SLOTS
    void addRowEdit();
    void deleteRowEdit(QWidget* widget);

    //uncheck other ai chkBoxes
    void uncheckOtherAI(QWidget* widget);

    //add enum dialog on return key for enum or set types
    void addEnumDialog(QWidget* widget);

    //read all row edits and create QSqlTableStructure object
    void createTable();
};

#endif // CREATETABLEWIDGET_H
