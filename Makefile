main: main.o sdisk.o block.o filesys.o shell.o table.o
	g++ main.o table.o shell.o filesys.o block.o  -o main

main2: main2.o sdisk.o block.o filesys.o shell.o
	g++ main2.o filesys.o block.o -o main2
    
table.o: table.cpp
	g++ -c table.cpp

shell.o: shell.cpp
	g++ -c shell.cpp

sdisk.o: sdisk.cpp
	g++ -c sdisk.cpp

block.o: block.cpp
	g++ -c block.cpp

filesys.o: filesys.cpp
	g++ -c filesys.cpp

main2.o: main2.cpp
	g++ -c main2.cpp

main.o: main.cpp
	g++ -c main.cpp
    
clean: 
	rm *.o main main2 disk1