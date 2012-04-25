/* 
 * File:   ControlReactivo.cpp
 * Author: alex
 * 
 * Created on 14 de abril de 2012, 12:43
 */

#include "ControlReactivo.h"
#include "../globalFunctions.h"

ControlReactivo::ControlReactivo() {
    
    rangeAction = 1.3;
    rangeActionFrontal=2.5;
    
    range.clear();
    points.clear();
    pointsObjectDanger.clear();
    pointsObject.clear();
    rangeObject.clear();
    danger = false;
    angleObject.clear();
    kreactivoGiro=1;
    kreactivoAvance=0.5;
    va=vg=0;
    outputAvance=outputGiro=0;
    pointsObjectFrontal.clear();
    pointsObjectFrontalDanger.clear();
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
    vector<Vector2D> pointsObjectFrontal_;
    vector<double> rangeObjectFront_;
    
    vision2d.GetData(point_,range_,yaw_,pos_,pointsObject_,rangeObject_,
                        angleObject_,pointsObjectFrontal_,rangeObjectFront_);
    
    points = point_;
    range = range_;
    yaw = yaw_;
    pos2 = pos_;
    pointsObject = pointsObject_;
    rangeObject = rangeObject_;
    angleObject=angleObject_;
    pointsObjectFrontal=pointsObjectFrontal_;
    rangeObjectFront=rangeObjectFront_;
    
    ObjectDanger();
}

bool ControlReactivo::ObjectDanger() {  //Danger si es menor que una distancia de accion
    danger = false;
    pointsObjectDanger.clear();
    for (int i = 0; i < rangeObject.size(); i++) {
        if (rangeObject[i] < rangeAction) { //Distancia menor que 2 metros, danger
            danger = true;
            pointsObjectDanger.push_back(pointsObject[i]);          
        }
    }
    
    pointsObjectFrontalDanger.clear();
    for (int i = 0; i < rangeObjectFront.size(); i++) {
        if(rangeObjectFront[i]<2.0){
            pointsObjectFrontalDanger.push_back(pointsObjectFrontal[i]);
        }
    }

    return danger;
   
}

void ControlReactivo::SetCommand(float vela, float velg)
{
    va=vela;
    vg=velg;
    Compute();
}

void ControlReactivo::Compute() {
    
    float auxrangeMin = 3;
    Angle auxangleMin;
    
    //**************** No hay object
    if(pointsObjectFrontalDanger.size() <= 0 || pointsObjectDanger.size() <= 0)
    {
        outputGiro = vg;
        outputAvance = va;
    }
    //****** 1) Velocidad de avance disminuye cuando se acerca a objeto
    else 
    {
        if (pointsObjectFrontalDanger.size() > 0) {
            for (int i = 0; i < rangeObjectFront.size(); i++) {
                if (rangeObjectFront[i] < auxrangeMin){
                    auxrangeMin = rangeObjectFront[i];
                }

            }

            float kequivalente = 1.1; 
            float error = va - kequivalente * (1 / auxrangeMin);
            if (error < 0) error = 0;

            outputAvance = kreactivoAvance * error;

        }

        //****** 2) Velocidad de giro    
        int indexangleaux=0;
        Angle auxangle1;
        Angle auxangle2;
        auxangle1.setValue(0.1);
        auxangle2.setValue(0.1);
        
        if (pointsObjectDanger.size() > 0) {
            auxangleMin.setValue(0.0);
            for (int i = 0; i < rangeObject.size(); i++) {
                if (rangeObject[i] < auxrangeMin){
                    auxangleMin=angleObject[i];
                    indexangleaux=i;
                }

            }
            //Promedio
//            if(angleObject.size()>=50){
//                auxangle1=angleObject[indexangleaux-25];
//                auxangle2=angleObject[indexangleaux-50];
//            }
            
            float errorg=PI/2-fabs(auxangleMin.getValue());
//            if((auxangleMin.getValue()<0)&&(auxangle1.getValue()<0)&&(auxangle2.getValue()<0))
//                outputGiro=-kreactivoGiro*errorg;
//            else
            //***DE MOMENTO SIEMPRE POR LA DERECHA
            outputGiro=-kreactivoGiro*errorg;


        }
    }
    //SATURACION
    if(outputGiro>2.0)
        outputGiro=2.0;
    if(outputAvance>2.0)
        outputAvance=2.0;
        

}

void ControlReactivo::GetVel(float& velaa, float& velgg)
{
    velaa = outputAvance;
    velgg = outputGiro; 
}

void ControlReactivo::Save() {

    std::ofstream file("../log/Kinect/Kinect2D(CtrlReact).txt");
    file << "puntosKinect: " << points.size() << endl; //Si queremos XY de los puntos en total, habra que transformarlos
    file << "objects: " << pointsObject.size() << endl;
    file << "objectsDanger: " << pointsObjectDanger.size() << endl;

    for (int i = 0; i < pointsObject.size(); i++) {
        file << i << " pointsObject: " << pointsObject[i] << " range: " << rangeObject[i] << endl;
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
        glPointSize(1.5);
        glColor3ub(255, 0, 255);
        for (int i = 0; i < pointsObjectDanger.size(); i++) {
            glBegin(GL_POINTS);
            glVertex3f(pointsObjectDanger[i].x, pointsObjectDanger[i].y, 0.4);
            glEnd();
        }
        glPopMatrix();
    }
    
    if(!pointsObjectFrontalDanger.empty())
    {
        //Puntos peligtosos frontales
        glPushMatrix();
        glPointSize(3.5);
        glColor3ub(255, 0, 0);
        for (int i = 0; i < pointsObjectFrontalDanger.size(); i++) {
            glBegin(GL_POINTS);
            glVertex3f(pointsObjectFrontalDanger[i].x, pointsObjectFrontalDanger[i].y, 0.4);
            glEnd();
        }
        glPopMatrix();
    }

}