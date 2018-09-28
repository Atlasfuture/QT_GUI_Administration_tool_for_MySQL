//Source file for class MdiMainWindow

#include "MdiMainWindow.h"
#include <QMessageBox>
#include <QErrorMessage>
#include <QMdiSubWindow>
#include <QComboBox>
#include <QSqlRecord>
#include <QDebug>

#include <QApplication>

#include "Core/Writer/TableWriter.h"




MdiMainWindow::MdiMainWindow(std::shared_ptr<QSqlDatabase> _sqlDb, std::shared_ptr<QSqlRelationalDataModel> _sqlModel, QApplication *_app, QTranslator* _translator, QWidget *parent)
{
    app = _app;
    translator = _translator;
    sqlDb = _sqlDb;
    sqlModel = _sqlModel;


    sqlModel->setEditStrategy(QSqlRelationalDataModel::OnFieldChange);

    currentDatabase = "";
    currentTable = "";
    currentColumn = "";

    wdgtConnect = nullptr;
    wdgtDB = nullptr;
    wdgtTable = nullptr;
    wdgtColumn = nullptr;
    wdgtRow = nullptr;
    wdgtLink = nullptr;
    wdgtFilter = nullptr;
    tableView = nullptr;
    wdgtStudent = nullptr;

    activate = true;
    tableActivated = false;

    mMapperCreateWidget = new QSignalMapper(this);
    connect(mMapperCreateWidget, SIGNAL(mapped(int)), SLOT(createWidget(int)));

    mMapperDeleteWidget = new QSignalMapper(this);
    connect(mMapperDeleteWidget, SIGNAL(mapped(int)), SLOT(deleteWidget(int)));

    mMapperDeleteObject = new QSignalMapper(this);
    connect(mMapperDeleteObject, SIGNAL(mapped(int)), SLOT(deleteObject(int)));

    mMapperSetLanguage = new QSignalMapper(this);
    connect(mMapperSetLanguage, SIGNAL(mapped(int)), SLOT(changeLanguage(int)));

    wdgtColumn = nullptr;

}





//EVENTS==========================================================================================



void MdiMainWindow::showEvent(QShowEvent *e)
{

    if (activate)
    {
        initElements();
        initText();

        setCurrentDatabase(cmbBoxDB->currentText());
        setCurrentTable(sqlModel->getTables().at(0));
        setCurrentColumn(sqlModel->getColumns().at(0));

        activate = false;
    }
}

void MdiMainWindow::changeEvent(QEvent *event)
{
    if (!activate)
    {
        if (event->type() == QEvent::LanguageChange)
        {
            initText();
        }
    }
}

void MdiMainWindow::closeEvent(QCloseEvent *e)
{
    emit close();
}



//GET METHODS==================================================================================



QString MdiMainWindow::getCurrentColumn()
{
    return currentColumn;
}



//ELEMENTS METHODS===================================================================================



void MdiMainWindow::initElements()
{
    //icon init
    iconLightBase = QIcon(app->applicationDirPath() + "\\LightBase.ico");

    //init mdi work area
    mdiArea = new QMdiArea(this);
    setCentralWidget(mdiArea);



//INIT COMBO BOXES------------------------------------------------------------

    cmbBoxDB = new QComboBox(this);
    cmbBoxTables = new QComboBox(this);
    cmbBoxColumns = new QComboBox(this);
    cmbBoxAgregates = new QComboBox(this);

    loadCmbBoxDB();
    setCurrentDatabase(cmbBoxDB->currentText());

    loadCmbBoxTables();
    setCurrentTable(cmbBoxTables->currentText());

    loadCmbBoxColumns();
    setCurrentColumn(cmbBoxColumns->currentText());

    loadCmbBoxAgregates();



    //init main menu
    menuBar = new QMenuBar;

    initActions();
    initMenus();
    initToolbars();

    setMenuBar(menuBar);

}


