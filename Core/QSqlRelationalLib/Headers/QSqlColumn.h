#ifndef QSQLCOLUMN_H
#define QSQLCOLUMN_H
#include <QString>
#include <map>


class QSqlTableStructure;   //previos declaration of friends class
class QSqlRelationalDataModel;

class QSqlColumn
{
public:

//enumeration of sql types
enum SqlDataType
{
    TINYINT = 1,
    SMALLINT = 2,
    MEDIUMINT = 3,
    INT = 4,
    BIGINT = 5,
    FLOAT = 6,
    DOUBLE = 7,
    DOUBLE_PRECISION = 8,
    REAL = 9,
    DECIMAL = 10,
    DATE = 11,
    DATETIME = 12,
    TIMESTAMP = 13,
    TIME = 14,
    YEAR = 15,
    CHAR = 16,
    VARCHAR = 17,
    TINYBLOB = 18,
    BLOB = 19,
    TEXT = 20,
    ENUM = 21,
    SET = 22
};

//overloaded constructors
QSqlColumn(QString _name);
QSqlColumn(QString _name, SqlDataType _type, int _precision);

//set methods for parameters of sql tables
void setName(QString _name);
void setType(SqlDataType _type);
void setPrecision(QString _precision);
void setNull(bool _null);
void setPrimaryKey(bool _prmKey);

//get methods
QString getName();

SqlDataType getCurrentType();                          //return the current type id
SqlDataType nameToId(QString typeName);         //return id of the type by its name
QString getTypeName();                          //returns the current type name
QString typeToName(SqlDataType _type);          //returns a name of a sql type by enum id
QStringList getTypesList();

QString getPrecision();                             //returns precision of the current

// bool get methods
bool isNull();
bool isPrimaryKey();
bool isAutoIncrement();

private:
    friend QSqlTableStructure;  //declaration of QSqlTableStructure class as friend to QSqlColumn
    friend QSqlRelationalDataModel;

    QString name;               //name of sql table
    SqlDataType type;           //sql type from SqlDatType
    QString precision;              //precision of sql field (column)

    bool null;
    bool primaryKey;
    bool autoIncrement;

    std::map<SqlDataType, QString> typeName;        //map of the sql types id and their names
    std::map<QString, SqlDataType> typeId;          //map of the sql types name and their id

};

#endif // QSQLCOLUMN_H
