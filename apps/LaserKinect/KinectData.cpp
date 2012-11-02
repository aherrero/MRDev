/* 
 * File:   KinectData.cpp
 * Author: Alex y Jorge
 * 
 * Created on 24 de octubre de 2011, 12:47
 */

#include "KinectData.h"
#include <math.h>
#include <stdio.h>
#include <GL/glut.h>
#include "../globalFunctions.h"

KinectData::KinectData()
{
    state = BOTH;
    // Fill in the cloud data
    cloud.width = 640;
    cloud.height = 480;
    cloud.points.resize(cloud.width * cloud.height);
}

KinectData::KinectData(const KinectData& orig){ }

KinectData::~KinectData(){ }

void KinectData::Draw(double yaw,Vector2D pos)
{
    /*Draw Kinect*/
    glColor3f(0.0f, 0.0f, 1.0f);
    glScalef(1, 5.0f, 1);
    glutSolidCube(0.05f);
    glScalef(1, .20f, 1);
    glDisable(GL_LIGHTING);

    /*Draw point cloud*/
    for(int j = 0;j < cloud.height - 20;j++){
	for(int i = 0;i < cloud.width;i++)
	{
	    int ind = j * cloud.width + i;
	    if(cloud.points[ind].x > 0.0f || state == RGB)
	    {
		if(state == DEPTH)
		    glColor3f(0.5f, 0.5f, 0.0f);
		else
		    glColor3f(camera.points[ind].r, camera.points[ind].g, camera.points[ind].b);

		glPointSize(0.8f);
		glBegin(GL_POINTS);
		if(state == DEPTH || state == BOTH){
                    Vector2D pointsTransf=gf::TransformationRT2D(Vector2D(cloud.points[ind].x,cloud.points[ind].y), yaw, pos);
		    glVertex3d(pointsTransf.x, pointsTransf.y, cloud.points[ind].z);
                }
		else
		    glVertex3d(i / 90.0f - cloud.width / 180.0f, 3.0f, -j / 90.0f + cloud.height / 180.0f);
	    }
	}
    }
    glEnd();
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

void KinectData::Update(pcl::PointCloud<pcl::PointXYZ> depth)
{
    cloud.width = depth.width;
    cloud.height = depth.height;
    cloud.points.resize(cloud.width * cloud.height);
    for(int i = 0;i < depth.size();i++)
    {
	cloud.points[i].x = depth.points[i].x;
	cloud.points[i].y = depth.points[i].y;
	cloud.points[i].z = depth.points[i].z;
    }
    state = DEPTH;
}
void KinectData::Update(pcl::PointCloud<pcl::RGB> cam)
{
    camera.width = cam.width;
    camera.height = cam.height;
    camera.points.resize(cloud.width * cloud.height);
    for(int i = 0;i < cam.size();i++)
    {
	camera.points[i].r = cam.points[i].r;
	camera.points[i].g = cam.points[i].g;
	camera.points[i].b = cam.points[i].b;
    }
    state = RGB;
}

void KinectData::Update(pcl::PointCloud<pcl::PointXYZ> depth, pcl::PointCloud<pcl::RGB> cam)
{
    if(depth.size()!= cam.size())
    {
	cout<<"error getting data from kinect!"<<endl;
	return;
    }
    cloud.width = depth.width;
    cloud.height = depth.height;
    cloud.points.resize(cloud.width * cloud.height);
    camera.width = cam.width;
    camera.height = cam.height;
    camera.points.resize(cloud.width * cloud.height);
    
    for(int i = 0;i < depth.size();i++)
    {
	cloud.points[i].x = depth.points[i].x;
	cloud.points[i].y = depth.points[i].y;
	cloud.points[i].z = depth.points[i].z;
	camera.points[i].r=cam.points[i].r;
	camera.points[i].g=cam.points[i].g;
	camera.points[i].b=cam.points[i].b;
    }
}

void KinectData::OnKeyboardDown(unsigned char key)
{
    if(key == 'k')
    {
	if(state == DEPTH) state = BOTH;
	else if(state == BOTH) state = RGB;
	else if(state == RGB) state = DEPTH;
    }
}