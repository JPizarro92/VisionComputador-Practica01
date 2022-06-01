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

#include "imagen.hpp"

using namespace std;
using namespace cv;


int ancho = 300;
int alto = 200;
string carpetas[4] = {"./img/ant/*","./img/motorbikes/*", "./img/sunflower/*", "./img/watch/*"};
string reporte = "";
int matriz_gray[4][4] = {(0,0,0,0),
                         (0,0,0,0),
                         (0,0,0,0),
                         (0,0,0,0)};
int matriz_bgr[4][4] = {(0,0,0,0),
                        (0,0,0,0),
                        (0,0,0,0),
                        (0,0,0,0)};

vector<string> globVector(const string& pattern){
    glob_t glob_result;
    glob(pattern.c_str(),GLOB_TILDE,NULL,&glob_result);
    vector<string> files;
    for(unsigned int i=0;i<glob_result.gl_pathc;++i){
        files.push_back(string(glob_result.gl_pathv[i]));
    }
    globfree(&glob_result);
    return files;
}

void imprimirVectorString(vector<string> arreglo){
    for(string elemento: arreglo){
        cout << elemento << endl;
    }
}

void ordenarAleatoriamente(vector<string>& arreglo){
    random_device rd;
    default_random_engine rng(rd());
    shuffle(arreglo.begin(), arreglo.end(),rng);
}

void cargarImagenes(vector<Imagen>& train, vector<Imagen>& test, vector<string> dir){
    int limit =(int)(dir.size()*0.8);
    //cout << "Limite:" << limit << endl;
    
    for(int i = 0; i < dir.size() ; i++){
        Imagen imagen;
        imagen.nombre=dir[i];

        Mat img = imread(dir[i]);
        
        resize(img, img, Size(ancho, alto), INTER_LINEAR);
        imagen.imagenColor=img;
        
        vector<Mat> bgr_canales;
        split( img, bgr_canales );

        imagen.b=bgr_canales[0];
        imagen.g=bgr_canales[1];
        imagen.r=bgr_canales[2];

        cvtColor(img,img,COLOR_BGR2GRAY);
        imagen.imagenGray=img;

        if(i < limit){
            train.push_back(imagen);
        }else{
            test.push_back(imagen);
        }

    }
}

int restarImagen(Mat& img1, Mat& img2){
    Mat resta;
    //subtract(img1,img2,resta);
    absdiff(img1,img2,resta);
    int resultado=0;
    for(int i = 0; i< resta.rows; i++){
        for (int j = 0; j < resta.cols; j++)
        {
            resultado+=(int)resta.at<uchar>(i,j);
            //cout << (int) resta.at<uchar>(i,j) << endl;
        }
    }
    //cout << resultado << endl;
    return resultado;
}

void predecirGray(  vector<Imagen>& test, 
                    vector<Imagen>& ants, vector<Imagen>& motorBikes,
                    vector<Imagen>& sunFlowers, vector<Imagen>& watches, int aux){
    int minimal = 255*100*200;//(255*200*100)
    int prediccion = 0;
    for(Imagen testing: test){
        for(Imagen ant: ants){
            int resta = restarImagen(testing.imagenGray,ant.imagenGray);
            if(resta <  minimal){
                minimal=resta;
            }
        }
        for(Imagen motorBike: motorBikes){
            int resta = restarImagen(testing.imagenGray,motorBike.imagenGray);
            if(resta <  minimal){
                minimal=resta;
                if(prediccion==0){
                    prediccion+=1;
                }
            }
        }
        for(Imagen sunFlower: sunFlowers){
            int resta = restarImagen(testing.imagenGray,sunFlower.imagenGray);
            if(resta <  minimal){
                minimal=resta;
                if(prediccion==1){
                    prediccion+=1;
                }
            }
        }
        for(Imagen watch: watches){
            int resta = restarImagen(testing.imagenGray,watch.imagenGray);
            if(resta <  minimal){
                minimal=resta;
                if(prediccion==2){
                    prediccion+=1;
                }
            }
        }
        //cout << "Predicción: " << prediccion << endl;
        matriz_gray[aux][prediccion]+=1;
    }
}

