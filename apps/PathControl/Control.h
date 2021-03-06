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
    void SetTray(vector <Vector2D>);
    void SetPose(Odometry recibeOdo);
    void SetVelLimit(float vg1,float va1, float va2);
    //OUT
    void GetVel(float &, float &);
    virtual void Save();

    virtual void ComputeControl() = 0;
    void ComputeCurrentSegment();
    void dataToSave();
    

    void drawGL();

    //Implementar vacío los siguientes metodos para que Control derive de Object
    string getClassName() {};
    void writeToStream(Stream& stream) {};
    void readFromStream(Stream& stream) {};
    

protected:

    Vector3D pos;
    double roll, pitch, yaw;

    vector <Vector2D> reftray;
    vector <Vector3D> posicionAcumulada;
    vector <Vector3D> posicionIdealAcumulada;
    vector <Vector2D> refAcumulada;
    
    vector<double> giroAcumulado;
    vector<double> yawAcumulado;
    float velavance, velgiro;
    bool finTray;
    float outputGiro, outputDist, outputProp;
    float velmaxav, velminav;
    float velmaxgi;
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

private:
    MRTime ti;
    bool runones;
    float longitudReal;
    float longitudIdeal;

};

#endif	/* CONTROL_H */

