<h1 align="center"> 基于AT89C52的八层双电梯调度仿真 :house:</h1>

<p align="center">基于Proteus仿真的嵌入式系统课程大作业~</p>



# 特性：

- 基于Proteus中的AT89C52 MCU仿真

- 使用LCD1602液晶显示屏显示电梯状态，辅助debug

- 八层、双电梯调度

- 实验报告

  

# 实验环境：

+ Proteus v8.8 sp1
+ Keil 5
+ VS Code



# 仿真原理图：

![schematic plot](https://gallery-1259614029.cos.ap-chengdu.myqcloud.com/img/20210525182659.bmp)



# 调度算法:

基于有穷状态机的调度算法：

每次循环初始状态可以按下表分为9种：

<img src="https://gallery-1259614029.cos.ap-chengdu.myqcloud.com/img/20210506165625.jpg" style="zoom:50%; argin:0 auto;"/>

每种状态可以用以下方式进行响应：

**向上：**

<img src="https://gallery-1259614029.cos.ap-chengdu.myqcloud.com/img/20210506165701.png" style="zoom:50%; margin:0 auto;"/>

**向下：**

<img src="https://gallery-1259614029.cos.ap-chengdu.myqcloud.com/img/20210506165721.png" style="zoom:50%; margin:0 auto; " />

**静止：**

<img src="https://gallery-1259614029.cos.ap-chengdu.myqcloud.com/img/20210506165740.png" style="zoom:50%; margin: 0 auto; "/>

**算法流程：**

<img src="https://gallery-1259614029.cos.ap-chengdu.myqcloud.com/img/20210506165822.png" style="zoom:50%; margin: 0 auto; "/>



# 参考内容:

+ [AT89C51-Elevator](https://github.com/Yifeng-J/AT89C51-Elevator)
+ [双电梯调度算法的简单分析](https://www.cnblogs.com/goWithHappy/p/2left_algorithm.html)

