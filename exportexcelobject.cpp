#include "exportexcelobject.h"
#include <QSqlQuery>
#include <QtDebug>
#include <QVariant>
#include <QSqlError>

int ExportExcelObject::export2Excel()
{
    if(fieldList.size() <= 0)
    {
        qDebug() << "ExportExcelObject::export2Excel failed: No fields defined.";
        return -1;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", "excelexport");
    if(!db.isValid())
    {
        qDebug() << "ExportExcelObject::export2Excel failed: QODBC not supported.";
        return -2;
    }

    // Set the DSN string
    QString dsn = QString("DRIVER={Microsoft Excel Driver (*.xls)};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;DBQ=%1").
                  arg(excelFilePath);
    db.setDatabaseName(dsn);
    if(!db.open())
    {
        qDebug() << "ExportExcelObject::export2Excel failed: Create Excel file failed by DRIVER={Microsoft Excel Driver (*.xls)}.";
        return -3;
    }

    QSqlQuery query(db);

    // Drop the table if it already exists
    QString sSql = QString("DROP TABLE [%1]").arg(sheetName);
    query.exec(sSql);

    // Create the table (sheet in Excel file)
    sSql = QString("CREATE TABLE [%1] (").arg(sheetName);
    for (int i = 0; i < fieldList.size(); i++)
    {
        sSql += QString("[%1] %2").arg(fieldList.at(i)->sFieldName).arg(fieldList.at(i)->sFieldType);
        if(i < fieldList.size() - 1)
            sSql += ", ";
    }
    sSql += ")";
    query.prepare(sSql);

    if(!query.exec())
    {
        qDebug() << "ExportExcelObject::export2Excel failed: Create Excel sheet failed.";
        return -4;
    }

    // Add all rows
    sSql = QString("INSERT INTO [%1] (").arg(sheetName);
    for (int i = 0; i < fieldList.size(); i++)
    {
        sSql += QString("[%1]").arg(fieldList.at(i)->sFieldName);
        if(i < fieldList.size() - 1)
            sSql += ", ";
    }
    sSql += ") VALUES (";
    for (int i = 0; i < fieldList.size(); i++)
    {
        sSql += QString(":data%1").arg(i);
        if(i < fieldList.size() - 1)
            sSql += ", ";
    }
    sSql += ")";

    // Debug the final SQL query
    qDebug() << "Constructed SQL Query:" << sSql;

    int r, iRet = 0;
    for(r = 0; r < tableView->model()->rowCount(); r++)
    {
        query.prepare(sSql);
        for (int c = 0; c < fieldList.size(); c++)
        {
            QVariant value = tableView->model()->data(tableView->model()->index(r, fieldList.at(c)->iCol));
            query.bindValue(QString(":data%1").arg(c), value);
        }

        if(query.exec())
            iRet++;
        else
            qDebug() << "Insert failed:" << query.lastError().text();

        if(r % 10 == 0)
            emit exportedRowCount(r);
    }

    emit exportedRowCount(r);

    return iRet;
}
