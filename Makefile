Debug:
	g++ -std=c++14 -g -DDEBUG -pthread -obin/Debug/3dnet main.cpp program.cpp console.cpp world.cpp scene.cpp camera.cpp object.cpp component.cpp components/transform.cpp components/model.cpp parser.cpp client.cpp misc.cpp -ltgui -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system -lGL -lGLU -lglut -lGLEW -Iglm -Icppcodec
	#g++ -std=c++14 -g -DDEBUG -obin/Debug/3dnet main.cpp program.cpp console.cpp world.cpp scene.cpp camera.cpp object.cpp component.cpp components/transform.cpp components/model.cpp -LSFML-2.5.1/lib -LTGUI-0.8.5/lib -ltgui -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system -lGL -lGLU -lglut -ISFML-2.5.1/include/ -ITGUI-0.8.5/include/ -Iglm

Release:
	g++ -std=c++14 -pthread -obin/Release/3dnet main.cpp program.cpp console.cpp world.cpp scene.cpp camera.cpp object.cpp component.cpp components/transform.cpp components/model.cpp parser.cpp client.cpp misc.cpp -ltgui -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system -lGL -lGLU -lglut -lGLEW -Iglm -Icppcodec
	#g++ -std=c++14 -obin/Release/3dnet main.cpp program.cpp console.cpp world.cpp scene.cpp camera.cpp object.cpp component.cpp components/transform.cpp components/model.cpp -LSFML-2.5.1/lib -LTGUI-0.8.5/lib -ltgui -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system -lGL -lGLU -lglut -ISFML-2.5.1/include/ -ITGUI-0.8.5/include/ -Iglm
