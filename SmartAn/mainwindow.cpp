#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCharts>
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
#include <string>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QGraphicsScene>
#include "entity.h"
#include "config.h"
#include "connection.h"
#include "patient_sql.h"
#include "patient_value_sql.h"
#include "patient_medicine_sql.h"

using namespace std;
using namespace QtCharts;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupRealtimeData(ui->widgetRATE, "心率", "血氧");
    setupRealtimeData(ui->widgetPressure, "舒张压", "收缩压");
    setupRealtimeData(ui->widgetBIS, "BIS", "");

    setupRealtimeData(ui->widgetHisPressure, "舒张压", "收缩压");
    setupRealtimeData(ui->widgetHisBIS, "血氧", "BIS");

    ui->widgetPressure->replot();
    connect(this, SIGNAL(messageSignal(QVariant, QString)), this, SLOT(recvMsg(QVariant, QString)), Qt::QueuedConnection);
    m_thread.setObjectName("recvMsg");
    m_thread.setParent(this);
    m_thread.start();
    QDateTime currentDateTime = QDateTime::currentDateTime();
    ui->dateHisStartTime->setDateTime(currentDateTime);
    ui->dateHisStopTime->setDateTime(currentDateTime);
    QDesktopWidget *desktopwidget = QApplication::desktop();
    connect(desktopwidget,SIGNAL(resized(int)),this,SLOT(dpichanged()));
    //m_autoResizeHandler=new AutoResize(this,this->rect().width(),this->rect().height());
    //先与要自适应的枚举量控件进行或运算算出值
    //m_autoResizeHandler->setAutoResizeFlag(
    //           AutoResize::INCLUDE_BUTTON|AutoResize::INCLUDE_COMBOBOX|
    //           AutoResize::INCLUDE_EDITOR|AutoResize::INCLUDE_LABEL
    //          );
    // m_autoResizeHandler->addOtherItem(ui->frame);
    //m_autoResizeHandler->pushAllResizeItem();
    initHiDpi();
}

/**
 * @brief 画图初始化
 * @param customPlot
 * @param first
 * @param second
 */
void MainWindow::setupRealtimeData(QCustomPlot *customPlot, const QString& first, const QString& second)
{
    customPlot->addGraph(); // blue line
    customPlot->graph(0)->setPen(QPen(QColor(100,149,237)));
    customPlot->graph(0)->setName(first);
    //customPlot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    //customPlot->graph(0)->setAntialiasedFill(false);
    customPlot->addGraph(); // red line
    customPlot->graph(1)->setPen(QPen(QColor(46,139,87)));
    customPlot->graph(1)->setName(second);
    //customPlot->graph(0)->setChannelFillGraph(customPlot->graph(1));

    customPlot->yAxis->setRange(0, 100);

    customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    customPlot->xAxis->setAutoTickStep(false);
    customPlot->xAxis->setTickStep(8);
    customPlot->axisRect()->setupFullAxesBox();

    // make left and bottom axes transfer their ranges to right and top axes:
    // connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    // connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    // connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    // dataTimer.start(2000); // Interval 0 means to refresh as fast as possible
    customPlot->legend->setVisible(true);
}

