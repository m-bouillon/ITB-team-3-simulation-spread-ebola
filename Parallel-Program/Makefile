all : parallel

debug : nxjson.o config.o tab.o
	if [ ! -d "out" ]; then mkdir out ;fi
	gcc -o parallel source/main.c bin/nxjson.o bin/config.o bin/tab.o -W -Wall -g -lpthread
	mv parallel out

parallel : nxjson.o config.o tab.o
	if [ ! -d "out" ]; then mkdir out ;fi
	gcc -o parallel source/main.c bin/nxjson.o bin/config.o bin/tab.o -W -Wall -lpthread
	mv parallel out

nxjson.o : source/nxjson/nxjson.c
	if [ ! -d "bin" ]; then mkdir bin; fi
	gcc -o nxjson.o -c source/nxjson/nxjson.c -W -Wall
	mv nxjson.o bin

config.o : source/config/config.c
	if [ ! -d "bin" ]; then mkdir bin ; fi
	gcc -o config.o -c source/config/config.c -W -Wall
	mv config.o bin

tab.o : source/tab.c
	if [ ! -d "bin" ]; then mkdir bin ; fi
	gcc -o tab.o -c source/tab.c -W -Wall -lpthread
	mv tab.o bin

clean :
	rm -rf bin/*.o