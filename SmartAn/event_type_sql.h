#ifndef EVENT_TYPE_H
#define EVENT_TYPE_H

#include <QString>
#include "entity.h"
#include "connection.h"

void selectEventType(QString strSql, std::vector<EventType>& vecEventType)
{
    QSqlDatabase db;
    createConnection(db);

    QSqlQuery result = db.exec(strSql);
    while(result.next())
    {
        EventType eventType;
        eventType.operate_type = result.value("operate_type").toInt();
        eventType.type = result.value("type").toInt();
        eventType.en_name = result.value("en_name").toString();
        eventType.cn_name = result.value("cn_name").toString();
        vecEventType.push_back(eventType);
    }
}


#endif // EVENT_TYPE_H