void  MainWindow::realtimeDataSlot(double RATE,double DIAP,
                                   double SYSP,double SpO2,
                                   double BISr)
{
    //key的单位是秒
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    qsrand(QTime::currentTime().msec() + QTime::currentTime().second() * 10000);

    /*******************************************************/
    if (RATE != MESSAGE_INVALID)
    {
        if (RATE >= g_message_RATE)
        {

        }
        else
        {
            ui->widgetRATE->graph(0)->addData(key, RATE); // 添加数据1到曲线1
        }
    }
    if (SpO2 != MESSAGE_INVALID)
    {
        ui->widgetRATE->graph(1)->addData(key, SpO2); // 添加数据2到曲线2
    }

    //删除8秒之前的数据。这里的8要和下面设置横坐标宽度的8配合起来
    //才能起到想要的效果，可以调整这两个值，观察显示的效果。
    ui->widgetRATE->graph(0)->removeDataBefore(key-60);
    ui->widgetRATE->graph(1)->removeDataBefore(key-60);

    //自动设定graph(1)曲线y轴的范围，如果不设定，有可能看不到图像
    //也可以用ui->widgetPressure->yAxis->setRange(up,low)手动设定y轴范围
    ui->widgetRATE->graph(0)->rescaleValueAxis();
    ui->widgetRATE->graph(1)->rescaleValueAxis(true);

    //这里的8，是指横坐标时间宽度为8秒，如果想要横坐标显示更多的时间
    //就把8调整为比较大到值，比如要显示60秒，那就改成60。
    //这时removeDataBefore(key-8)中的8也要改成60，否则曲线显示不完整。
    ui->widgetRATE->yAxis->setRange(0, 100);
    ui->widgetRATE->xAxis->setRange(key+0.25, 60, Qt::AlignRight);//设定x轴的范围
    ui->widgetRATE->replot();


    /*******************************************************/
    if (DIAP != MESSAGE_INVALID)
    {
        ui->widgetPressure->graph(0)->addData(key, DIAP);//添加数据1到曲线1
    }
    if (SYSP != MESSAGE_INVALID)
    {
        ui->widgetPressure->graph(1)->addData(key, SYSP);//添加数据2到曲线2
    }
    //删除8秒之前的数据。这里的8要和下面设置横坐标宽度的8配合起来
    //才能起到想要的效果，可以调整这两个值，观察显示的效果。
    ui->widgetPressure->graph(0)->removeDataBefore(key-60);
    ui->widgetPressure->graph(1)->removeDataBefore(key-60);

    //自动设定graph(1)曲线y轴的范围，如果不设定，有可能看不到图像
    //也可以用ui->widgetPressure->yAxis->setRange(up,low)手动设定y轴范围
    ui->widgetPressure->graph(0)->rescaleValueAxis();
    ui->widgetPressure->graph(1)->rescaleValueAxis(true);

    //这里的8，是指横坐标时间宽度为8秒，如果想要横坐标显示更多的时间
    //就把8调整为比较大到值，比如要显示60秒，那就改成60。
    //这时removeDataBefore(key-8)中的8也要改成60，否则曲线显示不完整。
    ui->widgetPressure->yAxis->setRange(0, 100);
    ui->widgetPressure->xAxis->setRange(key+0.25, 60, Qt::AlignRight);//设定x轴的范围
    ui->widgetPressure->replot();


    /*******************************************************/
    if (SpO2 != MESSAGE_INVALID)
    {
        ui->widgetBIS->graph(0)->addData(key, SpO2); // 添加数据1到曲线1
    }
    //删除8秒之前的数据。这里的8要和下面设置横坐标宽度的8配合起来
    //才能起到想要的效果，可以调整这两个值，观察显示的效果。
    ui->widgetBIS->graph(0)->removeDataBefore(key-60);

    //自动设定graph(1)曲线y轴的范围，如果不设定，有可能看不到图像
    //也可以用ui->widgetPressure->yAxis->setRange(up,low)手动设定y轴范围
    ui->widgetBIS->graph(0)->rescaleValueAxis();

    //这里的8，是指横坐标时间宽度为8秒，如果想要横坐标显示更多的时间
    //就把8调整为比较大到值，比如要显示60秒，那就改成60。
    //这时removeDataBefore(key-8)中的8也要改成60，否则曲线显示不完整。
    ui->widgetBIS->yAxis->setRange(0, 100);
    ui->widgetBIS->xAxis->setRange(key+0.25, 60, Qt::AlignRight); // 设定x轴的范围
    ui->widgetBIS->replot();

    /*******************************************************/
    if (RATE != MESSAGE_INVALID)
    {
        if (RATE >= g_message_RATE)
        {
            ui->lblMonitorRATECurrentVal->setStyleSheet("color: red;");
            ui->lblMonitorRATECurrentVal->setText(QString::number(RATE));
        }
        else
        {
            ui->lblMonitorRATECurrentVal->setStyleSheet("color: rgb(100,149,237);");
            ui->lblMonitorRATECurrentVal->setText(QString::number(RATE));
        }
    }
    if (SpO2 != MESSAGE_INVALID)
    {
        ui->lblMonitorSpO2CurrentVal->setText(QString::number(SpO2));
    }

    if (DIAP != MESSAGE_INVALID)
    {
        ui->lblMonitorDIAPCurrentVal->setText(QString::number(DIAP));
    }
    if (SYSP != MESSAGE_INVALID)
    {
        ui->lblMonitorSYSPCurrentVal->setText(QString::number(SYSP));
    }

    if (BISr != MESSAGE_INVALID)
    {
        ui->lblMonitorBISCurrentVal->setText(QString::number(BISr));
    }
}

/**
 * @brief historyShow
 * @param RATE
 * @param DIAP
 * @param SYSP
 * @param SpO2
 * @param SaO2
 * @param BIS
 */
