---
layout: post
title:  "Launch calc through shellcode"
subtitle: "弹出计算器程序的 shellcode"
date:   2015-04-15 15:25:23 +0800
categories: [geek]
---

简单地讲，ShellCode就是一段01二进制的机器码，试图利用软件中的漏洞完成自己想做的事情。ShellCode的编写需要考虑和顾及的东西很多，这里先写一个适合新手的范例。

* 需要的预备知识：

	1. 汇编语言（尤其要清楚了解函数是如何调用的）

	2. PE文件格式（可参考《Windows PE 权威指南》一书）

	3. 一点点WinAPI函数

* 使用的工具软件：

	1. MASM32

	2. Stud_PE

	3. OllyDBG

### 一、用C语言写出弹出计算器的代码
```c
LoadLibrary("kernel32.dll");
WinExec("calc.exe", 5);
ExitProcess(0);
```

### 二、找到这些函数函数的虚拟内存地址
WinExec()和ExitProcess()都是kernel32.dll中的函数，使用Stud_PE查看kernel32.dll的相关信息

![](/images/shellcode00.png)

这里我们注意到kernel32.dll的映像基址为0x7C800000，也就是说系统在加载kernel32.dll的时候，会将其加载到虚拟内存为0x7C800000的位置。

接着查看kernel32.dll中的函数的偏移地址，偏移地址+映像基址=函数在虚拟内存中的地址

![](/images/shellcode01.png)

可以看到WinExec()函数的偏移地址为0x000623AD，所以WinExec()在虚拟内存中的地址为0x7C8623AD，同理得到ExitProcess()的地址为0x7C81CAFA。

事实上一般的PE文件都会加载kernel32.dll，user32.dll，ntdll.dll等动态链接库，所以我们不必要调用LoadLibrary("kernel32.dll")，不过当我们的ShellCode需要使用到其他动态链接库的时候，如ws2_32.dll等，LoadLibrary()函数是必须的，故也将其地址找到：0x7C801D7B。

注：在实际的ShellCode编写中，函数的地址与操作系统的版本有关，故需要使用其他的技巧，这里只是写一个简单的范例。

### 三、将C语言转化为汇编语言

使用MASM编写编译

```
.386
    .model flat,stdcall
    option casemap:none

include     windows.inc
include     user32.inc
includelib  user32.lib
include     kernel32.inc
includelib  kernel32.lib

    .code
start:
    ;push ebp
    ;mov ebp,esp

    ;LoadLibrary("kernel32.dll");
    ;xor eax,eax
    ;push eax
    ;mov eax,6C6C642Eh   ;".dll"
    ;push eax
    ;mov eax,32336C65h   ;"el32"
    ;push eax
    ;mov eax,6E72656Bh   ;"kern"
    ;push eax
    ;mov eax,esp
    ;push eax            ;Arg1 = "kernel32.dll"
    ;mov eax,7C801D7Bh   ;kernel32.LoadLibrary
    ;call eax

    ;WinExec("calc.exe", 5);
    xor eax,eax
    push eax
    mov eax,6578652Eh   ;".exe"
    push eax
    mov eax,636C6163h   ;"calc"
    push eax
    mov eax,esp
    push 5              ;Arg2 = SW_SHOW
    push eax            ;Arg1 = "calc.exe"
    mov eax,7C8623ADh   ;kernel32.WinExec
    call eax

    ;ExitProcess(0);
    xor eax,eax
    push eax            ;Arg1 = 0
    mov eax,7C81CAFAh   ;kernel32.ExitProcess
    call eax

    ;mov esp,ebp
    ;pop ebp
end start
```

注意到把LoadLibrary()函数注释掉了，这不影响这一段代码的执行

### 四、完成ShellCode

编译完成后可以使用OllyDbg调试。

使用十六进制编辑器打开EXE文件，找到代码段

![](/images/shellcode02.png)

剩下的工作就简单了

```c
//LoadLibraryA("kernel32.dll");
//WinExec("calc.exe",SW_SHOW);

unsigned char shellcode[68] = {
    0x33, 0xC0, 0x50, 0xB8, 0x2E, 0x64, 0x6C, 0x6C, 
    0x50, 0xB8, 0x65, 0x6C, 0x33, 0x32, 0x50, 0xB8, 
    0x6B, 0x65, 0x72, 0x6E, 0x50, 0x8B, 0xC4, 0x50, 
    0xB8, 0x7B, 0x1D, 0x80, 0x7C, 0xFF, 0xD0, 0x33, 
    0xC0, 0x50, 0xB8, 0x2E, 0x65, 0x78, 0x65, 0x50, 
    0xB8, 0x63, 0x61, 0x6C, 0x63, 0x50, 0x8B, 0xC4, 
    0x6A, 0x05, 0x50, 0xB8, 0xAD, 0x23, 0x86, 0x7C, 
    0xFF, 0xD0, 0x33, 0xC0, 0x50, 0xB8, 0xFA, 0xCA, 
    0x81, 0x7C, 0xFF, 0xD0
};

int main()
{
    __asm
    {
        lea eax,shellcode;
        call eax;
    }
    return 0;
}
```