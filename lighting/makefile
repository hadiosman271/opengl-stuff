lighting.exe: ./.o/main.o ./.o/render.o ./.o/shader.o ./.o/camera.o ./.o/verts.o ./.o/images.o ./.o/glad.o
	powershell gcc -Wall -o lighting.exe ./.o/main.o ./.o/render.o ./.o/shader.o ./.o/camera.o ./.o/verts.o ./.o/images.o ./.o/glad.o -lglfw3 -lopengl32 -lgdi32 -L./libs

./.o/main.o: ./src/main.c ./src/render.h ./src/camera.h | ./.o
	powershell gcc -Wall -o ./.o/main.o -c ./src/main.c -I./libs/include

./.o/render.o: ./src/render.c ./src/images.h ./src/shader.h ./src/shader_src.h ./src/camera.h ./src/verts.h | ./.o
	powershell gcc -Wall -o ./.o/render.o -c ./src/render.c -I./libs/include

./.o/shader.o: ./src/shader.c ./src/shader.h | ./.o
	powershell gcc -Wall -o ./.o/shader.o -c ./src/shader.c -I./libs/include

./.o/camera.o: ./src/camera.c ./src/camera.h | ./.o
	powershell gcc -Wall -o ./.o/camera.o -c ./src/camera.c -I./libs/include

./.o/verts.o: ./src/verts.c ./src/verts.h | ./.o
	powershell gcc -Wall -o ./.o/verts.o -c ./src/verts.c -I./libs/include

./.o/images.o: ./src/images.c ./src/images.h ./src/data.h | ./.o
	powershell gcc -Wall -o ./.o/images.o -c ./src/images.c -I./libs/include

./.o/glad.o: ./libs/glad.c | ./.o
	powershell gcc -Wall -o ./.o/glad.o -c ./libs/glad.c -I./libs/include

./.o:
	powershell mkdir ./.o

clean:
	powershell rm -r ./.o
