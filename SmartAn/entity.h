#ifndef ENTITY_H
#define ENTITY_H

typedef struct patient
{
    QString strNumber;
    QString strAge;
    QString strSex;

}Patient;

typedef struct patient_value
{
    QString strNumber;
    QDateTime strCreateTime;
    int RATE;
    int DIAP;
    int SYSP;
    int SpO2;
    int BISr;

}PatientValue;

#endif // ENTITY_H