void  MainWindow::historyShow(QDateTime datatime, double RATE,double DIAP,
                              double SYSP,double SpO2,
                              double BIS)
{
    // key的单位是秒
    double key = datatime.toMSecsSinceEpoch()/1000.0;
    qsrand(QTime::currentTime().msec() + QTime::currentTime().second() * 10000);

    // if (ui->checkBox_temp->isChecked())
    ui->widgetHisPressure->graph(0)->addData(key, RATE);//添加数据1到曲线1
    // if (ui->checkBox_hui->isChecked())
    ui->widgetHisPressure->graph(1)->addData(key, DIAP);//添加数据2到曲线2
    //删除8秒之前的数据。这里的8要和下面设置横坐标宽度的8配合起来
    //才能起到想要的效果，可以调整这两个值，观察显示的效果。
    ui->widgetHisPressure->graph(0)->removeDataBefore(key-60);
    ui->widgetHisPressure->graph(1)->removeDataBefore(key-60);

    //自动设定graph(1)曲线y轴的范围，如果不设定，有可能看不到图像
    //也可以用ui->widgetPressure->yAxis->setRange(up,low)手动设定y轴范围
    ui->widgetHisPressure->graph(0)->rescaleValueAxis();
    ui->widgetHisPressure->graph(1)->rescaleValueAxis(true);

    //这里的8，是指横坐标时间宽度为8秒，如果想要横坐标显示更多的时间
    //就把8调整为比较大到值，比如要显示60秒，那就改成60。
    //这时removeDataBefore(key-8)中的8也要改成60，否则曲线显示不完整。
    ui->widgetHisPressure->yAxis->setRange(0, 100);
    ui->widgetHisPressure->xAxis->setRange(key+0.25, 60, Qt::AlignRight);//设定x轴的范围
    ui->widgetHisPressure->replot();


    /*******************************************************/

    // if (ui->checkBox_temp->isChecked())
    ui->widgetHisBIS->graph(0)->addData(key, SpO2);//添加数据1到曲线1
    // if (ui->checkBox_hui->isChecked())
    ui->widgetHisBIS->graph(1)->addData(key, BIS);//添加数据2到曲线2
    //删除8秒之前的数据。这里的8要和下面设置横坐标宽度的8配合起来
    //才能起到想要的效果，可以调整这两个值，观察显示的效果。
    ui->widgetHisBIS->graph(0)->removeDataBefore(key-60);
    ui->widgetHisBIS->graph(1)->removeDataBefore(key-60);

    //自动设定graph(1)曲线y轴的范围，如果不设定，有可能看不到图像
    //也可以用ui->widgetPressure->yAxis->setRange(up,low)手动设定y轴范围
    ui->widgetHisBIS->graph(0)->rescaleValueAxis();
    ui->widgetHisBIS->graph(1)->rescaleValueAxis(true);

    //这里的8，是指横坐标时间宽度为8秒，如果想要横坐标显示更多的时间
    //就把8调整为比较大到值，比如要显示60秒，那就改成60。
    //这时removeDataBefore(key-8)中的8也要改成60，否则曲线显示不完整。
    ui->widgetHisBIS->yAxis->setRange(0, 100);
    ui->widgetHisBIS->xAxis->setRange(key+0.25, 60, Qt::AlignRight);//设定x轴的范围
    ui->widgetHisBIS->replot();
}

void MainWindow::initHiDpi()
{
#ifdef Q_OS_LINUX
    bool defaultEnableHiDPI = false;
#else
    bool defaultEnableHiDPI = true;
#endif

    if (defaultEnableHiDPI) {
        QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
        QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    } else
        QApplication::setAttribute(Qt::AA_Use96Dpi);
}

void MainWindow::dpichanged()
{
    qDebug()<<"dpichanged"<<endl;
    //m_autoResizeHandler->doAutoResize();
    int currentScreenWidth = QApplication::desktop()->width();
    int currentScreenHeight = QApplication::desktop()->height();
    qDebug()<<"dpiwidth ="<< currentScreenWidth << "dpiheight = "<<currentScreenHeight<< endl;
    int wide = currentScreenWidth * m_fWidthScale;
    int height = currentScreenHeight * m_fHeightScale;
    this->resize(wide,height);
    caluteDpi();
}

