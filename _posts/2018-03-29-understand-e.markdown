---
layout: post
title:  "Understand e"
subtitle: "什么是自然对数的底"
date:   2018-03-29 13:12:21 +0800
categories: [math]
---

对于离散函数 $f(n) = 2^n (n \in \mathbb{Z})$，函数增长量 $\Delta f(n) = f(n+1) - f(n)$ 等于函数当前值 $f(n)$。

那么对于连续函数 $f(x) = a^x (x \in \mathbb{R})$，底数 $a$ 的值为多少时，有这样的性质呢？

我们还是从 $a = 2$ 开始，增长率即导数，

$$f'(x)=\lim_{dx\to0}\frac{2^{x+dx}-2^x}{dx}=\lim_{dx\to0}2^x(\frac{2^{dx}-1}{dx}).$$

令 $dx$ 为很小的数代入计算，

$$\frac{2^{dx}-1}{dx}\approx0.69314718055995.$$

为了使得 $f'(x) = f(x)$，需要该式的值为 $1$，这说明 $a = 2$ 不满足“函数在 $x_0$ 处的增长率等于函数在 $x_0$ 处的值”的性质。设 $e$ 为满足这一性质的值，则有

$$e=(1+dx)^{\frac{1}{dx}}.$$

这就是课本里所定义的

$$e=\lim_{x\to\infty}(1+\frac{1}{x})^x.$$

故称 $e$ 为自然对数之底，$(e^x)' = e^x$ 者也。