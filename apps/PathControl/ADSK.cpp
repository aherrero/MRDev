/* 
 * File:   ADSK.cpp
 * Author: alex
 * 
 * Created on 16 de febrero de 2012, 19:50
 */

#include "ADSK.h"
#include "../globalFunctions.h"

ADSK::ADSK() {
    kpg = 2;
    kpd = 2;
    kadsk = 0.95;
    distEndAcum.clear();
    controladskON=false;
    //Declaradas en Control.h
    //velmax=2.0;
    //velmink=0.5;
}

ADSK::ADSK(const ADSK& orig) {
}

ADSK::~ADSK() {
}

void ADSK::ComputeControl() {
    /******CONTROL*****/
    if (!finTray) //Si esta fuera de circunf de fin de segmento, hacer control.
    {
        ControlAnticipativo();
        ControlAngular();
        ControlDistToSeg();

        //SATURACION
        //velavance
        if ((outputProp < velmax) && (outputProp > velmink))
            velavance = outputProp;
        else if (outputProp >= velmax)
            velavance = velmax;
        else if (outputProp <= velmink)
            velavance = velmink;

        //velgiro        
        if (abs(outputDist + outputGiro) < velmax) //Saturacion de los motores
            velgiro = outputDist + outputGiro;
        else if ((outputDist + outputGiro) < 0) //si era negativo, velmax negativa
            velgiro = -velmax;
        else velgiro = velmax;
        
        //cout<<"Vel. Avance: "<<velavance<<"  Vel.Giro: "<<velgiro<<endl;

    } else //Si esta dentro, parar control y pasar a siguiente segmento, si hubiese
    {
        if (currentSegment < reftray.size() - 2) //el -2 necesario porque reftray tiene inicio 0,0,0
        {
            currentSegment++;
            cout << "CambioSegmento" << endl;

        } else {
            velavance = 0.0;
            velgiro = 0.0;
            cout << "No hay mas trayectoria" << endl;
        }
    }

}

bool ADSK::ControlAngular() {
    //Calculo de angulo ideal teniendo en cuenta signo de arcotangente.

    double calculo = atan(abs(reftray[currentSegment + 1].y - pos.y) / 
                          abs(reftray[currentSegment + 1].x - pos.x));

    if (pos.x > reftray[currentSegment + 1].x) {
        if (pos.y > reftray[currentSegment + 1].y) {
            //Primer cuadrante
            anguloideal = PI + calculo;
        } else {
            //Cuarto cuadrante
            anguloideal = PI - calculo;
        }
    } else //x menor
    {
        if (pos.y > reftray[currentSegment + 1].y) {
            //Segundo cuadrante
            anguloideal = 2 * PI - calculo;
        } else {
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
    if (abs(error1) > abs(error2)) {
        error = error2;
    } else {
        error = error1;
    }

    //cout<<"ang ideal "<<(int)(anguloideal*1000)<<" yaw "<<(int)(yaw*1000)<<endl;
    //salida
    outputGiro = kpg*error;

    if ((error < 0.001) && (error>-0.001)) return false; //Si error aprox cero, devuelve false
    else return true;


}

bool ADSK::ControlDistToSeg() {
    //Segmentos de trayectoria
    Vector3D p1 = reftray[currentSegment];
    Vector3D p2 = reftray[currentSegment + 1];
    Segment3D segTray(p1, p2);

    //Distancia del robot a la tray ideal
    double dist2Seg; 
    dist2Seg= gf::distanceCtrl(pos, segTray);
    

    errorVariable = dist2Seg;
    errorVariableAcumulado.push_back(Vector2D(errorVariable, currentSegment));

    //Posicion relativa robot respecto trayectoria
    Vector2D vectTray((p2.x - p1.x), (p2.y - p1.y));
    Vector2D perpvectTray = vectTray.perpendicularVector(); //Ortogonales si su prod escalar es cero.

    Vector2D vectPTray((pos.x - p1.x), (pos.y - p1.y));
    double prodEsc = vectPTray*perpvectTray;

    /******CONTROL DISTANCIA A LA TRAY IDEAL*****/

    float error;

    float distanceRef = 0.0;

    //Si esta a la derecha de la recta,prodEsc positivo
    if (prodEsc > 0)
        error = -(distanceRef - dist2Seg);
    else
        error = (distanceRef - dist2Seg);

    //salida
    outputDist = kpd*error;

    if ((error < 0.001) && (error>-0.001)) return false; //Si error aprox cero, devuelve false
    else return true;
}

bool ADSK::ControlAnticipativo() {
    
    double distEnd=1.0/distToFinCL;
    
    Vector3D ptoA=reftray[currentSegment];
    Vector3D ptoB=reftray[currentSegment+1];
    Vector3D vecBA=ptoA-ptoB;
    float distMinFin = vecBA.module() / 2.0; //Distancia donde comenzara a actuar el controlador
    
    float distEndMultiply=0.5*distEnd;      //Una proporcion para hacerlo equivalente con velmax
    float error= velmax-distEndMultiply;
    
    
    
    if(distToFinCL<distMinFin){                 //Regulador funcionando
        if((kadsk*error)>velmink)
            if((kadsk*error)<velmax)
                outputProp=kadsk*error;
            else
                outputProp=velmax;
        else
            outputProp=velmink;
        
        controladskON=true;
    }
    else{
        if(outputProp<velmax)
            outputProp=outputProp+0.01;         //Puede que esto no sea necesario en el robot real
                                                //Ya que en el real no va a aumentar la velocidad
                                                //tan rapido automaticamente
        else
            outputProp=velmax;
        
        controladskON=false;
    }

    
    distEndAcum.push_back(Vector3D(distEndMultiply,controladskON,currentSegment));
 
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

void ADSK::Save()
{    
    Control::Save();
    
    std::ofstream file("logs/data/Datos.csv",ios::app); 
    //ios::app Abre el archivo y escribe al final
    //Constantes
    file << "KpGiro" << ";" << "KpDistancia" << ";" << "KpAnticipativo" << endl;
    file << kpg << ";" << kpd << ";" << kadsk << endl;
    file << "distEndAcum"<<";"<<"controlAnticipativoON"<<";"<<"currentSeg"<<endl;
    
    for(int i=0;i<distEndAcum.size();i++)
    {
        file << (int)(1000*distEndAcum[i].x)<<";"<<distEndAcum[i].y<<";"<<distEndAcum[i].z<<endl;
    }
    
    file.close();
    cout << "Archivo CSV guardado" << endl;
    
    std::ofstream file2("logs/data/DatosError.txt",ios::app);
    //Constantes K
    file2 << kpg << " " << kpd << " " << kadsk << endl;
    
    file2.close();
    cout << "Archivo TXT guardado" << endl;
    
    
}