void MainWindow::caluteDpi()
{
    int wide = this->width();
    int height = this->height();
    int currentScreenWidth = QApplication::desktop()->width();
    int currentScreenHeight = QApplication::desktop()->height();
    m_fWidthScale = (float)wide/currentScreenWidth;
    m_fHeightScale = (float)height/currentScreenHeight;;
    qDebug()<<"dpiwidth ="<< currentScreenWidth << "dpiheight = "<<currentScreenHeight<< endl;
}
void MainWindow::initchildposscale()
{

    QList<QWidget*> widgetList = ui->tabMonitor->findChildren<QWidget *>();
    QList<QWidget*> widgetList2 = ui->tabHistoryData->findChildren<QWidget *>();
    int wide = this->width();
    int height = this->height();
    for (auto it = widgetList.begin(); it != widgetList.end(); ++it )
    {
        childpos pos;
        auto posnew = (*it)->geometry();
        pos.x = posnew.left();
        pos.y = posnew.top();
        pos.width = posnew.width();
        pos.height = posnew.height();
        pos.parentheight = height;
        pos.parentwidth = wide;
        m_mapChildScale.insert(std::make_pair(*it,pos));
    }
    for (auto it = widgetList2.begin(); it != widgetList2.end(); ++it )
    {
        childpos pos;
        auto posnew = (*it)->geometry();
        pos.x = posnew.left();
        pos.y = posnew.top();
        pos.width = posnew.width();
        pos.height = posnew.height();
        pos.parentheight = height;
        pos.parentwidth = wide;
        m_mapChildScale_his.insert(std::make_pair(*it,pos));
    }
}

void  MainWindow::resizeEvent(QResizeEvent *event)
{
    qDebug()<<"MainWindow::resizeEvent"<<endl;

    QList<QWidget*> widgetList = ui->tabMonitor->findChildren<QWidget *>();
    int wide = this->width();
    int height = this->height();
    for (auto it = widgetList.begin(); it != widgetList.end(); ++it )
    {
        auto mit = m_mapChildScale.find(*it);
        if(mit != m_mapChildScale.end())
        {
            float pos_x_scale = (float)mit->second.x /mit->second.parentwidth;
            float pos_y_scale = (float)mit->second.y /mit->second.parentheight;
            float size_x_scale = (float)mit->second.width /mit->second.parentwidth;
            float size_y_scale = (float)mit->second.height /mit->second.parentheight;
            int pos_x_new = wide* pos_x_scale;
            int pos_y_new = height* pos_y_scale;
            int size_x_new = wide* size_x_scale;
            int size_y_new = height* size_y_scale;
            (*it)->setGeometry(QRect(pos_x_new, pos_y_new, size_x_new, size_y_new));
        }
    }
    QList<QWidget*> widgetList2 = ui->tabHistoryData->findChildren<QWidget *>();
    for (auto it = widgetList2.begin(); it != widgetList2.end(); ++it )
    {
        auto mit = m_mapChildScale_his.find(*it);
        if(mit != m_mapChildScale_his.end())
        {
            float pos_x_scale = (float)mit->second.x /mit->second.parentwidth;
            float pos_y_scale = (float)mit->second.y /mit->second.parentheight;
            float size_x_scale = (float)mit->second.width /mit->second.parentwidth;
            float size_y_scale = (float)mit->second.height /mit->second.parentheight;
            int pos_x_new = wide* pos_x_scale;
            int pos_y_new = height* pos_y_scale;
            int size_x_new = wide* size_x_scale;
            int size_y_new = height* size_y_scale;
            (*it)->setGeometry(QRect(pos_x_new, pos_y_new, size_x_new, size_y_new));
        }
    }
    caluteDpi();
}

void MainWindow::sendData( smart_topic::Anesthesia* pData)
{
    void* naddress = pData;
    QVariant DataVar;
    DataVar.setValue(naddress); // 数据包装
    emit messageSignal(DataVar, QString(""));
}

void MainWindow::recvMsg(QVariant DataVar, QString strcontent)
{
    int i = 10;
    if (m_isStart)
    {
        void* nmsg = DataVar.value<void*>();
        smart_topic::Anesthesia* pmsg = static_cast<smart_topic::Anesthesia*>(nmsg);


        QDateTime currentDateTime =QDateTime::currentDateTime();


        QString strSql = "insert into patient_value (number, create_time, RATE, DIAP, SYSP, SpO2, BIS) values ('"
                + m_number + "', '"
                + currentDateTime.toString("yyyy-MM-dd hh:mm:ss") + "', "
                + QString::number(pmsg->RATE) + ", "
                + QString::number(pmsg->DIAP) + ", "
                + QString::number(pmsg->SYSP) + ", "
                + QString::number(pmsg->SpO2) + ", "
                + QString::number(pmsg->BIS)
                + ")";

        insertSql(strSql);

        realtimeDataSlot(pmsg->RATE, pmsg->DIAP,
                         pmsg->SYSP, pmsg->SpO2, pmsg->BIS);
        delete pmsg;
    }
}

