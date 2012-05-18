/* 
 * File:   ControlReactivo.h
 * Author: alex
 *
 * Created on 14 de abril de 2012, 12:43
 */

#ifndef CONTROLREACTIVO_H
#define	CONTROLREACTIVO_H

//#include "mrcore/mrcore.h" //Funciona bien, pero netbeans no reconoce mrcore
#include "/usr/local/mrcore/include/mrcore/mrcore.h"
#include "Vision2D.h"
#include <iostream>

class ControlReactivo {
public:
    ControlReactivo();
    ControlReactivo(const ControlReactivo& orig);
    virtual ~ControlReactivo();

    void SetPoseVision(Vision2D &vision2d);

    inline bool GetDistanceOk() {
        return danger;
    };
    void SetCommand(float vela,float velg,double dist);
    void GetVel(float &velaa, float &velgg);

    void Save();
    void Draw();

private:
    bool ObjectDanger();
    void Compute();

    vector <double> range;
    vector <Vector2D> points;
    vector <Vector2D> pointsObjectDanger;
    vector <Vector2D> pointsObject;
    vector <Vector2D> pointsObjectFrontal;
    vector <Vector2D> pointsObjectFrontalDanger;
    vector <double> rangeObjectFront;
    vector<double> rangeObject;
    vector<Angle> angleObject;
    
    float rangeAction;
    float rangeActionFrontal;

    double yaw;
    Vector2D pos2;

    float outputGiro;
    float outputAvance;
    bool danger;
    float rangeNeo;
    
    float kreactivoGiro;
    float kreactivoAvance;
    
    float va;
    float vg;
    
    double disttray;
    

};

#endif	/* CONTROLREACTIVO_H */
