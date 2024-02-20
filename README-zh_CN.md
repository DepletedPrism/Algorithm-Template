# 程序设计竞赛模板

[English](README.md) | 简体中文

这个 repository 是本人为程序设计相关竞赛准备的模板的集合. 考虑到大多数程序设计竞赛提交答案的形式是提交单个包含所有代码的文件, 因此这并不是一份 C++ 库, 而是众多相互间没有关联的代码的集合.

在形式上, 大多数情况下每个单独的源文件对应一道确定的模板题, 可从文件开头的注释中找到对应题目的编号, 编号与 Online Judge (OJ) 的对应关系能够在后文中的[附录](README-zh_CN.md#附录---题目编号与对应的-oj-网址)中找到. 也有单纯的模板, 不过和前者不同, 这样的代码并不能直接在 OJ 上测试.

> [!NOTE]
> 在维护这些模板的过程中, 我使用这些模板的方式, 以及我自身对 C++ 的理解在不断的发生变化. 在代码上则体现为不同时间的风格迥异. 在后续的维护中旧代码将会被逐步重构.

## 特性

- 时间复杂度与空间复杂度能够满足程序设计竞赛题目的要求
- 代码实现简洁而不失可读性
- 适合打印并作为赛制允许前提下携带的纸质材料
- 使用了 C++17 特性

## 使用

在 C++17 标准下可以编译.
推荐的编译器以及编译参数

```plain
g++ example.cpp -o example -std=c++17 -Wall -Wshadow -Wextra -O2
```
> [!CAUTION]
> 在 C++17 标准以外, 某些题目的代码使用了诸如 `__builtin_ctz` 之类的拓展, 来高效地进行一些位操作. 一些编译器可能无法编译这样的代码.
> 
> 不过, C++20 标准中加入了对位操作的支持. 未来我计划使用 C++20 标准对代码进行重构, 同时去除对这些标准外的函数的依赖.

## 附录 - 题目编号与对应的 OJ 网址

|题目编号|OJ 网址||Notes|
|-|-|-|-|
|UOJ *problem-id*|https://uoj.ac|https://uoj.ac/problem/*problem-id*||
|LOJ *problem-id*|https://loj.ac|https://loj.ac/p/*problem-id*||
|Luogu *problem-id*|https://www.luogu.com.cn|https://www.luogu.com.cn/problem/*problem-id*|请不要忽略 *problem-id* 开头的字母|
|CF *contest-id* *problem-id*|https://codeforces.com|https://codeforces.com/contest/*contest-id*/problem/*problem-id* </br> https://codeforces.com/problemset/problem/*contest-id*/*problem-id* </br> https://codeforces.com/gym/*contest-id*/problem/*problem-id*|请根据 *contest-id* 区分 CONTEST 与 GYM|