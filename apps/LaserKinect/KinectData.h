/* 
 * File:   KinectData.h
 * Author: Alex y Jorge
 *
 * Created on 24 de octubre de 2011, 12:47
 */

#ifndef KINECTDATA_H
#define	KINECTDATA_H
#include <vector>
#include "mrcore/mrcore.h"
#include "KinectCloud.h"
#include <pcl-1.6/pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl-1.6/pcl/io/pcd_io.h>
using namespace std;
using namespace mr;

class KinectData: public Kinect {
public:
    KinectData();
    KinectData(const KinectData& orig);
    virtual ~KinectData();
   
    Mutex m;
    KinectCloud kinect;
    PointCloud data;
    
    virtual bool getData(PointCloud& d){
		m.Lock();
                Update(kinect.GetDepth());
		d=data;
		m.Unlock();
		return true;
    }
    
    void Draw(double yaw,Vector2D pos);
    void Update(pcl::PointCloud<pcl::PointXYZ> depth, pcl::PointCloud<pcl::RGB> cam);
    void Update(pcl::PointCloud<pcl::RGB> cam);
    void Update(pcl::PointCloud<pcl::PointXYZ> depth);
    void OnKeyboardDown(unsigned char key);
    
    enum kinect_state {DEPTH,RGB,BOTH};
    kinect_state state;
private:
   // pcl::PointCloud<pcl::PointXYZRGB> cloud; //tambien se podría tener una unica nube de tipo PointXYZRGB
    pcl::PointCloud<pcl::PointXYZ> cloud;
    pcl::PointCloud<pcl::RGB> camera;
    
//    vector< Vector3D> cloud;
//    vector< Vector3D> cam;
};

#endif	/* KINECTDATA_H */

