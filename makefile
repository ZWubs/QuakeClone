LIBS= -lsfml-graphics -lsfml-window -lsfml-system
OPENGL= -lGLEW -lGL

all:
	@echo "Building the game engine..."
	g++ -c main.cpp -o main.o
	g++ -o engine.out main.o $(LIBS)

dda:
	@echo "Building DDA test..."
	g++ -c dda.cpp -o dda.o
	g++ -o dda.out dda.o $(LIBS) $(OPENGL)

test:
	@echo "Building DDA test..."
	g++ -c dda-new.cpp -o dda.o
	g++ -o dda.out dda.o $(LIBS)


clean:
	@echo "Removing objects files and executable..."
	rm -f engine.out *.o