void predecirColor( vector<Imagen>& test, 
                    vector<Imagen>& ants, vector<Imagen>& motorBikes,
                    vector<Imagen>& sunFlowers, vector<Imagen>& watches, int aux){
    int minimal = (255*200*100*3);
    int prediccion = 0;
    for(Imagen testing: test){
        for(Imagen ant: ants){
            int resta_b = restarImagen(testing.b,ant.b);
            int resta_g = restarImagen(testing.g,ant.g);
            int resta_r = restarImagen(testing.r,ant.r);
            int resta = resta_b + resta_g + resta_r;
            if(resta <  minimal){
                minimal=resta;
            }
        }
        for(Imagen motorBike: motorBikes){
            int resta_b = restarImagen(testing.b,motorBike.b);
            int resta_g = restarImagen(testing.g,motorBike.g);
            int resta_r = restarImagen(testing.r,motorBike.r);
            int resta = resta_b + resta_g + resta_r;
            if(resta <  minimal){
                minimal=resta;
                if(prediccion==0){
                    prediccion+=1;
                }
            }
        }
        for(Imagen sunFlower: sunFlowers){
            int resta_b = restarImagen(testing.b,sunFlower.b);
            int resta_g = restarImagen(testing.g,sunFlower.g);
            int resta_r = restarImagen(testing.r,sunFlower.r);
            int resta = resta_b + resta_g + resta_r;
            if(resta <  minimal){
                minimal=resta;
                if(prediccion==1){
                    prediccion+=1;
                }
            }
        }
        for(Imagen watch: watches){
            int resta_b = restarImagen(testing.b,watch.b);
            int resta_g = restarImagen(testing.g,watch.g);
            int resta_r = restarImagen(testing.r,watch.r);
            int resta = resta_b + resta_g + resta_r;
            if(resta <  minimal){
                minimal=resta;
                if(prediccion==2){
                    prediccion+=1;
                }
            }
        }
        //cout << "Predicción: " << prediccion << endl;
        matriz_bgr[aux][prediccion]+=1;
    }
}


void imprimirMatriz(int mat[4][4]){
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            cout << mat[i][j] << " " << ends;
            reporte += to_string(mat[i][j])+",";
        }
        reporte += "\n";
        cout << "\n" << endl;
    }
    
}

int main(){
    
    vector<string> dirAnt = globVector(carpetas[0]);
    vector<string> dirMotorBikes = globVector(carpetas[1]);
    vector<string> dirSunFlower = globVector(carpetas[2]);
    vector<string> dirWacth = globVector(carpetas[3]);

    ordenarAleatoriamente(dirAnt);
    ordenarAleatoriamente(dirMotorBikes);
    ordenarAleatoriamente(dirSunFlower);
    ordenarAleatoriamente(dirWacth);
    //imprimirVectorString(dirAnt);

    vector<Imagen> trainAnt;
    vector<Imagen> testAnt;
    cargarImagenes(trainAnt, testAnt, dirAnt);

    vector<Imagen> trainMotorBike;
    vector<Imagen> testMotorBike;
    cargarImagenes(trainMotorBike, testMotorBike, dirMotorBikes);

    vector<Imagen> trainSunFlower;
    vector<Imagen> testSunFlower;
    cargarImagenes(trainSunFlower, testSunFlower, dirSunFlower);

    vector<Imagen> trainWacth;
    vector<Imagen> testWacth;
    cargarImagenes(trainWacth, testWacth, dirWacth);

    
    //restarImagen(trainAnt.at(0).imagenGray,trainAnt.at(0).imagenGray);
    
    predecirGray(testAnt,trainAnt,trainMotorBike,trainSunFlower,trainWacth,0);
    predecirGray(testMotorBike,trainAnt,trainMotorBike,trainSunFlower,trainWacth,1);
    predecirGray(testSunFlower,trainAnt,trainMotorBike,trainSunFlower,trainWacth,2);
    predecirGray(testWacth,trainAnt,trainMotorBike,trainSunFlower,trainWacth,3);
    cout << "Matriz de confución de GrayScale" << endl;
    imprimirMatriz(matriz_gray);
    predecirColor(testAnt,trainAnt,trainMotorBike,trainSunFlower,trainWacth,0);
    predecirColor(testMotorBike,trainAnt,trainMotorBike,trainSunFlower,trainWacth,1);
    predecirColor(testSunFlower,trainAnt,trainMotorBike,trainSunFlower,trainWacth,2);
    predecirColor(testWacth,trainAnt,trainMotorBike,trainSunFlower,trainWacth,3);
    cout << "Matriz de confución de BGR" << endl;
    imprimirMatriz(matriz_bgr);
    
    ofstream file;
    file.open("./Reporte.csv", std::fstream::in | std::fstream::out | std::fstream::app);
    file << reporte;
    file.close();

    return EXIT_SUCCESS;
}
