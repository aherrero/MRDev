/* 
 * File:   CalculoError.h
 * Author: alex
 *
 * Created on 15 de febrero de 2012, 18:01
 */

#ifndef CALCULOERROR_H
#define	CALCULOERROR_H

#include "mrcore/mrcore.h"
#include "Control.h"
#include <GL/glut.h>
#include <iostream>

class CalculoError {
public:
    CalculoError();
    CalculoError(const CalculoError& orig);
    virtual ~CalculoError();
    
    void ReadFile();
    void Error();
private:
    float longitudReal;
    float longitudIdeal;
    float kpg;   //ganancia P del angular
    float kpd;   //ganancia P del control de distancia
    float kadsk; //ganancia P del actuador de velavance que reduce
    vector <Vector3D> posicionAcumulada;
    vector <int> globaltime;    //Asociado a cada pos
    vector <Vector2D> errorVariableAcumulado;
    vector <Vector3D> refAcumulada;
    vector <Vector3D> reftray;
    
    //Errores calculados en la propia clase
    double cocienteDistancias;
    int currentSeg;
    vector <int> currSegAcum;
    vector <float> overshoot;
    vector <float> timeStartReference;
    
    vector <float> timeovershoot;
    vector <float> peakTime;
    
    vector <float> timeSet;
    vector <float> settlingTime;
    
    vector <float> area;
    vector <float> cocienteAreaDist;

};

#endif	/* CALCULOERROR_H */

