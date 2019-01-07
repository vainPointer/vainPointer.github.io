---
layout: post
title:  "Hidden options in vanilla minecraft server"
subtitle: "自动保存，自动备份，出错重启"
date:   2015-02-17 19:34:01 +0800
categories: [hobby]
---

Vanilla 是 MineCraft 原版服务器的名称。事实上，原版服务器就可以配置好自动保存，自动备份，出错重启等很有用的功能，只不过少有人知。比如自动备份，只需在 server.properties 文件中添加如下代码：

```
autoSave=true 
autoSaveMins=300
autoRestart=true 
useSMPAPI=true 
c10tArgs=true 
c10tMins=60
```

下面是服务器配置文件 server.properties 的全部选项：

```
#默认配置
level-name=world    - 地图名字 　　
level-seed=         - 地图种子 　　
view-distance=10    - 视距 　　
gamemode=0          - 游戏模式(0生存1创造2冒险3旁观者)
difficulty=1        - 游戏难度 　　
allow-flight=true   - 允许飞行 　　
allow-nether=true   - 是否允许地狱 　　
spawn-monsters=true - 是否生成怪物 　　
spawn-animals=true  - 是否生成动物 　　
online-mode=false   - 正版认证 　　
pvp=true            - 玩家对战 　　
max-players=100     - 最大人数 　　
server-ip=          - 服务器ip 　　
server-port=25565   - 服务器端口 　　
white-list=false    - 服务器白名单 　　
motd=Server         - 服务器名字 　　
#隐藏配置: (除非主动在 server.properties 中写出并修改数值，否则不会自动出现) 　　 　　
verify-names=       - 检测昵称，重名昵称将被kick 　　
spawn-protection=   - 重生点保护范围(单位:格) 　　
levelName=          - 服务器名称 　　
onlineMode=         - 在线模式 　　
autoSave=           - 自动保存 　　
autoSaveMins=       - 服务器自动保存时间(单位:秒) 　　
autoBackup=         - 自动备份 　　
autoBackupMins=     - 自动备份时间(单位:秒) 　　
keepBackupHours=    - 保留备份时间,超过将删除备份(单位:天) 　　
autoRestart=        - 自动重启(服务端检测到错误时) 　　
autoRestartMins=    - 自动重启时间(单位:秒) 　　
exitOnFailure=      - 退出失效(未知) 　　
port=               - 端口 　　
internalPort=       - 内部端口 　　
rconPort=           - 远程端口 　　
rconPassword=       - 远程密码 　　
useWhitelist=       - 是否启用特定的人才可加入服务器 　　
useSMPAPI=          - 使用多核处理 　　
c10tArgs=           - 使用C10T参数(好像是10进制的意思) 　　
c10tMins=           - C10T处理时间(单位:秒) 　　
maxPlayers=         - 最大人数
defaultGroup=       - 默认组
useSlashes=         - 使用斜杠语法(默认dots语法) 　　
memory=             - 内存
useMsgFormats=      - 使用信息格式(未知) 　　
msgTitleFormat=     - 信息标题格式 　　
msgFormat=          - 信息格式 　　
localChatRadius=    - 当地信息交互(AAA协议) 　　
debug=              - 调试模式 　　
alternateJarFile=   - 备用的jar文件(建议留空) 　　
guestsCanViewComplex=   - 来宾可以查看(Complex好像是函数) 　　
ipAddress=          - IP地址(建议留空) 　　
javaArguments=      - Java参数
```

另外，以下几条 op 指令也是比较实用的。

```
/gamerule keepInventory true 死亡不会掉落物品
/gamerule doFireTick false   火势不会蔓延
/gamerule mobGriefing false  苦力怕爆炸不会破坏方块
```