void MdiMainWindow::initActions()
{

//DATABASE ACTIONS-------------------------------------------------------------------

    aCreateDB = new QAction(this);
    connect(aCreateDB, SIGNAL(triggered(bool)), mMapperCreateWidget, SLOT(map()));

    aSaveDump = new QAction(this);
    connect(aSaveDump, SIGNAL(triggered(bool)), SLOT(initSaveDumpDialog()));

    aLoadDump = new QAction(this);
    connect(aLoadDump, SIGNAL(triggered(bool)), SLOT(initLoadDumpDialog()));

    aExit = new QAction(this);
    connect(aExit, SIGNAL(triggered(bool)), this, SLOT(close()));

//TABLE ACTIONS-----------------------------------------------------------------------

    aShowTableView = new QAction(this);
    connect(aShowTableView, SIGNAL(triggered(bool)), mMapperCreateWidget, SLOT(map()));

    aRefreshTable = new QAction(this);
    connect(aRefreshTable, SIGNAL(triggered(bool)), SLOT(refreshTableView()));

    aCreateTable = new QAction(this);
    connect(aCreateTable, SIGNAL(triggered(bool)), mMapperCreateWidget, SLOT(map()));

    aInsertColumn = new QAction(this);
    connect(aInsertColumn, SIGNAL(triggered(bool)), mMapperCreateWidget, SLOT(map()));

    aInsertRow = new QAction(this);
    connect(aInsertRow, SIGNAL(triggered(bool)), mMapperCreateWidget, SLOT(map()));

    aLink = new QAction(this);
    connect(aLink, SIGNAL(triggered(bool)), mMapperCreateWidget, SLOT(map()));

    aFilter = new QAction(this);
    connect(aFilter, SIGNAL(triggered(bool)), mMapperCreateWidget, SLOT(map()));

//WIDGETS ACTIONS--------------------------------------------------------------------

    aCascadeWidgets = new QAction(this);
    connect(aCascadeWidgets, SIGNAL(triggered(bool)), mdiArea, SLOT(cascadeSubWindows()));

    aTileWidgets = new QAction(this);
    connect(aTileWidgets, SIGNAL(triggered(bool)), mdiArea, SLOT(tileSubWindows()));

    aCloseWidgets = new QAction(this);
    connect(aCloseWidgets, SIGNAL(triggered(bool)), mdiArea, SLOT(closeAllSubWindows()));

    aShowAllWidgets = new QAction(this);
    connect(aShowAllWidgets, SIGNAL(triggered(bool)), SLOT(showAllWidgets()));

    aHideAllWidgets = new QAction(this);
    connect(aHideAllWidgets, SIGNAL(triggered(bool)), SLOT(hideAllWidgets()));

//TOOLBARS ACTIONS-------------------------------------------------------------------

    aShowAllToolbars = new QAction(this);
    connect(aShowAllToolbars, SIGNAL(triggered(bool)), this, SLOT(showAllToolbars()));

    aHideAllToolbars = new QAction(this);
    connect(aHideAllToolbars, SIGNAL(triggered(bool)), this, SLOT(hideAllToolbars()));

//TEMPLATES ACTIONS------------------------------------------------------------------

    aCreateStudentAddon = new QAction(this);
    connect(aCreateStudentAddon, SIGNAL(triggered(bool)), mMapperCreateWidget, SLOT(map()));

//DELETE ACTIONS

    aDeleteRow = new QAction(this);
    connect(aDeleteRow, SIGNAL(triggered(bool)), mMapperDeleteObject, SLOT(map()));

    aPrintTable = new QAction(this);
    connect(aPrintTable, SIGNAL(triggered(bool)), this, SLOT(initPrintTableDialog()));



//INIT ACTION MAPPERS-----------------------------------------------------------------

    aSetEnglish = new QAction(this);
    connect(aSetEnglish, SIGNAL(triggered(bool)), mMapperSetLanguage, SLOT(map()));

    aSetUkrainian = new QAction(this);
    connect(aSetUkrainian, SIGNAL(triggered(bool)), mMapperSetLanguage, SLOT(map()));

    aSetRussian = new QAction(this);
    connect(aSetRussian, SIGNAL(triggered(bool)), mMapperSetLanguage, SLOT(map()));


    mMapperCreateWidget->setMapping(aCreateDB, CREATEDB_W);
    mMapperCreateWidget->setMapping(aCreateTable, CREATETABLE_W);
    mMapperCreateWidget->setMapping(aInsertColumn, INSERTCOLUMN_W);
    mMapperCreateWidget->setMapping(aInsertRow, INSERTROW_W);
    mMapperCreateWidget->setMapping(aLink, LINK_W);
    mMapperCreateWidget->setMapping(aFilter, FILTER_W);
    mMapperCreateWidget->setMapping(aShowTableView, TABLEVIEW_W);
    mMapperCreateWidget->setMapping(aCreateStudentAddon, STUDENT_W);

    mMapperDeleteObject->setMapping(aDeleteRow, ROW);

    mMapperSetLanguage->setMapping(aSetEnglish, ENGLISH);
    mMapperSetLanguage->setMapping(aSetUkrainian, UKRAINIAN);
    mMapperSetLanguage->setMapping(aSetRussian, RUSSIAN);
}


