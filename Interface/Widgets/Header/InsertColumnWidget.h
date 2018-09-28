#ifndef INSERTCOLUMN_H
#define INSERTCOLUMN_H

#include "Core/QSqlRelationalLib/Headers/QSqlRelationalDataModel.h"

#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>

#include <QVBoxLayout>
#include <QLabel>
#include "Interface/Dialogs/Headers/EnumDialog.h"


class InsertColumnWidget : public QWidget
{
    Q_OBJECT



//DATA FIELDS==============================================================



    //smart pointer on QSqlRelationalDataModel
    std::shared_ptr<QSqlRelationalDataModel> sqlModel;

    //current column name
    QString columnName;

    //enum dialog pointer
    EnumDialog* eDialog;


//ELEMENTS================================================================


    QHBoxLayout *hBoxLayout;

    QLabel* labelName;
    QLineEdit *lEditName;

    QLabel* labelNull;
    QCheckBox *chkBoxNull;

    QLabel* labelType;
    QComboBox *cmbBoxType;

    QLabel* labelPrecision;
    QLineEdit *lEditPrecision;

    QPushButton *pBtnConfirm;


//METHODS==============================================================


    //method fill cmbBox by names of types
    void initCmbBoxType(QComboBox *cmbBox);

    void initElements();
    void initText();

//EVENTS==============================================================

    void closeEvent(QCloseEvent* event);
    void changeEvent(QEvent* event);
public:

    enum ColumnMode
    {
        INSERT_MODE = 0,
        UPDATE_MODE = 1
    };

    explicit InsertColumnWidget(std::shared_ptr<QSqlRelationalDataModel> _sqlModel, QWidget* _parent, ColumnMode mode);


signals:
    void closed();
public slots:
    void insertColumn();
    void addEnumDialog();
    void fillFields(QString columnName);

private:
    ColumnMode mode;
};

#endif // INSERTCOLUMN_H
