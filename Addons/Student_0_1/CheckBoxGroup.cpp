#include "CheckBoxGroup.h"

CheckBoxGroup::CheckBoxGroup(QObject *parent) : QObject(parent)
{
    connect(&mapper, SIGNAL(mapped(QWidget*)), SLOT(uncheck(QWidget*)));
}

void CheckBoxGroup::addButton(QCheckBox *chkBox, int groupID)
{
    auto pair = qMakePair(chkBox, groupID);

    lChkBox.push_back(chkBox);

    chkBoxMap[chkBox] = groupID;

    mapper.setMapping(chkBox, chkBox);

    connect(chkBox, SIGNAL(clicked(bool)), &mapper, SLOT(map()));
}

void CheckBoxGroup::uncheck(QWidget *chkBox)
{
    int groupID = chkBoxMap[chkBox];

    foreach (QCheckBox* chkBox, lChkBox)
    {
        if (chkBoxMap[chkBox] != groupID)
        {
            chkBox->setChecked(false);
        }
    }
}
