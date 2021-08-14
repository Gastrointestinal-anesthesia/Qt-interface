# 图形界面与数据库

---

## 环境配置

### 图形界面配置

- 在/SmartMessage/src目录下删除CMakeList.txt, 执行

	``` 
	catkin_init_workspace
	```
- 回到上级目录，执行

	``` 
	catkin_make
	```
	
	创建 devel, bulid 等文件夹

- 打开Qt-Creator, 在打开项目选项下打开 Qt-Interface/SmartAn/SmartAn.pro 项目文件

- 编译项目，并将/SmartAn下的config.ini与screen.jpg复制到编译输出目录 /build-SmartAn-Desktop\_Qt\_5\_14\_2\_GCC\_64bit-Debug 下

### 数据库配置

建议使用Navicat图形界面查看与编辑数据库，否则

- 连接本地MySQL，在终端执行

	```
	mysql -uroot -p888888
	```

- 查看当前所有的数据库，在当前终端下执行

	```
	show databases;
	```
	
- 如需切换到SmartAn数据库，执行

	```
	use SmartAn;
	```

- 创建和重置数据库内的表格，在当前终端下执行(注意SmartAn.sql的路径)

	```
	source /home/lab129/Monitor-Project/SmartAn.sql 
	```

- 查看数据库内的表格，执行

	```
	show tables;
	```

## 运行

- 在/SmartMessage下，执行
	
	```
	source devel/setup.bash
	```
	
- 在同一终端下执行

	```
	rosrun smart_topic anesthesia_subscriber
	```
	
	打开消息监听程序

- 打开图形界面，可在Qt-Creator内直接运行，或在/build-SmartAn-Desktop\_Qt\_5\_14\_2\_GCC\_64bit-Debug 下，终端运行

	```
	./SmartAn
	```
	
