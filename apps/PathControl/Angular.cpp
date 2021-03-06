/* 
 * File:   Angular.cpp
 * Author: alex
 * 
 * Created on 14 de febrero de 2012, 19:16
 */

#include "Angular.h"
#include "../globalFunctions.h"

Angular::Angular()
{
    kpg=2;
}

Angular::Angular(const Angular& orig)
{
}

Angular::~Angular()
{
}

void Angular::ComputeControl()
{
    /******CONTROL*****/
    if (!finTray) //Si esta fuera de circunf de fin de segmento, hacer control.
    {
        ControlAngular();
        
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

bool Angular::ControlAngular()
{
    //Segmentos de trayectoria
    Vector3D p1 (reftray[currentSegment].x,reftray[currentSegment].y,0.0);
    Vector3D p2 (reftray[currentSegment+1].x,reftray[currentSegment+1].y,0.0);
    Segment3D segTray(p1, p2);

    //Distancia del robot a la tray ideal
    double dist2Seg; 
    dist2Seg= gf::distanceCtrl(pos, segTray);
    errorVariable=dist2Seg;
    errorVariableAcumulado.push_back(Vector2D(errorVariable,currentSegment));
    
    //Calculo de angulo ideal teniendo en cuenta signo de arcotangente.

    double calculo = atan(abs(reftray[currentSegment + 1].y - pos.y) / abs(reftray[currentSegment + 1].x - pos.x));
    //    Angle anglemr;
    //    anglemr.setValue(calculo);
    //    int cuadrante=anglemr.getQuadrant(anglemr);
    //    switch (cuadrante)
    //    {
    //        case 1: anguloideal = PI + calculo;
    //            break;
    //        case 2: anguloideal = 2 * PI - calculo;
    //            break;
    //        case 3:anguloideal = calculo;
    //            break;
    //        case 4: anguloideal = PI - calculo;
    //            break;
    //    }

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

void Angular::Save()
{    
    Control::Save();
    
    std::ofstream file("../../log/Control/DataError.csv",ios::app); 
    //ios::app Abre el archivo y escribe al final
    //Constantes
    file << "KpGiro"  << endl;
    file << kpg <<  endl;
    file.close();
    cout << "Archivo CSV guardado" << endl;
    
    std::ofstream file2("../../log/Control/DataError.txt",ios::app);
    //Constantes K
    file2 << kpg  << endl;
    
    file2.close();
    cout << "Archivo TXT guardado" << endl;
    
    
}