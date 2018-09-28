#ifndef LINKWIDGET_H
#define LINKWIDGET_H

#include <memory>
#include "Core/QSqlRelationalLib/Headers/QSqlRelationalDataModel.h"

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>

#include <QHBoxLayout>


class LinkWidget : public QWidget
{
    Q_OBJECT

    std::shared_ptr<QSqlRelationalDataModel> sqlModel;

    QString currentTable;

//ELEMENTS===================================================================================



    QPushButton *pBtnConfirm;

    QComboBox *cmbBoxFields;
    QComboBox *cmbBoxRefTables;
    QComboBox *cmbBoxRefPriKeys;

    QLabel* labelForeignKey;
    QLabel* labelReferenceTable;
    QLabel* labelPrimaryKey;


//LAYOUTS======================================================================================



    QHBoxLayout *hLayout;



//INIT ELEMENTS================================================================================



    void initElements();
    void initText();


//EVENTS=====================================================================================

   void closeEvent(QCloseEvent* event);
   void changeEvent(QEvent* event);

public:
    explicit LinkWidget(std::shared_ptr<QSqlRelationalDataModel> _sqlModel, QWidget* parent = 0);


signals:
    void closed();

public slots:
    void loadCmbBoxFields();
    void loadCmbBoxTables();
    void loadCmbBoxRefPriKeys(QString RefTableName);
    void confirm();
};

#endif // LINKWIDGET_H
