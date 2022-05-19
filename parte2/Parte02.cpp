#include <iostream>
#include <cstdlib>
#include <random>
#include <cmath>
#include <vector>
#include <string>
#include <dirent.h>
#include <ctime>
#include <fstream>

// Librerías de OpenCV
#include <opencv2/core/core.hpp>           // Contiene las definiciones base de matrices y estructuras
#include <opencv2/highgui/highgui.hpp>     // Interfaz gráfica de usuario
#include <opencv2/imgproc/imgproc.hpp>     // Procesamiento de imágenes
#include <opencv2/imgcodecs/imgcodecs.hpp> // Códecs para leer los distintos formatos de imágenes (JPG, PNG, etc.)
#include <opencv2/video/video.hpp>         // Lectura de vídeos
#include <opencv2/videoio/videoio.hpp>     // Lectura y escritura de videos
//#include <opencv2/opencv.hpp> // Permite cargar todas las librerías de OpenCV

using namespace std;
using namespace cv; // Espacio de nombres de OpenCV para llamar a los métodos de forma más directa

string carpetas[4] = {"./img/ant/","./img/motorbikes/", "./img/sunflower/", "./img/watch/"};
string carpetas_gray[4] = {"./img/gray_ant/","./img/gray_motorbikes/", "./img/gray_sunflower/", "./img/gray_watch/"};
string nombre_carpetas[4] = {"ant","motorbikes", "sunflower", "watch"};
double tiempos[2][4] = {(0,0,0,0),(0,0,0,0)};

void histograma(int histo[], int size, Mat imagen){
    for(int i=0;i<size;i++){
        histo[i] = 0;
    }
    int pixel = 0;
    for(int i=0;i<imagen.rows;i++){
        for(int j=0;j<imagen.cols;j++){
            pixel = imagen.at<uchar>(i,j);
            histo[pixel]++;
        }
    }
}

int maximo(int histo[],int size){
    int maximo = 0;
    for(int i=0;i<size;i++){
        //cout << histo[i] << " ";
        if (maximo<histo[i]){
            maximo = histo[i];
        }
    }
    return maximo;
}

void dibujarHistograma(int histo[], int size, Scalar color, string nombre){
    namedWindow(nombre, WINDOW_AUTOSIZE);
    int maximo_gray = maximo(histo, size);
    int ancho = 1024;
    int alto = 320;
    Mat histo_img = Mat(Size(ancho, alto), CV_8UC3, Scalar(0,0,0));
    double escala = ((double)alto) / ((double) maximo_gray);
    for(int i=0;i<256;i++){
        line(histo_img, Point(i*3,alto), Point(i*3, alto-escala*histo[i]), color, 2);
    }
    imwrite("./img/histogramas/"+nombre + ".png",histo_img);
    imshow(nombre, histo_img);
}

void calculoHistograma(int i){
    int size = 256;
    int histograma_gray[256];
    int histograma_color_b[256], histograma_color_g[256], histograma_color_r[256];
    DIR *direccion;
    struct dirent *elementos;
    if(direccion=opendir(carpetas[i].c_str())){
        while (elementos=readdir(direccion)){
            string elemento = elementos->d_name;
            if(elemento!="." && elemento!=".." && elemento.length() >2 ){
                string pathImagen = carpetas[i] + elemento;
                Mat imagen_color = imread(pathImagen);
                vector<Mat>canales;
                split(imagen_color,canales);
                histograma(histograma_color_b, size, canales[0]);
                histograma(histograma_color_g, size, canales[1]);
                histograma(histograma_color_r, size, canales[2]); 
                Mat imagen_gray = imread(pathImagen, IMREAD_GRAYSCALE); 
                histograma(histograma_gray, size, imagen_gray);
            }
        }
    }
    closedir(direccion);
    dibujarHistograma(histograma_gray, 256, Scalar(255,255,255), "Histograma - Gray Scale "+nombre_carpetas[i]);
    dibujarHistograma(histograma_color_b, 256, Scalar(255,0,0), "Histograma - Azul "+nombre_carpetas[i]);
    dibujarHistograma(histograma_color_g, 256, Scalar(0,255,0), "Histograma - Verde "+nombre_carpetas[i]);
    dibujarHistograma(histograma_color_r, 256, Scalar(0,0,255), "Histograma - Rojo "+nombre_carpetas[i]);
    waitKey();
    destroyAllWindows();
}

void convertirRBG2GRAY(int i){    
    DIR *direccion;
    struct dirent *elementos;
    if(direccion=opendir(carpetas[i].c_str())){
        while (elementos=readdir(direccion)){
            string elemento = elementos->d_name;
            if(elemento!="." && elemento!=".." && elemento.length() >2 ){
                string pathImagen = carpetas[i] + elemento;
                string gray_elemento = elemento.substr(0,elemento.length()-4)+".png";
                Mat img = imread(pathImagen, IMREAD_GRAYSCALE);
                imwrite(carpetas_gray[i]+gray_elemento,img);
            }
        }
    }
    closedir(direccion);
}

int main(){
    // 1.- Proceso de transformación de color a escala de gris
    for(int i = 0; i<4; i++){
        unsigned t0, t1;
        t0=clock();
        convertirRBG2GRAY(i);
        t1 = clock();
        double time = (double(t1-t0)/CLOCKS_PER_SEC);
        //cout << time << endl;
        tiempos[0][i]+=time;
    }
    // 2.- Proceso de histograma
    for(int i = 0; i<4; i++){
        unsigned t0, t1;
        t0=clock();
        calculoHistograma(i);
        t1 = clock();
        double time = (double(t1-t0)/CLOCKS_PER_SEC);
        //cout << time << endl;
        tiempos[0][i]+=time;
    }
    
    string reporte;
    reporte = "****************************************\n";
    reporte +="\t UNIVERSIDAD POLITÉNICA SALESIANA\n";
    reporte += "****************************************\n";
    reporte += "Proceso: conversión de color a gray-scale\n";

    for(int i = 0; i<4; i++){
        reporte += "Carpeta: " + nombre_carpetas[i] + "\n" +
                    "Tiempo demorado: " + to_string(tiempos[0][i]) + "\n"
                    "-----------------------------------------\n";
    }
    reporte+="****************************************\n"
             "Proceso: calculo histograma";
    for(int i = 0; i<4; i++){
        reporte += "Carpeta: " + nombre_carpetas[i] + "\n" +
                    "Tiempo demorado: " + to_string(tiempos[0][i]) + "\n"
                    "-----------------------------------------\n";
    }
    reporte+="Realizado por:\n \t\tWillam Mendiera \n\t\tJorge Pizarro\n";
    reporte+="****************************************\n";

    cout << reporte << endl;

    ofstream file;
    file.open("./parte1/Reporte.txt", std::fstream::in | std::fstream::out | std::fstream::app);
    file << reporte;
    file.close();
    
    return EXIT_SUCCESS;
}
