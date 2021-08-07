#ifndef PATIENTSQL_H
#define PATIENTSQL_H
#include "connection.h"

static int getMaxPatientNumber()
{
    QSqlDatabase db;
    createConnection(db);

    QString strSql = "SELECT MAX(number) AS number FROM patient";

    QSqlQuery result = db.exec(strSql);
    int number = 0;
    while(result.next())
    {
        number =  result.value("number").toInt();
    }
    QSqlDatabase::removeDatabase("QMYSQL");
    return number;
}


#endif // PATIENTSQL_H
