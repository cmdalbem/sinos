CC = gcc

CFLAGS = -Wall

OBJS = glm.o imageloader.o ObjectModel.o Object.o ObjectBall.o Material.o Camera.o

LIBS = -lglut



all: sinos

 
sinos: opengl.cpp $(OBJS)
	$(CC) $(FLAGS) $^ -o $@ $(FLAGS) $(LIBS) 


glm.o: lib/glm.cpp lib/glm.h
	$(CC) $(FLAGS) $< -c $(FLAGS) $(LIBS) 

imageloader.o: lib/imageloader.cpp lib/imageloader.h
	$(CC) $(FLAGS) $< -c $(FLAGS) $(LIBS) 

ObjectModel.o: ObjectModel.cpp ObjectModel.h
	$(CC) $(FLAGS) $< -c $(FLAGS) $(LIBS) 

Object.o: Object.cpp Object.h
	$(CC) $(FLAGS) $< -c $(FLAGS) $(LIBS) 

ObjectBall.o: ObjectBall.cpp
	$(CC) $(FLAGS) $< -c $(FLAGS) $(LIBS) 

Material.o: Material.cpp Material.h
	$(CC) $(FLAGS) $< -c $(FLAGS) $(LIBS) 

Camera.o: Camera.cpp Camera.h
	$(CC) $(FLAGS) $< -c $(FLAGS) $(LIBS) 


clean:
	rm -f sinos
	rm -f *.o
