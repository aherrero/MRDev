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
#include "configControlDefine.h"

using namespace mr;
using namespace std;
string pathinput;

class MyGlutApp : public GlutApp
{
public:

    MyGlutApp(string name, MobileRobot* r) : GlutApp(name), robot(r) {
        world += robot;
        scene.addObject(&world);
        scene.SetViewPoint(35, 160, 25);
        va = vg = 0;

        //Create the robot
        controlboth = new ADSK(K_TRAJECTORY_ROT, K_TRAJECTORY_ADV, K_TRAJECTORY_DIST);

        //Read path file
        vector<mr::Vector2D> path;
        path = gf::ReadPathFile(pathinput);

        //Send input path
        controlboth->SetTray(path);

        //control variable
        STOP = false;
        remoteControl = false;

    }

    void Draw(void) {

        scene.Draw();
        scene.BackgroundColor(0.9, 0.9, 0.9);

        controlboth->drawGL(); //draw trajectory
        cinematicmap.drawGL(); //draw obstacles points
        reactivecontrol.Draw(); //draw points of dangerous obstacles

        //**INFORMATION OF SPEED AND POSITION

        char mens[50];
        sprintf(mens, "VelAvance: %.3f VelGiro: %.3f",
                va2, vg2);
        //mr::GLTools::Print(mens,10,10,0);
        gf::Texto2D(mens, 10, 10, 255, 0, 0);

        char mens2[50];
        Odometry auxodom;
        double roll, pitch, yaw;
        robot->getOdometry(auxodom);
        auxodom.pose.orientation.getRPY(roll, pitch, yaw);
        sprintf(mens2, "Posicion X:%.3f Y:%.3f Yaw:%.3f", auxodom.pose.position.x,
                auxodom.pose.position.y, yaw);
        gf::Texto2D(mens2, 10, 30, 100, 255, 0);

    }

    void Timer(float time) {
        Odometry odom;
        LaserData laserData;

        robot->getOdometry(odom);
        robot->getLaserData(laserData);

        if (!remoteControl)
        {
            /***********PATH CONTROL***************/
            controlboth->SetVelLimit(MAX_SPEED_ROTATION, MAX_SPEED_FORWARD, MIN_SPEED_FORWARD);

            controlboth->SetPose(odom);
            controlboth->GetVel(va, vg);

            /************REACTIVE CONTROL***************/
            cinematicmap.setDistance(DISTANCE_MAX_OBSTACLE);
            cinematicmap.SetPose(odom);
            cinematicmap.SetLaser(laserData);

            reactivecontrol.RangeAction(RANGE_MAX_ACTION_FRONT, RANGE_MIN_ACTION_FRONT, RANGE_ACTION_LATERAL);
            reactivecontrol.ConfigReactiveControl(K_REACTIVE_ADV, K_REACTIVE_ROT,
                    MAX_SPEED_FORWARD, MAX_SPEED_ROTATION);

            reactivecontrol.SetObstacle(cinematicmap);
            reactivecontrol.SetCommand(va, vg, controlboth->GetDist2traj());

            va2 = va;
            vg2 = vg;
            reactivecontrol.GetCommand(va2, vg2);
        }
        else
        {
            //Remote control with ADWS
            va2 = vta;
            vg2 = vtg;
        }

        /************ROBOT OPERATOR***************/
        if (!STOP)
        {
            robot->move(va2, vg2);
        }
        else
        {
            va2 = vg2 = vta = vtg = 0.0;
            robot->move(0.0, 0.0);
        }
    }

    void Key(unsigned char key) {
        if (key == ' ')
            STOP = !STOP;
        else if (key == 't' || key == 'T')
            remoteControl = !remoteControl;

        else if (key == 'a' || key == 'A')
            vtg += 0.01;
        else if (key == 'd' || key == 'D')
            vtg -= 0.01;
        else if (key == 's' || key == 'S')
            vta -= 0.01;
        else if (key == 'w' || key == 'W')
            vta += 0.01;
        else
            vta = vtg = 0;
    }

    void MouseMove(int x, int y) {
        scene.MouseMove(x, y);
        glutPostRedisplay();
    }

    void MouseClick(int b, int state, int x, int y) {
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
    float vg, va; //Speed path control
    float vg2, va2; //Speed reactive control
    float vta, vtg; //Speed remote control

    GLScene scene;
    World world;
    MobileRobot* robot;

    //CONTROL:
    //Control *controlangular;
    //Control *controldistancia;
    Control *controlboth;

    CalculoError calculoerror;
    CinematicMap cinematicmap;
    ReactiveControl reactivecontrol;

    bool STOP;
    bool remoteControl;
};

void printUsage();

int main(int argc, char* argv[]) {
    //Default path introduced in argv[1]
    if (argc == 2)
        pathinput = string(argv[1]);
    else
    {
        cout << "You have not specify a configuration file as command line parameter" << endl;
        cout << "Please type configuration file: ";
        cin >> pathinput;
    }
    cout << "Loading configuration file: " << pathinput << endl;

    //Creation of a robot and connection
    MobileRobot* robot = new Neo();
    robot->connectClients("127.0.0.1", 13000); //Simulation
    //robot->connectClients("192.168.100.50",13000);        //Real          
    MyGlutApp myApp("teleop", robot);

    //Loop
    myApp.Run();
    return 0;
}

void printUsage() {
    cout << "-------- Usage -----------------" << endl;
    cout << "> teleop config.txt    " << endl;
    cout << "example:    " << endl;
    cout << "> teleop neo 127.0.0.1 13000    " << endl;
}
