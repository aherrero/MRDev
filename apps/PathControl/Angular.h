/* 
 * File:   Angular.h
 * Author: alex
 *
 * Created on 14 de febrero de 2012, 19:16
 */

#ifndef ANGULAR_H
#define	ANGULAR_H

#include "mrcore/mrcore.h"
#include "Control.h"
#include <GL/glut.h>
#include <iostream>

class Angular:public Control {
public:
    Angular();
    Angular(const Angular& orig);
    virtual ~Angular();
    
private:
    bool ControlAngular();
    void ComputeControl();
    
    void Save();
    
    float kpg; //ganancia P del angular
    
};

#endif	/* ANGULAR_H */