void MainWindow::createChart(QHBoxLayout *qHBoxLayout)
{
    //创建图表
    QChartView* chartView = new QChartView(this);
    // chartView->resize(this->size());

    qHBoxLayout->addWidget(chartView);

    QChart* chart = new QChart();
    chart->setTitle(QStringLiteral("测试折线"));
    chartView->setChart(chart);//视图设置图表

    //创建折线序列
    QLineSeries* series0 = new QLineSeries();
    QLineSeries* series1 = new QLineSeries();
    series0->setName(QStringLiteral("折线1"));

    chart->addSeries(series0); //将折线序列添加到图表
    chart->addSeries(series1);

    //序列添加数值
    series0->append(0.0, 5.0);
    series0->append(2.5, 6.0);
    series0->append(5.0, 87.0);
    series0->append(7.5, 18.0);
    series0->append(10.0, 56.0);

    //创建坐标轴
    QValueAxis* axisX = new QValueAxis();
    axisX->setRange(0, 10);

    axisX->setTitleText("时间(10 秒）");
    QValueAxis* axisY = new QValueAxis();
    axisY->setRange(0, 100);
    axisY->setTitleText("值");

    chart->setAxisX(axisX, series0);
    chart->setAxisY(axisY, series0);
    chart->setAxisX(axisX, series1);
    chart->setAxisY(axisY, series1);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void  MainWindow:: clearAllRow()
{
    int curRow=ui->tblHisPatient->rowCount();//当前行号
    for(int index = curRow -1  ; index >= 0 ; --index)
    {
        ui->tblHisPatient->removeRow(index);
    }
}

void MainWindow::createItemsARow(int rowNo,QString Strnum,QString Age,QString Sex)
{
    //为一行的单元格创建 Items
    QTableWidgetItem    *item;
    item=new  QTableWidgetItem(Strnum,MainWindow::ctNum);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter); //文本对齐格式
    ui->tblHisPatient->setItem(rowNo,MainWindow::colNum,item); //为单元格设置Item

    QTableWidgetItem    *itemage;
    itemage=new  QTableWidgetItem(Age,MainWindow::ctAge);
    itemage->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tblHisPatient->setItem(rowNo,MainWindow::colAge,itemage);

    QTableWidgetItem    *itemsex;
    itemsex=new  QTableWidgetItem(Sex,MainWindow::ctSex);
    itemsex->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tblHisPatient->setItem(rowNo,MainWindow::colSex,itemsex);

}


void MainWindow::on_tblHisPatient_itemClicked(QTableWidgetItem *item)
{
    if (item->column() == 0)
    {
        QString number = item->text();
        QString startTime = ui->dateHisStartTime->text();
        QString stopTime = ui->dateHisStopTime->text();

        vector<PatientValue> vecPatientValue;
        QString strSql = "";
        strSql = "select * from patient_value where number = '" + number + "' AND create_time >= '"
                + startTime
                + "' AND create_time <= '" + stopTime + "'";

        selectPatientValue(strSql, vecPatientValue);

        if (vecPatientValue.size() > 0)
        {
            for (auto it = vecPatientValue.begin(); it != vecPatientValue.end(); ++it)
            {
                historyShow(it->strCreateTime,it->RATE,it->DIAP,it->SYSP,it->SpO2,it->BISr);
            }
        }

    }
}

void MainWindow::on_btnHisQuery_clicked()
{
    std::vector<Patient> vecPatient;
    QString strSql = "";
    if (ui->txtHisNumber->text() == "")
    {
        strSql = "select * from patient";
    }
    else
    {
        strSql = "select * from patient where number like '%" + ui->txtHisNumber->text() + "%'";
    }
    selectPatient(strSql,vecPatient);
    clearAllRow();
    for(auto it = vecPatient.begin(); it != vecPatient.end(); ++it)
    {
        int curRow=ui->tblHisPatient->rowCount(); // 当前行号
        ui->tblHisPatient->insertRow(curRow); // 在表格尾部添加一行
        createItemsARow(curRow,it->strNumber,it->strAge,it->strSex);
    }
}

/**
 * @brief 开始录制点击事件
 */
