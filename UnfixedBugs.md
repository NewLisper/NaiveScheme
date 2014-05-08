symbol string bug
----

当一次求值循环中定义了一个长度为16的symbol,接着定义一个symbol或者string,都会被加到前者上。  
eg '(aaaaaaaaaaaaaaaa "bbb") 输出(symbol:aaaaaaaaaaaaaaaabbb bbb)  
如果前者长度不等于16则没有任何问题。  
