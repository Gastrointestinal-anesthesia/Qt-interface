#ifndef AUTORESIZE_H
#define AUTORESIZE_H

#include <QWidget>
#include <QRadioButton>
#include <QMap>
struct AutoResizeOriginalData
{
    QRect data_rect;
    QFont data_font;
};

class AutoResize
{
public:
    //使用只有一位1的二进制数，方便与操作
    typedef enum
    {
        INCLUDE_LABEL = 0x1, //1
        INCLUDE_BUTTON = 0x2,//10
        INCLUDE_EDITOR = 0x4,//100
        INCLUDE_COMBOBOX = 0x8 //1000
    }AUTORESIZEFLAG;
    //从构造函数中获得原始程序的宽高
    AutoResize(QWidget* obj,float base_width,float base_height);
    ~AutoResize(void);
    //用于重载resizeEvent(QResizeEvent *event)事件
    void doAutoResize();
    //添加枚举类型中没有的控件
    void addOtherItem(QWidget *other);
    //添加(栅格化)布局的控件
    void addIgnoreItem(QWidget *ignore);
    //设置自动缩放的flag
    void setAutoResizeFlag(int flag);
    //查找窗口中全部的QWidget对象，并记录其初始位置
    void pushAllResizeItem();
    //得到缩放宽的比例
    float getHorResizeRatio();
    //得到缩放高的比例
    float getVerResizeRatio();
    //得到缩放字体的比例
    float getFontResizeRatio();
    void resetBaseWidth2BaseHeight(float base_width,float base_height);
private:
    //是否自动缩放
    bool m_autoResize;
    QMap<QWidget*,AutoResizeOriginalData> m_resizeMap;
    QMap<QWidget*,AutoResizeOriginalData> m_fontMap;
    QList<QWidget*> m_otherItem;
    QList<QWidget*> m_ignoreItem;
    int m_autoResizeFlag;
    float m_horRatio;
    float m_verRatio;
    float m_fontRatio;
    float m_baseWidth;
    float m_baseHeight;
    QWidget *m_autoResizeObj;
    void calculateResizeRatio();
    void fontAutoResize(QWidget *obj,int fontSIze);
    void dealLayoutItems();
    void ignoreAllChiledren(QObject* obj);
};



#endif // AUTORESIZE_H
