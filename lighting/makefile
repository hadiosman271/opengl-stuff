lighting.exe: ./.o/main.o ./.o/render.o ./.o/shader.o ./.o/camera.o ./.o/cubie.o ./.o/glad.o
	powershell gcc -g -Wall -o lighting.exe ./.o/main.o ./.o/render.o ./.o/shader.o ./.o/camera.o ./.o/cubie.o ./.o/glad.o -lglfw3 -lopengl32 -lgdi32 -L./libs

./.o/main.o: ./src/main.c | ./.o
	powershell gcc -g -Wall -o ./.o/main.o -c ./src/main.c -I./libs/include

./.o/render.o: ./src/render.c | ./.o
	powershell gcc -g -Wall -o ./.o/render.o -c ./src/render.c -I./libs/include

./.o/shader.o: ./src/shader.c | ./.o
	powershell gcc -g -Wall -o ./.o/shader.o -c ./src/shader.c -I./libs/include

./.o/camera.o: ./src/camera.c | ./.o
	powershell gcc -g -Wall -o ./.o/camera.o -c ./src/camera.c -I./libs/include

./.o/cubie.o: ./src/cubie.c | ./.o
	powershell gcc -g -Wall -o ./.o/cubie.o -c ./src/cubie.c -I./libs/include

./.o/glad.o: ./libs/glad.c | ./.o
	powershell gcc -g -Wall -o ./.o/glad.o -c ./libs/glad.c -I./libs/include

./.o:
	powershell mkdir ./.o

clean:
	powershell rm -r ./.o
