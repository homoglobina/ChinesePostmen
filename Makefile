compile:
	g++ -Isrc/include -c src/*.cpp
	g++ *.o -o main -lm -lsfml-graphics -lsfml-window -lsfml-system

# link:

generate:
	g++ -Isrc/include -c generatorDanych/generateJson.cpp
	g++ generateJson.o -o genJson 
	g++ -Isrc/include -c generatorDanych/generateJsonTests.cpp
	g++ generateJson.o -o genJsonTests 

tester:
	g++ -Isrc/include -c sprawdzanieWyniko/resultTester.cpp
	g++ resultTester.o -o testJson 	


delete:
	rm *.o