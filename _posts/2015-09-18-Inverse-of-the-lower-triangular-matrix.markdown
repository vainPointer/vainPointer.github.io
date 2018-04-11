---
layout: post
title:  "Inverse of the lower triangular matrix"
subtitle: "两种方法求下三角矩阵的逆"
date:   2015-09-18 13:02:21 +0800
categories: [math]
---

方法一：单位矩阵消元

$$\left(A|I\right)\rightarrow\left(I|B\right)$$

```matlab
clear;
n = 500;
A = zeros(n,n);
for j = 1:n
    for i = j:n
        A(i,j) = (i + j)^2;
    end
end
C = A;
B = eye(n);

for i = 1:(n-1)
    B(i,:) = B(i,:)/A(i,i);
    A(i,:) = A(i,:)/A(i,i);
    for j = (i+1):n
        B(j,:) = B(j,:) - B(i,:)*A(j,i);
        A(j,:) = A(j,:) - A(i,:)*A(j,i);
    end
end
B(n,:) = B(n,:)/A(n,n);
A(n,:) = A(n,:)/A(n,n);
```

方法二：分块矩阵迭代

$$\left(\begin{matrix}A&C\\0&B\end{matrix}\right)^{-1}=\left(\begin{matrix}A^{-1}&-A^{-1}CB^{-1}\\0&B^{-1}\end{matrix}\right)$$

$$\left(\begin{matrix}A&0\\C&B\end{matrix}\right)^{-1}=\left(\begin{matrix}A^{-1}&0\\-B^{-1}CA^{-1}&B^{-1}\end{matrix}\right)$$

```matlab
clear;
n = 500;
A = zeros(n,n);
for j = 1:n
    for i = j:n
        A(i,j) = (i + j)^2;
    end
end

inv_p = 1/A(1,1);
for k = 2:n
    c = A(k,1:(k-1));
    zero = zeros((k-1),1);
    inv_q = [inv_p, zero; -(1/A(k,k))*c*inv_p, 1/A(k,k)];
    inv_p = inv_q;
end
```