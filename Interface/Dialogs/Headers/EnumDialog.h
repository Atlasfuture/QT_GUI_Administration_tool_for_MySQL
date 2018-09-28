#ifndef ENUMDIALOG_H
#define ENUMDIALOG_H

#include <QDialog>
#include <QEvent>
#include <QLineEdit>
#include <QPushButton>
#include <QList>
#include <QMap>
#include <QVBoxLayout>
#include <QSignalMapper>

class EnumDialog : public QDialog
{
    Q_OBJECT



//ELEMENTS===============================



    QHBoxLayout* hMainLayout;

    QVBoxLayout* vEditsLayout;
    QVBoxLayout* vButtonsDelete;
    QVBoxLayout* vButtonsManage;

    QList<QLineEdit*> lst_LineEdits;
    QList<QPushButton*> lst_DeleteButtons;

    QPushButton* pBtnAddEdit;
    QPushButton* pBtnConfirm;

    QSignalMapper* mMapperDelete;

    QMap<QWidget*, QWidget*> map;



//METHODS===============================

    void initElements();
    void initText();

//EVENTS=================================
    void changeEvent(QEvent* event);

public:
    EnumDialog();

signals:
    void enumCreated(QString strEnum);
public slots:
    void addEdit();
    void deleteEdit(QWidget *widget);
    void confirm();
};

#endif // ENUMDIALOG_H
