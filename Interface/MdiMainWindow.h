/*=====================================================
Class MdiMainWindow
Inherits class QMainWindow
Class contains smart pointer on QSqlRelationalDataModel,
graphic elements, layouts managers, signal mappers,
toolbars, actions, pointers on widgets, menus,
context menus, slots - methods that are connected to signals and
additional methods.
*======================================================*/




#ifndef MDIMAINWINDOW_H
#define MDIMAINWINDOW_H



#include <memory>
#include <QMap>

#include <QMainWindow>
#include <QApplication>

#include <QEvent>
#include <QMdiArea>
#include <QMenuBar>
#include <QMenu>
#include <QTableView>
#include <QList>
#include <QAction>
#include <QSignalMapper>
#include <QToolBar>
#include <QIcon>
#include <QTranslator>

//CORE LIBS====================================================================

#include "Core/QSqlRelationalLib/Headers/QSqlRelationalDataModel.h"

//WIDGETS HEADERS==============================================================

#include "Interface/Dialogs/Headers/ConnectDialog.h"
#include "Interface/Widgets/Header/CreateDBwidget.h"
#include "Interface/Widgets/Header/CreateTableWidget.h"
#include "Interface/Widgets/Header/InsertColumnWidget.h"
#include "Interface/Widgets/Header/InsertRowWidget.h"
#include "Interface/Widgets/Header/LinkWidget.h"
#include "Interface/Widgets/Header/FindSortWidget.h"
#include "Interface/Widgets/Header/QTableViewS.h"

//DIALOGS======================================================================

#include <QFileDialog>


//MDI TEMPLATES HEADERS========================================================

#include "Addons/Student_0_1/StudentWidget.h"


class MdiMainWindow : public QMainWindow
{
    Q_OBJECT


//ICONS

    QIcon iconLightBase;

//MAIN DATA FIELDS============================================================

    QApplication* app;
    QTranslator* translator;

    //database and SqlRelationalDataModel smart pointers
    std::shared_ptr<QSqlDatabase> sqlDb;
    std::shared_ptr<QSqlRelationalDataModel> sqlModel;

    //current objects strings
    QString currentDatabase;
    QString currentTable;
    QString currentColumn;


    //enumeration of database objects
    enum baseObjects
    {
        DATABASE = 0,
        TABLE = 1,
        COLUMN = 2,
        ROW = 3
    };

//SIGNAL MAPPERS==============================================================


    QSignalMapper *mMapperCreateWidget;
    QSignalMapper *mMapperDeleteWidget;
    QSignalMapper *mMapperDeleteObject;
    QSignalMapper *mMapperSetLanguage;



//STATUS DATA FIELDS==========================================================

    bool activate;
    bool tableActivated;

//ACTIONS=====================================================================

    //database actions
    QAction* aConnectBase;
    QAction* aCreateDB;
    QAction* aSaveDump;
    QAction* aLoadDump;
    QAction* aExit;

    //table actions
    QAction* aShowTableView;
    QAction* aRefreshTable;
    QAction* aCreateTable;
    QAction* aInsertColumn;
    QAction* aInsertRow;
    QAction* aLink;
    QAction* aFilter;


    //widgets actions
    QAction* aCascadeWidgets;
    QAction* aTileWidgets;
    QAction* aCloseWidgets;
    QAction* aShowAllWidgets;
    QAction* aHideAllWidgets;

    //toolbars actions
    QAction* aShowAllToolbars;
    QAction* aHideAllToolbars;
    QAction* aCreateStudentAddon;

    //delete objects actions
    QAction* aDeleteRow;
    QAction* aPrintTable;

    //change language actions

    //set english language action
    QAction* aSetEnglish;
    QAction* aSetUkrainian;
    QAction* aSetRussian;

    enum Languages
    {
        ENGLISH = 0,
        UKRAINIAN = 1,
        RUSSIAN = 2
    };

//MDI AREA POINTER============================================================


    QMdiArea* mdiArea;

//WIDGETS POINTERS============================================================


    ConnectDialog* wdgtConnect;
    CreateDBWidget* wdgtDB;
    CreateTableWidget* wdgtTable;
    InsertColumnWidget* wdgtColumn;
    InsertRowWidget* wdgtRow;
    LinkWidget* wdgtLink;
    FindSortWidget* wdgtFilter;
    StudentWidget* wdgtStudent;

