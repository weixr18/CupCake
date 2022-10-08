# Cupcake

A toy C(subset) Compiler.

## A 设计概述

本编译器的源语言是C语言的一个子集，目标语言为RISC-V32汇编码。

编译器分为前端和后端。**前端**包括：基于符号表进行词法分析(scanner)，基于LL(1)分析表和回溯进行递归下降语法分析(parser)。语法分析的生成结果是抽象语法树(AST)。在经过语义分析(analyser)后，进行中间代码(IR)生成(generater)。**后端**包括后端优化(optimizer)和目标代码生成(assembler)。

**词法分析器** (scanner) 接受代码输入，输出终结符组成的token序列。

**语法分析器** (parser) 接受token序列输入，输出AST。语法分析使用基于LL(1)分析表和递归下降的分析方法。LL(1)分析表由脚本语言代码 (src/gen_ll1_table.py) 从文法列表 (info/grammar.txt) 自动生成。我们使用的文法是左递归和有二义性的。因此，设计了基于栈进行语法分支和回退的分析器。此外，还对语法进行了一些限制（详见下方"D.3 特殊语法限制"）。

**语义分析器** (analyser) 根据AST构建符号表，并进行静态语法检查。

**IR生成器** (generater) 根据AST和符号表，生成中间代码。

**后端优化器** (optimizer) 包括两个部分：控制流分析、数据流分析。它们将为接下来的寄存器分配提供必要信息。

**目标代码生成器** (assembler) 也叫汇编器，包括寄存器分配和指令选择两部分。寄存器分配是指为中间代码中的虚拟寄存器分配实际的物理寄存器，涉及物理寄存器的调度分配。指令选择是指选用合适的汇编指令来翻译中间代码指令。汇编器最终会输出汇编语言代码。

编译器当前支持的功能较少（详见下方"D 语法支持"）。编译器的组成部分也不完全。目前编译器包括的部分有：**词法分析**、**语法分析**。不包括的部分有：静态语法检查、预处理、三地址码生成、目标代码生成。

## B 运行

```ps1
python ./src/gen_ll1_table.py
make -j4
./main.exe "test/0.c"
```

## C 规范

### C.1 文法规范

+ 每条语法左端为非终结符，右端为终结符或非终结符序列，以':='隔开
+ 每条语法占用一行, 同一非终结符对应的不同表达式使用'|'隔开
+ 终结符如果为标点，就是其本身。否则，是用<>包括的小写字母和下划线串
+ 非终结符是大写字母和下划线组成的字符串
+ 所有的终结符和非终结符见info/symbols.txt

### C.2 三地址码(TAC)规范

见info/generator/TAC.md

### C.3 目标代码规范

编译器最终生成RISC-V汇编码。这是一套定长指令集，被用于许多开源芯片，备受学术界欢迎。

## D 语法支持

### D.1 支持的语法

+ 词法/语法分析
  + 基本运算 2022.5.8
    + 暂不支持运算符的优先级判别
  + 基本语句 2022.5.8
  + 函数调用 2022.5.8
  + 全局变量 2022.5.8
  + 数组
    + 一维数组 2022.5.9 [v1.1.0]
    + 多维数组 2022.5.9 [v1.2.0]
+ 语义分析
+ IR生成

### D.2 暂不支持的语法

+ 指针, 字符串,
+ 宏定义/include, 
+ struct/enum/union, 
+ const/static/register/volatile, 
+ signed/unsigned, 类型前缀long/short, 
+ auto, typedef, extern, goto
+ 三目运算符
+ 带类型标记的数字(3.14f)
+ 强制类型转换
+ 非ascii编码

### D.3 特殊语法限制

由于语法分析器基于LL(1)表实现，因此许多标准C语法的情况无法处理。以下是所有的语法限制。

+ 运算表达式
  + 目前使用时需要**手动用括号标明优先级**。
+ 语句
  + 不支持所有单句的else/while/for
    + 原因：计算FOLLOW集时，单句结尾会造成循环依赖
    + 这意味着不能写 else if (...)
    + 可能会让人用着不爽，但强制花括号也可以降低语法错误发生的概率
+ 变量定义和声明
  + 支持单个变量声明组成的列表，
  + 不支持定义列表
  + 不支持数组声明组成的列表
