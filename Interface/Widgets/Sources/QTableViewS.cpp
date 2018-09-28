#include "Interface/Widgets/Header/QTableViewS.h"
#include <QMessageBox>


void QTableViewS::closeEvent(QCloseEvent *event)
{
    emit closed();
}

QTableViewS::QTableViewS(QWidget *parent) : QTableView(parent)
{

}
