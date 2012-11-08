#include "mrcore/mrcore.h"
#include "kinectReal.h"

int main(int argc, char** argv)
{
	mrcoreInit();
        
        //KinectData kinectdata;
        KinectReal kinectreal;
	KinectServer kinectserver(&kinectreal,"kinect");
	
	kinectserver.init(13001,1,true);
	while(1)
	{
		Sleep(500);                
	}	

	//server.close();
	LOG_INFO("End of server");
	return 1;
}
