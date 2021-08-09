#ifndef PATIENT_MEDICINE_SQL_H
#define PATIENT_MEDICINE_SQL_H
#include <QString>
#include "connection.h"

bool judgePatientMedicine(QString strSql)
{
    QSqlDatabase db;
    createConnection(db);

    QSqlQuery result = db.exec(strSql);
    while(result.next())
    {

        int count  = result.value("count").toInt();
        if (count == 0)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
}


void selectMedicineInfo(QString strSql, std::vector<PatientMedicine>& vecPatientMedicine)
{
    QSqlDatabase db;
    createConnection(db);

    QSqlQuery result = db.exec(strSql);
    while(result.next())
    {
        PatientMedicine patientMedicine;
        patientMedicine.name = result.value("name").toString();
        patientMedicine.value = result.value("value").toString();
        patientMedicine.strCreateTime = result.value("create_time").toString();
        vecPatientMedicine.push_back(patientMedicine);
    }
}


#endif // PATIENT_MEDICINE_SQL_H
