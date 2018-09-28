#include "Core/QSqlRelationalLib/Headers/QSqlColumn.h"
#include <QStringList>

QSqlColumn::QSqlColumn(QString _name)
{
    name = _name;
    null = true;
    autoIncrement = false;

    typeName[TINYINT] = "TINYINT";
    typeName[SMALLINT] = "SMALLINT";
    typeName[MEDIUMINT] = "MEDIUMINT";
    typeName[INT] = "INT";
    typeName[BIGINT] = "BIGINT";
    typeName[FLOAT] = "FLOAT";
    typeName[DOUBLE] = "DOUBLE";
    typeName[DOUBLE_PRECISION] = "DOUBLE PRECISION";
    typeName[REAL] = "REAL";
    typeName[DECIMAL] = "DECIMAL";
    typeName[DATE] = "DATE";
    typeName[DATETIME] = "DATETIME";
    typeName[TIMESTAMP] = "TIMESTAMP";
    typeName[TIME] = "TIME";
    typeName[YEAR] = "YEAR";
    typeName[CHAR] = "CHAR";
    typeName[VARCHAR] = "VARCHAR";
    typeName[TINYBLOB] = "TINYBLOB";
    typeName[BLOB] = "BLOB";
    typeName[TEXT] = "TEXT";
    typeName[ENUM] = "ENUM";
    typeName[SET] = "SET";

    typeId["TINYINT"] = TINYINT;
    typeId["SMALLINT"] = SMALLINT;
    typeId["MEDIUMINT"] = MEDIUMINT;
    typeId["INT"] = INT;
    typeId["BIGINT"] = BIGINT;
    typeId["FLOAT"] = FLOAT;
    typeId["DOUBLE"] = DOUBLE;
    typeId["DOUBLE PRECISION"] = DOUBLE_PRECISION;
    typeId["REAL"] = REAL;
    typeId["DECIMAL"] = DECIMAL;
    typeId["DATE"] = DATE;
    typeId["DATETIME"] = DATETIME;
    typeId["TIMESTAMP"] = TIMESTAMP;
    typeId["TIME"] = TIME;
    typeId["YEAR"] = YEAR;
    typeId["CHAR"] = CHAR;
    typeId["VARCHAR"] = VARCHAR;
    typeId["TINYBLOB"] = TINYBLOB;
    typeId["BLOB"] = BLOB;
    typeId["TEXT"] = TEXT;
    typeId["ENUM"] = ENUM;
    typeId["SET"] = SET;


}

QSqlColumn::QSqlColumn(QString _name, SqlDataType _type, int _precision)
{
    name = _name;
    type = _type;
    precision = _precision;
    autoIncrement = false;

    typeName[TINYINT] = "TINYINT";
    typeName[SMALLINT] = "SMALLINT";
    typeName[MEDIUMINT] = "MEDIUMINT";
    typeName[INT] = "INT";
    typeName[BIGINT] = "BIGINT";
    typeName[FLOAT] = "FLOAT";
    typeName[DOUBLE] = "DOUBLE";
    typeName[DOUBLE_PRECISION] = "DOUBLE PRECISION";
    typeName[REAL] = "REAL";
    typeName[DECIMAL] = "DECIMAL";
    typeName[DATE] = "DATE";
    typeName[DATETIME] = "DATETIME";
    typeName[TIMESTAMP] = "TIMESTAMP";
    typeName[TIME] = "TIME";
    typeName[YEAR] = "YEAR";
    typeName[CHAR] = "CHAR";
    typeName[VARCHAR] = "VARCHAR";
    typeName[TINYBLOB] = "TINYBLOB";
    typeName[BLOB] = "BLOB";
    typeName[TEXT] = "TEXT";
    typeName[ENUM] = "ENUM";
    typeName[SET] = "SET";

    typeId["TINYINT"] = TINYINT;
    typeId["SMALLINT"] = SMALLINT;
    typeId["MEDIUMINT"] = MEDIUMINT;
    typeId["INT"] = INT;
    typeId["BIGINT"] = BIGINT;
    typeId["FLOAT"] = FLOAT;
    typeId["DOUBLE"] = DOUBLE;
    typeId["DOUBLE PRECISION"] = DOUBLE_PRECISION;
    typeId["REAL"] = REAL;
    typeId["DECIMAL"] = DECIMAL;
    typeId["DATE"] = DATE;
    typeId["DATETIME"] = DATETIME;
    typeId["TIMESTAMP"] = TIMESTAMP;
    typeId["TIME"] = TIME;
    typeId["YEAR"] = YEAR;
    typeId["CHAR"] = CHAR;
    typeId["VARCHAR"] = VARCHAR;
    typeId["TINYBLOB"] = TINYBLOB;
    typeId["BLOB"] = BLOB;
    typeId["TEXT"] = TEXT;
    typeId["ENUM"] = ENUM;
    typeId["SET"] = SET;
}

void QSqlColumn::setName(QString _name)
{
    name = _name;
}

void QSqlColumn::setType(SqlDataType _type)
{
    type = _type;
}

void QSqlColumn::setPrecision(QString _precision)
{
    precision = _precision;
}


void QSqlColumn::setNull(bool _null)
{
    null = _null;
}

void QSqlColumn::setPrimaryKey(bool _prmKey)
{
    primaryKey = _prmKey;
}

QString QSqlColumn::getName()
{
    return name;
}

QSqlColumn::SqlDataType QSqlColumn::getCurrentType()
{
    return type;
}

QSqlColumn::SqlDataType QSqlColumn::nameToId(QString typeName)
{
     return typeId[typeName];
}

QString QSqlColumn::getPrecision()
{
    return precision;
}

bool QSqlColumn::isNull()
{
    return null;
}

bool QSqlColumn::isPrimaryKey()
{
    return primaryKey;
}

bool QSqlColumn::isAutoIncrement()
{
    return autoIncrement;
}

QString QSqlColumn::getTypeName()
{
    return typeName[type];
}

QString QSqlColumn::typeToName(SqlDataType _type)
{
    return typeName[_type];
}

QStringList QSqlColumn::getTypesList()
{
    QStringList strListTypes;

    strListTypes <<  typeName[TINYINT];
    strListTypes << typeName[SMALLINT];
    strListTypes << typeName[MEDIUMINT];
    strListTypes << typeName[INT];
    strListTypes << typeName[BIGINT];
    strListTypes << typeName[FLOAT];
    strListTypes << typeName[DOUBLE];
    strListTypes << typeName[DOUBLE_PRECISION];
    strListTypes << typeName[REAL];
    strListTypes << typeName[DECIMAL];
    strListTypes << typeName[DATE];
    strListTypes << typeName[DATETIME];
    strListTypes << typeName[TIMESTAMP];
    strListTypes << typeName[TIME];
    strListTypes << typeName[YEAR];
    strListTypes << typeName[CHAR];
    strListTypes << typeName[VARCHAR];
    strListTypes << typeName[TINYBLOB];
    strListTypes << typeName[BLOB];
    strListTypes << typeName[TEXT];
    strListTypes << typeName[ENUM];
    strListTypes << typeName[SET];

    return strListTypes;
}



