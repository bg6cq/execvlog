

记录execv 的调用参数 到 /tmp/execv.log


使用方式：

一定要测试完备后再修改ld.so.preload


测试：

编译后的　execv.so  放到  /usr/local/lib

export LD_PRELOAD=/usr/local/lib/execvlog.so
bash 新启动一个shell，这时所有执行的命令都会记录在 /tmp/execv.log


正常使用：

如果工作正常，可以 vi /etc/ld.so.preload，增加一行
/usr/local/lib/execvlog.so


