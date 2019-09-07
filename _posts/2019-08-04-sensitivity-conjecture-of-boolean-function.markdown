---
layout: post
title:  "Sensitivity conjecture of boolean function"
subtitle: "布尔函数的敏感度猜想"
date:   2019-08-04 19:27:45 +0800
categories: [math]
---

上个月，[黄皓](http://www.mathcs.emory.edu/~hhuan30/)仅用来[两页](http://www.mathcs.emory.edu/~hhuan30/papers/sensitivity_1.pdf)证明了困扰数学家30年之久的布尔函数的敏感度猜想。让我们来看看这是怎么回事？

## 布尔函数的敏感度

为了形象地表示在电路计算过程中的比特翻转错误，我们可以用一个 $n$ 维的超立方体的每个顶点表示一个 $n$ 比特输入的布尔函数的参数，相邻的两个顶点相差一个比特，用两种不同的颜色表示不同的输出值。见下面的来自 [Quanta Magazine](https://www.quantamagazine.org/mathematician-solves-computer-science-conjecture-in-two-pages-20190725/) 的示意图：

![](/images/sensitivity00.jpg)

对于一个顶点，与之相邻的颜色相异的顶点的数量，称为这个布尔函数的局部敏感度（local sensitivity），而所有局部敏感度的最大值就称为这个布尔函数的敏感度。

## 敏感度猜想

那么敏感度猜想又是怎么回事呢？长期以来，除了敏感度，数学家已经分析清楚了所有有关布尔函数的度量之间的多项式的不等式关系，即

$$m_A \le poly(m_B),$$

$m_A$ 和 $m_B$ 是有关布尔函数的某些度量。比如，与敏感度有几分类似的[块敏感度](http://people.cs.uchicago.edu/~kutin/publications/sens.iandc.pdf)，就与 certificate complexity, the decision tree complexity, the quantum query complexity, and the degree of the boolean function 具有多项式的不等式关系。

对于敏感度 $s(f)$ 可以知道其小于等于块敏感度 $bs(f)$，即知道上界。但是却一直不了解其下界，即是否存在约束敏感度的某个度量，

$$ploy(some\_metric) \le sensitivity\ ?$$

具体来说，是否存在 $K,d$ 使得 $bs(f)\le K(s(f))^d$ 成立？
数学家提出了与之等价的问题 [[Gotsman and Linial 1992](https://www.sciencedirect.com/science/article/pii/0097316592900608)]：对于任意 $n$ 维超立方体的顶点数大于一半的子图 $H$，最大的度 $\Delta(H)$ 是否大于 $n$ 的多项式？

## 黄皓的证明

黄皓证明了

$$\Delta(H)=\sqrt{n}$$

具体的说，对于任意的顶点数为 $2^{n-1}+1$ 的 $n$ 维超立方体的子图 $H$，最大的度 $\Delta(H) \ge \sqrt{n}$。并且当 $n$ 为立方数时，等号成立。

**引理1** (Gauchy's Interlace Theorem) 设 $A$ 是 $n \times n$ 的对称阵，$B$ 是 $m \times m \ (m < n)$ 的 $A$ 的主子阵。如果 $A$ 的特征值为 $\lambda_1 \ge \lambda_2 \ge \cdots \ge \lambda_n$，且 $B$ 的特征值为 $\mu_1 \ge \mu_2 \ge \cdots \ge \mu_m$，则有
$$\lambda_i \ge \mu_i \ge \lambda_{i+n-m}.$$

**引理2** 设 

$$A_1=\begin{bmatrix} 0 & 1 \\ 1 & 0 \end{bmatrix}, A_n=\begin{bmatrix} A_{n-1} & I \\ I & - A_{n-1} \end{bmatrix},$$

则 $A_n$ 特征值为 $\sqrt{n}, -\sqrt{n}$ ，重数都为 $2^{n-1}$。

**证明** 由数学归纳法易证 $A_n^2 = nI$，因此 $A_n$ 的特征值为 $\sqrt{n}$ 或 $-\sqrt{n}$，又因为 $Tr[A_n]=0$，因此正负两者特征值各占一半。

**引理3** 设 $H$ 是顶点数为 $m$ 的无向图，$A$ 是 $H$ 的接邻矩阵，不过其中的元素为 {-1, 0, 1}，即将一些 $1$ 变成 $-1$，则

$$\Delta(H) \ge \lambda_1(A).$$

**证明** 假设 $\lambda_1$ 对应的特征向量是 $\boldsymbol{v}$，即 $\lambda_1\boldsymbol{v} = A\boldsymbol{v}$。不失一般性，设 $v_1$ 是 $\boldsymbol{v}$ 中绝对值最大的元素，则

$$|\lambda_1 v_1|=|(A\boldsymbol{v})_1|=|\sum_{j=1}^{m}A_{1,j}v_j|\le\sum_{j=1}^m|A_{1,j}||v_1|\le\Delta(H)|v_1|. \ \Box$$

有了这几个引理，就可以证明主定理了。

**证明** 令 $A_n$ 是引理2定义的矩阵序列，$A_n$ 中的元素为 {-1, 0, 1}，注意到如果把 $A_n$ 的所有 $-1$ 变成 $1$ 正好是 $n$ 维超立方体的接邻矩阵，因此 $A_n$ 满足引理3。并且，$A_n$ 的主子阵 $A_H$ 也正好是 $n$ 维超立方体的子图 $H$ 的接邻矩阵，$A_H$ 也满足引理3，故

$$\Delta(H)\ge\lambda_1(A_H).$$

另一方面，由引理2知道 $A_n$ 的特征值为

$$\sqrt{n},\cdots,\sqrt{n},-\sqrt{n},\cdots,-\sqrt{n}$$

$A_H$ 是 $(2^{n-1}+1) \times (2^{n-1}+1)$ 的 $A_n$ 的主子阵，由 Cauchy's Interlace Theorem，

$$\lambda_1(A_H) \ge \lambda(A_n)=\sqrt{n}$$

综上，即有 $\Delta(H) \ge \sqrt{n}.\ \Box$

### 简化的证明

以下两个证明不依赖 Cauchy's Interlace Theorem：

[Shalev Ben-David](https://www.scottaaronson.com/blog/?p=4229#comment-1813084) 的证明

[Don Knuth](https://www.cs.stanford.edu/~knuth/papers/huang.pdf) 的证明

## 附录

### [Cauchy's Interlace Theorem](http://matrix.skku.ac.kr/Series-E/Monthly-E.pdf)

**定理** 设 $A$ 是赫米特矩阵，特征值 $\lambda_n \le \cdots \le \lambda_1$。$B$ 是 $A$ 的主子阵，特征值为 $\mu_n \le \cdots \mu_2$，则 $\lambda_n \le \mu_n \cdots \le \lambda_2 \le \mu_2 \le \lambda_1$。

**证明** 设 
$$A=\begin{bmatrix} a & y^* \\ y & B \end{bmatrix},$$

令 
$$D=\begin{bmatrix}\mu_2 & & \\ & \ddots & \\ & & \mu_n \end{bmatrix},$$

则存在酉矩阵 $U$ 使得 $U^*BU=D$。令

$$U^*y = \boldsymbol{z} = (z_2, z_3, \cdots, z_n)^T, z_i \neq 0.$$

若 $\mu_n < \cdots < \mu_2$，令 

$$V=\begin{bmatrix}1 & 0^T \\ 0 & U \end{bmatrix},$$

则 
$$V^*AV = \begin{bmatrix}\ 1 & z^* \\ z & D \end{bmatrix},$$

令

$$f(x)=det(xI-A)=det(xI-V^*AV)$$

根据第一行展开，有

$$f(x)=(x-a)(x-\mu_2)\cdots(x-\mu_n)-\sum_{i=2}^nf_i(x)$$

其中

$$f_i(x)=|z_i|^2(x-\mu_2)\cdots\widehat{(x-\mu_i)}\cdots(x-\mu_n),\ f_i(u_j)=0,$$

故有

$$f_i(u_i)\left\{ \begin{aligned} >0, &\ i\ even \\ <0, &\ i\ odd \end{aligned} \right. \Rightarrow f(u_i)\left\{ \begin{aligned} <0, &\ i\ even \\ >0, &\ i\ odd \end{aligned} \right.$$

由中值定理，得 $f(n)$ 的 $n$ 个根 $\lambda_n < \mu_n < \cdots < \lambda_2 < \lambda_2 < \lambda_1.$

对于一般情况 $\mu_n \le \cdots \le \mu_2$，令 $\{\epsilon_i\}$ 为单调递减趋近于 $0$ 的正实数无穷序列，令

$$C_k = \begin{bmatrix} a & z(\epsilon_k)^* \\ z(\epsilon) & D(\epsilon) \end{bmatrix},$$

使得 $A_k = VC_KV^*$，
其中
$z_i(\epsilon_k) = z_i + \epsilon_k \neq 0,\ i=2,\cdots,n,\ k=1,2,\cdots$，$D(\epsilon_k) = D + \epsilon_k\cdot diag(2,\cdots,n)$。那么

$$A_k\rightarrow A.$$

令 $\lambda^{(k)}_n \le \cdots \lambda^{(k)}_2 \le \lambda^{(k)}_1$ 为 $A_k$ 的特征值，则有

$$\lambda^{(k)}_n < \mu_n + n\epsilon_k < \lambda^{(k)}_{n-1} < \cdots < \lambda^{(k)}_2 < \mu_2 + 2\epsilon_k < \lambda^{(k)}_1 $$

由于特征多项式 $f_k(x)$ 逼近于 $f(x)$，因此特征值 $\lambda_i^{(k)}$ 也逼近与 $\lambda_i. \ \Box$

### Further reading：

#### For the general public:

  - [Quanta Magazine](https://www.quantamagazine.org/mathematician-solves-computer-science-conjecture-in-two-pages-20190725/)

#### For students and non-experts:

  - [Zhang Wenjie's blog](http://www.zhengwenjie.net/huanghao/)
  - [Scott Asronson's blog](https://www.scottaaronson.com/blog/?p=4229)

#### For the experts:

  - [Terence Tao's blog](https://terrytao.wordpress.com/2019/07/26/twisted-convolution-and-the-sensitivity-conjecture/)
  - [Gil Kalai's blog](https://gilkalai.wordpress.com/2019/07/02/amazing-hao-huang-proved-the-sensitivity-conjecture/)
  - [Variations on the Sensitivity Conjecture](https://cims.nyu.edu/~regev/toc/articles/gs004/gs004.pdf)