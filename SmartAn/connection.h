#ifndef CONNECTION_H
#define CONNECTION_H

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>

/**
 * mysql 全局配置属性
 */
inline QString g_mysql_hostName = "hostname";
inline int g_mysql_port = 3306;
inline QString g_mysql_user = "root";
inline QString g_mysql_password = "888888";
inline QString g_mysql_databaseName = "SmartAn";

static bool createConnection(QSqlDatabase& db)
{
    //打开数据库
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(g_mysql_hostName);      //连接数据库主机名，这里需要注意（若填的为”127.0.0.1“，出现不能连接，则改为localhost)
    db.setPort(g_mysql_port);                 //连接数据库端口号，与设置一致
    db.setUserName(g_mysql_user);          //数据库用户名，与设置一致
    db.setPassword(g_mysql_password);    //数据库密码，与设置一致
    db.setDatabaseName(g_mysql_databaseName);      //连接数据库名，与设置一致
    db.open();
    if(!db.open())
    {
        return false;
    }
    if (!db.open()) {
        QMessageBox::critical(0, QObject::tr("无法打开数据库"),
                              "无法创建数据库连接！ ", QMessageBox::Cancel);
        return false;
    }
    return true;
}

static bool insertSql(QString strSql)
{
    QSqlDatabase db;
    createConnection(db);

    QSqlQuery query(db);
    bool result = query.exec(strSql);
    QSqlDatabase::removeDatabase("QMYSQL");
    return result;
}

#endif // CONNECTION_H
