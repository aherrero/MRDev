/* 
 * File:   globalFunctions.h
 * Author: alex
 *
 * Created on 14 de abril de 2012, 18:24
 */

#ifndef GLOBALFUNCTIONS_H
#define	GLOBALFUNCTIONS_H

#include "/usr/local/mrcore/include/mrcore/mrcore.h"

namespace gf {

    inline double distanceCtrl(const Vector3D& p, const Segment3D& s) {
        //Implementacion de distance de MRCORE modificando el valor de retorno NAN
        Vector3D aux1 = s.p2 - s.p1;
        Vector3D aux2 = p - s.p1;
        double norma = aux1*aux1;
        double esc = aux1*aux2;
        if (esc <= 0)return sqrt(aux2 * aux2);
        if (esc > norma)return (p - s.p2).module();
        double module = sqrt(norma);
        esc /= module;
        if ((aux2 * aux2 - esc * esc) >= 0) return sqrt(aux2 * aux2 - esc * esc);
        else return 0.0001; //En lugar de NAN

    };

    inline Vector2D TransformationRT2D(Vector2D input, double theta, Vector2D position) {

        //1 Rotacion Sobre eje del origen Z, 2 Traslación
        Vector2D rotation;
        Vector2D rot_translation;

        //Rotacion sobre eje Z del origen
        rotation.x = input.x * cos(theta) - input.y * sin(theta);
        rotation.y = input.x * sin(theta) + input.y * cos(theta);
        //Traslacion
        rot_translation = rotation + position;

        return rot_translation;
    };
    
//    void inline Texto2D(char *mensaje, int x, int y, float r, float g, float b)
//    {
//        glDisable (GL_LIGHTING);
//	glMatrixMode(GL_PROJECTION);
//	glPushMatrix();
//	glLoadIdentity();
//	gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT) );
//
//	glMatrixMode(GL_MODELVIEW);
//	glPushMatrix();
//	glLoadIdentity();
//
//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//	glDisable(GL_DEPTH_TEST);
//	glDisable(GL_BLEND);
//	glColor3f(r,g,b);
//	glRasterPos3f(x, glutGet(GLUT_WINDOW_HEIGHT)-18-y, 0);
//	int len = strlen (mensaje );
//
//	for (int i = 0; i < len; i++) 
//		glutBitmapCharacter (GLUT_BITMAP_HELVETICA_18, mensaje[i] );
//		
//	glMatrixMode(GL_PROJECTION);
//	glPopMatrix();
//
//	glMatrixMode(GL_MODELVIEW);
//	glPopMatrix();
//
//	glEnable( GL_DEPTH_TEST );
//    };


}

#endif	/* GLOBALFUNCTIONS_H */
