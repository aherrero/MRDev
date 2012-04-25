/* 
 * File:   Vision2D.h
 * Author: alex
 *
 * Created on 13 de abril de 2012, 12:02
 */

#ifndef VISION2D_H
#define	VISION2D_H

#include "mrcore/mrcore.h"
#include <iostream>

class Vision2D : public GLObject {
public:
    Vision2D();
    Vision2D(const Vision2D& orig);
    virtual ~Vision2D();

    //Set and Get
    void SetPose(Odometry odometry);
    void SetLaser(LaserData laserdata);
    void GetData(vector <Vector2D> &point_, vector <double> &range_, double &yaw_,
            Vector2D &pos_, vector <Vector2D> &pointsObject_,
            vector <double> &rangeObject_,vector<Angle> &angleObject_);

    //Other functions
    void Save();
    void drawGL();

    //Para que herede de GLObject

    string getClassName() {
    };

    void writeToStream(Stream& stream) {
    };

    void readFromStream(Stream& stream) {
    };

private:
    //Metodos
    void Object();

    //Pose
    Vector3D pos;
    double roll, pitch, yaw;
    //Laser2D
    vector<Vector2D> pointLaser;
    vector<double> range;
    vector<Angle> anglelaser;
    double startangle;
    double steplaser;
    //LaserKinect
    vector<Vector2D> pointKinect2D;
    vector<double> rangeKinect2D;
    vector<Angle> angleKinect2D;
    vector <Angle> angleObject;
    //Objetos
    vector<int> indexObject;
    vector <Vector2D> pointsObject;
    vector<double>rangeObject;



};

#endif	/* VISION2D_H */

