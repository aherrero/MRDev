/* 
 * File:   ReactiveControl.h
 * Author: alex
 *
 * Created on 17 de mayo de 2012, 20:02
 */

#ifndef REACTIVECONTROL_H
#define	REACTIVECONTROL_H

//#include "mrcore/mrcore.h" //It works, but netbeans not reconize the functions
#include "/usr/local/mrcore/include/mrcore/mrcore.h"
#include "CinematicMap.h"
#include <iostream>

class ReactiveControl {
public:
    ReactiveControl();
    ReactiveControl(const ReactiveControl& orig);
    virtual ~ReactiveControl();
    
    void RangeAction(float frontal,float frontalmin, float lateral);
    void ConfigReactiveControl(float kav,float krt, float veladv, float velrot);
    void SetObstacle(CinematicMap &obstacle);
    void SetCommand(float va_,float vg_);
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
    void Compute();
    
    //ReactiveControl
    float outputRotation;
    float outputAdvance;
    float kadv;
    float krot;
    float velmaxad;
    float velmaxrot;
    
    //Min Distance Obstacle
    float rangeMaxActionFront;
    float rangeMinActionFront;
    float rangeActionLateral;
    
    float pointSecureObstacle;
    
    //Vision angle
    float openingangleFR;
    float openingangleLT;
    
    int turning;

};

#endif	/* REACTIVECONTROL_H */

