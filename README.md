毕业设计项目
=====
项目简介
----
用c实现的一个基本完整的scheme解释器。
参考资料
----
read,write层以及基本的数据结构参考了peter michaux的[bootstrap-scheme][1]博客和源码。
   
continuation的实现参考了flysnowxg的[tinyscheme revised][2]版本源码以及R.Kent Dybvig的[paper][3]。
   
垃圾回收参考了peter michaux的[博文][4]及[标记清除算法][5]。
   
宏展开系统参考了R.Kent Dybvig的[paper][6]，但未能实现完整的syntax-case系统，只实现了pattern match风格的不健康的宏展开系统。
   
[1]:http://peter.michaux.ca/articles/scheme-from-scratch-introduction
[2]:http://flysnowxg.googlecode.com/svn/tinyscheme_note/
[3]:http://www.cs.indiana.edu/~dyb/papers/3imp.pdf
[4]:http://peter.michaux.ca/articles/scheme-from-scratch-bootstrap-v0_22-garbage-collection
[5]:https://www.google.com.hk/webhp?hl=zh-CN#hl=zh-CN&newwindow=1&q=标记清除算法&safe=strict
[6]:http://www.cs.indiana.edu/%7Edyb/pubs/bc-syntax-case.pdf

数据类型
----
1.integer

2.float

3.bool
4.char
5.string
6.nil
7.proper list
8.improper list
9.symbol
10.closure
11.continuation
12.macro
13.input-port
14.output-port

原语操作
----
见include文件夹下的primitive.h文件





