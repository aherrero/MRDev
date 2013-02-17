#include "kinectReal.h"
#include <math.h>
#include <stdlib.h>

//constructors

KinectReal::KinectReal(void) {
    //freenect_sync_stop();
}

KinectReal::~KinectReal(void) {

}

bool KinectReal::getData(mr::PointCloud& d) {
    m.Lock();
    //updateSensorData();
    short *result = 0;
    vector <mr::Vector3D> depth;
    //pcl::PointCloud<pcl::PointXYZ> depth;
    int width = 640;
    int height = 480;
//    depth.width = width;
//    depth.height = height;
    depth.resize(width*height);
    
    uint32_t ts;
    if (freenect_sync_get_depth((void**) &result, &ts, 0, FREENECT_DEPTH_11BIT) < 0)
        printf("Hey, Kinect is not connected\n");

    else//transform collected data from kinect to cm 
    {
        for (int j = 0; j < height - 20; j++)
            for (int i = 0; i < width; i++) {
                int ind = j * width + i;
                depth[ind].x = (0.1236 * tan((result[ind] / 2842.5) + 1.1863));
                depth[ind].y = (i - width / 2)*0.001602 * depth[ind].x; /// 0.001602=1.025/640 from: http://www.google.es/url?sa=t&rct=j&q=view-dependent%203d%20projection%20using%20depth-image-based%20head%20tracking&source=web&cd=1&ved=0CCAQFjAA&url=http%3A%2F%2Fwww.cs.ubc.ca%2Flabs%2Fimager%2FPROCAMS2011%2F0008.pdf&ei=vCqjTvSyIYi3hQerrOmaDA&usg=AFQjCNHL8nvvxEbONsTlFjw-5NbnIL0DpQ&sig2=4NSb91eQt0szqNXrwsXuvA&cad=rja
                depth[ind].z = -(j - height / 2)*0.001602 * depth[ind].x;
            }
    }
    Update(depth, width, height);
    d = data;

    m.Unlock();
    return true;
}

void KinectReal::Update(vector <mr::Vector3D> depth, int width, int height) {
    
    //Convertir a PointCloud
    data.width = width;
    data.height = height;
    data.points.resize(data.width * data.height);
    for (int j = 0; j <  data.height - 20; j++) {
        for (int i = 0; i < data.width; i++) {
            int ind = j * data.width + i;
            if (depth[ind].x > 0.0f) {

                data.points[ind].x = depth[ind].x;
                data.points[ind].y = depth[ind].y;
                data.points[ind].z = depth[ind].z;

                //it works with cloud.points[ind].x;
                //Eigen::Map<const Eigen::Vector3f> aux;
                //aux=cloud.points[ind].getVector3fMap()
            }
        }
    }
}