    enum widgetsId
    {
        CONNECT_W = 0,
        CREATEDB_W = 1,
        CREATETABLE_W = 2,
        INSERTCOLUMN_W = 3,
        ALTERCOLUMN_W = 4,
        INSERTROW_W = 5,
        LINK_W = 6,
        TABLEVIEW_W = 7,
        FILTER_W = 8,
        STUDENT_W = 9
    };


//COMBO BOX POINTERS==========================================================

    QComboBox* cmbBoxDB;
    QComboBox* cmbBoxTables;
    QComboBox* cmbBoxColumns;
    QComboBox* cmbBoxAgregates;

//TABLE VIEW POINTER==========================================================

    QTableViewS* tableView;



//MAIN MENU POINTERS==========================================================


    QMenuBar* menuBar;

    QMenu* menuBase;
    QMenu* menuTable;
    QMenu* menuWidgets;
    QMenu* menuToolbars;
    QMenu* menuTemplates;

    //templates sub menus
    QMenu* subMenuCollege;

    QMenu* menuLanguages;

//TOOLBARS POINTERS===========================================================

    //Database toolbar pointer
    QToolBar* tBarMain;

    //main toolbar elements
    QLabel* labelDatabases;

    QPushButton* pBtnCreateDB;
    QPushButton* pBtnDropDB;

    //Table toolbar pointer
    QToolBar* tBarTable;

    QLabel* labelTables;

    QPushButton* pBtnRefreshTable;
    QPushButton* pBtnCreateTable;
    QPushButton* pBtnDropTable;

    //Column toolbar pointer
    QToolBar* tBarColumn;

    //Column toolbar elements
    QLabel* labelColumns;

    QPushButton* pBtnCreateColumn;
    QPushButton* pBtnAlterColumn;
    QPushButton* pBtnDropColumn;
    QPushButton* pBtnAddLink;

    //Row toolbar pointer
    QToolBar* tBarRow;

    //Row toolbar elements
    QLabel* labelRows;

    QPushButton* pBtnInsertRow;
    QPushButton* pBtnDeleteRow;
    QPushButton* pBtnFilter;

    //Agregate toolbar pointer
    QToolBar* tBarAgregate;

    //Agregate toolbar elements
    QLabel* labelAgregate;

    QPushButton* pBtnAgregate;


//ELEMENTS METHODS===========================================================



    void initActions();
    void initElements();
    void initMenus();

    void initText();

//EVENTS=====================================================================


    void showEvent(QShowEvent* event);
    void changeEvent(QEvent* event);
    void closeEvent(QCloseEvent* event);
public:

    explicit MdiMainWindow(std::shared_ptr<QSqlDatabase> _sqlDb, std::shared_ptr<QSqlRelationalDataModel> _sqlModel, QApplication* _app, QTranslator* _translator, QWidget *parent = 0);

//GET METHODS================================================================


    QString getCurrentColumn();


signals:
     void dbSelected(QString dbName);
     void tableSelected(QString tableName);
     void columnSelected(QString columnName);

public slots:

//SET CURRENT OBJECT SLOTS===================================================

     void setCurrentDatabase(QString dbName);
     void setCurrentTable(QString tableName);
     void setCurrentColumn(QString columnName);


//COMBO BOX LOAD SLOTS=====================================================


     void loadCmbBoxDB();
     void loadCmbBoxTables();
     void loadCmbBoxColumns();
     void loadCmbBoxAgregates();

//TOOLBARS SLOTS=============================================================

     void initToolbars();
     void showAllToolbars();
     void hideAllToolbars();

//WIDGETS SLOTS==============================================================


     void createWidget(int idWidget);
     void deleteWidget(int id);
     void deleteObject(int idObject);

     void showAllWidgets();
     void hideAllWidgets();

     //execute agregate function for current column
     void agregate();


//TABLE VIEW SLOTS===========================================================


     void initTableViewContext(QPoint point);
     void refreshTableView();
     void initPrintTableDialog();

//DIALOG INITALIZATION SLOTS=======================================================

     void initSaveDumpDialog();
     void initLoadDumpDialog();


//CHANGE LANGUAGE SLOT

     void changeLanguage(int lang);
};

#endif // MDIMAINWINDOW_H
