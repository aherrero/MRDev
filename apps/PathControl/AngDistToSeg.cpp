/* 
 * File:   AngDistToSeg.cpp
 * Author: alex
 * 
 * Created on 14 de febrero de 2012, 19:18
 */

#include "AngDistToSeg.h"
#include "../globalFunctions.h"
AngDistToSeg::AngDistToSeg()
{
    kpg=2.0;
    kpd=2.5;
}

AngDistToSeg::AngDistToSeg(const AngDistToSeg& orig)
{
}

AngDistToSeg::~AngDistToSeg()
{
}

void AngDistToSeg::ComputeControl()
{
    /******CONTROL*****/
    if (!finTray) //Si esta fuera de circunf de fin de segmento, hacer control.
    {
        ControlAngular();
        ControlDistToSeg();
        velavance = outputProp;
        //SATURACION
        
        //velgiro        
        if (abs(outputDist + outputGiro) < velmaxgi) //Saturacion de los motores
            velgiro = outputDist + outputGiro;
        else if ((outputDist + outputGiro) < 0) //si era negativo, velmax negativa
            velgiro = -velmaxgi;
        else velgiro = velmaxgi;
        

    }     else //Si esta dentro, parar control y pasar a siguiente segmento, si hubiese
    {
        if (currentSegment < reftray.size() - 2)
        {
            currentSegment++;
            cout << "Segment Change" << endl;

        }
        else
        {
            velavance = 0.0;
            velgiro = 0.0;
            cout << "No more path" << endl;
        }
        
    }

}

bool AngDistToSeg::ControlAngular()
{
    //Calculo de angulo ideal teniendo en cuenta signo de arcotangente.

    double calculo = atan(abs(reftray[currentSegment + 1].y - pos.y) / abs(reftray[currentSegment + 1].x - pos.x));

    if (pos.x > reftray[currentSegment + 1].x)
    {
        if (pos.y > reftray[currentSegment + 1].y)
        {
            //Primer cuadrante
            anguloideal = PI + calculo;
        } else
        {
            //Cuarto cuadrante
            anguloideal = PI - calculo;
        }
    } else //x menor
    {
        if (pos.y > reftray[currentSegment + 1].y)
        {
            //Segundo cuadrante
            anguloideal = 2 * PI - calculo;
        } else
        {
            //Tercer cuadrante
            anguloideal = calculo;
        }

    }

    /******CONTROL ANGULAR*****/

    float error, error1, error2;

    //ecuacion del error
    error1 = (anguloideal - yaw);
    error2 = (anguloideal - yaw - 2 * PI);

    //Minimo de los dos
    if (abs(error1) > abs(error2))
    {
        error = error2;
    } else
    {
        error = error1;
    }


    //salida
    outputGiro = kpg*error;

    if ((error < 0.001) && (error>-0.001)) return false; //Si error aprox cero, devuelve false
    else return true;


}

bool AngDistToSeg::ControlDistToSeg()
{
    //Segmentos de trayectoria
    Vector3D p1 (reftray[currentSegment].x,reftray[currentSegment].y,0.0);
    Vector3D p2 (reftray[currentSegment+1].x,reftray[currentSegment+1].y,0.0);
    Segment3D segTray(p1, p2);

    //Distancia del robot a la tray ideal

    double dist2Seg; 
    dist2Seg= gf::distanceCtrl(pos,segTray);
    
    errorVariable=dist2Seg;
    errorVariableAcumulado.push_back(Vector2D(errorVariable,currentSegment));

    //Posicion relativa robot respecto trayectoria
    Vector2D vectTray((p2.x - p1.x), (p2.y - p1.y));
    Vector2D perpvectTray = vectTray.perpendicularVector(); //Ortogonales si su prod escalar es cero.

    Vector2D vectPTray((pos.x - p1.x), (pos.y - p1.y));
    double prodEsc = vectPTray*perpvectTray;

    /******CONTROL DISTANCIA A LA TRAY IDEAL*****/

    float error;
    
    float kdd = 0.01;
    double ddInput;
    double dlastDist;
    
    float distanceRef = 0.0;

    //Si esta a la derecha de la recta,prodEsc positivo
    if (prodEsc > 0)
        error = -(distanceRef - dist2Seg);
    else
        error = (distanceRef - dist2Seg);


    //ddInput = dist2Seg - dlastDist;

    //salida
    outputDist = kpd*error; //-kdd*ddInput;

    //dlastDist = dist2Seg;


    if ((error < 0.001) && (error>-0.001)) return false; //Si error aprox cero, devuelve false
    else return true;
}

void AngDistToSeg::Save()
{    
    Control::Save();
    
    std::ofstream file("../../log/Control/DataError.csv",ios::app); 
    //ios::app Abre el archivo y escribe al final
    //Constantes
    file << "KpGiro" << ";" << "KpDistancia"  << endl;
    file << kpg << ";" << kpd << endl;
    file.close();
    cout << "Archivo CSV guardado" << endl;
    
    std::ofstream file2("../../log/Control/DataError.txt",ios::app);
    //Constantes K
    file2 << kpg << " " << kpd  << endl;
    
    file2.close();
    cout << "Archivo TXT guardado" << endl;
    
    
}