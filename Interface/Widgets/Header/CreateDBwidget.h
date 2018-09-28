#ifndef CREATEDBWIDGET_H
#define CREATEDBWIDGET_H

#include "Core/QSqlRelationalLib/Headers/QSqlRelationalDataModel.h"

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class CreateDBWidget : public QWidget
{
    Q_OBJECT

    std::shared_ptr<QSqlRelationalDataModel> sqlModel;

//ELEMENTS===============================================

    QLabel *labelDbName;
    QLineEdit *lEditDbName;
    QPushButton *pBtnConfirm;


//METHODS================================================


    void initElements();

    void initText();

//EVENTS==================================================
    void closeEvent(QCloseEvent *event);
    void changeEvent(QEvent *event);
public:
    explicit CreateDBWidget(std::shared_ptr<QSqlRelationalDataModel> _sqlModel, QWidget *parent = 0);


signals:
    void closed();
    void dbCreated();
public slots:
    void createDataBase();
};

#endif // CREATEDBWIDGET_H
