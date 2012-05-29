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
#include "globalFunctions.h"

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
        //RobotReal
        controlboth = new ADSK(1.4,3.9,0.95);   //kpg,kpd,kadsk
        //RobotSimulado
        //controlboth = new ADSK(); 
        
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
        
        char mens[50];
        sprintf(mens,"VelAvance: %.3f VelGiro: %.3f",
                va2,vg2);
        //mr::GLTools::Print(mens,10,10,0);
        gf::Texto2D(mens,10,10,255,0,0); 
    }

    void Timer(float time)
    {

        Odometry odom;
        LaserData laserData;


        robot->getOdometry(odom);
        bool laser_on=robot->getLaserData(laserData);

        /************CONTROL TRAYECTORIA***************/
        //float va3,vg3;
        controlboth->SetVelLimit(1.8,0.2,0.05); //vgiro,vmaxav,vminav
        //Si no introducimos limites, tiene unos por defecto.
        controlboth->SetPose(odom);
        controlboth->GetVel(va, vg);

        /************CONTROL REACTIVO***************/

        if (laser_on)
        {
            cinematicmap.SetPose(odom);
            cinematicmap.SetLaser(laserData);
            
            reactivecontrol.SetObstacle(cinematicmap);
            reactivecontrol.SetCommand(va, vg,controlboth->GetSideOfPath());
        }

        va2 = va;
        vg2 = vg;
        if (laser_on) reactivecontrol.GetCommand(va2, vg2);

        if (STOP) va2 = vg2 = 0.0f;

        /************ACTUACION ROBOT***************/
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
    float vg2,va2;
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
