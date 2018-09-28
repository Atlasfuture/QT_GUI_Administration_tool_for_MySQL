#include "TableWriter.h"

TableWriter::TableWriter(QStringList headerStr)
{
    QTextTableFormat tableFormat;
    tableFormat.setCellPadding(5);
    tableFormat.setHeaderRowCount(1);
    tableFormat.setBorder(QTextTableFormat::BorderStyle_Solid);

    tableFormat.setWidth(QTextLength(QTextLength::PercentageLength, 100));

    txtCursor = QTextCursor(&txtDocument);

    txtCursor.insertTable(1, headerStr.count(), tableFormat);

    txtTable = txtCursor.currentTable();

    for (int i = 0; i < txtTable->columns(); i++)
    {
        txtCursor.insertText(headerStr.at(i));
        txtCursor.movePosition(QTextCursor::NextCell);
    }
    currentRow = 0;
    currentColumn = 0;
}

void TableWriter::insertRow(QStringList rowStr)
{
    txtTable->appendRows(1);
    txtCursor.movePosition(QTextCursor::PreviousRow);

    for(int i = 0; i < rowStr.count(); i++)
    {
        txtCursor.movePosition(QTextCursor::NextCell);
        txtCursor.insertText(rowStr.at(i));

    }
}


void TableWriter::writeTableFile(QString fileName)
{
    QString fName = fileName;

    if (!fileName.contains(".odt"))
    {
        fName += ".odt";
    }

    QTextDocumentWriter docWriter(fName);
    docWriter.write(&txtDocument);

}