void MdiMainWindow::initMenus()
{


//INIT BASE MENU-------------------------------------------------------------------

    menuBase = new QMenu;
    menuBar->addMenu(menuBase);

    menuBase->addAction(aCreateDB);
    menuBase->addAction(aSaveDump);
    menuBase->addAction(aLoadDump);
    menuBase->addAction(aExit);


//INIT TABLE MENU-------------------------------------------------------------------

    menuTable = new QMenu;
    menuBar->addMenu(menuTable);

    menuTable->addAction(aShowTableView);

    menuTable->addAction(aCreateTable);

    menuTable->addAction(aInsertColumn);

    menuTable->addAction(aInsertRow);

    menuTable->addAction(aLink);



//INIT WIDGETS MENU----------------------------------------------------------------


    menuWidgets = new QMenu;
    menuBar->addMenu(menuWidgets);

    menuWidgets->addAction(aCascadeWidgets);

    menuWidgets->addAction(aTileWidgets);

    menuWidgets->addAction(aShowAllWidgets);

    menuWidgets->addAction(aHideAllWidgets);

    menuWidgets->addAction(aCloseWidgets);


//INIT TOOLBARS MENU-----------------------------------------------------------------


    menuToolbars = new QMenu;
    menuBar->addMenu(menuToolbars);


    menuToolbars->addAction(aShowAllToolbars);


    menuToolbars->addAction(aHideAllToolbars);


//INIT TEMPLATES MENU------------------------------------------------------------------


    menuTemplates = new QMenu;

    subMenuCollege = new QMenu;

    menuBar->addMenu(menuTemplates);

    menuTemplates->addMenu(subMenuCollege);

    subMenuCollege->addAction(aCreateStudentAddon);

//INIT LANGUAGES MENU------------------------------------------------------------------


    menuLanguages = new QMenu;
    menuBar->addMenu(menuLanguages);

    menuLanguages->addAction(aSetEnglish);
    menuLanguages->addAction(aSetUkrainian);
    menuLanguages->addAction(aSetRussian);

}

void MdiMainWindow::initText()
{
    aCreateDB->setText(tr("Create database"));
    aSaveDump->setText(tr("Save dump"));
    aLoadDump->setText(tr("Load dump"));
    aExit->setText(tr("Exit"));

    aShowTableView->setText(tr("Show table"));
    aRefreshTable->setText(tr("Refresh table"));
    aCreateTable->setText(tr("Create table"));
    aInsertColumn->setText(tr("Insert column"));
    aInsertRow->setText(tr("Insert record"));
    aLink->setText(tr("Add link"));
    aFilter->setText(tr("Filter"));

    aCascadeWidgets->setText(tr("Cascade widgets"));
    aTileWidgets->setText(tr("Tile widgets"));
    aCloseWidgets->setText(tr("Close widgets"));
    aShowAllWidgets->setText(tr("Show all widgets"));
    aHideAllWidgets->setText(tr("Hide all widgets"));

    aShowAllToolbars->setText(tr("Show toolbars"));
    aHideAllToolbars->setText(tr("Hide toolbars"));

    aDeleteRow->setText(tr("Delete record"));
    aPrintTable->setText(tr("Print table"));

    aCreateStudentAddon->setText(tr("Student"));

    aSetEnglish->setText(tr("English"));
    aSetUkrainian->setText(tr("Ukrainian"));
    aSetRussian->setText(tr("Russian"));

    menuBase->setTitle(tr("Base"));
    menuTable->setTitle(tr("Table"));
    menuWidgets->setTitle(tr("Widgets"));
    menuToolbars->setTitle(tr("Toolbars"));
    menuTemplates->setTitle(tr("Templates"));



    labelDatabases->setText(tr("Databases"));
    pBtnCreateDB->setText(tr("Create"));
    pBtnDropDB->setText(tr("Drop"));

    labelTables->setText(tr("Tables"));
    pBtnRefreshTable->setText(tr("Refresh"));
    pBtnCreateTable->setText(tr("Create"));
    pBtnDropTable->setText(tr("Drop"));

    labelColumns->setText(tr("Columns"));
    pBtnCreateColumn->setText(tr("Create"));
    pBtnAlterColumn->setText(tr("Alter"));
    pBtnDropColumn->setText(tr("Delete"));
    pBtnAddLink->setText(tr("Add link"));

    labelRows->setText(tr("Record"));
    pBtnInsertRow->setText(tr("Insert"));
    pBtnDeleteRow->setText(tr("Delete"));
    pBtnFilter->setText(tr("Filter"));

    labelAgregate->setText(tr("Agregate functions"));
    pBtnAgregate->setText(tr("Execute"));

    subMenuCollege->setTitle(tr("Student"));

    menuLanguages->setTitle(tr("Languages"));



}



