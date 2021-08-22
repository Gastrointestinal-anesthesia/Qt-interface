#ifndef MEDICINE_TYPE_H
#define MEDICINE_TYPE_H
#include <QString>
#include "entity.h"
#include "connection.h"

void selectMedicineType(QString strSql, std::vector<MedicineType>& vecMedicineType)
{
    QSqlDatabase db;
    createConnection(db);

    QSqlQuery result = db.exec(strSql);
    while(result.next())
    {
        MedicineType medicineType;
        medicineType.type = result.value("type").toInt();
        medicineType.en_name = result.value("en_name").toString();
        medicineType.cn_name = result.value("cn_name").toString();
        medicineType.unit = result.value("unit").toString();
        vecMedicineType.push_back(medicineType);
    }
}


#endif // MEDICINE_TYPE_H
