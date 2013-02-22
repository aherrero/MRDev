/* 
 * File:   CinematicMap.h
 * Author: alex
 *
 * Created on 17 de mayo de 2012, 18:31
 */

#ifndef CINEMATICMAP_H
#define	CINEMATICMAP_H

//#include "mrcore/mrcore.h" //It works, but netbeans not reconize the functions
#include "/usr/local/mrcore/include/mrcore/mrcore.h"
#include <iostream>

//#include <pcl-1.6/pcl/point_types.h>
//#include <pcl/io/pcd_io.h>
//#include <pcl-1.6/pcl/io/pcd_io.h>

class CinematicMap : public GLObject {
public:
    CinematicMap();
    CinematicMap(const CinematicMap& orig);
    virtual ~CinematicMap();
    
    //Func of GLOBject
    void drawGL();
    string getClassName() {};
    void writeToStream(Stream& stream) {};
    void readFromStream(Stream& stream) {};
    
    //FuncCinematicMap
    void setDistance(float distanceObstacle);
    void SetPose(Odometry odom);
    void SetLaser(LaserData laserdata);
    void SetLaser3D(PointCloud kinectData);
    void GetObstacle(vector<Vector2D> &pointsObstacle_,
            vector<double> &rangeObstacle_,vector<Angle> &angleObstacle_);
        
private:
    Vector3D pos;
    double roll,pitch,yaw;
    float distMaxObstacle;
    
    //Cloud points
    vector<Vector3D> pointsCloud;
    vector<double> rangeCloud;
    vector<Angle> angleCloud;
    
    //Obstacle
    void Obstacle();
    vector<Vector3D> pointsObstacle;
    vector<double> rangeObstacle;
    vector<Angle> angleObstacle;

};

#endif	/* CINEMATICMAP_H */