//SET CURRENT OBJECT SLOTS===========================================================================



void MdiMainWindow::setCurrentDatabase(QString dbName)
{
    currentDatabase = dbName;
    sqlModel->setDatabase(currentDatabase);

    if (currentDatabase != "")
    {
        emit dbSelected(currentDatabase);
    }
}

void MdiMainWindow::setCurrentTable(QString tableName)
{
    currentTable = tableName;

    sqlModel->setTable(currentTable);
    sqlModel->select();

    if (currentTable != "")
    {
        emit tableSelected(currentTable);
    }
}

void MdiMainWindow::setCurrentColumn(QString columnName)
{
    currentColumn = columnName;

    if (currentColumn != "")
    {
        emit columnSelected(currentColumn);
    }
}



//COMBO BOX LOAD SLOTS===================================================================



void MdiMainWindow::loadCmbBoxDB()
{
    cmbBoxDB->clear();
    cmbBoxDB->addItems(sqlModel->getDatabases());

    loadCmbBoxTables();
}

void MdiMainWindow::loadCmbBoxTables()
{
    qDebug() << "LOAD TABLES";
    cmbBoxTables->clear();

    if (!sqlModel->getTables().isEmpty())
    {
        cmbBoxTables->addItems(sqlModel->getTables());
    }
}

void MdiMainWindow::loadCmbBoxColumns()
{
    cmbBoxColumns->clear();
    cmbBoxColumns->addItems(sqlModel->getColumns());
}

void MdiMainWindow::loadCmbBoxAgregates()
{
    cmbBoxAgregates->clear();
    cmbBoxAgregates->addItems(sqlModel->getAgregateNames());
}



//TOOLBARS SLOTS========================================================================================