void MainWindow::on_btnInfoStart_clicked()
{
    QMessageBox:: StandardButton result= QMessageBox::information(NULL, "提示", "确定开始录制吗？",QMessageBox::Yes|QMessageBox::No);
    switch (result)
    {
    case QMessageBox::Yes:
    {
        QDateTime *datetime=new QDateTime(QDateTime::currentDateTime());
        int number = getMaxPatientNumber();

        number += 1;; // 设置显示格式

        m_number = QString::number(number);

        ui->txtInfoNumber->setText(m_number);

        /**
         * 自动保存病人编号
         */
        QString sex = QString::number(ui->cmbInfoSex->currentIndex());
        if (sex == "")
        {
            sex = "null";
        }

        QString height = ui->txtInfoHeight->text();
        if (height == "")
        {
            height = "null";
        }
        QString weight = ui->txtInfoWeight->text();
        if (weight == "")
        {
            weight = "null";
        }
        QString age = ui->txtInfoAge->text();
        if (age == "")
        {
            age = "null";
        }


        QString strSql = "INSERT INTO patient (number, sex, height, weight, age, create_date, create_time, update_time) VALUES('"
                + m_number + "',"
                + sex + ","
                + height + ","
                + weight + ","
                + age + ", '"
                + datetime->toString("yyyy-MM-dd") + "', '"
                + datetime->toString("yyyy-MM-dd hh:mm:ss") + "', '"
                + datetime->toString("yyyy-MM-dd hh:mm:ss")
                + "')";

        qDebug() << strSql << endl;

        bool result = insertSql(strSql);


        m_isStart = true;
        ui->tabSmart->setCurrentIndex(1);
        ui->btnInfoStart->setDisabled(true);
        ui->btnInfoStop->setEnabled(true);

        ui->btnInfoStart->setStyleSheet(QString::fromUtf8("width: 270px; /* \345\256\275\345\272\246 */\n"
                                                          "	height: 40px; /* \351\253\230\345\272\246 */\n"
                                                          "	border-width: 0px; /* \350\276\271\346\241\206\345\256\275\345\272\246 */\n"
                                                          "	border-radius: 3px; /* \350\276\271\346\241\206\345\215\212\345\276\204 */\n"
                                                          "	background: #B0B0B0; /* \350\203\214\346\231\257\351\242\234\350\211\262 */\n"
                                                          "	cursor: pointer; /* \351\274\240\346\240\207\347\247\273\345\205\245\346\214\211\351\222\256\350\214\203\345\233\264\346\227\266\345\207\272\347\216\260\346\211\213\345\212\277 */\n"
                                                          "	outline: none; /* \344\270\215\346\230\276\347\244\272\350\275\256\345\273\223\347\272\277 */\n"
                                                          "	font-family: Microsoft YaHei; /* \350\256\276\347\275\256\345\255\227\344\275\223 */\n"
                                                          "	color: white; /* \345\255\227\344\275\223\351\242\234\350\211\262 */\n"
                                                          "	font-size: 15px; /* \345\255\227\344\275\223\345\244\247\345\260\217 *"));

        ui->btnInfoStop->setStyleSheet(QString::fromUtf8("width: 270px; /* \345\256\275\345\272\246 */\n"
                                                         "	height: 40px; /* \351\253\230\345\272\246 */\n"
                                                         "	border-width: 0px; /* \350\276\271\346\241\206\345\256\275\345\272\246 */\n"
                                                         "	border-radius: 3px; /* \350\276\271\346\241\206\345\215\212\345\276\204 */\n"
                                                         "	background: #36648B; /* \350\203\214\346\231\257\351\242\234\350\211\262 */\n"
                                                         "	cursor: pointer; /* \351\274\240\346\240\207\347\247\273\345\205\245\346\214\211\351\222\256\350\214\203\345\233\264\346\227\266\345\207\272\347\216\260\346\211\213\345\212\277 */\n"
                                                         "	outline: none; /* \344\270\215\346\230\276\347\244\272\350\275\256\345\273\223\347\272\277 */\n"
                                                         "	font-family: Microsoft YaHei; /* \350\256\276\347\275\256\345\255\227\344\275\223 */\n"
                                                         "	color: white; /* \345\255\227\344\275\223\351\242\234\350\211\262 */\n"
                                                         "	font-size: 15px; /* \345\255\227\344\275\223\345\244\247\345\260\217 *"));
    }
        break;
    case QMessageBox::No:
        qDebug()<<"NO";
        break;
    default:
        break;
    }
}

