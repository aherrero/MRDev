/* 
 * File:   ReactiveControl.cpp
 * Author: alex
 * 
 * Created on 17 de mayo de 2012, 20:02
 */

#include <vector>

#include "ReactiveControl.h"

ReactiveControl::ReactiveControl() {
    rangeActionFront=1.5;
    rangeActionLateral=1.2;
    kadv=1;
    krot=2;
    
    velmaxad = 1.0;
    velmaxrot = 1.5;
    
    outputAdvance=0.0;
    outputRotation=0.0;
    va=0.0;
    vg=0.0;
    pointsObstacle.clear();
    rangeObstacle.clear();
    angleObstacle.clear();
}

ReactiveControl::ReactiveControl(const ReactiveControl& orig) {
}

ReactiveControl::~ReactiveControl() {
}

void ReactiveControl::SetObstacle(CinematicMap& obstacle)
{
    obstacle.GetObstacle(pointsObstacle,rangeObstacle,angleObstacle);
    ObstacleDangerFront();
    ObstacleDangerLateral();
}

void ReactiveControl::SetCommand(float va_, float vg_,double dist2traj_,bool sideofpath_)
{
    va=va_;
    vg=vg_;
    dist2traj=dist2traj_;
    outputAdvance=va;
    outputRotation=vg;
    Compute(sideofpath_);
}

void ReactiveControl::ObstacleDangerFront()
{
    pointsObstacleDangerFR.clear();
    rangeObstacleDangerFR.clear();
    angleObstacleDangerFR.clear();
    
    for (int i = 0; i < pointsObstacle.size(); i++) {
        if(fabs(angleObstacle[i].getValue())<PI/4)         //Solo nos importa de -pi/4 a pi/4
        {
            if (rangeObstacle[i] < rangeActionFront) {          //Cuando dist menor que 2metros example
                pointsObstacleDangerFR.push_back(pointsObstacle[i]);   
                rangeObstacleDangerFR.push_back(rangeObstacle[i]);
                angleObstacleDangerFR.push_back(angleObstacle[i]);
            }
        }
    }
}

void ReactiveControl::ObstacleDangerLateral()
{
    pointsObstacleDangerLT.clear();
    rangeObstacleDangerLT.clear();
    angleObstacleDangerLT.clear();
    
    for (int i = 0; i < pointsObstacle.size(); i++) {
        if(fabs(angleObstacle[i].getValue())<PI/2)         //Solo nos importa de -90º a 90º
        {
            if (rangeObstacle[i] < rangeActionLateral) {          //Cuando dist menor que 2metros example
                //danger = true;
                pointsObstacleDangerLT.push_back(pointsObstacle[i]);     
                rangeObstacleDangerLT.push_back(rangeObstacle[i]);
                angleObstacleDangerLT.push_back(angleObstacle[i]);
            }
        }
    }
}

void ReactiveControl::Compute(bool sideofpath)
{
    outputRotation = vg;
    outputAdvance = va;
    //**************** No hay object
    cout<<"LT "<<pointsObstacleDangerLT.size()<<" FT "<<pointsObstacleDangerFR.size()<<endl;
    if(pointsObstacleDangerLT.size() <= 20 && pointsObstacleDangerFR.size() <= 20)
    {
        cout<<"TRAJECTORY CONTROL"<<endl;
        return;
    }
    //****** 1) Velocidad de avance disminuye cuando se acerca a objeto
    else 
    {
        float auxrangeMin = rangeActionFront;
        float distmax=rangeActionFront;
        float distmin=0.3;
        int angdch=0;
        int angizq=0;
        
        if (pointsObstacleDangerFR.size() > 0) {
            for (int i = 0; i < pointsObstacleDangerFR.size(); i++) {
                if (rangeObstacleDangerFR[i] < auxrangeMin){
                    auxrangeMin = rangeObstacleDangerFR[i];
                    if(angleObstacleDangerFR[i].getValue()<0)           
                        angdch++;                       
                    else                 
                        angizq++;
                }

            }

            float error = (auxrangeMin-distmin)/(distmax-distmin); 

            cout<<"REACTCTRL. ADVANCE"<<endl;
            outputAdvance = kadv * error *va;

        }

        //****** 2) Velocidad de giro  
        double auxangleMin;
        float auxrangeGiroMin = rangeActionLateral;
               
        if (pointsObstacleDangerLT.size() > 0) {
                        
            for (int i = 0; i < pointsObstacleDangerLT.size(); i++) {
                if (rangeObstacleDangerLT[i] < auxrangeGiroMin){
                    auxrangeGiroMin=rangeObstacleDangerLT[i];
                    auxangleMin=angleObstacleDangerLT[i].getValue();
                }
            }  
            
            

            float errorg=PI/2+0.01-fabs(auxangleMin);
                 
            if(angizq<angdch)
            {
                if (!sideofpath)
                {
                    cout << "REACTCTRL. LEFT" << endl;
                    outputRotation = krot*errorg;
                }
                else
                {
                    cout << "REACTCTRL. RIGHT OBL" << endl;
                    outputRotation = -krot*errorg;
                }
            }
            else
            {
                if (sideofpath)
                {
                    cout<<"REACTCTRL. RIGHT"<<endl;
                    outputRotation=-krot*errorg;  
                }
                else
                {
                    cout << "REACTCTRL. LEFT OBL" << endl;
                    outputRotation = krot*errorg;
                }
            }
            
            //if(outputAdvance<0.02) gira y avanza a cualquier direccion
        }
    }
    //SATURACION
    if(outputAdvance>velmaxad)
        outputAdvance=velmaxad;
    
    if(abs(outputRotation)>velmaxrot)
    {
        if(outputRotation>0)
            outputRotation=velmaxrot;
        else if(outputRotation<0)
            outputRotation= -velmaxrot;
        else
            outputRotation=outputRotation;
    }
    
    if(va==0 && vg==0){  //Fin de la trayectoria
        outputRotation=0.0;
        outputAdvance=0.0;
    }
 
}

void ReactiveControl::GetCommand(float& vad, float& vrot)
{
    vad=outputAdvance;
    vrot=outputRotation;
}


void ReactiveControl::Draw() {

    if(!pointsObstacleDangerLT.empty())
    {
        glPushMatrix();
        //Puntos Peligrosos (menor que rangeAction)
        glPointSize(1.5);
        glColor3ub(50, 0, 255);
        for (int i = 0; i < pointsObstacleDangerLT.size(); i++) {
            glBegin(GL_POINTS);
            glVertex3f(pointsObstacleDangerLT[i].x, pointsObstacleDangerLT[i].y, 0.4);
            glEnd();
        }
        glPopMatrix();
    }
    
    if(!pointsObstacleDangerFR.empty())
    {
        //Puntos peligtosos frontales
        glPushMatrix();
        glPointSize(3.5);
        glColor3ub(255, 0, 0);
        for (int i = 0; i < pointsObstacleDangerFR.size(); i++) {
            glBegin(GL_POINTS);
            glVertex3f(pointsObstacleDangerFR[i].x, pointsObstacleDangerFR[i].y, 0.4);
            glEnd();
        }
        glPopMatrix();
    }

}