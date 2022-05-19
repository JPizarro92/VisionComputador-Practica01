all1:
	g++ ./parte1/Parte01.cpp -o ./parte1/vision.bin -I//home/computacion/opencv/opencvi/include/opencv4 -L/home/computacion/opencv/opencvi/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lopencv_video -lopencv_videoio
all2:
	g++ ./parte2/Parte02.cpp -o ./parte2/vision.bin -I//home/computacion/opencv/opencvi/include/opencv4 -L/home/computacion/opencv/opencvi/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lopencv_video -lopencv_videoio
all3:
	g++ ./parte3/Parte03.cpp -o ./parte3/vision.bin -I//home/computacion/opencv/opencvi/include/opencv4 -L/home/computacion/opencv/opencvi/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lopencv_video -lopencv_videoio
run1:
	./parte1/vision.bin
run2:
	./parte2/vision.bin
run3:
	./parte3/vision.bin