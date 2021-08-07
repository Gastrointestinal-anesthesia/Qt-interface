#include"auto_resize.h"
#include <QLabel>
#include <QMapIterator>
#include <QObject>
#include <QDebug>
#include <QComboBox>
#include <QLineEdit>
#include <QLayout>

AutoResize::AutoResize(QWidget* obj,float base_width,float base_height):m_autoResizeObj(obj)
{
    m_autoResize = false;//15
    //从构造函数中获得原始程序的宽和高
    m_baseWidth = base_width;
    m_baseHeight = base_height;
    m_horRatio = 1.0;
    m_verRatio = 1.0;
    m_fontRatio = 1.0;
    //获得layout(栅格化)布局的控件
    dealLayoutItems();
}


AutoResize::~AutoResize(void)
{
}

//添加所有需要缩放的控件，获得要缩放控件的位置和宽高（自定义控件缩放）
void AutoResize::pushAllResizeItem()
{
    qDebug()<<"get item start";
    QWidget *item=NULL;
    AutoResizeOriginalData resizeData;
    QRect tmp;
    //add other items
    for(auto it = m_otherItem.begin();it != m_otherItem.end();it++)
    {
        //qDebug()<<*it;
        item = *it;
        tmp = item->geometry();
        tmp.setX(item->x());
        tmp.setY(item->y());
        tmp.setWidth(abs(tmp.width()));
        tmp.setHeight(abs(tmp.height()));
        resizeData.data_rect = tmp;
        resizeData.data_font = item->font();
        m_resizeMap[item] = resizeData;
    }
        //再与需要自适应的枚举量控件进行与操作来判断是否需要自适应
    if(m_autoResizeFlag&INCLUDE_LABEL)
    {
        QList<QLabel*> _labelList=m_autoResizeObj->findChildren<QLabel *>();
        for(auto it=_labelList.begin();it!=_labelList.end();it++)
        {
            item = *it;
            tmp = item->geometry();
            tmp.setX(item->x());
            tmp.setY(item->y());
            tmp.setWidth(abs(tmp.width()));
            tmp.setHeight(abs(tmp.height()));
            resizeData.data_rect=tmp;
            resizeData.data_font=item->font();
            m_resizeMap[item]=resizeData;
        }
    }
    if(m_autoResizeFlag&INCLUDE_COMBOBOX)
    {
        QList<QComboBox*> _comboxList=m_autoResizeObj->findChildren<QComboBox *>();
        for(auto it=_comboxList.begin();it!=_comboxList.end();it++)
        {
            item = *it;
            tmp = item->geometry();
                       tmp.setX(item->x());
                       tmp.setY(item->y());
                       tmp.setWidth(abs(tmp.width()));
                       tmp.setHeight(abs(tmp.height()));
                       resizeData.data_rect = tmp;
                       resizeData.data_font = item->font();
                       m_resizeMap[item] = resizeData;
                   }
               }
               if(m_autoResizeFlag&INCLUDE_BUTTON)
               {
                   QList<QAbstractButton*> _buttonList=m_autoResizeObj->findChildren<QAbstractButton*>();
                   for(auto it=_buttonList.begin();it!=_buttonList.end();it++)
                   {
                       //qDebug()<<*it;
                       item=*it;
                       tmp=item->geometry();
                       tmp.setX(item->x());
                       tmp.setY(item->y());
                       tmp.setWidth(abs(tmp.width()));
                       tmp.setHeight(abs(tmp.height()));
                       resizeData.data_rect=tmp;
                       resizeData.data_font=item->font();
                       m_resizeMap[item]=resizeData;
                   }
               }
               if(m_autoResizeFlag&INCLUDE_EDITOR)
               {
                   QList<QLineEdit*> _editorList=m_autoResizeObj->findChildren<QLineEdit*>();
                   for(auto it=_editorList.begin();it!=_editorList.end();it++)
                   {
                               item=*it;
                               tmp=item->geometry();
                               tmp.setX(item->x());
                               tmp.setY(item->y());
                               tmp.setWidth(abs(tmp.width()));
                               tmp.setHeight(abs(tmp.height()));
                               resizeData.data_rect=tmp;
                               resizeData.data_font=item->font();
                               m_resizeMap[item]=resizeData;
                           }
                       }
                       for(auto it=m_ignoreItem.begin();it!=m_ignoreItem.end();it++)
                       {
                           if(m_resizeMap.contains(*it))
                               m_resizeMap.remove(*it);
                       }
                       m_autoResize=true;
                   }

 float AutoResize::getHorResizeRatio()
 {
     return m_horRatio;
}

float AutoResize::getVerResizeRatio()
 {
    return m_verRatio;
 }

float AutoResize::getFontResizeRatio()
{
    return m_fontRatio;
}
 void AutoResize::resetBaseWidth2BaseHeight(float base_width,float base_height)
 {
     m_baseWidth = base_width ;
     m_baseHeight = base_height;
 }
//计算缩放的比例
void AutoResize::calculateResizeRatio()
{
    //宽的比例（水平）
    m_horRatio = m_autoResizeObj->width()/m_baseWidth;
    //高的比例（垂直）
    m_verRatio = m_autoResizeObj->height()/m_baseHeight;
    //字体的比例，高比列为0时，使用宽比例，否则使用高比例
    m_fontRatio = m_horRatio<m_verRatio?m_horRatio:m_verRatio;
}

