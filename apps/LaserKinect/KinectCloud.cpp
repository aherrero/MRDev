/* 
 * File:   KinectCloud.cpp
 * Author: Jorge y Alex
 * 
 * Created on 19 de octubre de 2011, 11:27
 */

#include "KinectCloud.h"
#include "libfreenect.h"
#include "libfreenect_sync.h"

KinectCloud::KinectCloud()
{
    //servoAngle = 0;
    width = 640;
    height = 480;
    freenect_sync_stop();
    //SetServoAngle(0);
    //state = 0;
}

KinectCloud::KinectCloud(const KinectCloud& orig){ }

KinectCloud::~KinectCloud(){ }

pcl::PointCloud<pcl::RGB> KinectCloud::GetRGB()
{
    unsigned char *video;
    
    pcl::PointCloud<pcl::RGB> cam;
    cam.width = width;
    cam.height = height;
    cam.points.resize(cam.width * cam.height);
    //SetSevoAngle(servoAngle);
    uint32_t ts;
    if(freenect_sync_get_video((void**) &video, &ts, 0, FREENECT_VIDEO_RGB) < 0)
	printf("Hey, Kinect is not connected\n");

    else//transform collected data from kinect to cm 
    {
	for(int j = 0;j < height - 20;j++)
	    for(int i = 0;i < width;i++)
	    {
		int ind=j * width + i;
		int calibration = 3 * ((j + 20)*640 + (i + 15)*29 / 32);
		cam.points[ind].r = video[calibration];
		cam.points[ind].g = video[calibration + 1];
		cam.points[ind].b = video[calibration + 2];
	    }
    }
    return cam;
}

pcl::PointCloud<pcl::PointXYZ> KinectCloud::GetDepth()
{

    short *result = 0;
    pcl::PointCloud<pcl::PointXYZ> depth;
    depth.width = width;
    depth.height = height;
    depth.points.resize(depth.width * depth.height);
    uint32_t ts;
    if(freenect_sync_get_depth((void**) &result, &ts, 0, FREENECT_DEPTH_11BIT) < 0)
	printf("Hey, Kinect is not connected\n");

    else//transform collected data from kinect to cm 
    {
	for(int j = 0;j < height - 20;j++)
	    for(int i = 0;i < width;i++)
	    {
		int ind = j * width + i;
		depth.points[ind].x = (0.1236 * tan((result[ind] / 2842.5) + 1.1863));
		depth.points[ind].y = (i - width / 2)*0.001602 * depth.points[ind].x;/// 0.001602=1.025/640 from: http://www.google.es/url?sa=t&rct=j&q=view-dependent%203d%20projection%20using%20depth-image-based%20head%20tracking&source=web&cd=1&ved=0CCAQFjAA&url=http%3A%2F%2Fwww.cs.ubc.ca%2Flabs%2Fimager%2FPROCAMS2011%2F0008.pdf&ei=vCqjTvSyIYi3hQerrOmaDA&usg=AFQjCNHL8nvvxEbONsTlFjw-5NbnIL0DpQ&sig2=4NSb91eQt0szqNXrwsXuvA&cad=rja
		depth.points[ind].z = -(j - height / 2)*0.001602 * depth.points[ind].x;
	    }
    }
    return depth;
}

void KinectCloud::OnKeyboardDown(unsigned char key)
{
    /*
    if(key == 'o')SetServoAngle(++servoAngle);
    else if(key == 'p')SetServoAngle(--servoAngle);
    else if(key == 'i') SetServoAngle(0);
    else if(key == '0') SetLed(LED_OFF);
    else if(key == '1') SetLed(LED_GREEN);
    else if(key == '2') SetLed(LED_RED);
    else if(key == '3') SetLed(LED_YELLOW);
    else if(key == '4') SetLed(LED_BLINK_GREEN);
    else if(key == '5') SetLed(LED_BLINK_RED_YELLOW);
    else if(key == 'a') cout << "accel" << GetAccel().x << " " << GetAccel().y << " " << GetAccel().z << endl;
    */
}
/*
void KinectCloud::SetServoAngle(int degree)
{
    state = 0;
    servoAngle = degree;
    if(freenect_sync_get_tilt_state(&state, 0))
    {

	printf("Hey, Kinect is not connected or degree is not between -30 and 30\n");
    }

    if(servoAngle > 30)servoAngle = 30;
    else if(servoAngle<-30)servoAngle = -30;
    freenect_sync_set_tilt_degs(degree, 0);
}

Vector3D KinectCloud::GetAccel()
{
    Vector3D aux;
    freenect_sync_get_tilt_state(&state, servoAngle);
    freenect_get_mks_accel(state, &aux.y, &aux.z, &aux.x);
    aux.z = -aux.z;
    aux.y = -aux.y;
    return aux;
}

void KinectCloud::SetLed(freenect_led_options led)
{
    if(freenect_sync_set_led(led, 0))
	printf("Hey, Kinect is not connected\n");

}
*/