#ifndef QTABLEVIEWS_H
#define QTABLEVIEWS_H

#include <QTableView>


class QTableViewS : public QTableView
{
    Q_OBJECT

    void closeEvent(QCloseEvent *event);
public:
    explicit QTableViewS(QWidget* parent);
signals:
    void closed();
};

#endif // QTABLEVIEWS_H
