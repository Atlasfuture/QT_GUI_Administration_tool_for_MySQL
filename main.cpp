#include <QApplication>
#include <memory>
#include <QSqlDatabase>
#include <QTranslator>
#include <QDebug>
#include <QPluginLoader>


#include "Interface/MdiMainWindow.h"
#include "Interface/Dialogs/Headers/ConnectDialog.h"
#include "Core/QSqlRelationalLib/Headers/QSqlRelationalDataModel.h"

#include "Interface/Dialogs/Headers/EnumDialog.h"




int main(int argc, char** argv)
{
     qDebug() << qVersion();

    QApplication app(argc, argv);

    QTranslator* lbTranslate = new QTranslator;

    QString strLangPath = app.applicationDirPath() + "/Languages";

    app.installTranslator(lbTranslate);


    QSqlDatabase sqlDatabase = QSqlDatabase::addDatabase("QMYSQL");

    qDebug() << QApplication::libraryPaths();

    std::shared_ptr<QSqlDatabase> sptr_sqlDatabase(&sqlDatabase);
    std::shared_ptr<QSqlRelationalDataModel> sptr_sqlModel(new QSqlRelationalDataModel(sqlDatabase));

    std::shared_ptr<ConnectDialog> sptr_wdgtConnect(new ConnectDialog(sptr_sqlDatabase, &app, lbTranslate));
    sptr_wdgtConnect->show();

    std::shared_ptr<MdiMainWindow> sptr_mdiMain(new MdiMainWindow(sptr_sqlDatabase, sptr_sqlModel, &app, lbTranslate));

    app.connect(sptr_wdgtConnect.get(), SIGNAL(confirmed()), sptr_mdiMain.get(), SLOT(showMaximized()));


    return app.exec();
}
