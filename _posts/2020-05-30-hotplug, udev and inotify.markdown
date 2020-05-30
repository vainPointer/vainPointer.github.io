---
layout: post
title:  "Monitoring: hotplug, udev and inotify"
subtitle: "Linux 系统监视 API"
date:   2020-05-30 14:48:43 +0800
categories: [geek]
---

## 一、引言

- hotplug：通报关于热插拔设备事件的机制
- udev： 动态地维护 /dev 下的设备文件
- inotify：监视文件系统事件的 API

在 2.6.13 之后，inotify 取代了旧有的 dnotify，克服了它的缺陷。

## 二、inotify

### 1. 用户接口

```c
#include <sys/inotify.h>
/* 创建 inotify 实例，fd 指向一个 struct inotify_event 队列 */
int fd = inotify_init();
/* 向 fd 中添加对 path 的由 mask 指定的监视，返回监视描述符 */
int wd = inotify_add_watch(fd, path, mask);
/* 移除 fd 中的 wd 监视 */
int ret = inotify_rm_watch(fd, wd);

struct inotify_event {
  int    		wd;          /* Watch descriptor */
  uint32_t  mask;        /* Mask describing event */
  uint32_t	cookie;      /* cookie to synchronize two events */
  uint32_t	len;         /* length (including nulls) of name */
  char   		name[];      /* stub for possible name */
};
```

可以在函数 ```inotify_init()``` 返回的 fd 上使用 ```select()```，```poll()``` 或 ioctl 命令 FIONREAD 来得到当前队列的长度。另外 ```close(fd)``` 将删除所有添加到 fd 中的 watch 并做必要的清理。

### 2. 例子

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>

#define EVENT_NUM 12
char *event_str[EVENT_NUM] = {
  "IN_ACCESS",          // File was accessed
  "IN_MODIFY",          // File was modified
  "IN_ATTRIB",          // File attributes were changed
  "IN_CLOSE_WRITE",     // Writtable file closed
  "IN_CLOSE_NOWRITE",   // Unwrittable file closed
  "IN_OPEN",            // File was opened
  "IN_MOVED_FROM",      // File was moved from X
  "IN_MOVED_TO",        // File was moved to Y
  "IN_CREATE",          // Subfile was created
  "IN_DELETE",          // Subfile was deleted
  "IN_DELETE_SELF",     // Self was deleted
  "IN_MOVE_SELF",       // Self was moved
};

int main(int argc, char *argv[]) {
  int fd, wd, len, nread;
  char buf[1024];
  struct inotify_event *event;
  
  fd = inotify_init();
  wd = inotify_add_watch(fd, argv[1], IN_ALL_EVENTS);
  buf[sizeof(buf) - 1] = 0;
  while ((len = read(fd, buf, sizeof(buf) - 1)) > 0) {
    nread = 0;
    while (len > nread) {
      event = (struct inotify_event *)&buf[nread];
      if (event->mask & IN_ISDIR) printf("Directory:\n");
      for (int i = 0; i < EVENT_NUM; i++) {
        if ((event->mask >> i) & 1) {
          if (event->len > 0) 
            printf("%-20s --- %s\n", event_str[i], event->name);
          else printf("%-20s --- %s\n", event_str[i], " ");
        } // end if ((event->mask >> i) & 1)
      } // end for (int i = 0; i < EVENT_NUM; i++)
      nread = nread + sizeof(struct inotify_event) + event->len;
    } // end while (len > nread)
  } // end while ((len = read(fd, buf, sizeof(buf) - 1)) > 0)
  return 0;
}
```



## 三、参考资料

- [](https://www.ibm.com/developerworks/cn/linux/l-inotifynew/index.html)

