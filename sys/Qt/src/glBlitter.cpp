/* gnuboy4cute
 *
 * Copyright (C) 2011 Summeli <summeli@summeli.fi>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "glBlitter.h"

#include <QPainter>
#include <QPaintEngine>
#include <math.h>

#include "cuteDebug.h"

extern unsigned short *BaseAddress;

float vertexCoords[] =
{
    -1, 1,
    -1, -1,
    1, 1,
    1, -1
};
/*
float texCoords[] =
{
    0.0, 0.0,
    0.0, 1.0,
    1.0, 0.0,
    1.0, 1.0
};*/

float texCoords[] =
{
    0.0, 0.0,
    0.0, 0.54,
    0.62, 0.0,
    0.62, 0.54
};

GLushort indices[] = { 0, 1, 2, 1, 2, 3 };

glBlitter::glBlitter(QWidget *parent)
: QGLWidget(parent)
	{
  //  setAttribute(Qt::WA_PaintOnScreen);
    //setAutoBufferSwap(false);
	// setMinimumSize(640, 360);
	setMinimumSize(400, 360);
     showFullScreen();
	}

glBlitter::~glBlitter()
	{
	// TODO Auto-generated destructor stub
	}

#define INDEX 0 //ARRAY

void glBlitter::checkError()
{
    /* Check for error conditions. */
    GLenum gl_error = glGetError( );
    if( gl_error != GL_NO_ERROR ) {
        __DEBUG2(" OpenGL error: ", gl_error ); 
    }
}

void glBlitter::initializeGL ()
{
    GLuint fragmentShader;
    GLuint vertexShader;
    GLint linked;

    glDepthFunc(GL_ALWAYS);
    checkError();
    glDisable(GL_DEPTH_TEST);
    checkError();
    glDisable(GL_STENCIL_TEST);
    checkError();
    glDisable(GL_CULL_FACE);
    checkError();
   

    QString srcVertShader =  
        "attribute vec4 a_position;   \n"
        "attribute vec2 a_texCoord;   \n"
        "varying vec2 v_texCoord;     \n"
        "void main()                  \n"
        "{                            \n"
        "   gl_Position = a_position; \n"
        "   v_texCoord = a_texCoord;  \n"
        "}                            \n";

    QString srcFragShader =  
        "precision mediump float;                            \n"
        "varying vec2 v_texCoord;                            \n"
        "uniform sampler2D s_texture;                        \n"
        "void main()                                         \n"
        "{                                                   \n"
        "  gl_FragColor = texture2D( s_texture, v_texCoord );\n"
        "}                                                   \n";

    // Create the program object
    m_program = glCreateProgram();
    if(!m_program)
      return;

    // Load the shaders
    vertexShader = loadShader(qPrintable(srcVertShader), GL_VERTEX_SHADER);
    fragmentShader = loadShader(qPrintable(srcFragShader), GL_FRAGMENT_SHADER);
    
    glAttachShader(m_program, vertexShader);
    glAttachShader(m_program, fragmentShader);
    		
    // Link the program
    glLinkProgram(m_program);

    // Check the link status
    glGetProgramiv(m_program, GL_LINK_STATUS, &linked);
    if(!linked){
      GLint infoLen = 0;
      glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &infoLen);
      if(infoLen > 1){
        char* infoLog = (char*)malloc(sizeof(char) * infoLen);
        glGetProgramInfoLog(m_program, infoLen, NULL, infoLog);
        qDebug() << infoLog;
        free(infoLog);
      }
      glDeleteProgram(m_program);
      return;
    }
    
    m_posLoc = glGetAttribLocation ( m_program, "a_position" );
    m_texLoc = glGetAttribLocation ( m_program, "a_texCoord" );
    
    // Get the sampler location
    m_samplerLoc = glGetUniformLocation ( m_program, "s_texture" );
    
    InitTextures();
    
    glClearColor(.3,.4,.6,1);

}

void glBlitter::paintGL()
{

   glClear( GL_COLOR_BUFFER_BIT );
   
   // Use the program object
   glUseProgram ( m_program );
   checkError();

   glVertexAttribPointer( m_posLoc, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), vertexCoords );
   checkError();
   glVertexAttribPointer( m_texLoc, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), texCoords );

   checkError();

   glEnableVertexAttribArray( m_posLoc );
   checkError();
   glEnableVertexAttribArray( m_texLoc );
   checkError();

   glBindTexture(GL_TEXTURE_2D, m_textures[0] );
   
   copyScreen( m_ScreenPtr, BaseAddress );
   
   glTexSubImage2D( GL_TEXTURE_2D,0,
             0,0, 256,256,
             GL_RGB,GL_UNSIGNED_SHORT_5_6_5, m_ScreenPtr );

   checkError();

   //sampler texture unit to 0
   glUniform1i( m_samplerLoc, 0 );
   checkError();

   glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices );
 //  checkError();
}

GLuint glBlitter::loadShader(const char* src, GLenum type)
   {
   GLuint shader;
   GLint compiled;

   // Create the shader object
   shader = glCreateShader(type);
   if(!shader)
	 return 0;

   // Load the shader source
   glShaderSource(shader, 1, &src, NULL);

   // Compile the shader
   glCompileShader(shader);

   // Check the compile status
   glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
   if(!compiled){
	 GLint infoLen = 0;
	 glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
	 if(infoLen > 1){
	   char* infoLog = (char*)malloc(sizeof(char) * infoLen);
	   glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
	   qDebug() << infoLog;
	   free(infoLog);
	 }

	 glDeleteShader(shader);
	 return 0;
   }

   return shader;
   }

void glBlitter::InitTextures( )
{
   
   //Init also empty textures here
	m_ScreenPtr = (quint16*) malloc( 256*256*2);
	
   // Generate a texture object
   glGenTextures ( 1, &m_textures[0] );
   
   // Bind the texture object
   glBindTexture ( GL_TEXTURE_2D, m_textures[0] );
   
   
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
   checkError();
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
   checkError();

   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
   checkError();
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
   checkError();

   // Set the filtering mode
   glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
   glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

   glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB,
   				 256, 256, 0,
   				 GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);
   

}

void glBlitter::copyScreen( quint16* dest, quint16* src ) 
{
	quint16* a = dest;
	quint16* b = src;
	for(TInt j=0; j<144;j++)
		{
		for( TInt i=0; i<160; i++)
			{
			*a = *b;
			a++;
			b++;
			}
		a += 96;
		}
}
