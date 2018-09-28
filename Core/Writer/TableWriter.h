#ifndef TABLEWRITER_H
#define TABLEWRITER_H

#include <memory>


#include <QTextDocument>
#include <QTextCursor>
#include <QTextTable>
#include <QTextTableFormat>
#include <QTextDocumentWriter>



class TableWriter
{
    QTextDocument txtDocument;
    QTextCursor txtCursor;
    QTextTable *txtTable;
    QTextDocumentWriter docWriter;

    //table coordinates
    int currentRow;
    int currentColumn;


public:
    TableWriter(QStringList headerStr);

    void insertRow(QStringList rowStr);
    void writeTableFile(QString fileName);
};

#endif // TABLEWRITER_H
