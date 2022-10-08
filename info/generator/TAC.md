# 三地址码（TAC）设计

## 设计原则

+ 保留临时变量名
  + 原因：为了后续的后端优化，不能在这一步就转化为偏移量
  + 影响：在这一阶段后不能丢弃符号表信息，要留到代码生成
+ 假设所有变量都立即可以取得，不设计读取内存命令
  + 原因：不确定目标平台指令集，所以不考虑所有内存-寄存器传输问题

## TAC文件构成

### 段

TAC文件由**段**组成。
段包括：**全局变量段**和**程序段**
段由**标签**和**命令**组成。

### 标签

标签分为：**过程标签**、**临时过程标签**和**变量标签**。

### 全局变量段

全局变量段代码只包括全局变量的变量标签。符号表会提供其他信息。

### 过程段

过程段包括所有函数定义和**初始化过程**。
初始化过程是对全局变量进行初始化的所有过程。
过程段代码包括：命令、临时过程标签、变量标签

## TAC功能实现

+ 基本运算
  + 由AST遍历直接生成命令序列
  + AST节点对应临时变量和操作符
  + 抛弃原变量名，使用统一的临时变量名
    + 同步修改符号表中的对应项
    + 原因：原符号名可能很长，不方便
+ 控制语句
  + 仅设计条件跳转和直接跳转指令
  + 各种条件、循环语句在该步就进行拆分
  + 跳转目标为临时过程标签
+ 过程调用
  + TAC仅给出形式上过程调用的方法：添加参数-调用-设返回值-读返回值
  + TAC不提供具体的调用实现，而交给目标代码生成阶段
    + 一种可能的过程调用实现
      + 定义参数变量栈并维护
      + 添加参数对应压栈
      + 形参赋值对应出栈
      + 返回值由callee压栈，caller出栈
+ 全局变量
  + 在全局变量段设置变量标签
  + 标签名称和变量名直接相关，便于后续多文件处理
  + 变量信息储存在符号表中，符号表要更新标签信息
  + 访问时，使用变量标签+取变量/写变量命令
+ 数组
  + 数组访问涉及地址偏移量计算，需要知道变量实际占用空间大小。在该阶段，使用sizeof标记延迟计算。
  + 在目标代码生成阶段，要结合代码中的sizeof、符号表类型信息、目标平台实际情况替换sizeof数值。
  + 数组定义/声明时生成变量标签
    + 翻译为目标代码时，这些标签会转换为帧指针和偏移量
  + 访问数组元素时使用变量标签+取变量/写变量命令
    + 翻译为目标代码时，会转化为相应的读写内存指令


## TAC 命令 (9+21=30)

### 符号含义

num: 立即数
var: 临时变量/立即数
label: 过程标签
tlabel: 临时过程标签
addr: 变量标签

### 核心命令

+ 控制语句
  + 条件跳转 BEQZ var0 tlabel # if(var0 == 0) then goto label
  + 强制跳转 JUMP tlabel # goto label
+ 函数调用
  + 参数放置 PPRM var0 # *(pParam) = var0; pParam++;
  + 调用过程 CALL label num
  + 返回结果 RTRN var0 # *ret = var0
  + 读返回值 GRTN var0 # var0 = *ret
+ 数组和全局变量
  + 取变量 READ var0 addr # var0 = *addr
  + 写变量 WRIT var0 addr # *addr = var0
  + 计算偏移量 SZOF var0 var1 # var1 = sizeof(var0)

### 运算命令

+ 四则
  + 加 ADD var0 var1 var2 # var2 = var1 + var0
  + 减 SUB var0 var1 var2 # var2 = var1 - var0
  + 乘 MUL var0 var1 var2 # var2 = var1 * var0
  + 除 DIV var0 var1 var2 # var2 = var1 / var0
  + 取余 RMN var0 var1 var2 # var2 = var1 % var0
+ 比较
  + 大于 GRT var0 var1 var2 # var2 = var0 > var1
  + 小于 LET var0 var1 var2 # var2 = var0 < var1
  + 等于 EQU var0 var1 var2 # var2 = var0 == var1
  + 大于等于 GRE var0 var1 var2 # var2 = var0 >= var1
  + 小于等于 LEE var0 var1 var2 # var2 = var0 <= var1
  + 不等于 NEQ var0 var1 var2 # var2 = var0 != var1
+ 逻辑运算
  + 或 LORR var0 var1 var2 # var2 = var0 || var1
  + 与 LAND var0 var1 var2 # var2 = var0 && var1
  + 非 LNOT var0 var1  # var1 = !var0
  + 异或 LXOR var0 var1 var2 # var2 = (var0 && var1) || (!var0 && !var1)
+ 位运算
  + 位与 BAND var0 var1 var2 # var2 = var0 & var1
  + 位或 BORR var0 var1 var2 # var2 = var0 | var1
  + 位非 BNOT var0 var1 # var1 = ~var0
  + 位异或 BXOR var0 var1 var2 # var2 = var0 ^ var1
  + 左移 LSFT var0 var1 var2 # var2 = var0 << var1
  + 右移 RSFT var0 var1 var2 # var2 = var0 >> var1

