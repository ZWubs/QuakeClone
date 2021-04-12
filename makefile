LIBS=-lsfml-graphics -lsfml-window -lsfml-system

all:
	@echo "Building the game engine..."
	g++ -c main.cpp -o main.o
	g++ -o engine.out main.o $(LIBS)

clean:
	@echo "Removing objects files and executable..."
	rm -f engine.out *.o
