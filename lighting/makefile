lighting.exe: ./.o/main.o ./.o/window.o ./.o/glad.o
	powershell gcc -g -Wall -o lighting.exe ./.o/main.o ./.o/window.o ./.o/glad.o -lglfw3 -lopengl32 -lgdi32 -L./libs

./.o/main.o: ./src/main.c | ./.o
	powershell gcc -g -Wall -o ./.o/main.o -c ./src/main.c -I./libs/include

./.o/window.o: ./src/window.c | ./.o
	powershell gcc -g -Wall -o ./.o/window.o -c ./src/window.c -I./libs/include
./.o/glad.o: ./libs/glad.c | ./.o
	powershell gcc -g -Wall -o ./.o/glad.o -c ./libs/glad.c -I./libs/include

./.o:
	powershell mkdir ./.o

clean:
	powershell rm -r ./.o
