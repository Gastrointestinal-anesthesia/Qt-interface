# To do list

## 1.数据有效性

​	在已有代码基础上，对数据传输部分做以下修改

* 修改数据格式，从6个uint8改为3个int16，3个int8，具体情况以.msg中的内容为准
* 约定-1表示该包内该项数据无效，此时折线图及显示面板中该项数据的数值保持不变，MySQL中记录为无效（例如记录为-1）
* QChart控件中，当某个chart中涉及到的数据全部无效时，该chart不更新此包数据内容；当涉及到的数据只有部分更新时，未更新的数据维持上一次的数据进行绘图
* 已知收缩压与舒张压必定成对出现，两者在同一包数据内有效性相同

## 2.编号生成机制

​	病人编号生成需要做以下改动

* 病人编号改回自然数编码
* 在patient表中额外记录病人的日期、开始时间
* 保持自动编码功能，对病人编号自动加一

## 3.性别信息录入

​	将性别信息录入从原来的两栏改为

| 条目 | MySQL存储数值 |
| ---- | ---- |
| 待输入 | 0 |
| 男 | 1 |
| 女 | 2 |

​	默认选择为待输入，以防止未录入信息的病人的信息录入错误

## 4.2K屏幕显示支持

​	现阶段2K分辨率屏幕的支持效果不好，需要对高分辨率屏幕进行修正

## 5.注射器控件

​	包括图示百分比、文字显示注射毫升数的功能

## 6.启动界面

​	启动界面背景透明，显示PNG格式的图片，在一定时间后生成MainWindow并将自己析构

## 7.警告修正

​	在运行过程中出现了以下警告

* duplicate connection

* unknown property