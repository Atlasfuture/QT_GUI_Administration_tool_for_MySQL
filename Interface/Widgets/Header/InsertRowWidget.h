#ifndef INSERTROWWIDGET_H
#define INSERTROWWIDGET_H

#include <memory>
#include "Core/QSqlRelationalLib/Headers/QSqlRelationalDataModel.h"
#include <list>

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>


class InsertRowWidget : public QWidget
{
    Q_OBJECT


//DATA FIELDS==================================================


    std::shared_ptr<QSqlRelationalDataModel> sqlModel;

    QSqlTableStructure tableStructure;

    int autoIncrement;


//ELEMENTS===================================================



    QPushButton* pBtnConfirm;
    QLineEdit* lEditFields;
    QLabel* labelFields;
    QLabel* labelTypes;

    std::vector<QLineEdit*> vct_lEditField;



//LAYOUTS===================================================


    QVBoxLayout* vBoxLayout;
    std::vector<QHBoxLayout*> vct_hBoxEditField;


//METHODS=====================================================

    void initElements();
    void initText();

    void setFieldMask(QLineEdit* lEdit, int typeId);


//EVENTS======================================================

    void closeEvent(QCloseEvent* event);
    void changeEvent(QEvent* event);

public:
    explicit InsertRowWidget(std::shared_ptr<QSqlRelationalDataModel> _sqlModel, QWidget *parent = 0);


signals:
    void closed();
public slots:
    void insertRow();
    void clearWidget();
};

#endif // INSERTROWWIDGET_H
