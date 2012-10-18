/* 
 * File:   Kinect.h
 * Author: Jorge y Alex
 *
 * Created on 19 de octubre de 2011, 11:27
 */

#ifndef KINECTCLOUD_H
#define	KINECTCLOUD_H
#include <vector>
#include "mrcore/mrcore.h"
#include "libfreenect.h"
#include "libfreenect_sync.h"
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
using namespace std;
using namespace mr;

class KinectCloud {
public:
    KinectCloud();
    KinectCloud(const KinectCloud& orig);
    virtual ~KinectCloud();

    void SetServoAngle(int degree);

    /*int GetServoAngle() {
        return servoAngle;
    }*/
    
    void OnKeyboardDown(unsigned char key);
    //void SetLed(freenect_led_options led);
    //Vector3D GetAccel();
    pcl::PointCloud<pcl::PointXYZ> GetDepth();
    pcl::PointCloud<pcl::RGB> GetRGB();

    int width;
    int height;
private:

    //int servoAngle; //this angle is absolute, it is measured by the kinect's accelerometers 
    //freenect_raw_tilt_state *state;

};

#endif	/* KINECTCLOUD_H */

