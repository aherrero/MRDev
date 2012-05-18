#include "mrcore/mrcore.h"

#include <iostream>
#include "glutapp.h"

#include "PathControl/ADSK.h"
#include "PathControl/AngDistToSeg.h"
#include "PathControl/Angular.h"
#include "PathControl/CalculoError.h"
#include "PathControl/Control.h"
#include "ReactiveControl/CinematicMap.h"
#include "ReactiveControl/ReactiveControl.h"

using namespace mr;
using namespace std;
string pathinput;

class MyGlutApp : public GlutApp
{
public:

    MyGlutApp(string name, MobileRobot* r) : GlutApp(name), robot(r)
    {
        world += robot;
        scene.addObject(&world);
        scene.SetViewPoint(35, 160, 25);
        va = vg = 0;

        //Leer archivo trayectoria
        std::ifstream file(pathinput.c_str());
        if (!file.is_open())
        {
            printf("File not found!!\n");
        }
        vector<Vector2D> path;
        path.clear();
        int numpath;
        file >> numpath;
        path.resize(numpath);
        for (int i = 0; i < numpath; i++)
        {
            file >> path[i].x >> path[i].y;
        }
        file.close();

        //Escribir trayectoria
        vector<Vector3D> auxpath; //Hasta que SetTray de Control reciba vectores2d
        auxpath.clear();
        auxpath.resize(path.size());
        for (int i = 0; i < path.size(); i++)
        {
            auxpath[i].x = path[i].x;
            auxpath[i].y = path[i].y;
            auxpath[i].z = 0.0;
        }

        //Crea el control
        controlboth = new ADSK();
        //Mandar la trayectoria de entrada
        controlboth->SetTray(auxpath);
        STOP = false;
    }

    void Draw(void)
    {
        
        scene.Draw();
        scene.BackgroundColor(0.9,0.9,0.9);

        controlboth->drawGL();
        cinematicmap.drawGL();
        reactivecontrol.Draw();
    }

    void Timer(float time)
    {

        Odometry odom;
        LaserData laserData;

        bool laser_on = true;

        robot->getOdometry(odom);
        if (laser_on) robot->getLaserData(laserData);

        //The odometry is full 3D, lets handle it only in 2D, as a Pose (x, y, theta)
        //Transformation3D pose=odom.pose;
        //double roll,pitch,yaw;
        //pose.orientation.getRPY(roll,pitch,yaw);
        //Pose2D robotPose(pose.position.x,pose.position.y,yaw);

        /************CONTROL TRAYECTORIA***************/
        //float va3,vg3;
        controlboth->SetPose(odom);
        controlboth->GetVel(va, vg);

        /************CONTROL REACTIVO***************/ //ARREGLAR CLASE VISION2D!!

        if (laser_on)
        {
            cinematicmap.SetPose(odom);
            cinematicmap.SetLaser(laserData);
            
            reactivecontrol.SetObstacle(cinematicmap);
            reactivecontrol.SetCommand(va, vg,
                    controlboth->GetDist2traj(),controlboth->GetSideOfPath());
        }

        float va2 = va, vg2 = vg;
        if (laser_on) reactivecontrol.GetCommand(va2, vg2);

        if (STOP) va2 = vg2 = 0.0f;

        /************ACTUACION ROBOT***************/
        //cout << "velavance: " << va2 << " velgiro: " << vg2 << endl;
        robot->move(va2, vg2);
    }

    void Key(unsigned char key)
    {
        if (key == 'a')
            vg += 0.01;
        else if (key == 'd')
            vg -= 0.01;
        else if (key == 's')
            va -= 0.01;
        else if (key == 'w')
            va += 0.01;
        else if (key == ' ')
            STOP = !STOP;
        else
        {
            va = vg = 0;
        }
    }

    void MouseMove(int x, int y)
    {
        scene.MouseMove(x, y);
        glutPostRedisplay();
    }

    void MouseClick(int b, int state, int x, int y)
    {
        bool down = (state == GLUT_DOWN);
        int button;
        if (b == GLUT_LEFT_BUTTON)
            button = MOUSE_LEFT_BUTTON;
        if (b == GLUT_RIGHT_BUTTON)
            button = MOUSE_RIGHT_BUTTON;

        int specialKey = glutGetModifiers();
        bool ctrlKey = (specialKey & GLUT_ACTIVE_CTRL) ? true : false;
        bool sKey = specialKey&GLUT_ACTIVE_SHIFT;

        scene.MouseButton(x, y, b, down, sKey, ctrlKey);
        glutPostRedisplay();
    }
private:
    float vg, va;
    GLScene scene;
    World world;
    MobileRobot* robot;

    //Control
    Control *controlangular;
    Control *controldistancia;
    Control *controlboth;
    CalculoError calculoerror;
    CinematicMap cinematicmap;
    ReactiveControl reactivecontrol;
    bool STOP;
};

void printUsage();

int main(int argc, char* argv[])
{
    if (argc == 2)
        pathinput = string(argv[1]);
    else
    {
        cout << "You have not specify a configuration file as command line parameter" << endl;
        cout << "Please type configuration file: ";
        string pathinput;
        cin >> pathinput;
    }
    cout << "Loading configuration file: " << pathinput << endl;

    MobileRobot* robot = new Neo();
    robot->connectClients("127.0.0.1", 13000); //Simulation
    //robot->connectClients("192.168.100.50",12000);        //Real          
    MyGlutApp myApp("teleop", robot);

    //Bucle
    myApp.Run();
    return 0;
}

void printUsage()
{
    cout << "-------- Usage -----------------" << endl;
    cout << "> teleop config.txt    " << endl;
    cout << "example:    " << endl;
    cout << "> teleop neo 127.0.0.1 13000    " << endl;
}
