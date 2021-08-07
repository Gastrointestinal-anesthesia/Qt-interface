#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QHBoxLayout>
#include <QGraphicsScene>
#include <QTimer>
#include <QVariant>
#include <QMetaType>
#include <string>
#include "message_thread.hpp"
#include "qcustomplot.h"
#include "anesthesia.h"
using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    enum   CellType{ctNum=1000,ctAge,ctSex}; //各单元格的类型
    enum    FieldColNum{colNum=0, colAge,colSex};
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void createChart(QHBoxLayout *qHBoxLayout);
    // 设置 qcustomplot 画图属性，实时
    void setupRealtimeData(QCustomPlot *customPlot, const QString &, const QString &);
    void realtimeDataSlot(double RATE, double DIAP, double SYSP, double SpO2, double BIS);

    void sendData( smart_topic::Anesthesia* pData);

    //
    void historyShow(QDateTime datatime,double RATE,double DIAP,
                     double SYSP,double SpO2,
                     double BIS);

    // 根据分辨率自适应尺寸
    void autoResize();


private slots:
    // 添加实时数据槽
    void createItemsARow(int rowNo,QString Strnum,QString Age,QString Sex); // 为某一行创建items
    void clearAllRow();
    void on_tblHisPatient_itemClicked(QTableWidgetItem *item);

    void on_btnHisQuery_clicked();

    void on_btnInfoOK_clicked();

    void on_btnInfoStart_clicked();

    void on_btnInfoStop_clicked();

signals:
    void mySignal(QVariant ,QString );     // 信号只能声明不能定义
public slots:
    void recvmsg(QVariant ,  QString);
private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QTimer dataTimer;
    MessageThread m_thread;

    bool m_isStart = false;
    QString m_number = "";
};


#endif // MAINWINDOW_H
