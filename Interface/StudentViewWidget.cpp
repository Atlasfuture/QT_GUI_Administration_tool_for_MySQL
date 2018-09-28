#include "StudentViewWidget.h"

StudentViewWidget::StudentViewWidget(std::shared_ptr<QSqlRelationalDataModel> _sqlModel, QWidget *parent) : QWidget(parent)
{
    sqlModel = _sqlModel;
}

void StudentViewWidget::initElements()
{
    vLayout = new QVBoxLayout;

    tableView = new QTableView;

    vLayout->addWidget(tableView);
    setLayout(tableView);


}
