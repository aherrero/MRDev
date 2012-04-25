/* 
 * File:   AngDistToSeg.h
 * Author: alex
 *
 * Created on 14 de febrero de 2012, 19:18
 */

#ifndef ANGDISTTOSEG_H
#define	ANGDISTTOSEG_H

#include "/usr/local/mrcore/include/mrcore/mrcore.h"
#include <GL/glut.h>
#include <iostream>
#include "Control.h"

class AngDistToSeg:public Control {
public:
    AngDistToSeg();
    AngDistToSeg(const AngDistToSeg& orig);
    virtual ~AngDistToSeg();
    
private:
    bool ControlAngular();
    bool ControlDistToSeg();
    
    void ComputeControl();
    void Save();
    
    float kpg; //ganancia P del angular
    float kpd; //ganancia P del control de distancia
};

#endif	/* ANGDISTTOSEG_H */

