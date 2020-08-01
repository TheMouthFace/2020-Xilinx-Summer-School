# 2020-Xilinx-Summer-School
2020年新工科联盟-Xilinx暑期学校（Summer School）项目
摇摇乐-文煜轩2018112735-张皓2018112741  
# 项目描述
用于调用sea-board开发板上陀螺仪模块获取数据，并将数据处理成记录60s内摇摆次数，最后将次数上传至AWS云端  
# 文件说明
* Gyro_Demo文件夹中为vivado工程文件，用于读取板载陀螺仪数据
* Gyro2AWSIOT中为Arduino IDE控制ESP32模块控制WIFI模块文件，其中也包括了摇摇乐的数据处理文件
* AWS_IOT.zip和SimpleDHT.zip为上述Arduino代码运行前所必要添加的两个库
