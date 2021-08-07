#ifndef PATIENT_VALUE_SQL_H
#define PATIENT_VALUE_SQL_H
#include "connection.h"

bool selectPatient(QString strSql, std::vector<Patient>& vecPatient)
{
    QSqlDatabase db;
    createConnection(db);

    QSqlQuery result = db.exec(strSql);
    while(result.next())
    {
        Patient paient;
        paient.strNumber = result.value("number").toString();
        if (paient.strNumber.length() <= 0)
            continue;
        paient.strAge= result.value("age").toString();
        int nsex = result.value("sex").toInt();
        paient.strSex = "";
        switch (nsex) {
        case 0:
        {
            paient.strSex = "未知";
        }
            break;
        case 1:
        {
            paient.strSex = "男";
        }
            break;
        case 2:
        {
            paient.strSex= "女";
        }
            break;
        default:
            break;
        }

        vecPatient.push_back(paient);
    }

}

/**
 * @brief selectPatientValue
 * @param strSql
 * @param vecPatientValue
 * @return
 */
bool selectPatientValue(QString strSql, std::vector<PatientValue>& vecPatientValue)
{
    QSqlDatabase db;
    createConnection(db);

    QSqlQuery result = db.exec(strSql);
    while(result.next())
    {
        PatientValue patientValue;
        patientValue.strNumber = result.value("number").toString();
        if(patientValue.strNumber.length() <= 0)
            continue;
        patientValue.RATE= result.value("RATE").toInt();
        patientValue.DIAP= result.value("DIAP").toInt();
        patientValue.SYSP= result.value("SYSP").toInt();
        patientValue.SpO2= result.value("SpO2").toInt();
        patientValue.BISr= result.value("BISr").toInt();
        patientValue.strCreateTime= result.value("create_time").toDateTime();

        vecPatientValue.push_back(patientValue);
    }
}


#endif // PATIENT_VALUE_SQL_H
