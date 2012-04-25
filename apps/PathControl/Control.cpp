/* 
 * File:   Control.cpp
 * Author: alex
 * 
 * Created on 21 de diciembre de 2011, 10:28
 */

#include "Control.h"
#include "../globalFunctions.h"

//IMPLEMENT_MR_OBJECT(Control);

Control::Control() {
    currentSegment = 0;
    finTray = false;
    velavance = velgiro = 0.0;
    pos.x = pos.y = pos.z = 0.0;
    roll = pitch = yaw = 0.0;
    outputGiro = outputDist = anguloideal = 0.0;
    outputProp = 2;
    posicionAcumulada.clear();
    posicionIdealAcumulada.clear();
    errorVariableAcumulado.clear();
    reftray.clear();
    errorVariable = 0.0;

    velmax = 2.0;
    velmink = 0.5;

    globaltime.clear();
    runones = false;

    distanciaTrayReal.clear();
    distanciaTrayIdeal.clear();
    distIdealAcum.clear();
    distIdealAcum.push_back(0.0);
    longitudReal = 0.0;
    longitudIdeal = 0.0;
    refAcumulada.clear();
    refAcumulada.push_back(Vector3D(0, 0, 0));
    distToFinCL=0.0f;
    
    yawAcumulado.clear();

}

Control::~Control() {
}

void Control::SetTray(vector <Vector3D> trayTeleop) {
    for (int i = 0; i < trayTeleop.size(); i++)
        reftray = trayTeleop;

}

void Control::SetPose(Odometry recibeOdo) {
    pos = recibeOdo.pose.position;
    recibeOdo.pose.orientation.getRPY(roll, pitch, yaw);
    posicionAcumulada.push_back(pos);
    yawAcumulado.push_back(yaw);

    //Func
    dataToSave();

}

void Control::GetVel(float & vela, float & velg) {
    ComputeCurrentSegment();
    ComputeControl();

    vela = velavance;
    velg = velgiro;

}

void Control::drawGL() {
    if(!reftray.empty())
    {
        //***Trayectoria IDEAL***//
        for (int i = 0; i < reftray.size() - 1; i++) {
            glPushMatrix();
            glLineWidth(4.0);
            if (i % 2) glColor3ub(0, 0, 200);
            else glColor3ub(0, 100, 200);
            glBegin(GL_LINES);
            glVertex3f(reftray[i].x, reftray[i].y, reftray[i].z); // V0
            glVertex3f(reftray[i + 1].x, reftray[i + 1].y, reftray[i + 1].z); // V1
            glEnd();

            glPopMatrix();
        }

        //***Dibujando trayectoria REAL en puntos****/
        for (int i = 0; i < posicionAcumulada.size(); i++) {
            glPushMatrix();
            glDisable(GL_LIGHTING);
            glColor3f(errorVariableAcumulado[i].x / 0.8, 1 - errorVariableAcumulado[i].x / 0.8, 0);

            glPointSize(4.0);
            glBegin(GL_POINTS);
            glVertex3f(posicionAcumulada[i].x, posicionAcumulada[i].y, posicionAcumulada[i].z);
            glEnd();
            glEnable(GL_LIGHTING);
            glPopMatrix();
        }
    }
}

void Control::ComputeCurrentSegment() {
    //POR UNA PERPENDICULAR AL FINAL DEL SEGMENTO
    Vector3D p1 = reftray[currentSegment];
    Vector3D p2 = reftray[currentSegment + 1];
    Vector2D vectTray((p2.x - p1.x), (p2.y - p1.y));
    perpvectTray = vectTray.perpendicularVector();

    //perpvectTray es el vector perpendicular al segmento de trayectoria en el punto 
    //punto medio del segmento. Lo trasladamos al final del segmento
    Vector2D perpvectTrayPtoFIn(p2.x + perpvectTray.x, p2.y + perpvectTray.y);
    //Si la distanca del robot a esa recta es casi cero, cambia de segmento
    Vector3D auxii(perpvectTrayPtoFIn.x, perpvectTrayPtoFIn.y, 0);
    Segment3D segPerpFin(p2, auxii);
    distToFinCL = gf::distanceCtrl(pos,segPerpFin);
    

    //Y cuando esta dentro de una circunferencia con centro fin de segmento
    //x²+y²=radio²
    float radioPosicionDeseada = 0.2;
    float calculoCoord = pow((pos.x - reftray[currentSegment + 1].x), 2) +
            pow((pos.y - reftray[currentSegment + 1].y), 2);


    //Cuando pasa uno u otro caso:
    if ((calculoCoord <= pow(radioPosicionDeseada, 2)) || (distToFinCL < 0.05)) {
        //Dentro de circunf o distancia al vector perpenciular casi cero
        finTray = true;

        distIdealAcum.push_back(distanciaTrayIdeal.back());

    } else {
        //Cuando esta dentro de un segmento (normalmente)
        finTray = false;

    }
}

