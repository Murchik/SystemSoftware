all:
	
lab1:
	g++ src\lab1.cpp -o builds\lab1.exe -mwindows
	builds\lab1.exe

lab2:
	g++ src\lab1.cpp -o builds\lab1.exe -mwindows
	g++ src\lab2.cpp -o builds\lab2.exe -mwindows

lab4:
	g++ -std=c++17 src\lab4.cpp src\bathroom.cpp src\person.cpp -o builds\lab4.exe
	builds\lab4.exe

clean:
	DEL builds\*
