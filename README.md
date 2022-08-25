# Algorithm Template for Competitive Programming 

为程序设计相关竞赛准备的模板, 每一份文件之间没有关联. 大多数情况是一份文件对应一道确定的模板题, 可从文件开头的注释中找到对应题目的编号. 也有单纯的模板.

在 C++17 标准下可以编译. 此外, 也使用了一些诸如 `__builtin_ctz` 之类的函数, 如果不能编译请使用 GCC 或者 Clang.

推荐的编译参数

```plain
g++ example.cpp -o example -std=c++17 -Wall -Wshadow -Wextra -O2
```