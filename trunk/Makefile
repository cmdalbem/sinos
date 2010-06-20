CC = g++

CFLAGS = -Wall

OBJS = glm.o imageloader.o ObjectModel.o Object.o ObjectStick.o ObjectBall.o Material.o LightInfo.o Level.o Camera.o Texture.o utils.o

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

ObjectStick.o: ObjectStick.cpp ObjectStick.h
	$(CC) $(FLAGS) $< -c $(FLAGS) $(LIBS)

ObjectBall.o: ObjectBall.cpp
	$(CC) $(FLAGS) $< -c $(FLAGS) $(LIBS) 

Material.o: Material.cpp Material.h
	$(CC) $(FLAGS) $< -c $(FLAGS) $(LIBS) 

Camera.o: Camera.cpp Camera.h
	$(CC) $(FLAGS) $< -c $(FLAGS) $(LIBS)

LightInfo.o: LightInfo.cpp LightInfo.h
	$(CC) $(FLAGS) $< -c $(FLAGS) $(LIBS)
	
Level.o: Level.cpp Level.h
	$(CC) $(FLAGS) $< -c $(FLAGS) $(LIBS) 
		
Texture.o: lib/Texture.cpp lib/Texture.h
	$(CC) $(FLAGS) $< -c $(FLAGS) $(LIBS) 

Utils.o: utils.cpp utils.h
	$(CC) $(FLAGS) $< -c $(FLAGS) $(LIBS) 

clean:
	rm -f sinos
	rm -f *.o
