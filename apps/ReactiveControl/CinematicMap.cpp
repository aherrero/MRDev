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
}

CinematicMap::CinematicMap(const CinematicMap& orig)
{
}

CinematicMap::~CinematicMap()
{
}

void CinematicMap::setDistance(float distanceObstacle)
{
    distMaxObstacle=distanceObstacle;
}

void CinematicMap::SetPose(Odometry odom){
    pos = odom.pose.position;
    odom.pose.orientation.getRPY(roll, pitch, yaw);
}

void CinematicMap::SetLaser(LaserData laserdata)
{
    vector<Vector2D> points2D=laserdata.getPoints();
    double zpos=0.4f;   //high laser2D
    pointsCloud.resize(points2D.size());
    for(int i=0;i<points2D.size();i++)
    {
        pointsCloud[i]=Vector3D(points2D[i].x,points2D[i].y,zpos);
    }
    angleCloud = laserdata.getAngles();
    rangeCloud = laserdata.getRanges();

    Obstacle();
    
}

void CinematicMap::SetLaser3D(PointCloud kinectData)
{
    //Obtain points from kinect with PCL
    
    //....
    
    //Filter with PCL
    
    //...
    
    //And then, fill pointsCloud with points
    pointsCloud=kinectData.points;
    
    rangeCloud.resize(pointsCloud.size());
    for(int i=0;i<pointsCloud.size();i++)
    {
        //Calculate the r²=x²+y² for know the range of all points
        
        double xx=pointsCloud[i].x*pointsCloud[i].x;
        double yy=pointsCloud[i].y*pointsCloud[i].y;
        double range=sqrt(xx+yy);
        rangeCloud[i]=range;
        
        //rangeCloud[i]=5.0;      //Obstacle always
        
    }
    
    angleCloud.resize(pointsCloud.size());
    for(int i=0;i<pointsCloud.size();i++)
    {
        angleCloud[i]=mr::Angle();
    }
    
    
    Obstacle();
    
    
}

void CinematicMap::Obstacle()
{
    pointsObstacle.clear();
    rangeObstacle.clear();
    angleObstacle.clear();
    
    //if(pointsObstacle[i].z<=0 && pointsObstacle[i].z<alturarobot) //obstaculo
    for (int i = 0; i < pointsCloud.size(); i++)
    {
        if(rangeCloud[i]<distMaxObstacle)               //Menor que 10
        {
            Vector2D auxpoints;
            auxpoints=gf::TransformationRT2D(Vector2D(pointsCloud[i].x,pointsCloud[i].y),
                                yaw, Vector2D(pos.x,pos.y));
            pointsObstacle.push_back(Vector3D(auxpoints.x,auxpoints.y,pointsCloud[i].z));
            rangeObstacle.push_back(rangeCloud[i]);
            angleObstacle.push_back(angleCloud[i]);
        }
    }
}

void CinematicMap::GetObstacle(vector<Vector2D>& pointsObstacle_,
        vector<double>& rangeObstacle_, vector<Angle>& angleObstacle_)
{
    pointsObstacle_.resize(pointsObstacle.size());
    for(int i=0;i<pointsObstacle.size();i++)
        pointsObstacle_[i]=Vector2D(pointsObstacle[i].x,pointsObstacle[i].y);
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
            glVertex3f(pointsObstacle[i].x, pointsObstacle[i].y, pointsObstacle[i].z);
            glEnd();

        }
        glPopMatrix();
    }
    
    if(!pointsCloud.empty())
    {
        //Laser verde central
        glPushMatrix();
        Vector2D auxcenter(pointsCloud[pointsCloud.size()/2].x,pointsCloud[pointsCloud.size()/2].y);
        Vector2D center = gf::TransformationRT2D(auxcenter, yaw, Vector2D(pos.x, pos.y));

        glLineWidth(1.0);
        glColor3ub(0, 200, 0);

        glBegin(GL_LINES);
        glVertex3f(pos.x, pos.y, pointsCloud[pointsCloud.size()/2].z); // V0
        glVertex3f(center.x, center.y, pointsCloud[pointsCloud.size()/2].z); // V1
        glEnd();
        glPopMatrix();
    }
}