void Control::dataToSave() {
    //Punto en tray ideal correspondiente a posicion robot.
    Vector2D ini(reftray[currentSegment].x, reftray[currentSegment].y);
    Vector2D fin(pos.x, pos.y);
    float aux = errorVariable;
    //float diss = mr::distance(ini, fin); //Nuevo mrcore no funciona así
    Vector2D distaux=fin-ini;
    Vector2D vecinifin(distaux.x,distaux.y);
    float diss=vecinifin.module();
    
    float pitagoras = 0;
    if ((diss * diss - aux * aux) >= 0)
        pitagoras = sqrt(diss * diss - aux * aux);
    else pitagoras = 0.0;

    distanciaTrayIdeal.push_back(pitagoras);

    //RefAcum
    refAcumulada.push_back(reftray[currentSegment + 1]);

    //Modulo de la distancia de una posicion actual a posicion anterior.
    Vector3D posant;
    if (runones) { //Ejectuar solo cuando tenga al menos 2 posAcum
        int aux = posicionAcumulada.size() - 1; //Ultima pos
        posant = posicionAcumulada[aux - 1]; //pos anterior
    }
    Vector3D distTray1(pos - posant);
    distanciaTrayReal.push_back(distTray1.module());

    //Cada pos tendrá su tiempo global
    if (!runones) {
        ti.tic(); //Ejecuta una vez
        runones = true;
    }
    globaltime.push_back(ti.toc());

}

void Control::Save() {
    longitudReal = 0;
    longitudIdeal = 0;

    longitudIdeal = distIdealAcum.back() + distanciaTrayIdeal.back();

    for (int i = 0; i < distanciaTrayReal.size(); i++)
        longitudReal = longitudReal + distanciaTrayReal[i];

    //Archivo CSV para hacer graficas
    //Para solucionar separador de excel (utiliza comas en lugar de puntos)
    int multiply = 1000000;
    //Otra solucion seria cambiar variable de entorno LC_NUMERIC
    
    std::ofstream file("logs/data/Datos.csv");
    
    //Longitud en micrometros de las trayectorias
    file <<"LongitudTrayIdeal[umeters]" << ";" << "LongitudTrayReal[umeters]" << endl;
    file << (int) (longitudIdeal * multiply) << ";" << (int) (longitudReal * multiply) << endl;

    //Posicion del robot, tiempo transcurrido y error a la tray ideal.
    file << "posicionRealX[umeters]" << ";" << "posicionRealY[umeters]" << ";" 
            << "posicionRealZ" << ";" << "TiempoGlobal[ms]" << ";"
            << "ErrorToTrayIdeal" << ";" << "currentSegment" << ";" 
            << "TrayX" << ";" << "TrayY" << ";" << "TrayZ" << ";"<<"yaw"<<endl;
    for (int j = 0; j < posicionAcumulada.size(); j++) 
    {
        file << (int) (posicionAcumulada[j].x * multiply) << ";" 
                << (int) (posicionAcumulada[j].y * multiply) << ";"
                << (int) (posicionAcumulada[j].z * multiply) << ";"
                << globaltime[j] << ";"
                << (int) (errorVariableAcumulado[j].x * multiply) << ";" 
                << errorVariableAcumulado[j].y << ";"
                << (int) (refAcumulada[j].x * multiply) << ";" 
                << (int) (refAcumulada[j].y * multiply) << ";"
                << (int) (refAcumulada[j].z * multiply) << ";"
                << (int) (yawAcumulado[j] * 1000) << endl;
    }

    file.close();

    //ARCHIVO DE TEXTO PARA QUE LO LEA CLASE CALCULOERROR
    std::ofstream file2("logs/data/DatosError.txt");

    //Longitud en micrometros de las trayectorias
    file2 << longitudIdeal << " " << longitudReal << endl;
    
    //RefTray
    file2 << reftray.size() << endl;
    for (int i = 0; i < reftray.size(); i++)
        file2 << reftray[i] << endl;

    //Posicion del robot, tiempo transcurrido y error a la tray ideal.
    int numDatos=0;
    numDatos =posicionAcumulada.size();
    file2 << numDatos << endl;
    
    for (int j = 0; j < posicionAcumulada.size(); j++) {
        file2 << posicionAcumulada[j] << " " << globaltime[j] 
                << " " << refAcumulada[j] << endl;
    }
    
    //El error tiene menos terminos que la posAcum(no debería..)
    file2<<errorVariableAcumulado.size()<<endl;
    for(int i=0;i<errorVariableAcumulado.size();i++)
        file2<<errorVariableAcumulado[i]<<endl;

    file2.close();

}