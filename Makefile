all: execvlog.so

execvlog.so: execvlog.c
	gcc -shared -fPIC -o execvlog.so execvlog.c -ldl

install: execvlog.so
	cp -f execvlog.so /usr/local/lib/
