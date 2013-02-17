#ifndef __MRCORE__kinectReal__H
#define __MRCORE__kinectReal__H

#include "mrcore/mrcore.h"
#include "libfreenect.h"
#include "libfreenect_sync.h"
#include <vector>

//#include <pcl-1.6/pcl/point_types.h>
//#include <pcl/io/pcd_io.h>
//#include <pcl-1.6/pcl/io/pcd_io.h>


using namespace std;

class KinectReal : public Kinect {
    
protected:
    Mutex m;

public:
    //attributes

    //pcl::PointCloud<pcl::PointXYZ> cloud;
    mr::PointCloud data;

    //constructors

    //Basic constructor
    KinectReal(void);

    //Destructor
    virtual ~KinectReal(void);

    //methods

    bool getData(mr::PointCloud& d);
    //void Update(pcl::PointCloud<pcl::PointXYZ> depth);
    void Update(vector <mr::Vector3D> depth, int width, int height);
};

#endif
