#include <iostream>
#include <cstdlib>
#include <random>
#include <cmath>
#include <string>
#include <dirent.h>
#include <ctime>
#include <fstream>

//Librerías de OpenCV
#include <opencv2/core/core.hpp>           // Contiene las definiciones base de matrices y estructuras
#include <opencv2/highgui/highgui.hpp>     // Interfaz gráfica de usuario
#include <opencv2/imgproc/imgproc.hpp>     // Procesamiento de imágenes
#include <opencv2/imgcodecs/imgcodecs.hpp> // Códecs para leer los distintos formatos de imágenes (JPG, PNG, etc.)
#include <opencv2/video/video.hpp>         // Lectura de vídeos
#include <opencv2/videoio/videoio.hpp>     // Lectura y escritura de videos

using namespace std;
using namespace cv;

class Imagen{
    public:
        string nombre;
        cv::Mat b;
        cv::Mat g;
        cv::Mat r;
        cv::Mat imagenColor;
        cv::Mat imagenGray;
};