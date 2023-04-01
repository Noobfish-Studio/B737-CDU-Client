# CDU for B737 - Client
上位机客户端

## 版本更新记录
### v0.1.1.230401-alpha
1.键位映射完成，现在可以使用函数模拟鼠标点击CDU键盘完成输入操作的事件  
2.现在程序可以读取到刚连接上时的屏幕数据，而不需要等到屏幕变化后才能读到（但是本质上还是需要的，只是模拟了一下亮度调亮再调暗的操作罢了，这样不需要玩家主动去改变数据）
3.控制台强制使用UTF-8输出，现在不会因为特殊字符乱码直接把程序炸掉（现在特殊字符只是显示？）

## 项目结构

### inc
项目使用的头文件
### lib
项目使用的链接库（包括MSFS和P3DV4两个端的Simconnect）
### src
项目源文件，目前包括两个控制台应用（分别读取MSFS/P3D中PMDG737的CDU0内容）和两个窗口应用（分别读取MSFS/P3D中PMDG737的CDU0内容并实时显示）
