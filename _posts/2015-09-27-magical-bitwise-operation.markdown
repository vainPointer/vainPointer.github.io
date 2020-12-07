---
layout: post
title:  "Magical bitwise operation"
subtitle: "神奇的位操作"
date:   2015-09-27 15:12:32 +0800
categories: [geek]
---

1.判断奇偶

```c
if (a & 1)
{
    printf("%i is an odd number", a);
}
```

2.交换两数

```c
void Swap(int &a, int &b)
{
    if (a != b)
    {
        a ^= b;
        b ^= a;
        a ^= b;
    }
}
```

3.变换符号

```c
int SignReversal(int a)
{
    return ~a + 1;
}
```

4.求绝对值

```c
int my_abs(int a)
{
    int i = a >> 31;
    return ((a ^ i) - i);
}
```

注：如果 a 是负数，那么 a 右移 31 位为 -1

5.高低位交换

```c
#include <stdio.h>

template <class T>
void PrintfBinary(T a)
{
    int i;
    for (i = sizeof(a) * 8 - 1; i >= 0; --i)
    {
        if ((a >> i) & 1)
            putchar('1');
        else
            putchar('0');
        if (i == 8)
            putchar(' ');
    }
    putchar ('\n');
}

int main()
{
    printf("Before: ");
    unsighed short a = 12345;
    PrintfBinary(a);

    printf("After: ");
    a = (a >> 8) | (a << 8);
    PrintfBinary(a);
    return 0;
}
```

6.二进制逆序

```c
#include <stdio.h>
int main()
{
    printf("Before: ");
    unsigned short a = 12345;
    PrintfBinary(a);

    printf("After: ");
    a = ((a & 0xAAAA) >> 1) | ((a & 0x5555) << 1);
    a = ((a & 0xCCCC) >> 2) | ((a & 0x3333) << 2);
    a = ((a & 0xF0F0) >> 4) | ((a & 0x0F0F) <<4);
    a = ((a & 0xFF00) >> 8) | ((a & 0x00FF) <<8);
    PrintfBinary(a);

    return 0;
}
```

7.二进制中 1 的个数

```c
#include <stdio.h>
int main()
{
    unsigned short a = 12345;
    a = ((a & 0xAAAA) >> 1) + (a & 0x5555);
    a = ((a & 0xCCCC) >> 2) + (a & 0x3333);
    a = ((a & 0xF0F0) >> 4) + (a & 0x0F0F);
    a = ((a & 0xFF00) >> 8) + (a & 0x00FF);
    printf("Have %i ones in binary", a);
    return 0;
}
```

更多内容可以参考 *Hacker's Delight* 一书。