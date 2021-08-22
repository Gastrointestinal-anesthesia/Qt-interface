#ifndef ENTITY_H
#define ENTITY_H

#include <QString>
#include <QObject>
#include <QDateTime>

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
    int BIS;

}PatientValue;

typedef struct patient_medicine
{
    QString strNumber;
    QString value;
    QString name;
    QString strCreateTime;
}PatientMedicine;

typedef struct medicine_type
{
    int type;
    QString en_name;
    QString cn_name;
    QString unit;
}MedicineType;

typedef struct event_type
{
    int operate_type;
    int type;
    QString en_name;
    QString cn_name;
}EventType;


#endif // ENTITY_H
