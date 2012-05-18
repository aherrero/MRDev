/* 
 * File:   Control.h
 * Author: alex
 *
 * Created on 21 de diciembre de 2011, 10:28
 */

#ifndef CONTROL_H
#define	CONTROL_H

#include "mrcore/mrcore.h"
#include <iostream>

using namespace mr;
using namespace std;

class Control : public GLObject {
    //DECLARE_MR_OBJECT(Control)
public:
    Control();
    virtual ~Control();

    //IN
    void SetTray(vector <Vector3D>);
    void SetPose(Odometry recibeOdo);
    //OUT
    void GetVel(float &, float &);
    virtual void Save();
    double GetDist2traj(){return errorVariable;};

    virtual void ComputeControl() = 0;
    void ComputeCurrentSegment();
    void dataToSave();
    

    void drawGL();
    bool GetSideOfPath(){return sideofpath;};

    //Implementar vacío los siguientes metodos para que Control derive de Object
    string getClassName() {};
    void writeToStream(Stream& stream) {};
    void readFromStream(Stream& stream) {};
    

protected:

    Vector3D pos;
    double roll, pitch, yaw;

    vector <Vector3D> reftray;
    vector <Vector3D> posicionAcumulada;
    vector <Vector3D> posicionIdealAcumulada;
    vector <Vector3D> refAcumulada;
    vector<double> giroAcumulado;
    vector<double> yawAcumulado;
    float velavance, velgiro;
    bool finTray;
    float outputGiro, outputDist, outputProp;
    float velmaxav, velmink;
    float velmax;
    float anguloideal;
    
    vector <Vector2D> errorVariableAcumulado;
    double errorVariable;

    int currentSegment;

    vector <int> globaltime; //Asociado a cada pos
    vector <float> distanciaTrayReal;
    vector <float> distanciaTrayIdeal;
    vector <float> distIdealAcum;
    Vector2D perpvectTray;
    
    double distToFinCL;
    
    bool sideofpath;

private:
    MRTime ti;
    bool runones;
    float longitudReal;
    float longitudIdeal;

};

#endif	/* CONTROL_H */

