SRC := $(wildcard *.c)

qqserver: $(SRC)
	gcc -g $^ -o $@ -lpthread -L/usr/local/lib -L/usr/lib/mysql -lmysqlclient

.PHONY: clean

clean:
	rm qqserver 