void MdiMainWindow::initToolbars()
{



//INIT DATABASE TOOLBAR----------------------------------------------------------------------------



    tBarMain = new QToolBar(this);
    addToolBar(tBarMain);

    labelDatabases = new QLabel;
    tBarMain->addWidget(labelDatabases);
    tBarMain->addWidget(cmbBoxDB);
    connect(cmbBoxDB, SIGNAL(currentTextChanged(QString)), this, SLOT(setCurrentDatabase(QString)));
    connect(cmbBoxDB, SIGNAL(currentTextChanged(QString)), this, SLOT(loadCmbBoxTables()));

    pBtnCreateDB = new QPushButton;
    mMapperCreateWidget->setMapping(pBtnCreateDB, CREATEDB_W);
    connect(pBtnCreateDB, SIGNAL(clicked(bool)), mMapperCreateWidget, SLOT(map()));
    tBarMain->addWidget(pBtnCreateDB);

    pBtnDropDB = new QPushButton;
    mMapperDeleteObject->setMapping(pBtnDropDB, DATABASE);
    connect(pBtnDropDB, SIGNAL(clicked(bool)), mMapperDeleteObject, SLOT(map()));
    tBarMain->addWidget(pBtnDropDB);

    tBarTable = new QToolBar(this);
    addToolBar(tBarTable);

    labelTables = new QLabel;
    tBarTable->addWidget(labelTables);
    tBarTable->addWidget(cmbBoxTables);
    connect(cmbBoxTables, SIGNAL(currentTextChanged(QString)), this, SLOT(setCurrentTable(QString)));
    connect(cmbBoxTables, SIGNAL(currentTextChanged(QString)), this, SLOT(loadCmbBoxColumns()));


    pBtnRefreshTable = new QPushButton;
    connect(pBtnRefreshTable, SIGNAL(clicked(bool)), SLOT(refreshTableView()));
    tBarTable->addWidget(pBtnRefreshTable);

    pBtnCreateTable = new QPushButton;
    mMapperCreateWidget->setMapping(pBtnCreateTable, CREATETABLE_W);
    connect(pBtnCreateTable, SIGNAL(clicked(bool)), mMapperCreateWidget, SLOT(map()));
    tBarTable->addWidget(pBtnCreateTable);

    pBtnDropTable = new QPushButton;
    mMapperDeleteObject->setMapping(pBtnDropTable, TABLE);
    connect(pBtnDropTable, SIGNAL(clicked(bool)), mMapperDeleteObject, SLOT(map()));
    tBarTable->addWidget(pBtnDropTable);




//INIT COLUMN TOOLBAR---------------------------------------------------------------------------------



    tBarColumn = new QToolBar(this);
    addToolBar(tBarColumn);


    labelColumns = new QLabel;
    tBarColumn->addWidget(labelColumns);
    tBarColumn->addWidget(cmbBoxColumns);
    connect(cmbBoxColumns, SIGNAL(currentTextChanged(QString)), this, SLOT(setCurrentColumn(QString)));

    pBtnCreateColumn = new QPushButton;
    mMapperCreateWidget->setMapping(pBtnCreateColumn, INSERTCOLUMN_W);
    connect(pBtnCreateColumn, SIGNAL(clicked(bool)), mMapperCreateWidget, SLOT(map()));
    tBarColumn->addWidget(pBtnCreateColumn);

    pBtnAlterColumn = new QPushButton;
    mMapperCreateWidget->setMapping(pBtnAlterColumn, ALTERCOLUMN_W);
    connect(pBtnAlterColumn, SIGNAL(clicked(bool)), mMapperCreateWidget, SLOT(map()));
    tBarColumn->addWidget(pBtnAlterColumn);

    pBtnDropColumn = new QPushButton;
    mMapperDeleteObject->setMapping(pBtnDropColumn, COLUMN);
    connect(pBtnDropColumn, SIGNAL(clicked(bool)), mMapperDeleteObject, SLOT(map()));
    tBarColumn->addWidget(pBtnDropColumn);

    pBtnAddLink = new QPushButton;
    mMapperCreateWidget->setMapping(pBtnAddLink, LINK_W);
    connect(pBtnAddLink, SIGNAL(clicked(bool)), mMapperCreateWidget, SLOT(map()));
    tBarColumn->addWidget(pBtnAddLink);



//INIT ROW TOOLBAR------------------------------------------------------------------------------------



    tBarRow = new QToolBar(this);
    addToolBar(tBarRow);

    labelRows = new QLabel;
    tBarRow->addWidget(labelRows);

    pBtnInsertRow = new QPushButton;
    mMapperCreateWidget->setMapping(pBtnInsertRow, INSERTROW_W);
    connect(pBtnInsertRow, SIGNAL(clicked(bool)), mMapperCreateWidget, SLOT(map()));
    tBarRow->addWidget(pBtnInsertRow);

    pBtnDeleteRow = new QPushButton;
    mMapperDeleteObject->setMapping(pBtnDeleteRow, ROW);
    connect(pBtnDeleteRow, SIGNAL(clicked(bool)), mMapperDeleteObject, SLOT(map()));
    tBarRow->addWidget(pBtnDeleteRow);

    pBtnFilter = new QPushButton;
    mMapperCreateWidget->setMapping(pBtnFilter, FILTER_W);
    connect(pBtnFilter, SIGNAL(clicked(bool)), mMapperCreateWidget, SLOT(map()));
    tBarRow->addWidget(pBtnFilter);

    //init toolbar agregate

    tBarAgregate = new QToolBar;
    addToolBar(tBarAgregate);

    labelAgregate = new QLabel;
    tBarAgregate->addWidget(labelAgregate);
    tBarAgregate->addWidget(cmbBoxAgregates);

    pBtnAgregate = new QPushButton;
    connect(pBtnAgregate, SIGNAL(clicked(bool)), this, SLOT(agregate()));
    tBarAgregate->addWidget(pBtnAgregate);


    tBarMain->show();
}



void MdiMainWindow::showAllToolbars()
{
    tBarMain->show();
    tBarTable->show();
    tBarColumn->show();
    tBarRow->show();
    tBarAgregate->show();
}



void MdiMainWindow::hideAllToolbars()
{
    tBarMain->hide();
    tBarTable->hide();
    tBarColumn->hide();
    tBarRow->hide();
    tBarAgregate->hide();
}



