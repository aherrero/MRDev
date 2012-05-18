/* 
 * File:   ControlReactivo.cpp
 * Author: alex
 * 
 * Created on 14 de abril de 2012, 12:43
 */

#include "ControlReactivo.h"
#include "../globalFunctions.h"

ControlReactivo::ControlReactivo() {
    
    rangeAction = 1;
    rangeActionFrontal=2.5;
    
    kreactivoGiro=2;
    kreactivoAvance=1;
    
    range.clear();
    points.clear();
    pointsObjectDanger.clear();
    pointsObject.clear();
    rangeObject.clear();
    danger = false;
    angleObject.clear();

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

void ControlReactivo::SetCommand(float vela, float velg,double dist)
{
    va=vela;
    vg=velg;
    disttray=dist;
    Compute();
}

void ControlReactivo::Compute() {
    
    /**************************************
     * 
     * HACER FUNCIÓN DE IMPRESIÓN EN TXT ESTES DONDE ESTES
     */
    
    
    //**************** No hay object
    if(pointsObjectFrontalDanger.size() <= 0 && pointsObjectDanger.size() <= 0)
    {
        outputGiro = vg;
        outputAvance = va;
    }
    //****** 1) Velocidad de avance disminuye cuando se acerca a objeto
    else 
    {
        float auxrangeMin = rangeActionFrontal;
        float distmax=rangeActionFrontal;
        float distmin=0.3;
        
        if (pointsObjectFrontalDanger.size() > 0) {
            for (int i = 0; i < rangeObjectFront.size(); i++) {
                if (rangeObjectFront[i] < auxrangeMin){
                    auxrangeMin = rangeObjectFront[i];
                }

            }

            float error = (auxrangeMin-distmin)/(distmax-distmin); 

            outputAvance = kreactivoAvance * error *va;

        }

        //****** 2) Velocidad de giro   
        Angle auxangleMin;
        auxangleMin.setValue(0.0);
        float auxrangeGiroMin = rangeAction;
        int angdch=0;
        int angizq=0;
               
        if (pointsObjectDanger.size() > 0) {
                        
            for (int i = 0; i < rangeObject.size(); i++) {
                if (rangeObject[i] < auxrangeGiroMin){
                    auxrangeGiroMin=rangeObject[i];
                    auxangleMin=angleObject[i];

                    if(angleObject[i]<=0)           
                        angdch++;                       
                    else                 
                        angizq++;
                }
            }  
            
            

            float errorg=PI/2+0.01-fabs(auxangleMin.getValue());
                       
            if(angizq<angdch)   //IZQ
                outputGiro=kreactivoGiro*errorg;
            else                //DCH
                outputGiro=-kreactivoGiro*errorg;
            
            if(disttray<0.2)//No hagas caso a react y sigue tray!
                outputGiro=vg;
            
//            //Que no solo tenga prioridad esquivar obstaculos, tb distancia a tray
//            cout<<"dist "<<disttray<<endl;
//            double distPreferenciaReactivo=0.70;
            
//            if(angizq<angdch)
//            {
//                if(disttray>distPreferenciaReactivo)
//                {
//                    //Derecha obligada
//                    outputGiro=-kreactivoGiro*errorg;
//                    cout<<"dch obl"<<endl;
//                }
//                else
//                {
//                    //Izquierda
//                    outputGiro=kreactivoGiro*errorg;
//                    cout<<"izq"<<endl;
//                }
//            }
//            
//            else
//            {
//                    if(disttray>distPreferenciaReactivo)
//                    {
//                        //Izquierda obligada
//                        outputGiro=kreactivoGiro*errorg;
//                        cout<<"izq obl"<<endl;
//                    }
//                    else
//                    {
//                        //Derecha
//                        outputGiro=-kreactivoGiro*errorg;
//                        cout<<"dch"<<endl;
//                    }
//            }
            
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
