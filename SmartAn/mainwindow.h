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
#include <map>
#include "entity.h"
#include "message_thread.hpp"
#include "qcustomplot.h"
#include "anesthesia.h"
#include "auto_resize.h"
using namespace std;
namespace Ui {
class MainWindow;
}
struct childpos
{
public:
    childpos() {}
public:
    int x;
    int y;
    int width;
    int height;
    int parentwidth;
    int parentheight;
};
class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    enum  CellType{ ctNum=1000, ctAge, ctSex }; // 各单元格的类型
    enum  FieldColNum{ colNum=0, colAge, colSex };
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void paintEvent(QPaintEvent *event) override;

    void createChart(QHBoxLayout *qHBoxLayout);
    // 设置 qcustomplot 画图属性，实时
    void setupRealtimeData(QCustomPlot *customPlot, const QString &, double tickStep);
    void setupRealtimeData(QCustomPlot *customPlot, const QString &, const QString &, double tickStep);
    void realtimeDataSlot(double RATE, double DIAP, double SYSP, double SpO2, double BIS);

    void sendData( smart_topic::Anesthesia* pData);

    //
    void historyShow(QDateTime datatime,double RATE,double DIAP,
                     double SYSP,double SpO2,
                     double BIS);

    // 根据类型动态生成控件
    void generateMedicineControl();
    void generateEventControl();

    // 根据分辨率自适应尺寸
    void autoResize();
    void initHiDpi();
    void initchildposscale();

    virtual void resizeEvent(QResizeEvent *event);

    void savePatientMedicine(int type, QString name, QString value);

    void cancelPatientMedicine(int type);

    void savePatientEvent(int operateType, int type, QString value);
private slots:
    // 添加实时数据槽
    void createItemsARow(int rowNo,QString Strnum,QString Age,QString Sex); // 为某一行创建items
    void clearAllRow();
    void on_tblHisPatient_itemClicked(QTableWidgetItem *item);

    void on_btnHisQuery_clicked();

    void on_btnInfoStart_clicked();

    void on_btnInfoStop_clicked();

    void on_btnInfoModify_clicked();

    void on_btnMonitorOK_clicked(int);

    void on_btnMonitorCancel_clicked(int);

    void on_btnEventOK_clicked(int);

    void on_txtEventOK_clicked(int);

signals:
    void messageSignal(QVariant, QString);     // 信号只能声明不能定义
public slots:
    void recvMsg(QVariant, QString);
    void caluteDpi();

    void dpichanged();
private:
    const int MESSAGE_INVALID = -1; // 约定无效值

    QSignalMapper* m_medicineTypeOKMapper;
    QSignalMapper* m_medicineTypeCancelMapper;
    QSignalMapper* m_eventTypeBtnOKMapper;
    QSignalMapper* m_eventTypeTxtOKMapper;

    vector<MedicineType> m_vecMedicineType;
    vector<EventType> m_vecBtnEventType;
    vector<EventType> m_vecTxtEventType;

    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QTimer dataTimer;
    MessageThread m_thread;

    bool m_isStart = false;
    QString m_number = "";

private:
    //自定义界面变量
    AutoResize *m_autoResizeHandler;
    float m_fWidthScale;
    float m_fHeightScale;
    std::map<QWidget*,childpos> m_mapChildScale;
    std::map<QWidget*,childpos> m_mapChildScale_his;

};


#endif // MAINWINDOW_H
