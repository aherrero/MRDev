/* 
 * File:   ADSK.cpp
 * Author: alex
 * 
 * Created on 16 de febrero de 2012, 19:50
 */

#include "ADSK.h"
#include "../globalFunctions.h"

ADSK::ADSK() {
    //Simulate default
    kpg = 1.2;
    kpd = 0.5;
    kadsk = 0.95;

    distEndAcum.clear();
    controladskON = false;
}

ADSK::ADSK(float k1, float k2, float k3) : kpg(k1), kpd(k2), kadsk(k3) {
    distEndAcum.clear();
    controladskON = false;
}

ADSK::ADSK(const ADSK& orig) {
}

ADSK::~ADSK() {
}

void ADSK::ComputeControl() {
    /******CONTROL*****/
    if (!finTray) //Si esta fuera de Segmento, hacer control.
    {
        ControlAnticipativo();
        ControlAngular();
        ControlDistToSeg();

        //SATURACION
        //velavance
        if ((outputProp < velmaxav) && (outputProp > velminav))
            velavance = outputProp;
        else if (outputProp >= velmaxav)
            velavance = velmaxav;
        else if (outputProp <= velminav)
            velavance = velminav;

        //velgiro        
        if (abs(outputDist + outputGiro) < velmaxgi) //Saturacion de los motores
            velgiro = outputDist + outputGiro;
        else if ((outputDist + outputGiro) < 0) //si era negativo, velmax negativa
            velgiro = -velmaxgi;
        else velgiro = velmaxgi;

    }
    else //Si esta dentro, parar control y pasar a siguiente segmento, si hubiese
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

bool ADSK::ControlAngular() {

    //Calculo de angulo ideal
    double calculo = atan(abs(reftray[currentSegment + 1].y - pos.y) /
            abs(reftray[currentSegment + 1].x - pos.x));

    //Signo de arcotagente
    if (pos.x > reftray[currentSegment + 1].x)
    {
        if (pos.y > reftray[currentSegment + 1].y)
        {
            //Primer cuadrante
            anguloideal = PI + calculo;
        }
        else
        {
            //Cuarto cuadrante
            anguloideal = PI - calculo;
        }
    }
    else //x menor
    {
        if (pos.y > reftray[currentSegment + 1].y)
        {
            //Segundo cuadrante
            anguloideal = 2 * PI - calculo;
        }
        else
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
    }
    else
    {
        error = error1;
    }

    //salida
    outputGiro = kpg*error;

    if ((error < 0.001) && (error>-0.001)) return false; //Si error aprox cero, devuelve false
    else return true;


}

bool ADSK::ControlDistToSeg() {
    //Segmentos de trayectoria
    Vector3D p1(reftray[currentSegment].x, reftray[currentSegment].y, 0.0);
    Vector3D p2(reftray[currentSegment + 1].x, reftray[currentSegment + 1].y, 0.0);
    //Los calculos con distancia a segmenteos se hacen mejor con 3D en mrcore
    Segment3D segTray(p1, p2);
    double dist2Seg;
    //Distancia del robot a la tray ideal
    dist2Seg = gf::distanceCtrl(pos, segTray);


    errorVariable = dist2Seg;
    errorVariableAcumulado.push_back(Vector2D(errorVariable, currentSegment));

    //Posicion relativa robot respecto trayectoria
    Vector2D vectTray((p2.x - p1.x), (p2.y - p1.y));
    Vector2D perpvectTray = vectTray.perpendicularVector(); //Ortogonales si su prod escalar es cero.

    Vector2D vectPTray((pos.x - p1.x), (pos.y - p1.y));
    double prodEsc = vectPTray*perpvectTray;

    /******CONTROL DISTANCIA A LA TRAY IDEAL*****/

    float error = 0.0;
    float distMax = 10.0; //DistMax para que haga caso control distancia

    //float distanceRef = 0.0;

    //Si esta a la derecha de la recta,prodEsc positivo
    //Haremos una parabola invertida, para que el control de distancia
    //Actue suave cerca de la recta, fuerte a una distancia media, y despreciable
    //Cuando se aleja mucho. Parabola invertida: y=-ax²+bx+c

    if (prodEsc > 0)    //line a la izq
    { 
        //error = -(distanceRef - dist2Seg);
        error =((-(dist2Seg*dist2Seg))/distMax)+dist2Seg;
    }
    else
    {
        //error = (distanceRef - dist2Seg);
        error =-((-(dist2Seg*dist2Seg))/distMax)-dist2Seg;
    }

    //salida
    if(dist2Seg<=distMax)
        outputDist = kpd*error;
    else
        outputDist=outputDist;

    if ((error < 0.001) && (error>-0.001)) return false; //Si error aprox cero, devuelve false
    else return true;
}

bool ADSK::ControlAnticipativo() {

    double distEnd = 1.0 / distToFinCL;

    Vector3D ptoA(reftray[currentSegment].x, reftray[currentSegment].y, 0.0);
    Vector3D ptoB(reftray[currentSegment + 1].x, reftray[currentSegment + 1].y, 0.0);

    Vector3D vecBA = ptoA - ptoB;
    float distMinFin = vecBA.module() / 3.0; //Distancia donde comenzara a actuar el controlador

    float distEndMultiply = 0.1 * distEnd; //Una proporcion para hacerlo equivalente con velmax
    float error = velmaxav - distEndMultiply;



    if (distToFinCL < distMinFin)
    { //Regulador funcionando
        if ((kadsk * error) > velminav)
            if ((kadsk * error) < velmaxav)
                outputProp = kadsk * error;
            else
                outputProp = velmaxav;
        else
            outputProp = velminav;

        controladskON = true;
    }
    else
    {
        if (outputProp < velmaxav)
            outputProp = outputProp + 0.01; //Puede que esto no sea necesario en el robot real
            //Ya que en el real no va a aumentar la velocidad
            //tan rapido automaticamente
        else
            outputProp = velmaxav;

        controladskON = false;
    }


    distEndAcum.push_back(Vector3D(distEndMultiply, controladskON, currentSegment));

    /*******CALCULO DEL ANGULO A CONTINUACIÓN***********************
    El siguiente codigo calcula si el ángulo del siguiente segmento es muy
    cerrado. El problema es que suponemos que el robot llegaba al segmento
    siendo estable y mirando hacia el objetivo.
     ****************************************************************/

    //    //Calculamos angulo entre segmento y segmento
    //    //Si es muy cerrado, la velocidad al llegar sera menor
    //    Vector3D posrobot = pos;
    //    Vector3D ptoA = reftray[currentSegment];
    //    Vector3D ptoB = reftray[currentSegment + 1];
    //    Vector3D ptoC = reftray[currentSegment + 2];
    //    Vector3D vecBA = ptoA - ptoB;
    //    Vector3D vecBC = ptoC - ptoB;
    //
    //    double anguloAC = acos((vecBA * vecBC) / (vecBA.module() * vecBC.module()));
    //
    //    //Tratamos el cambio de segmento como una perturbacion
    //    double refangK = PI; //PI seria lo ideal, el camino recto, 180º
    //    double errorangK = refangK - anguloAC;
    //
    //    //No puede frenar bruscamente, lo haremos proporcional a la llegada del cambio de seg
    //    float distFin = (ptoB - posrobot).module();
    //    float distMinFin = vecBA.module() / 3.0; //Distancia donde comenzara a actuar el controlador
    //
    //    float kcaK = 1;
    //
    //    if (distFin > distMinFin) {
    //        outputProp = outputProp + 0.05; //sube gradualmente velocidad si output+0.1
    //    } else
    //        outputProp = velmax - errorangK * kadsk * (kcaK / distFin);

}

void ADSK::Save() {
    Control::Save();

    std::ofstream file("../../log/Control/DataError.csv", ios::app);
    //ios::app Abre el archivo y escribe al final
    //Constantes
    file << "KpGiro" << ";" << "KpDistancia" << ";" << "KpAnticipativo" << endl;
    file << kpg << ";" << kpd << ";" << kadsk << endl;
    file << "distEndAcum" << ";" << "controlAnticipativoON" << ";" << "currentSeg" << endl;

    for (int i = 0; i < distEndAcum.size(); i++)
    {
        file << (int) (1000 * distEndAcum[i].x) << ";" << distEndAcum[i].y << ";" << distEndAcum[i].z << endl;
    }

    file.close();
    cout << "Archivo CSV guardado" << endl;

    std::ofstream file2("../../log/Control/DataError.txt", ios::app);
    //Constantes K
    file2 << kpg << " " << kpd << " " << kadsk << endl;

    file2.close();
    cout << "Archivo TXT guardado" << endl;


}