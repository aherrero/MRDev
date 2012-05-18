/* 
 * File:   CinematicMap.cpp
 * Author: alex
 * 
 * Created on 17 de mayo de 2012, 18:31
 */

#include "CinematicMap.h"
#include "../globalFunctions.h"

CinematicMap::CinematicMap()
{
    roll=0.0;
    pitch=0.0;
    yaw=0.0;
    pointsCloud.clear();
    rangeCloud.clear();
    angleCloud.clear();
    pointsObstacle.clear();
    rangeObstacle.clear();
    angleObstacle.clear();
    
    distMaxObstacle=10.0;
}

CinematicMap::CinematicMap(const CinematicMap& orig)
{
}

CinematicMap::~CinematicMap()
{
}

void CinematicMap::SetPose(Odometry odom){
    pos = odom.pose.position;
    odom.pose.orientation.getRPY(roll, pitch, yaw);
}

void CinematicMap::SetLaser(LaserData laserdata)
{
    
    pointsCloud= laserdata.getPoints();
    angleCloud = laserdata.getAngles();
    rangeCloud = laserdata.getRanges();

    Obstacle();
    
}

void CinematicMap::Obstacle()
{
    pointsObstacle.clear();
    rangeObstacle.clear();
    angleObstacle.clear();
    
    //if(pointsObstacle[i].z<0 && pointsObstacle[i].z<alturarobot) //obstaculo
    for (int i = 0; i < pointsCloud.size(); i++)
    {
        if(rangeCloud[i]<distMaxObstacle)
        {
            pointsObstacle.push_back
                    (gf::TransformationRT2D(pointsCloud[i],yaw,Vector2D(pos.x,pos.y)));
            rangeObstacle.push_back(rangeCloud[i]);
            angleObstacle.push_back(angleCloud[i]);
        }
    }
}

void CinematicMap::GetObstacle(vector<Vector2D>& pointsObstacle_,
        vector<double>& rangeObstacle_, vector<Angle>& angleObstacle_)
{
    pointsObstacle_=pointsObstacle;
    rangeObstacle_=rangeObstacle;
    angleObstacle_=angleObstacle;
}

void CinematicMap::drawGL()
{
    if(!pointsObstacle.empty())
    {
        //Puntos de los objetos (VERDE)
        glPushMatrix();
        glColor3ub(0, 200, 0);
        glPointSize(0.5);
        for (int i = 0; i < pointsObstacle.size(); i++) {
            glBegin(GL_POINTS);
            glVertex3f(pointsObstacle[i].x, pointsObstacle[i].y, 0.4);
            glEnd();
        }
        glPopMatrix();
    }
}