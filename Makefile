CC = g++
ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
CFLAGS = -g -DGL_GLEXT_PROTOTYPES -DOSX
INCFLAGS = -I./glm-0.9.2.7 -I/usr/X11/include -I./include/ -I/usr/include/
LDFLAGS = -L./osxlib/ \
		-lm -lstdc++ -lfreeimage
else
CFLAGS = -g -DGL_GLEXT_PROTOTYPES 
INCFLAGS = -I./glm-0.9.2.7 -I./include/ -I/usr/X11R6/include -I/sw/include \
		-I/usr/sww/include -I/usr/sww/pkg/Mesa/include
LDFLAGS = -L/usr/X11R6/lib -L/sw/lib -L/usr/sww/lib \
		-L/usr/sww/bin -L/usr/sww/pkg/Mesa/lib -lglut -lGLU -lGL -lX11 -lfreeimage
endif

RM = /bin/rm -f 
all: raytracer
raytracer: main.o read_file.o camera.o directional_light.o graphic_object.o light.o point_light.o sphere.o primitive.o triangle.o transform.o
	$(CC) $(CFLAGS) -o raytracer main.o ReadFile.o Camera.o DirectionalLight.o GraphicObject.o Light.o PointLight.o Sphere.o Primitive.o Triangle.o Transform.o $(INCFLAGS) $(LDFLAGS) 
main.o: main.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c main.cpp
read_file.o: ReadFile.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c ReadFile.cpp
camera.o: Camera.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c Camera.cpp
directional_light.o: DirectionalLight.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c DirectionalLight.cpp
graphic_object.o: GraphicObject.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c GraphicObject.cpp
light.o: Light.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c Light.cpp
point_light.o: PointLight.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c PointLight.cpp
sphere.o: Sphere.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c Sphere.cpp
primitive.o: Primitive.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c Primitive.cpp
triangle.o: Triangle.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c Triangle.cpp
transform.o: Transform.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c Transform.cpp
clean: 
	$(RM) *.o raytracer *.png


 
