/* 
 * File:   CalculoError.cpp
 * Author: alex
 * 
 * Created on 15 de febrero de 2012, 18:01
 */

#include "CalculoError.h"

CalculoError::CalculoError() {
    longitudReal = 0;
    longitudIdeal = 0;
    kpg = kpd = kadsk = 0;
    posicionAcumulada.clear();
    globaltime.clear();
    errorVariableAcumulado.clear();
    refAcumulada.clear();
    reftray.clear();

    cocienteDistancias = 0;
    currentSeg = 0;
    overshoot.clear();
    area.clear();
    cocienteAreaDist.clear();
    currSegAcum.clear();
    timeovershoot.clear();
    timeStartReference.clear();
    settlingTime.clear();
    peakTime.clear();
    timeSet.clear();

}

CalculoError::CalculoError(const CalculoError& orig) {
}

CalculoError::~CalculoError() {
}

void CalculoError::ReadFile() {
    //LeerDatos

    ifstream file("logs/data/DatosError.txt");

    if (!file.is_open()) {
        printf("File not found!!\n");
    }

    //Longitud
    file >> longitudIdeal >> longitudReal;

    //Ref
    int auxref = 0;
    file >> auxref;
    reftray.resize(auxref);
    for (int i = 0; i < reftray.size(); i++)
        file >> reftray[i].x >> reftray[i].y >> reftray[i].z;

    //Posicion del robot, tiempo transcurrido y error a la tray ideal.
    int numDatos = 0;
    file >> numDatos;

    posicionAcumulada.resize(numDatos);
    globaltime.resize(numDatos);
    refAcumulada.resize(numDatos);

    for (int j = 0; j < posicionAcumulada.size(); j++) {
        file >> posicionAcumulada[j].x >> posicionAcumulada[j].y >> posicionAcumulada[j].z >>
                globaltime[j] >>
                refAcumulada[j].x >> refAcumulada[j].y >> refAcumulada[j].z;
    }

    int auxervar = 0;
    file >> auxervar;
    errorVariableAcumulado.resize(auxervar);
    for (int i = 0; i < errorVariableAcumulado.size(); i++) {
        file >> errorVariableAcumulado[i].x >> errorVariableAcumulado[i].y;
    }


    file >> kpg >> kpd >> kadsk;

    file.close();
    cout << "Datos Leidos por CalculoError " << endl;
    currentSeg = errorVariableAcumulado.back().y;
    currSegAcum.resize(currentSeg + 1);
    for (int i = 0; i < currentSeg + 1; i++)
        currSegAcum[i] = i;

    //func
    
    Error();

}

void CalculoError::Error() {

    //cocienteDistancias
    cocienteDistancias = longitudIdeal / longitudReal;

    //Obtenemos Mp de cada segmento (Maximo de errorVariableAcum)
    overshoot.resize(currentSeg + 1);
    timeovershoot.resize(currentSeg+1);
    timeStartReference.resize(currentSeg+1);
    peakTime.resize(currentSeg+1);
    settlingTime.resize(currentSeg+1);
    timeSet.resize(currentSeg+1);
    
    for (int i = 0; i < overshoot.size(); i++){
        overshoot[i] = 0.000001;
        timeovershoot[i]=0.0001;
        timeStartReference[i]=0.00001;
        peakTime[i]=0.00001;
        settlingTime[i]=0.00001;
        timeSet[i]=0.00001;
    }
    int contVar=0;
    for (int i = 0; i < overshoot.size(); i++) //de 0 a currentSeg
    {
        for (int j = 0; j < errorVariableAcumulado.size(); j++)//Todo el errVar
        {
            if (errorVariableAcumulado[j].y == i) //Si el surrentSeg es el primero
            { 
                //Para el tiempo desde que inicia la referencia
                if(contVar==i){ 
                    timeStartReference[i]=globaltime[j];
                    contVar++;
                }
                //Cogo el maxima distancia a la tray
                if (errorVariableAcumulado[j].x > overshoot[i]){
                    overshoot[i] = errorVariableAcumulado[j].x;
                    timeovershoot[i]=globaltime[j];        //Porque error variable acumulado tiene
                                                        //el mismo num de datos que globaltime
                }
                
                //Tiempo de establecimiento
                if ((errorVariableAcumulado[j].x<0.05)&&(errorVariableAcumulado[j].x>0.03)){
                    timeSet[i]=globaltime[j];
                    //Entre el 3% y 5% estará el TS
                }
                    
                
            } //Si es el segundo (i=1) el siguiente seg..
            
        }
        
        peakTime[i]=timeovershoot[i]-timeStartReference[i];
        settlingTime[i]=timeSet[i]-timeStartReference[i];
    }

    

    //Area
    area.resize(currentSeg + 1);
    float auxarea = 0;

    for (int i = 0; i < area.size(); i++) {
        auxarea = 0; //En cada segmento reiniciamos el area
        for (int j = 0; j < errorVariableAcumulado.size(); j++) {
            if (errorVariableAcumulado[j].y == i) {
                auxarea = auxarea + errorVariableAcumulado[j].x * (globaltime[j] - globaltime[j - 1]);
            }
        }

        area[i] = auxarea;
    }

    //Cociente area y distancia
    Vector3D auxrefc;
    cocienteAreaDist.resize(currentSeg + 1);
    for (int i = 0; i < currentSeg + 1; i++) {
        auxrefc = reftray[i + 1] - reftray[i];
        cocienteAreaDist[i] = area[i] / auxrefc.module();
    }

    //Salida error al archivo
    std::ofstream file2("logs/data/Error.csv");
    file2 << "CocienteEntreDistancias[percent]" << endl;
    file2 << (int) (cocienteDistancias * 100) << endl;
    file2 << "currentSegment" << ";" << "Overshoot[mm]" <<
            ";"<<"TiempoPico[ms]"<<";"<<"TiempoEstable[ms]"<<
            ";" << "Area[mm2]" << ";" << "Area/Dist[mm]" << endl;
    for (int i = 0; i < currentSeg + 1; i++) {
        file2 << currSegAcum[i] << ";" << (int) (overshoot[i]*1000) <<
                ";"<<peakTime[i]<<";"<<settlingTime[i]<<
                ";" <<(int) area[i] << ";" << (int) (cocienteAreaDist[i]*1000) << endl;
    }

    file2.close();

}