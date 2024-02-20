# Algorithm Template for Competitive Programming 

English | [简体中文](README-zh_CN.md)

This repository is a collection of code which I’ve prepared for competitive
programming. Considering that most competitive programming submissions only
involve a single file containing all the necessary code, this repository isn't
a C++ library. Instead, it consists of numerous unrelated code snippets.

In terms of structure, in most cases, each individual source file corresponds
to a specific typical problem. You can find the corresponding problem number in
the comments at the beginning of each file. The mapping between problem numbers
and Online Judge (OJ) URLs can be found in the
[Appendix](README.md#appendix---problem-numbers-and-corresponding-oj-urls)
later in this file. There are also pure code snippets, but unlike the former,
these snippets cannot be directly tested on OJ platforms.

> [!NOTE]
> While maintaining this repository, my usage of it and my understanding of C++
> have evolved over time. This is reflected in varying coding styles across
> different time periods. In future maintenance, old code will gradually
> undergo refactoring.

## Features

- Time and space complexities meet the requirements of competitive programming
  problems.
- Code implementations are concise yet readable.
- Suitable for printing and carrying as hardcopy materials, if allowed by the
  contest rules.
- Utilizes C++17 features.

## Usage

This code can be compiled under the C++17 standard.
Recommended compiler and compilation parameters:

```plain
g++ example.cpp -o example -std=c++17 -Wall -Wshadow -Wextra -O2
```

> [!CAUTION]
> Outside the C++17 standard, some code may use extensions like `__builtin_ctz`
> for performing efficient bit operations. Certain compilers may not handle
> such code.
> 
> However, C++20 has introduced the support for bit operations. In the future,
> I plan to refactor the code using C++20 standards and eliminate dependencies
> on non-standard functions.

## Appendix - Problem Numbers and Corresponding OJ URLs

|Problem Number|OJ URL||Notes|
|-|-|-|-|
|UOJ *problem-id*|https://uoj.ac|https://uoj.ac/problem/*problem-id*||
|LOJ *problem-id*|https://loj.ac|https://loj.ac/p/*problem-id*||
|Luogu *problem-id*|https://www.luogu.com.cn|https://www.luogu.com.cn/problem/*problem-id*|Pease do not ignore the initial letter in *problem-id*|
|CF *contest-id* *problem-id*|https://codeforces.com|https://codeforces.com/contest/*contest-id*/problem/*problem-id* </br> https://codeforces.com/problemset/problem/*contest-id*/*problem-id* </br> https://codeforces.com/gym/*contest-id*/problem/*problem-id*|Please distinguish between CONTEST and GYM according to *contest-id*|