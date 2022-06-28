lighting.exe: ./.o/main.o ./.o/render.o ./.o/verts.o ./.o/shader.o ./.o/texture.o ./.o/camera.o ./.o/glad.o
	powershell gcc -Wall -o lighting.exe ./.o/main.o ./.o/render.o ./.o/verts.o ./.o/shader.o ./.o/texture.o ./.o/camera.o ./.o/glad.o -lglfw3 -lopengl32 -lgdi32 -L./libs

./.o/main.o: ./src/main.c ./src/common.h ./src/render.h ./src/camera.h | ./.o
	powershell gcc -Wall -o ./.o/main.o -c ./src/main.c -I./libs/include

./.o/render.o: ./src/render.c ./src/common.h ./src/verts.h ./src/shader.h ./src/texture.h ./src/camera.h | ./.o
	powershell gcc -Wall -o ./.o/render.o -c ./src/render.c -I./libs/include

./.o/verts.o: ./src/verts.c ./src/verts.h | ./.o
	powershell gcc -Wall -o ./.o/verts.o -c ./src/verts.c -I./libs/include

./.o/shader.o: ./src/shader.c ./src/common.h ./src/shader.h | ./.o
	powershell gcc -Wall -o ./.o/shader.o -c ./src/shader.c -I./libs/include

./.o/texture.o: ./src/texture.c ./src/common.h ./src/texture.h | ./.o
	powershell gcc -Wall -o ./.o/texture.o -c ./src/texture.c -I./libs/include

./.o/camera.o: ./src/camera.c ./src/common.h ./src/camera.h | ./.o
	powershell gcc -Wall -o ./.o/camera.o -c ./src/camera.c -I./libs/include

./.o/glad.o: ./libs/glad.c | ./.o
	powershell gcc -Wall -o ./.o/glad.o -c ./libs/glad.c -I./libs/include

./.o:
	powershell mkdir ./.o

clean:
	powershell rm -r ./.o