//WIDGETS SLOTS==========================================================================================



void MdiMainWindow::createWidget(int idWidget)
{
    QMdiSubWindow* subMdiWidget = nullptr;

    switch(idWidget)
    {
    case CREATEDB_W:

        if (wdgtDB == nullptr)
        {
            wdgtDB = new CreateDBWidget(sqlModel, this);
            subMdiWidget = mdiArea->addSubWindow(wdgtDB);
            connect(wdgtDB, SIGNAL(dbCreated()), this, SLOT(loadCmbBoxDB()));


            mMapperDeleteWidget->setMapping(wdgtDB, CREATEDB_W);
            connect(wdgtDB, SIGNAL(closed()), mMapperDeleteWidget, SLOT(map()));
        }
        break;

    case CREATETABLE_W:

        if (wdgtTable == nullptr)
        {
            wdgtTable = new CreateTableWidget(sqlModel, this);

            subMdiWidget = mdiArea->addSubWindow(wdgtTable);;

            connect(wdgtTable, SIGNAL(tableCreated()), SLOT(loadCmbBoxTables()));
            sqlModel->select();

            mMapperDeleteWidget->setMapping(wdgtTable, CREATETABLE_W);
            connect(wdgtTable, SIGNAL(closed()), mMapperDeleteWidget, SLOT(map()));
        }
        break;

    case INSERTCOLUMN_W:

        if (wdgtColumn == nullptr)
        {
            wdgtColumn = new InsertColumnWidget(sqlModel, this, InsertColumnWidget::INSERT_MODE);

            subMdiWidget = mdiArea->addSubWindow(wdgtColumn);;

            connect(wdgtColumn, SIGNAL(closed()), SLOT(loadCmbBoxColumns()));

            mMapperDeleteWidget->setMapping(wdgtColumn, INSERTCOLUMN_W);
            connect(wdgtColumn, SIGNAL(closed()), mMapperDeleteWidget, SLOT(map()));
        }
        break;

    case ALTERCOLUMN_W:

        if (wdgtColumn == nullptr)
        {
            wdgtColumn = new InsertColumnWidget(sqlModel, this, InsertColumnWidget::UPDATE_MODE);

            subMdiWidget = mdiArea->addSubWindow(wdgtColumn);
            wdgtColumn->fillFields(currentColumn);

            connect(this, SIGNAL(columnSelected(QString)), wdgtColumn, SLOT(fillFields(QString)));
            connect(wdgtColumn, SIGNAL(closed()), SLOT(loadCmbBoxColumns()));

            mMapperDeleteWidget->setMapping(wdgtColumn, ALTERCOLUMN_W);
            connect(wdgtColumn, SIGNAL(closed()), mMapperDeleteWidget, SLOT(map()));
        }
        break;

    case INSERTROW_W:

        if (wdgtRow == nullptr)
        {
            wdgtRow = new InsertRowWidget(sqlModel, this);

            connect(this, SIGNAL(tableSelected(QString)), wdgtRow, SLOT(clearWidget()));

            subMdiWidget = mdiArea->addSubWindow(wdgtRow);
            sqlModel->refreshTable();

            mMapperDeleteWidget->setMapping(wdgtRow, INSERTROW_W);
            connect(wdgtRow, SIGNAL(closed()), mMapperDeleteWidget, SLOT(map()));
        }
        break;

    case LINK_W:
        if (wdgtLink == nullptr)
        {
            wdgtLink = new LinkWidget(sqlModel, this);

            connect(this, SIGNAL(dbSelected(QString)), wdgtLink, SLOT(loadCmbBoxTables()));
            connect(this, SIGNAL(tableSelected(QString)), wdgtLink, SLOT(loadCmbBoxFields()));

            subMdiWidget = mdiArea->addSubWindow(wdgtLink);

            mMapperDeleteWidget->setMapping(wdgtLink, LINK_W);
            connect(wdgtLink, SIGNAL(closed()), mMapperDeleteWidget, SLOT(map()));
        }
        break;

    case TABLEVIEW_W:

        if (tableView == nullptr)
        {
            tableView = new QTableViewS(this);
            tableView->setModel(sqlModel.get());
            tableView->setContextMenuPolicy(Qt::CustomContextMenu);
            connect(tableView, SIGNAL(customContextMenuRequested(QPoint)), SLOT(initTableViewContext(QPoint)));

            subMdiWidget = mdiArea->addSubWindow(tableView);


            mMapperDeleteWidget->setMapping(tableView, TABLEVIEW_W);
            connect(tableView, SIGNAL(closed()), mMapperDeleteWidget, SLOT(map()));
        }
        break;

    case FILTER_W:

        if (wdgtFilter == nullptr)
        {
            wdgtFilter = new FindSortWidget(sqlModel, this);
            connect(this, SIGNAL(tableSelected(QString)), wdgtFilter, SLOT(clearWidget()));

            subMdiWidget = mdiArea->addSubWindow(wdgtFilter);

            mMapperDeleteWidget->setMapping(wdgtFilter, FILTER_W);
            connect(wdgtFilter, SIGNAL(closed()), mMapperDeleteWidget, SLOT(map()));
        }
        break;

    case STUDENT_W:

        if (wdgtStudent == nullptr)
        {
            wdgtStudent = new StudentWidget(sqlModel->database(), mdiArea, this);

            subMdiWidget = mdiArea->addSubWindow(wdgtStudent);

            wdgtStudent->show();

            mMapperDeleteWidget->setMapping(wdgtStudent, STUDENT_W);
            connect(wdgtStudent, SIGNAL(closed()), mMapperDeleteWidget, SLOT(map()));
        }

        hideAllToolbars();

        break;
    }

    if (subMdiWidget != nullptr)
    {
        subMdiWidget->setAttribute(Qt::WA_DeleteOnClose);
        subMdiWidget->setWindowIcon(iconLightBase);
        subMdiWidget->show();
    }
}