void MainWindow::on_btnInfoStop_clicked()
{
    QMessageBox:: StandardButton result= QMessageBox::information(NULL, "提示", "确定停止录制吗？",QMessageBox::Yes|QMessageBox::No);
    switch (result)
    {
    case QMessageBox::Yes:
    {
        m_isStart = false;
        ui->btnInfoStop->setDisabled(true);
        ui->btnInfoStart->setEnabled(true);

        ui->btnInfoStop->setStyleSheet(QString::fromUtf8("width: 270px; /* \345\256\275\345\272\246 */\n"
                                                         "	height: 40px; /* \351\253\230\345\272\246 */\n"
                                                         "	border-width: 0px; /* \350\276\271\346\241\206\345\256\275\345\272\246 */\n"
                                                         "	border-radius: 3px; /* \350\276\271\346\241\206\345\215\212\345\276\204 */\n"
                                                         "	background: #B0B0B0; /* \350\203\214\346\231\257\351\242\234\350\211\262 */\n"
                                                         "	cursor: pointer; /* \351\274\240\346\240\207\347\247\273\345\205\245\346\214\211\351\222\256\350\214\203\345\233\264\346\227\266\345\207\272\347\216\260\346\211\213\345\212\277 */\n"
                                                         "	outline: none; /* \344\270\215\346\230\276\347\244\272\350\275\256\345\273\223\347\272\277 */\n"
                                                         "	font-family: Microsoft YaHei; /* \350\256\276\347\275\256\345\255\227\344\275\223 */\n"
                                                         "	color: white; /* \345\255\227\344\275\223\351\242\234\350\211\262 */\n"
                                                         "	font-size: 15px; /* \345\255\227\344\275\223\345\244\247\345\260\217 *"));

        ui->btnInfoStart->setStyleSheet(QString::fromUtf8("width: 270px; /* \345\256\275\345\272\246 */\n"
                                                          "	height: 40px; /* \351\253\230\345\272\246 */\n"
                                                          "	border-width: 0px; /* \350\276\271\346\241\206\345\256\275\345\272\246 */\n"
                                                          "	border-radius: 3px; /* \350\276\271\346\241\206\345\215\212\345\276\204 */\n"
                                                          "	background: #36648B; /* \350\203\214\346\231\257\351\242\234\350\211\262 */\n"
                                                          "	cursor: pointer; /* \351\274\240\346\240\207\347\247\273\345\205\245\346\214\211\351\222\256\350\214\203\345\233\264\346\227\266\345\207\272\347\216\260\346\211\213\345\212\277 */\n"
                                                          "	outline: none; /* \344\270\215\346\230\276\347\244\272\350\275\256\345\273\223\347\272\277 */\n"
                                                          "	font-family: Microsoft YaHei; /* \350\256\276\347\275\256\345\255\227\344\275\223 */\n"
                                                          "	color: white; /* \345\255\227\344\275\223\351\242\234\350\211\262 */\n"
                                                          "	font-size: 15px; /* \345\255\227\344\275\223\345\244\247\345\260\217 *"));
    }
        break;
    case QMessageBox::No:

        break;
    default:
        break;
    }
}

void MainWindow::on_btnInfoModify_clicked()
{
    QMessageBox:: StandardButton btnResult= QMessageBox::information(NULL, "提示", "确定修改吗？",QMessageBox::Yes|QMessageBox::No);
    QDateTime *datetime=new QDateTime(QDateTime::currentDateTime());

    switch (btnResult)
    {
    case QMessageBox::Yes:
    {
        QString sex = QString::number(ui->cmbInfoSex->currentIndex());
        QString height = ui->txtInfoHeight->text();
        QString weight = ui->txtInfoWeight->text();
        QString age = ui->txtInfoAge->text();
        if (height == "")
        {
            height = "null";
        }

        if (weight == "")
        {
            weight = "null";
        }

        if (age == "")
        {
            age = "null";
        }


        if (m_number != "")
        {
            QString strSql = "UPDATE patient SET "
                    + QString(" sex = ") + sex + ", "
                    + QString(" height = ") + height + ","
                    + QString(" weight = ") + weight + ","
                    + QString(" age = ") + age + ", "
                    + QString(" update_time = '") + datetime->toString("yyyy-MM-dd hh:mm:ss") + "'"
                    + QString(" WHERE number = '") + m_number + "'";

            bool result = updateSql(strSql);

            if (result)
            {
                QMessageBox::information(NULL, "提示", "修改成功！");
            }
            else
            {
                QMessageBox::information(NULL, "提示", "修改失败！");
            }
        }
        else
        {
            QMessageBox::information(NULL, "提示", "请先保存病人信息！");
        }
    }
        break;
    default:
        break;
    }
}