+ 函数调用
  + **函数调用出现在表达式中**，需要手动加括号
    + 原因：代码中的无穷递归检测会排除正确选择
    + 后续方案：预处理token序列，自动为函数调用加括号
  + **运算表达式作为实参**，需要手动加括号
    + 原因：同上
+ 数组
  + 花括号定义的**数组项为表达式**，需要用括号括起
  + **数组作为函数形参**时，需要完整写出每一维度，并且用括号括起
  + **数组元素被赋值**时，需要用括号括起
  + **数组元素在表达式中**，需要用括号括起
+ 右值
  + 不支持**逗号表达式**作为右值
    + 原因：在生成FIRST表时，逗号表达式和表达式互为循环依赖，会陷入死循环
    + 取消后逗号表达式仅作为函数调用的组成部分，而不能作为一个语句或某个实参。
    + 该限制并不影响除炫技以外的其他使用
  + 不支持**赋值表达式**作为右值
    + 即不支持形如 int a = (b = 3 + 2) + 3; 的语句
    + 正常情况下不会有此需求，该限制并不影响使用。

### D.4 支持的运算符

+ LEFT_OPR: 
  + ++ | -- | ! | ~ 
+ RIGHT_OPR:
  +  ++ | -- 
+ OPR_TWO:
  + \+ | - | * | / | % |
  +  < | > | <= | >= | == | != |
  + && | || | ! |
  + \| | & | ^ | << | >> |
  +  = | += | -= | *= | /= |
  + |= | &= | ^= | <<= | >>= 

### D.5 支持的数据类型

+ 整型
  + int
  + char
  + long
  + short
+ 布尔型
  + bool
+ 浮点型
  + float
  + double
+ 空类型
  + void
  + 暂不支持写在形参列表中

## E 移植到其他语言

由于语法分析代码没有硬编码文法，而是使用从文法自动生成的分析表，Cupcake有移植到其他语言的可能性。移植步骤如下

+ 词法分析
  + 将scanner.cpp中的符号表修改为新语言的符号表
  + 编写和运行样例，修改scanner.cpp中的分词算法，直到可以正确进行token生成
+ 语法分析
  + 在grammar.txt中以当前规范（"C.1 文法规范"）书写语言的所有文法
  + 修改gen_ll1_table.py中的Punctuators为新语言的标点符
  + 运行gen_ll1_table.py并修改文法，直到可以正常生成分析表
  + 编写和运行样例，修改parser.cpp中的代码，直到可以正确生成AST
+ 语义分析
+ 中间代码生成

## F 项目结构

+ include/
  + cupcake.h
    + 对外接口，编译器的组成部分
  + mtree.h
    + 实现AST用到的多叉树数据结构
  + parser.h
    + 语法分析器，从token序列生成AST
  + scanner.h
    + 词法分析器，从代码生成token序列
  + token.h
    + 定义token数据结构
  + utils.h
    + 一些通用函数声明
+ src/
  + main.cpp
    + 主函数
  + scanner.cpp
    + 词法分析器，从代码生成token序列
  + parser.cpp
    + 语法分析器，从token序列生成AST
  + mtree.cpp
    + 实现AST用到的多叉树数据结构
  + utils.cpp
    + 一些通用函数实现
  + ll1_table.cpp
    + LL1分析表，自动生成
  + gen_ll1_table.py
    + 从grammar.txt自动生成LL1分析表
+ info/
  + grammars/
    + 文法历史版本
  + scanner/
    + symbols.txt
      + 文法的终结符和非终结符
  + parser/
    + grammar.txt
      + 文法定义
    + grammar-expressions.txt
      + 拆解后的文法解析式，自动生成
    + ll1_table_example.cpp
      + ll1_table.cpp的格式样例
  + bugs.txt
    + bug记录
+ test/
  + 用于测试的各种C语言代码

## G 其他

### G.1 Contact

Contact me via my email: weixr0605@sina.com .

### G.2 参考文档 

+ 总体框架 https://decaf-lang.github.io/minidecaf-tutorial/docs/step1/arch.html
+ RISC-V 指令集
  + https://riscv.org/technical/specifications/
  + https://github.com/TheThirdOne/rars/wiki/Supported-Instructions