void MdiMainWindow::deleteWidget(int id)
{
    switch (id)
    {
    case CREATEDB_W:
        wdgtDB = nullptr;
        break;

    case CREATETABLE_W:
        wdgtTable = nullptr;
        break;

    case INSERTCOLUMN_W:
        wdgtColumn = nullptr;
        break;

    case ALTERCOLUMN_W:
        wdgtColumn = nullptr;
        break;

    case INSERTROW_W:
        wdgtRow = nullptr;
        break;

    case LINK_W:
        wdgtLink = nullptr;
        break;

    case TABLEVIEW_W:
        tableView = nullptr;
        break;

    case FILTER_W:
        wdgtFilter = nullptr;
        break;

    case STUDENT_W:
        wdgtStudent = nullptr;
        showAllToolbars();
        break;
    }
}

void MdiMainWindow::deleteObject(int idObject)
{
    QMessageBox msgBox;
    QErrorMessage errorMsg;

    QString msgText;
    int res;

    switch(idObject)
    {
    case DATABASE:
        res = QMessageBox::question(this, tr("Delete database?"), tr("Are you sure?"), QMessageBox::Yes | QMessageBox::No);
        if (res == QMessageBox::Yes)
        {
            if (sqlModel->dropDataBase(currentDatabase))
            {
                  msgText = tr("Database ") + currentDatabase + tr(" has been deleted");
                  msgBox.setText(msgText);
                  msgBox.exec();
                  loadCmbBoxDB();
                  setCurrentDatabase(sqlModel->getDatabases().at(0));
            }
            else
            {
                errorMsg.showMessage(sqlModel->lastError().text());
                errorMsg.exec();
            }
        }
        break;
    case TABLE:
        res = QMessageBox::question(this, tr("Delete table?"), tr("Are you sure you want to delete a table?"), QMessageBox::Yes | QMessageBox::No);
        if (res == QMessageBox::Yes)
        {
            if (sqlModel->dropTable(currentTable))
            {
                msgText = tr("Table ") + currentTable + tr(" has been deleted");
                msgBox.setText(msgText);
                msgBox.exec();
                loadCmbBoxTables();

                if (!sqlModel->getTables().isEmpty())
                {
                    setCurrentTable(sqlModel->getTables().at(0));
                }
            }
            else
            {
                errorMsg.showMessage(sqlModel->lastError().text());
                errorMsg.exec();
            }
        }
        break;
    case ROW:

        if (tableView != nullptr)
        {
            int rowIndex = tableView->currentIndex().row();

            if (rowIndex != -1)
            {
                sqlModel->removeRow(tableView->currentIndex().row());
                sqlModel->refreshTable();
            }
            else
            {
                QMessageBox errorMsg;
                errorMsg.setText(tr("Record is not selected"));
                errorMsg.exec();
            }
        }
        else
        {
            QMessageBox errorMsg;
            errorMsg.setText(tr("At first activate table widget and choose record then"));
            errorMsg.exec();
        }

         break;
    case COLUMN:
        res = QMessageBox::question(this, tr("Delete column?"), tr("Are you sure you want to delete a column?"), QMessageBox::Yes | QMessageBox::No);
        if (res == QMessageBox::Yes)
        {
            if (sqlModel->dropColumn(currentColumn))
            {
                msgText = tr("Column ") + currentColumn + tr(" has been deleted");
                msgBox.setText(msgText);
                msgBox.exec();
                loadCmbBoxColumns();

                if (!sqlModel->getColumns().isEmpty())
                {
                    setCurrentColumn(sqlModel->getColumns().at(0));
                }
            }
            else
            {
                errorMsg.showMessage(sqlModel->lastError().text());
                errorMsg.exec();
            }
        }
        break;
    }
}



