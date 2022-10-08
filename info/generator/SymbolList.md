# 符号表设计

+ 对象
  + 全局变量
  + 局部变量
  + 函数定义和声明
+ 表项
  + 变量
    + 变量名
    + 声明位置
    + 定义位置
    + 变量类型（类型树）
    + 作用域（以起止token id表示）
  + 函数
    + 函数名
    + 声明位置
    + 定义位置
    + 返回值类型（类型树）
    + 参数列表类型（类型树列表）
    + 作用域（以起止token id表示）

struct CUP_TYPE_NODE{
    enum CUP_METATYPE{
        BASE,
        ARRAY,
        // POINTER,
    } type;
    union{
        const char* b; // base
        int array_len; // array
    } data;
    CUP_TYPE_NODE* element;
};