void MainWindow::on_btnMonitorPOK_clicked()
{
    if (m_number == "")
    {
        QMessageBox::information(NULL, "提示", "请先保存病人信息！");
        return;
    }

    QDateTime *datetime=new QDateTime(QDateTime::currentDateTime());

    QString strPropofol = ui->txtMonitorPropofol->text();

    QString strSql = "INSERT INTO patient_medicine (number, type, value, create_time) VALUES('"
            + m_number + "', 0, '"
            + strPropofol + "', '"
            + datetime->toString("yyyy-MM-dd hh:mm:ss") + "'"
            + ")";

    bool result = insertSql(strSql);
    if (result)
    {
        ui->txtMonitorMedicineTip->append("病人编号："  + m_number + " 注射 丙泊酚 " + ui->txtMonitorPropofol->text() + " ml 成功！");
        QMessageBox::information(NULL, "提示", "保存成功！");
    }
    else
    {
        ui->txtMonitorMedicineTip->append("病人编号："  + m_number + " 注射 丙泊酚 " + ui->txtMonitorPropofol->text() + " ml 失败！");
        QMessageBox::information(NULL, "提示", "保存失败！");
    }

}

void MainWindow::on_btnMonitorSOK_clicked()
{
    if (m_number == "")
    {
        QMessageBox::information(NULL, "提示", "请先保存病人信息！");
        return;
    }

    QDateTime *datetime=new QDateTime(QDateTime::currentDateTime());

    QString strSufentanil = ui->txtMonitorSufentanil->text();

    QString strSql = "INSERT INTO patient_medicine (number, type, value, create_time) VALUES('"
            + m_number + "', 1, '"
            + strSufentanil + "', '"
            + datetime->toString("yyyy-MM-dd hh:mm:ss") + "'"
            + ")";
    bool result = insertSql(strSql);
    if (result)
    {
        ui->txtMonitorMedicineTip->append("病人编号："  + m_number + " 注射 舒芬太尼 " + ui->txtMonitorPropofol->text() + " ml 成功！");
        QMessageBox::information(NULL, "提示", "保存成功！");
    }
    else
    {
        ui->txtMonitorMedicineTip->append("病人编号："  + m_number + " 注射 舒芬太尼 " + ui->txtMonitorPropofol->text() + " ml 失败！");
        QMessageBox::information(NULL, "提示", "保存失败！");

    }
}


void MainWindow::on_btnMonitorCancel_clicked()
{
    QMessageBox:: StandardButton btnResult= QMessageBox::information(NULL, "提示", "确定撤销吗？",QMessageBox::Yes|QMessageBox::No);

    switch (btnResult)
    {
    case QMessageBox::Yes:
    {
        QString strSql = "SELECT COUNT(*) AS count FROM patient_medicine WHERE number = '" + m_number + "' ORDER BY create_time DESC LIMIT 1 ";
        bool result = judgePatientMedicine(strSql);
        if (!result)
        {
            QMessageBox::information(NULL, "提示", "暂无数据可撤销！");
            return;
        }

        vector<PatientMedicine> vecPatientMedicine;

        strSql = "SELECT t.name, p.value, p.create_time FROM patient_medicine AS p LEFT JOIN medicine_type AS t ON p.type = t.type"
                + QString(" WHERE p.number = ") + m_number + " ORDER BY p.create_time DESC LIMIT 1 ";
        selectMedicineInfo(strSql, vecPatientMedicine);

        strSql = "DELETE FROM patient_medicine WHERE number = '" + m_number + "' ORDER BY create_time DESC LIMIT 1 ";
        result = deleteSql(strSql);
        if (result)
        {
            ui->txtMonitorMedicineTip->append("病人编号："  + m_number + " 撤销注射 " + vecPatientMedicine[0].name
                    + " "  + vecPatientMedicine[0].value +  " ml 成功！");
            QMessageBox::information(NULL, "提示", "撤销成功！");
        }
        else
        {
            ui->txtMonitorMedicineTip->append("病人编号："  + m_number + " 撤销注射 " + vecPatientMedicine[0].name
                    + " "  + vecPatientMedicine[0].value +  " ml 失败！");
            QMessageBox::information(NULL, "提示", "撤销失败！");
        }

    } break;
    default:
        break;
    }

}
