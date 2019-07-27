---
layout: post
title:  "Three assembly questions"
subtitle: "三个汇编语言问题"
date:   2015-05-03 15:12:32 +0800
categories: [geek]
---

遇到的三个有关汇编语言的问题

1. Intel 汇编与 AT&T 汇编

2. C 语言中 #define 和 const 的汇编区别

3. 标准调用 \_\_stdcal 和 C 调用 \_\_cdecl 的区别

### 一、Intel汇编与AT&T汇编

汇编指令的两大风格分别是 Intel 汇编与 AT&T 汇编，分别被 Windows 与 Linux 两大阵营所采用：

![](/images/assembly00.png)

### 二、#define和const的区别

使用这样一段简单的代码做测试：

```c
#define NUM 10
#define motd "HELLOWORLD"

int main()
{
    const int n=20;
    char* s=motd;
    return 0;
}
```

汇编的结果是：

```
6:        const int n=20;
00401028   mov         dword ptr [ebp-4],1Ch
7:        char* s=motd;
0040102F   mov         dword ptr [ebp-8],offset string "HELLOWORLD" (0042201c)
```

再把生成的可执行文件放到 OllyDbg 中查看，00422000 是这个可执行程序的只读数据段：

![](/images/assembly01.png)

内存中数据为：

```
00422000  00 00 00 00 DF 29 3A 55 00 00 00 00 02 00 00 00  ....?:U.......
00422010  4C 00 00 00 00 00 00 00 00 A0 02 00 48 45 4C 4C  L........?.HELL
00422020  4F 57 4F 52 4C 44 00 00 00 00 00 00 00 00 00 00  OWORLD..........
```

可以看到 #define 定义的变量存储在文件只读数据段中，而 const 修饰的变量直接在栈中管理。

const 在汇编后和普通变量的定义是一模一样的。const 只是由编译器判断实现的常量，是一个假常量。在实际中，使用 const 定义的变量，最终还是一个变量，只是在编译器内进行了检查，发现有修改则报错。由于编译器在编译期间对 const 变量进行检查，因此被const 修饰过的变量是可以修改的。利用指针获取到 const 修饰过的变量地址，强制将指针的 const 修饰去掉，就可以修改对应的数据内容。

这幅图片摘抄自《C++反汇编与逆向分析技术揭秘》，演示了 const 修饰的变量被修改后的情况。被 const 修饰后，变量本质上并没有改变，还是可以修改的。

![](/images/assembly02.png)

\#define 和 const 的区别

![](/images/assembly03.png)

### 三、\_\_cdecl, \_\_stdcal 和 \_\_fastcall 的区别

调用约定(calling convention)决定以下内容：

1. 函数参数的压栈顺序

2. 由调用者还是被调用者把参数弹出栈

3. 产生函数修饰名的方法

|           |1       |2       |
|:---       |:---    |:---    |
|__cdel     |从右到左 |调用函数者|
|__stdcal   |从右到左 |被调用函数|
|__fastcall |从左开始的两个参数放在 ECX 和 EDX <br /> 其余参数仍然自右向左压栈 <br /> __fastcall 参数一般不超过 4 个字节|被调用函数|

\_\_cdecl 和 \_\_stdcal 之间的唯一区别在于返回时是由被调用者清理栈，还是由调用者清理栈。

除了诸如 fprintf() 这样的参数可变不定长的函数，被调用者事先无法知道参数的长度，事后的清除工作也无法正常的进行，因此，这种情况下我们只能使用 \_\_cdel。在程序中没有涉及可变参数的情况下，最好使用 \_\_stdcall 关键字。
