#ifndef STUDENTVIEWWIDGET_H
#define STUDENTVIEWWIDGET_H

#include <QWidget>
#include <QTableView>

#include <Core/QSqlRelationalLib/Headers/QSqlRelationalDataModel.h>

#include <memory>

#include <QVBoxLayout>

class StudentViewWidget : public QWidget
{
    Q_OBJECT


    std::shared_ptr<QSqlRelationalDataModel> sqlModel;

    QTableView* tableView;


    //layout
    QVBoxLayout *vLayout;

    void initElements();
public:
    explicit StudentViewWidget(std::shared_ptr<QSqlRelationalDataModel> _sqlModel, QWidget *parent = 0);

signals:

public slots:
};

#endif // STUDENTVIEWWIDGET_H
