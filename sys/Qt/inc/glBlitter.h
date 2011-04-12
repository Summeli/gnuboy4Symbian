/*
 * glBlitter.h
 *
 *  Created on: Jan 30, 2011
 *      Author: Summeli
 */

#ifndef GLBLITTER_H_
#define GLBLITTER_H_

#include <QGLWidget>
#include <QtGui/qvector3d.h>
#include <QtGui/qmatrix4x4.h>
#include <QtOpenGL/qglshaderprogram.h>
#include <QTime>
#include <QVector>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

class glBlitter: public QGLWidget 
	{
Q_OBJECT

public:
	glBlitter( QWidget *parent = 0 );
	virtual ~glBlitter();
	
protected:
    void paintGL ();
    void initializeGL ();
    GLuint loadShader(const char* src, GLenum type);
    void InitTextures( );
    void checkError();
    void copyScreen( quint16* dest, quint16* src );
    
private: //data
    GLuint m_textures[2];  
    GLuint  m_program;
    
    // Attribute locations
    GLint  m_posLoc;
    GLint  m_texLoc;

    // Sampler location
    GLint m_samplerLoc;

    quint16* m_ScreenPtr;
	};

#endif /* GLBLITTER_H_ */