void MdiMainWindow::showAllWidgets()
{
    foreach (QMdiSubWindow* mdiSubWindow, mdiArea->subWindowList())
    {
        mdiSubWindow->show();
    }

}

void MdiMainWindow::hideAllWidgets()
{
    foreach (QMdiSubWindow* mdiSubWindow, mdiArea->subWindowList())
    {
        mdiSubWindow->hide();
    }
}



void MdiMainWindow::agregate()
{
    sqlModel->agregateExec(currentColumn, cmbBoxAgregates->currentIndex());
}



//TABLE VIEW SLOTS=========================================================================



void MdiMainWindow::initTableViewContext(QPoint point)
{
    QMenu* menu = new QMenu(this);

    menu->addAction(aInsertRow);

    menu->addAction(aDeleteRow);

    menu->addAction(aFilter);

    menu->addAction(aRefreshTable);

    menu->addAction(aPrintTable);

    menu->popup(tableView->mapToGlobal(point));
}

void MdiMainWindow::refreshTableView()
{
    sqlModel->refreshTable();
}

void MdiMainWindow::initPrintTableDialog()
{
    QString fileName = QFileDialog::getSaveFileName(
    this,
    tr("Save Document"),
    QDir::currentPath(),
    tr("Documents (*.odt)") );

    TableWriter tblWriter(sqlModel->getColumns());

    for (int i = 0; i < sqlModel->rowCount(); i++)
    {
        QStringList strList;
        for (int j = 0; j < sqlModel->columnCount(); j++)
        {
            strList.push_back(sqlModel->record(i).value(j).toString());
        }
        tblWriter.insertRow(strList);
    }

    tblWriter.writeTableFile(fileName);
}


//DIALOG INITALIZTION SLOTS===================================================================


void MdiMainWindow::initSaveDumpDialog()
{
    if (currentDatabase != "")
    {
        QString fileName = "";

        fileName = QFileDialog::getSaveFileName(
        this,
        tr("Save dump"),
        QDir::currentPath(),
        tr("Documents (*.sql)") );

        if (fileName != "")
        {
            sqlModel->writeDump(fileName);

            QMessageBox msgBox;
            msgBox.setText(tr("Dump saved"));
            msgBox.exec();
        }
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Database is not selected"));
        msgBox.exec();
    }
}

void MdiMainWindow::initLoadDumpDialog()
{
    if (currentDatabase != "")
    {
        QString fileName = "";

        fileName = QFileDialog::getOpenFileName(
        this,
        tr("Load dump"),
        QDir::currentPath(),
        tr("Documents (*.sql)") );

        if (fileName != "")
        {
            sqlModel->readDump(fileName);

            loadCmbBoxTables();

            QMessageBox msgBox;
            msgBox.setText(tr("Dump loaded"));
            msgBox.exec();
        }
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Database is not selected"));
        msgBox.exec();
    }
}



//CHANGE LANGUAGE SLOT


void MdiMainWindow::changeLanguage(int lang)
{
    QString strLangPath = app->applicationDirPath() + "/Languages";

    switch(lang)
    {
        case ENGLISH:
            app->removeTranslator(translator);
        break;

        case UKRAINIAN:
            app->removeTranslator(translator);

            qDebug() << strLangPath;

            translator->load("LightBase_ua", strLangPath);
            app->installTranslator(translator);
        break;

        case RUSSIAN:
            app->removeTranslator(translator);
            translator->load("LightBase_ru", strLangPath);
            app->installTranslator(translator);
        break;
    }
}
