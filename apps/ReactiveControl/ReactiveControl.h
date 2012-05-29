/* 
 * File:   ReactiveControl.h
 * Author: alex
 *
 * Created on 17 de mayo de 2012, 20:02
 */

#ifndef REACTIVECONTROL_H
#define	REACTIVECONTROL_H

//#include "mrcore/mrcore.h" //Funciona bien, pero netbeans no reconoce mrcore
#include "/usr/local/mrcore/include/mrcore/mrcore.h"
#include "CinematicMap.h"
#include <iostream>

class ReactiveControl {
public:
    ReactiveControl();
    ReactiveControl(const ReactiveControl& orig);
    virtual ~ReactiveControl();
    
    void SetObstacle(CinematicMap &obstacle);
    void SetCommand(float va_,float vg_,bool sideofpath_);
    void GetCommand(float &vad,float &vrot);
    void Draw();
    
private:
    //Pose
    Vector3D pos;
    double yaw;
    
    //Obstacle
    vector<Vector2D> pointsObstacle;
    vector<double> rangeObstacle;
    vector<Angle> angleObstacle;
    
    //ObstacleDanger
    void ObstacleDangerFront();
    vector<Vector2D> pointsObstacleDangerFR;
    vector<double> rangeObstacleDangerFR;
    vector<Angle> angleObstacleDangerFR;
    void ObstacleDangerLateral();
    vector<Vector2D> pointsObstacleDangerLT;
    vector<double> rangeObstacleDangerLT;
    vector<Angle> angleObstacleDangerLT;
    
    //Compute
    float va;
    float vg;
    void Compute(bool sideofpath);
    
    //ReactiveControl
    float outputRotation;
    float outputAdvance;
    float kadv;
    float krot;
    double dist2traj;
    float velmaxad;
    float velmaxrot;
    
    //Min Distance Obstacle
    float rangeActionFront;
    float rangeActionLateral;

};

#endif	/* REACTIVECONTROL_H */