/*
 缩放字体大小
 字体大小的两种方式：
 1，PixelSize：以像素为单位
    相同尺寸上更高分辨率的屏幕上，由于其单位长度内的像素点数更多，即像素密度更大，
    因此对于更好分辨率的屏幕来说，字体会看起来小一些
 2，PointSize：以字体在屏幕实际显示的大小为单位的
   作用：不同分辨率的屏幕上显示的实际字体的大小是一样的

 处理：对于已经采用PixelSize的字体来说，就要对其进行控制缩放
 */

void AutoResize::fontAutoResize(QWidget *obj,int fontSize)
{
    qDebug()<<"set"<<obj<<fontSize;

    if(fontSize<=0)
        return;
    bool hasTextStyle = false;
    fontSize *= m_fontRatio;
    //正则表达式
    QString fontTextReg = "font:\\s+[0-9]+pt";
    QString fontFormat = "font: %1pt";
    QString fontSizeReg = "[0-9]+";
    QRegExp reg = QRegExp(fontTextReg);
    QRegExp size = QRegExp(fontSizeReg);
    QString styleText = obj->styleSheet();
    QString fontText = "";
    QString fontSizeText = "";
    QFont changedFont;
    if(reg.indexIn(obj->styleSheet()) != -1)
    {
        fontText = reg.capturedTexts().at(0);
        if(size.indexIn(fontText) != -1)
        {
            hasTextStyle = true;
        }
    }
    //set stylesheet to change font size
    if(hasTextStyle)
    {
        styleText.replace(reg,fontFormat.arg(fontSize));
        obj->setStyleSheet(styleText);
    }
    //set font pointsize to change font size
    else
    {
        changedFont = obj->font();
        changedFont.setPointSize(fontSize);
        obj->setFont(changedFont);
    }
 }


//获得layout(栅格化)布局
void AutoResize::dealLayoutItems()
{
    QString desName = "layoutWidget";
    //查找窗口中全部的QWidget对象，并记录其初始位置
    QList<QLayout*> layoutList = m_autoResizeObj->findChildren<QLayout*>();
    for(auto it = layoutList.begin();it!=layoutList.end();it++)
    {
        QString objName = (*it)->parent()->objectName();
        qDebug()<<"layout parent->"<<objName;
        if(objName.contains(desName))
        {
            qDebug()<<"find layoutWidget->"<<*it;
            QWidget* layoutWidget = qobject_cast<QWidget*>((*it)->parent());
            m_otherItem.push_back(layoutWidget);
            ignoreAllChiledren(layoutWidget);
        }
    }
}
/*
如果界面中有一部分控件包含在layout(栅格化)中，
在查找QWidget对象时会把这一部分控件也包含在其中，
这样的话在进行尺寸缩放时回合layout相互影响，
导致界面发成错位，应去除这些控件，交由layout来控制其缩放，具体操作如下：
首先定义一个函数用于移除其所有子对象
*/
void AutoResize::ignoreAllChiledren(QObject* obj)
{
    QList<QObject*> children = obj->children();
    for(auto it = children.begin();it != children.end();it++)
    {
        QWidget *item = qobject_cast<QWidget*>(*it);
        m_ignoreItem.push_back(item);
        AutoResizeOriginalData resizeData;
        if(!item)
            continue;
        resizeData.data_font = item->font();
        m_fontMap[item] = resizeData;
    }
}

//添加缩放的控件
void AutoResize::addOtherItem(QWidget *other)
{
    m_otherItem.push_back(other);
}
//添加栅格化控件
void AutoResize::addIgnoreItem(QWidget *ignore)
{
    m_ignoreItem.push_back(ignore);
}

void AutoResize::setAutoResizeFlag(int flag)
{
    m_autoResizeFlag = flag;
}

//用于重载resizeEvent(QResizeEvent *event)事件
void AutoResize::doAutoResize()
{
    //计算自动缩放的比列
    calculateResizeRatio();
    if(m_autoResize)
    {
        QMapIterator<QWidget*, AutoResizeOriginalData> _itarator(m_resizeMap);
        QFont changedFont;
        //缩放控件
        while(_itarator.hasNext())
        {
            _itarator.next();
            QWidget* _item = _itarator.key();
            QRect tmp = _itarator.value().data_rect;
            tmp.setWidth(tmp.width()*m_horRatio);
            tmp.setHeight(tmp.height()*m_verRatio);
            QRect after = QRect(tmp.x()*m_horRatio,tmp.y()*m_verRatio,tmp.width(),tmp.height());
            changedFont = _itarator.value().data_font;
            fontAutoResize(_item, changedFont.pointSize());
            _item->setGeometry(after);
        }
        //缩放字体
        QMapIterator<QWidget*,AutoResizeOriginalData> _fontIt(m_fontMap);
        while(_fontIt.hasNext())
        {
            _fontIt.next();
            QWidget* _item = _fontIt.key();
              changedFont = _fontIt.value().data_font;
              fontAutoResize(_item,changedFont.pointSize());
        }
    }
}
