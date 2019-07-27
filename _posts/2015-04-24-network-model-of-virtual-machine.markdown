---
layout: post
title:  "Network model of virtual machine"
subtitle: "虚拟机网络模式"
date:   2015-04-24 15:12:32 +0800
categories: [geek]
---

对于虚拟机桥接、Host-Only 和 NAT 的网络模式认识一直比较模糊，写篇文章总结一下。

首先介绍一些常用网络设备：

1. 调制解调器(Modem): 实现模拟信号与数字信号的相互转换，可以忽略成一条网线。

2. 路由器(Router): 提供路由、DHCP、NAT 等功能或服务的专门设备，连接不同的网络。

3. 交换机或多接口网桥(Switch \| HUB): 交换机和网桥的功能类似，基本上可以认为是一样的。交换机比网桥更智能一些。交换机的接口数量比网桥的多，交换机的通过专门的集成电路或芯片来实现与网桥相同的桥接功能，而网桥没有专门的处理芯片。

以 Vmware 为例，安装完 Vmware 之后，真实主机上会多出 VMware Network Adepter1 和 VMware Network Adepter8 两块虚拟网卡，他们是与 VMnet1 和 VMnet8 两个虚拟交换机相连的，同时可以对虚拟机中的虚拟网卡进行设置。

![](/images/vmnet00.png)

### 一、桥接模式

虚拟机就像一个新增加的、与真实主机有着同等物理地位的一台电脑，桥接模式可以享受所有可用的服务，包括文件服务、打印服务等，并且在此模式下你将获得最简易的从真实主机获取资源的方法，假设真实主机网卡的 IP 地址被配置成 192.168 网段，则虚拟机的 IP 地址也要配成 192.168 网段，这样虚拟机才能和真实主机进行通信。

![](/images/vmnet01.png)

桥接模式下，虚拟机通过虚拟交换机(VMnet0)直接与外部网络连接。

![](/images/vmnet02.png)

### 二、Host-Only 模式

在 Host-Only 模式下，虚拟网络是一个全封闭的网络，它唯一能够访问的就是主机。其实 Host-Only 网络和 NAT 网络很相似，不同的地方就是 Host-Only 网络没有 NAT 服务，所以虚拟网络不能连接到 Internet。主机和虚拟机之间的通信是通过 VMware Network Adepter VMnet1 虚拟网卡连接 VMnet1 虚拟交换机来实现的。Host-Only 拓扑如图所示。

![](/images/vmnet03.png)

Host-Only 模式下，虚拟机无法与外部网络通信，真实主机相当于安装了两块网卡，一块用于与外部网络通信，另一块则用于与虚拟机构成的局域网通信。

![](/images/vmnet04.png)

### 三、NAT模式

NAT(Network Address Translation)模式其实可以理解成为方便地使虚拟机连接到公网，代价是桥接模式下的其他功能都不能享用。使用 NAT 模式可以隐藏虚拟机的拓扑。

![](/images/vmnet05.png)

NAT 模式下，虚拟机上网通过连接在虚拟交换机 VMnet8 上的虚拟 NAT 设备，虚拟 NAT 再连接真实主机上 Internet 网的网卡，将 IP 地址转化为真实主机(PC)网卡的网段，由真实主机代理收发数据，在外部看来，虚拟机(VPC)好像不存在一样。同时，虚拟 NAT 设备还充当代理 DNS 服务器的角色。

![](/images/vmnet06.png)

