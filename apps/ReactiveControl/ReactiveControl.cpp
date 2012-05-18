/* 
 * File:   ReactiveControl.cpp
 * Author: alex
 * 
 * Created on 17 de mayo de 2012, 20:02
 */

#include "ReactiveControl.h"

ReactiveControl::ReactiveControl() {
    rangeActionFront=2.5;
    rangeActionLateral=1.5;
    kadv=1;
    krot=2;
    
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

void ReactiveControl::SetCommand(float va_, float vg_,double dist2traj_)
{
    va=va_;
    vg=vg_;
    dist2traj=dist2traj_;
    outputAdvance=va;
    outputRotation=vg;
    Compute();
}

void ReactiveControl::ObstacleDangerFront()
{
    pointsObstacleDangerFR.clear();
    rangeObstacleDangerFR.clear();
    angleObstacleDangerFR.clear();
    
    for (int i = 0; i < pointsObstacle.size(); i++) {
        if(fabs(angleObstacle[i]<PI/6))         //Solo nos importa de -pi/6 a pi/6
        {
            if (rangeObstacle[i] < rangeActionLateral) {          //Cuando dist menor que 2metros example
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
        if(fabs(angleObstacle[i]<PI/2))         //Solo nos importa de -90º a 90º
        {
            if (rangeObstacle[i] < rangeActionFront) {          //Cuando dist menor que 2metros example
                //danger = true;
                pointsObstacleDangerLT.push_back(pointsObstacle[i]);     
                rangeObstacleDangerLT.push_back(rangeObstacle[i]);
                angleObstacleDangerLT.push_back(angleObstacle[i]);
            }
        }
    }
}

void ReactiveControl::Compute()
{
    //**************** No hay object
    if(pointsObstacleDangerLT.size() <= 0 && pointsObstacleDangerFR.size() <= 0)
    {
        outputRotation = vg;
        outputAdvance = va;
    }
    //****** 1) Velocidad de avance disminuye cuando se acerca a objeto
    else 
    {
        outputRotation = vg;
        float auxrangeMin = rangeActionFront;
        float distmax=rangeActionFront;
        float distmin=0.3;
        
        if (pointsObstacleDangerFR.size() > 0) {
            for (int i = 0; i < pointsObstacleDangerFR.size(); i++) {
                if (rangeObstacleDangerFR[i] < auxrangeMin){
                    auxrangeMin = rangeObstacleDangerFR[i];
                }

            }

            float error = (auxrangeMin-distmin)/(distmax-distmin); 

            outputAdvance = kadv * error *va;

        }

        //****** 2) Velocidad de giro  
        outputAdvance = va;
        Angle auxangleMin;
        auxangleMin.setValue(0.0);
        float auxrangeGiroMin = rangeActionLateral;
        int angdch=0;
        int angizq=0;
               
        if (pointsObstacleDangerLT.size() > 0) {
                        
            for (int i = 0; i < pointsObstacleDangerLT.size(); i++) {
                if (rangeObstacleDangerLT[i] < auxrangeGiroMin){
                    auxrangeGiroMin=rangeObstacleDangerLT[i];
                    auxangleMin=angleObstacleDangerLT[i];

                    if(angleObstacleDangerLT[i]<=0)           
                        angdch++;                       
                    else                 
                        angizq++;
                }
            }  
            
            

            float errorg=PI/2+0.01-fabs(auxangleMin.getValue());
                       
            if(angizq<angdch)   //IZQ
                outputRotation=krot*errorg;
            else                //DCH
                outputRotation=-krot*errorg;  
            
            if(dist2traj<0.2)//No hagas caso a react y sigue trayectoria!
                outputRotation=vg;
        }
    }
    //SATURACION
    if(outputRotation>2.0)
        outputRotation=2.0;
    if(outputAdvance>2.0)
        outputAdvance=2.0;
 
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
        glColor3ub(255, 0, 255);
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