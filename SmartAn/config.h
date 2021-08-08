#ifndef CONFIG_H
#define CONFIG_H
#include <QString>

/**
 * mysql 全局配置默认属性
 */
inline QString g_mysql_hostName = "hostname";
inline int g_mysql_port = 3306;
inline QString g_mysql_user = "root";
inline QString g_mysql_password = "888888";
inline QString g_mysql_databaseName = "SmartAn";

/**
 * message 全局配置默认属性
 */
inline int g_message_RATE = 255;
inline int g_message_DIAP = 255;
inline int g_message_SYSP = 255;
inline int g_message_SpO2 = 255;
inline int g_message_BISr = 255;

#endif // CONFIG_H
