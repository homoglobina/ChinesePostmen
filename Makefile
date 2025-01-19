compile:
	g++ -Isrc/include -c src/*.cpp
	g++ *.o -o main -lm -lsfml-graphics -lsfml-window -lsfml-system

# link:


delete:
	rm *.o