#include "mainwindow.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QSplashScreen>
#include <QPixmap>
#include <QDebug>
#include <QElapsedTimer>
#include <QDateTime>
#include "config.h"

QSettings *g_psetting = nullptr;

/**
 * @brief SetConfigData
 * @param qstrnodename
 * @param qstrkeyname
 * @param qvarvalue
 */
void SetConfigData(QString qstrnodename, QString qstrkeyname, QVariant qvarvalue)
{
    if (g_psetting) {
        g_psetting->setValue(QString("/%1/%2").arg(qstrnodename).arg(qstrkeyname), qvarvalue);
    }
}

/**
 * @brief GetConfigData
 * @param qstrnodename
 * @param qstrkeyname
 * @return
 */
QVariant GetConfigData(QString qstrnodename, QString qstrkeyname)
{
    QVariant qvar = -1;
    if (g_psetting) {
        qvar = g_psetting->value(QString("/%1/%2").arg(qstrnodename).arg(qstrkeyname));
    }
    return qvar;
}

int main(int argc, char *argv[])
{
#if (QT_VERSION >= QT_VERSION_CHECK(5,9,0))
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication a(argc, argv);

    QString filePath = QCoreApplication::applicationDirPath();

    QPixmap pixmap(filePath + "/screen.jpg");
    QSplashScreen screen(pixmap);
    screen.show();
    screen.setFont(QFont("宋体", 30, QFont::Bold));
    //QColor color;

    //color.setRgb(0,100,149,237);
    //screen.showMessage("麻醉辅助系统", Qt::AlignCenter, color);

    int delayTime = 3;
    QElapsedTimer timer;
    timer.start();
    while(timer.elapsed() < (delayTime * 1000))
    { 
        a.processEvents();
    }


    screen.hide();

    QString configFileName = filePath + "/config.ini";
    g_psetting = new QSettings(configFileName, QSettings::IniFormat);
    QFileInfo fileInfo(configFileName);
    if(!fileInfo.exists())
    {
        // 打印创建文件的路径
        qDebug("%s is not exists.", configFileName.toLatin1().data());
        // 写入默认值
        //        SetConfigData(“mysql”, “on_off”, false);
        //        SetConfigData(“mysql”, “width”, default_mysql_width);
        //        SetConfigData(“mysql”, “height”, default_mysql_height);
    }
    else
    {
        g_mysql_hostName = GetConfigData("mysql", "hostname").toString();
        g_mysql_port = GetConfigData("mysql", "port").toInt();
        g_mysql_user = GetConfigData("mysql", "user").toString();
        g_mysql_password = GetConfigData("mysql", "password").toString();
        g_mysql_databaseName = GetConfigData("mysql", "databaseName").toString();

        g_message_RATE = GetConfigData("message", "RATE").toInt();
        g_message_DIAP = GetConfigData("message", "DIAP").toInt();
        g_message_SYSP = GetConfigData("message", "SYSP").toInt();
        g_message_SpO2 = GetConfigData("message", "SpO2").toInt();
        g_message_BISr = GetConfigData("message", "BISr").toInt();
    }


    MainWindow w;

    w.show();
    w.initchildposscale();
    w.caluteDpi();
    w.setWindowIcon(QIcon("logo.ico"));
    return a.exec();
}
