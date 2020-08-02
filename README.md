# 2020-Xilinx-Summer-School
2020年新工科联盟-Xilinx暑期学校（Summer School）项目
<<<<<<< HEAD
摇摇乐-文煜轩2018112735-张皓2018112741  
# 项目描述
用于调用sea-board开发板上陀螺仪模块获取数据，并将数据处理成记录60s内摇摆次数，最后将次数上传至AWS云端  
=======
# 项目名称：摇摇乐(陀螺仪+AWS)
摇摇乐-文煜轩2018112735-张皓2018112741  
# 项目描述
用于调用sea-board开发板上陀螺仪模块获取数据，并将数据处理成记录60s内摇摆次数，最后将次数上传至AWS云端  
#技术方向
* ESP32配置WiFi并连接AWS IoT
* FPGA通过QSPI与ESP32通信
* 陀螺仪IMU数据采集
* 摇摇乐算法实现  
# 项目成员
文煜轩 张 皓  
# ⼯具版本
* vivado2018.3
* ardunio1.8  
# 板卡型号与外设列表
* 板卡型号: SEA
* 芯片型号：spartan7 xcs15  
>>>>>>> The Fourth commit
# 文件说明
* Gyro_Demo文件夹中为vivado工程文件，用于读取板载陀螺仪数据
* Gyro2AWSIOT中为Arduino IDE控制ESP32模块控制WIFI模块文件，其中也包括了摇摇乐的数据处理文件
* AWS_IOT.zip和SimpleDHT.zip为上述Arduino代码运行前所必要添加的两个库
<<<<<<< HEAD
=======
* 演示视频中将时间间隔缩短到了10s计数，方便演示
>>>>>>> The Fourth commit
