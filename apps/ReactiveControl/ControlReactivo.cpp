/* 
 * File:   ControlReactivo.cpp
 * Author: alex
 * 
 * Created on 14 de abril de 2012, 12:43
 */

#include "ControlReactivo.h"
#include "../globalFunctions.h"

ControlReactivo::ControlReactivo() {
    range.clear();
    points.clear();
    indexObjectDanger.clear();
    pointsObjectDanger.clear();
    pointsObject.clear();
    rangeObject.clear();
    rangeAction = 3;
    danger = false;
    angleObject.clear();
    kreactivoGiro=0.8;
    kreactivoAvance=0.95;
    va=vg=0;
    outputAvance=outputGiro=0;
}

ControlReactivo::ControlReactivo(const ControlReactivo& orig) {
}

ControlReactivo::~ControlReactivo() {
}

void ControlReactivo::SetPoseVision(Vision2D &vision2d) {
    vector <Vector2D> point_;
    vector <double> range_;
    double yaw_;
    Vector2D pos_;
    vector<Vector2D> pointsObject_;
    vector<double> rangeObject_;
    vector<Angle> angleObject_;
    
    vision2d.GetData(point_,range_,yaw_,pos_,pointsObject_,rangeObject_,angleObject_);
    
    points = point_;
    range = range_;
    yaw = yaw_;
    pos2 = pos_;
    pointsObject = pointsObject_;
    rangeObject = rangeObject_;
    angleObject=angleObject_;
    
    ObjectDanger();
}

bool ControlReactivo::ObjectDanger() {  //Danger si es menor que una distancia de accion
    danger = false;
    pointsObjectDanger.clear();
    indexObjectDanger.clear();
    indexObjectDanger.resize(rangeObject.size());
    for (int i = 0; i < rangeObject.size(); i++) {
        if (rangeObject[i] < rangeAction) { //Distancia menor que 2 metros, danger
            danger = true;
            indexObjectDanger[i] = 1;
            pointsObjectDanger.push_back(pointsObject[i]);
        } else
            indexObjectDanger[i] = 0;
    }

}

void ControlReactivo::SetCommand(float vela, float velg)
{
    va=vela;
    vg=velg;
    Compute();
}

void ControlReactivo::Compute() {

    //Habrá que verlo con los puntos mas cercanos al eje del robot, no con todos.
    float auxrangeMin = rangeAction;
    Angle auxangleMin;
    auxangleMin.setValue(0.0);
    if (pointsObjectDanger.size() > 0) {
        for (int i = 0; i < range.size(); i++) {
            if (range[i] < auxrangeMin){
                auxrangeMin = range[i];
                auxangleMin=angleObject[i];
            }

        }

    }
    if(pointsObjectDanger.size()>0){
        //****** 1) Velocidad de avance disminuye cuando se acerca a objeto
        float kequivalente = 0.7; 
        float error = va - kequivalente * (1 / auxrangeMin);
        if (error < 0) error = 0;

        outputAvance = kreactivoAvance * error;

        //****** 2) Velocidad de giro       
        float errorg=PI/2-fabs(auxangleMin.getValue());

        if(auxangleMin.getValue()<0)
            outputGiro=kreactivoGiro*errorg;
        else
            outputGiro=-kreactivoGiro*errorg;
    }
    else 
    { 
        //No hay object
        outputGiro = vg;
        outputAvance = va;
    }
    

}

void ControlReactivo::GetVel(float& velaa, float& velgg)
{
    velaa = outputAvance;
    velgg = outputGiro; 
}

void ControlReactivo::Save() {

    std::ofstream file("logs/data/Kinect2D(CtrlReact).txt");
    file << "puntosKinect: " << points.size() << endl; //Si queremos XY de los puntos en total, habra que transformarlos
    file << "objects: " << pointsObject.size() << endl;
    file << "objectsDanger: " << pointsObjectDanger.size() << endl;

    for (int i = 0; i < pointsObject.size(); i++) {
        file << i << " pointsObject: " << pointsObject[i] << " range: " << rangeObject[i]
                << " danger?: " << indexObjectDanger[i] << endl;
    }
    file << "********************************************" << endl;
    for (int i = 0; i < pointsObjectDanger.size(); i++) {
        file << i << " pointsDanger: " << pointsObjectDanger[i] << endl;
    }

    file.close();
}

void ControlReactivo::Draw() {

    if(!pointsObjectDanger.empty())
    {
        glPushMatrix();
        //Puntos Peligrosos (menor que rangeAction)
        glPointSize(2.0);
        glColor3ub(255, 0, 0);
        for (int i = 0; i < pointsObjectDanger.size(); i++) {
            glBegin(GL_POINTS);
            glVertex3f(pointsObjectDanger[i].x, pointsObjectDanger[i].y, 0.4);
            glEnd();
        }
        glPopMatrix();
    }

}