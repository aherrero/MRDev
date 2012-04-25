/* 
 * File:   ADSK.h
 * Author: alex
 *
 * Created on 16 de febrero de 2012, 19:50
 */

#ifndef ADSK_H
#define	ADSK_H

#include "AngDistToSeg.h"

class ADSK: public Control {
public:
    ADSK();
    ADSK(const ADSK& orig);
    virtual ~ADSK();
    
private:
    bool ControlAngular();
    bool ControlDistToSeg();
    bool ControlAnticipativo();
    
    void ComputeControl();
    
    float kpg; //ganancia P del angular
    float kpd; //ganancia P del control de distancia
    float kadsk; //ganancia P del actuador de velavance que reduce
                //la vel dependiendo si esta cerca del objetivo
    vector<Vector3D> distEndAcum;
    bool controladskON;
    void Save();
};

#endif	/* ADSK_H */

