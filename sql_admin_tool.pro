TEMPLATE += app

QTPLUGIN += qsqlmysql


CONFIG += C++11

QT += sql
QT += widgets

ICON += LightBase.ico

SOURCES += \
    main.cpp \
    Core/QSqlRelationalLib/Sources/QSqlColumn.cpp \
    Core/QSqlRelationalLib/Sources/QSqlRelationalDataModel.cpp \
    Core/QSqlRelationalLib/Sources/QSqlTableStructure.cpp \
    Interface/Widgets/Sources/CreateDBwidget.cpp \
    Interface/Widgets/Sources/CreateTableWidget.cpp \
    Interface/Widgets/Sources/InsertColumnWidget.cpp \
    Interface/Widgets/Sources/InsertRowWidget.cpp \
    Interface/Widgets/Sources/LinkWidget.cpp \
    Interface/MdiMainWindow.cpp \
    Interface/Widgets/Sources/FindSortWidget.cpp \
    Core/Writer/TableWriter.cpp \
    Interface/Widgets/Sources/QTableViewS.cpp \
    Interface/Dialogs/Sources/ConnectDialog.cpp \
    Interface/Dialogs/Sources/EnumDialog.cpp \
    Core/QSqlRelationalLib/Sources/DataDump.cpp \
    Addons/Student_0_1/CourseDialog.cpp \
    Addons/Student_0_1/DepartmentDialog.cpp \
    Addons/Student_0_1/GroupDialog.cpp \
    Addons/Student_0_1/LocationDialog.cpp \
    Addons/Student_0_1/StudentDialog.cpp \
    Addons/Student_0_1/StudentTemplate.cpp \
    Addons/Student_0_1/StudentViewWidget.cpp \
    Addons/Student_0_1/StudentWidget.cpp \
    Addons/Student_0_1/Reports/AgeDistributionReport.cpp \
    Addons/Student_0_1/CheckBoxGroup.cpp

HEADERS += \
    Core/QSqlRelationalLib/Headers/QSqlColumn.h \
    Core/QSqlRelationalLib/Headers/QSqlRelationalDataModel.h \
    Core/QSqlRelationalLib/Headers/QSqlTableStructure.h \
    Interface/Widgets/Header/CreateDBwidget.h \
    Interface/Widgets/Header/CreateTableWidget.h \
    Interface/Widgets/Header/InsertColumnWidget.h \
    Interface/Widgets/Header/InsertRowWidget.h \
    Interface/Widgets/Header/LinkWidget.h \
    Interface/MdiMainWindow.h \
    Interface/Widgets/Header/FindSortWidget.h \
    Core/Writer/TableWriter.h \
    Interface/Widgets/Header/QTableViewS.h \
    Interface/Dialogs/Headers/ConnectDialog.h \
    Interface/Dialogs/Headers/EnumDialog.h \
    Core/QSqlRelationalLib/Headers/DataDump.h \
    Addons/Student_0_1/Address.h \
    Addons/Student_0_1/CourseDialog.h \
    Addons/Student_0_1/DepartmentDialog.h \
    Addons/Student_0_1/GroupDialog.h \
    Addons/Student_0_1/LocationDialog.h \
    Addons/Student_0_1/Phone.h \
    Addons/Student_0_1/StudentDialog.h \
    Addons/Student_0_1/StudentTemplate.h \
    Addons/Student_0_1/StudentViewWidget.h \
    Addons/Student_0_1/StudentWidget.h \
    Addons/Student_0_1/Reports/AgeDistributionReport.h \
    Addons/Student_0_1/Student.h \
    Addons/Student_0_1/CheckBoxGroup.h

TARGET = LightBase

TRANSLATIONS += LightBase_UA.ts \
                LightBase_ru.ts

RC_FILE = LightBase.rc

