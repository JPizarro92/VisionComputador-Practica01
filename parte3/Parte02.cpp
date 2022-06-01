#include <iostream>
#include <cstdlib>
#include <random>
#include <cmath>
#include <vector>
#include <string>
#include <dirent.h>
#include <ctime>
#include <fstream>
#include <glob.h>
#include <stdio.h>
#include <unistd.h>

//Librerías de OpenCV
#include <opencv2/core/core.hpp>           // Contiene las definiciones base de matrices y estructuras
#include <opencv2/highgui/highgui.hpp>     // Interfaz gráfica de usuario
#include <opencv2/imgproc/imgproc.hpp>     // Procesamiento de imágenes
#include <opencv2/imgcodecs/imgcodecs.hpp> // Códecs para leer los distintos formatos de imágenes (JPG, PNG, etc.)
#include <opencv2/video/video.hpp>         // Lectura de vídeos
#include <opencv2/videoio/videoio.hpp>     // Lectura y escritura de videos
//#include <opencv2/opencv.hpp> // Permite cargar todas las librerías de OpenCV

using namespace std;
using namespace cv;

int thres=0;
vector<string> espacios = {"COLOR_BGR2GRAY", "COLOR_BGR2HSV", "COLOR_BGR2Lab", "COLOR_BGR2YCrCb"};
Mat frame;
Mat nuevoEspacio;

void eventoTrack(int v, void *data){
    
    //imshow(espacios.at(thres), nuevoEspacio);
}



int main(){
    
    
    

    VideoCapture video("/dev/video0");
    //VideoCapture video("Italia.mp4");
    if(video.isOpened()){
        namedWindow("Video", WINDOW_AUTOSIZE);
        while(3==3){
            video >> frame;

            if(frame.empty()){
                break;
            }
            //circle(frame, Point(50,50), 23, Scalar(0,0,255), 5);
            resize(frame, frame, Size(), 1.2, 1.2);
            flip(frame, frame, 1);//Utilizado para invertir la imagen
            createTrackbar("Espacio", "Video", &thres, 3, eventoTrack, NULL);

            /*if(nuevoEspacio.empty()){
                nuevoEspacio = frame.clone();
            }*/
            
            if(thres==0){
                cvtColor(frame, nuevoEspacio, COLOR_BGR2GRAY);
            }else if(thres==1){
                cvtColor(frame, nuevoEspacio, COLOR_BGR2HSV);
            }else if(thres==2){
                cvtColor(frame, nuevoEspacio, COLOR_BGR2Lab);
            }else if(thres==3){
                cvtColor(frame, nuevoEspacio, COLOR_BGR2YCrCb);
            }

            imshow("Video", frame);
            imshow("Cambio", nuevoEspacio);

            //anterior = nueva.clone();

            if(waitKey(33)==27){
                break;
            }
        }
        video.release();
        destroyAllWindows();
    }

    return EXIT_SUCCESS;
}
