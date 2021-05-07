LIBS= -lsfml-graphics -lsfml-window -lsfml-system
OPENGL = -lGL -lGLU -lglut

all:
	@echo "Building the game engine..."
	g++ -c main.cpp -o main.o
	g++ -o engine.out main.o $(LIBS) $(OPENGL)

dda:
	@echo "Building DDA test..."
	g++ -c dda.cpp -o dda.o
	g++ -o dda.out dda.o $(LIBS) $(OPENGL)

opengl:
	@echo "Building OpenGL test..."
	g++ -o opengl.out opengl.cpp $(LIBS) $(OPENGL)

clean:
	@echo "Removing objects files and executables..."
	rm -f engine.out *.o *.out

clean-o:
	@echo "Removing objects files..."
	rm -f engine.out *.o
