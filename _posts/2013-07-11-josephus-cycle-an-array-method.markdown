---
layout: post
title:  "Josephus Cycle: an array method"
subtitle: "犹太历史学家 Josephus 的故事"
date:   2013-07-11 15:12:32 +0800
categories: [geek]
---

传说犹太历史学家 Josephus 有如下故事：

在罗马人占领 Giotapart 后，39 个犹太人与 Josephus 及他的朋友躲到一个洞中，39 个犹太人决定宁愿死也不要被人抓到，于是决定了一个自杀方式，41 个人排成一个圆圈，由第 1 个人开始报数，每报数到第 3 人该人就必须自杀，然后再由下一个重新报数，直到所有人都自杀身亡为止。

然而 Josephus 和他的朋友并不想遵从，Josephus 要他的朋友先假装遵从，他将朋友与自己安排在第 16 个与第 31 个位置，于是逃过了这场死亡游戏。

最自然的方法是用循环链表，可以参考 [rosettacode](http://rosettacode.org/wiki/Josephus_problem)。而我写了一个采用数组法的程序：

```c
#include <stdio.h>

int main()
{
    int N = 41;
    int i = 1;
    int m = 3;
    int MAX = N;
    int k, t;
    int a[100], b[100];
    for (k = 0; k < N; k++)
    {
        b[k] = k + 1;
    }
    for(k = 0; k < MAX; k++)
    {
        t = (i + m - 1) % N;
        if (t - 1 < 0) 
        {
            a[k] = b[N-1];
            i = N;
        }
        else 
        {
            a[k] = b[t-1];
            i = t;
            while (t < N) 
            {
                b[t-1] = b[t];
                t++;
            }
        }
        N = N - 1;
        printf("%i ", a[k]);
    }
    printf("\n");
    return 0;
}
```

