# Versions

## v1

### v1.0

实现词法分析器和语法分析器，实现最基本的语法的支持

### v1.1

#### v1.1.0

实现了对一维数组的支持。
接下来，逐步实现对多维数组的支持。

#### v1.1.1 多维数组声明

VAR_IDENTIFIER_A := <identifier> | VAR_IDENTIFIER_A [ <number> ]

#### v1.1.2 多维数组定义

ARRAY_DEF := <type> VAR_IDENTIFIER_A = { COMMA_EXP } ;
COMMA_EXP := COMMA_EXP_ELE | COMMA_EXP, COMMA_EXP_ELE
COMMA_EXP_ELE := <number> | (EXPR)

#### v1.1.3 多维数组形参

PARAMETER := <type> <identifier> | <type> ( PARAMETER_IDF )
PARAMETER_IDF := PARAMETER_IDF_PRE [ ]
PARAMETER_IDF_PRE := <identifier> | PARAMETER_IDF_PRE [ <number> ]

#### v1.1.4 多维数组-左值

PARAMETER := <type> <identifier> | <type> ( PARAMETER_IDF )
PARAMETER_IDF := <identifier> | PARAMETER_IDF [ <number> ]
ASSIGN_EXP := <identifier> = EXPR | ( ARRAY_VISIT ) = EXPR 
ARRAY_VISIT := <identifier> | ARRAY_VISIT [ EXPR ] 

#### v1.1.5 多维数组-右值-准备

EXPR := EXPR_ | EXPR <opr_right> | EXPR <opr_two> EXPR | EXPR <opr_minus> EXPR | <opr_left> EXPR 
EXPR_ :=  <identifier> | <number> |  <sizeof>(TYPE) | <opr_minus> <number> | ( EXPR ) | <identifier> [ EXPR ] | FUNC_CALL 
COMMA_EXP_ELE := EXPR_ | ( EXPR )

#### v1.1.6 多维数组-右值

EXPR_ :=  <identifier> | <number> |  <sizeof>(TYPE) | <opr_minus> <number> | ( EXPR ) | ARRAY_VISIT | FUNC_CALL 

该版本重命名为v1.2.0

### v1.2

在词法分析器和语法分析器的基础上，开发语义分析器，进行符号表构建和静态类型检查。

#### v1.2.0

仅含词法分析器和语法分析器，实现了对多维数组的支持。
