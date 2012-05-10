#include "mrcore/mrcore.h"


#include <iostream>
#include "glutapp.h"
//#include "reactivecontrol.h"
//#include "trajcontrol.h"
#include "PathControl/ADSK.h"
#include "PathControl/AngDistToSeg.h"
#include "PathControl/Angular.h"
#include "PathControl/CalculoError.h"
#include "PathControl/Control.h"
#include "ReactiveControl/ControlReactivo.h"
#include "ReactiveControl/Vision2D.h"

using namespace mr;
using namespace std;

class MyGlutApp: public GlutApp
{
public:
	MyGlutApp(string name,MobileRobot* r):GlutApp(name),robot(r)
	{
		world+=robot;
		scene.addObject(&world);
		scene.SetViewPoint(35,160,25);	
		va=vg=0;

		/*vector<Vector2D> path;
		path.push_back(Vector2D(0,0));
		path.push_back(Vector2D(20,0));
		path.push_back(Vector2D(20,10));
		path.push_back(Vector2D(-1,10));
		path.push_back(Vector2D(-1,1));
		traj.setPath(path);*/

                controlboth=new ADSK();
                STOP=false;
	}
	void Draw(void)
	{
		scene.Draw();
		//traj.drawGL();
		//control.drawGL();
                controlboth->drawGL();
                vision2d.drawGL();
                reactivecontrol.Draw();
	}
	void Timer(float time)
	{
		Odometry odom;
		LaserData laserData;

		robot->getOdometry(odom);
		robot->getLaserData(laserData);

		//The odometry is full 3D, lets handle it only in 2D, as a Pose (x, y, theta)
		//Transformation3D pose=odom.pose;
		//double roll,pitch,yaw;
		//pose.orientation.getRPY(roll,pitch,yaw);
		//Pose2D robotPose(pose.position.x,pose.position.y,yaw);

	//	traj.setData(robotPose);
	//	traj.getSpeed(va,vg);
                controlboth->SetPose(odom);
                controlboth->GetVel(va,vg);

	//	control.setCommand(va,vg);
	//	control.setData(laserData);
                vision2d.SetPose(odom);
                vision2d.SetLaser(laserData);             
                reactivecontrol.SetPoseVision(vision2d);
                reactivecontrol.SetCommand(va,vg);
                
		float va2=va,vg2=vg;
              	//control.getSpeed(va2,vg2);	
                
                reactivecontrol.GetVel(va2, vg2);
                if (STOP) va2 = vg2 = 0.0f;
                //Cuidado con la aceleracion
		robot->move(va2,vg2);
	}
	void Key(unsigned char key)
	{
		if(key=='a')
			vg+=0.05;
		else if(key=='d')
			vg-=0.05;
		else if(key=='s')
			va-=0.05;
		else if(key=='w')
			va+=0.05;
                else if(key==' ')
                        STOP=!STOP;
		else 
		{
			va=vg=0;
		}
	}
	void MouseMove(int x,int y)
	{
		scene.MouseMove(x,y);
		glutPostRedisplay();
	}
	void MouseClick(int b,int state, int x,int y)
	{
		bool down=(state==GLUT_DOWN);
		int button;
		if(b==GLUT_LEFT_BUTTON)
			button=MOUSE_LEFT_BUTTON;
		if(b==GLUT_RIGHT_BUTTON)
			button=MOUSE_RIGHT_BUTTON;
			
		int specialKey = glutGetModifiers();
		bool ctrlKey= (specialKey & GLUT_ACTIVE_CTRL)?true:false ;
		bool sKey= specialKey&GLUT_ACTIVE_SHIFT ;
		
		scene.MouseButton(x,y,b,down,sKey,ctrlKey);
		glutPostRedisplay();
	}
        Control *controlboth; //De momento, para que el path input pueda cogerlo
private:
	float vg,va;
	GLScene scene;
	World world;
	MobileRobot* robot;
	//ReactiveControl control;
	//TrajControl traj;
        
        //Control
        Control *controlangular;
        Control *controldistancia;
        CalculoError calculoerror;
        Vision2D vision2d;
        ControlReactivo reactivecontrol;
        bool STOP;
};

void printUsage();

int main(int argc,char* argv[])
{
/*	if(argc!=2)
	{
		printUsage();
		return -1;
	}
	string configFile(argv[1]);

	int port=-1;
	if(robotname=="nemo")
		robot=new Nemo;*/
    
        string text;
	if(argc==2)
		text=string(argv[1]);
	else
	{
		cout<<"You have not specify a configuration file as command line parameter"<<endl;
		cout<<"Please type configuration file: ";
		string text;
		cin>>text;
	}
	cout<<"Loading configuration file: "<<text<<endl;
        
	MobileRobot* robot=new Neo();
	robot->connectClients("127.0.0.1",13000);
	MyGlutApp myApp("teleop",robot);
        
        //Leer archivo
	std::ifstream file(text.c_str());
        if (!file.is_open()) {
            printf("File not found!!\n");
        }
        vector<Vector2D> path;
        path.clear();
        int numpath;
        file>>numpath;
        path.resize(numpath);
        for (int i = 0; i < numpath; i++) {
            file>>path[i].x>>path[i].y;
        }
        file.close();
        
        //Escribir trayectoria
        vector<Vector3D> auxpath;       //Hasta que SetTray reciba vectores2d
        auxpath.clear();
        auxpath.resize(path.size());
        for (int i = 0; i < path.size(); i++) {
            auxpath[i].x=path[i].x;
            auxpath[i].y=path[i].y;
            auxpath[i].z=0.0;
        }

        myApp.controlboth->SetTray(auxpath);
        
        
	myApp.Run();
	return 0;   
}

void printUsage()
{
	cout<<"-------- Usage -----------------"<<endl;
	cout<<"> teleop config.txt    "<<endl;
	cout<<"example:    "<<endl;
	cout<<"> teleop neo 127.0.0.1 13000    "<<endl;
}
