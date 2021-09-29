all:
	
lab1:
	g++ src\lab1\lab1.cpp -o builds\lab1.exe -mwindows
	builds\lab1.exe

lab2:
	g++ src\lab1\lab1.cpp -o builds\lab1.exe -mwindows
	g++ src\lab2\lab2.cpp -o builds\lab2.exe -mwindows

clean:
	DEL builds\*
