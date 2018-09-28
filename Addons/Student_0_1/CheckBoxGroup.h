#ifndef BUTTONGROUP_H
#define BUTTONGROUP_H

#include <QObject>
#include <QList>
#include <QCheckBox>
#include <QPair>
#include <QSignalMapper>
#include <QMap>

class CheckBoxGroup : public QObject
{
    Q_OBJECT

    QList<QCheckBox*> lChkBox;

    QMap<QWidget*, int> chkBoxMap;

    QSignalMapper mapper;
public:
    explicit CheckBoxGroup(QObject *parent = 0);

    void addButton(QCheckBox* chkBox, int groupID);
signals:

public slots:

    void uncheck(QWidget* chkBox);
};

#endif // BUTTONGROUP_H
