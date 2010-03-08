/*
 *  ofxFFGLPlugin.cpp
 *  FFGLExample
 *
 *  Created by ensta on 2/4/10.
 *  Copyright 2010 Daniel Berio. All rights reserved.
 *
 *  http://www.enist.org
 */

#include "ofxFFGLPlugin.h"


ofFFGLPlugin::ofFFGLPlugin( ofFFGLApp * app, int minInputs, int maxInputs )
{
	// Input properties
	SetMinInputs(minInputs);
	SetMaxInputs(maxInputs);
	
	_app = app;

	initParameters();
}

ofFFGLPlugin::~ofFFGLPlugin()
{
}
	

void ofFFGLPlugin::initParameters()
{
	
	for(int i = 0; i < _app->getNumParameters(); i++ )
	{
		ofFFGLParameter * v = _app->getParameter(i);
		
		switch( v->getType() )
		{
			case PARAM_FLOAT:
			{
				SetParamInfo(i, v->getName(), FF_TYPE_STANDARD, v->getFloat());
				break;
			}
			
			case PARAM_STRING:
			{
				SetParamInfo(i,v->getName(),FF_TYPE_TEXT,v->getString());
				break;
			}
			
		}
		
	}
	
}

DWORD ofFFGLPlugin::InitGL(const FFGLViewportStruct *vp)
{
	_ofWin = new ofFFGLWindow();
	
	ofSetupOpenGL(_ofWin, vp->width, vp->height, OF_WINDOW); 
	
	ofRunApp(_app);
	
	/// TODO! 
	/// handle Resouce folder in mac, and .rc files on windoz
	//NSString* dataPath = [NSString stringWithFormat:@"%@/Contents/Resources/Data/", [[NSBundle bundleForClass:[self class]] bundlePath]];
	//ofSetDataPathRoot([dataPath cString]);
	
	
	return FF_SUCCESS;
}

DWORD ofFFGLPlugin::DeInitGL()
{
	_app->exit();
	return FF_SUCCESS;
}
	
	
void	ofFFGLPlugin::setupInputTextures(ProcessOpenGLStruct* pGL)
{
	for(int i = 0; i < pGL->numInputTextures; i++ )
	{
		// create texture here if it isnt there...
		if( ! _app->inputTextures[i] )
			_app->inputTextures[i] = new ofTexture();
			
		FFGLTextureStruct &tex = *(pGL->inputTextures[i]);
		ofTexture * ofTex = _app->inputTextures[i];
		
		// from ofQC by vade.
	//?	ofTex->texData.textureName[0] = newTextureID;
		ofTex->texData.textureID = tex.Handle;
		ofTex->texData.textureTarget = GL_TEXTURE_2D;
		ofTex->texData.width = tex.Width;
		ofTex->texData.height = tex.Height;
		ofTex->texData.bFlipTexture = true;
		ofTex->texData.tex_w = tex.Width;
		ofTex->texData.tex_h = tex.Height;
		ofTex->texData.tex_t = 1.0f;
		ofTex->texData.tex_u = 1.0f;
		ofTex->texData.glTypeInternal = GL_RGBA; 
		ofTex->texData.glType = GL_RGBA;
		ofTex->texData.bAllocated = true;
	}
	
}



DWORD	ofFFGLPlugin::ProcessOpenGL(ProcessOpenGLStruct* pGL)
{
	_ofWin->update();
	
	setupInputTextures(pGL);
	
	
	GLint vp[4];
	glGetIntegerv(GL_VIEWPORT,vp);
	
	GLint mmode;
	glGetIntegerv(GL_MATRIX_MODE,&mmode);
	
	//push all matrices
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	// set up coordinate system based on our proxy window.
	glOrtho(0.0, _ofWin->windowSize.x, _ofWin->windowSize.y, 0.0, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	
	// draw
	_ofWin->draw();
	
	
	glPopAttrib();
	
	//reset all matrices
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	
	glMatrixMode(mmode);
	
	
	return FF_SUCCESS;
}


DWORD	ofFFGLPlugin::SetTime(double time)
{
	// todo.....
	 return FF_SUCCESS; 
}



DWORD ofFFGLPlugin::GetParameter(DWORD dwIndex)
{
	DWORD dwRet;

	ofFFGLParameter * v = _app->getParameter(dwIndex);
	
	if(!v)
	{
		return FF_FAIL;
	}
	
	switch(v->getType())
	{
		case PARAM_FLOAT:
		{
			float val = (v->getFloat() - v->getMin()) / (v->getMax()-v->getMin());
			 *((float *)(unsigned)&dwRet) = val;
			return dwRet;
		}	
		
		case PARAM_STRING:
		{
			const char * str = v->getString();
			dwRet = (DWORD)str;
			return dwRet;
		}
		
		default:
		{
			return FF_FAIL;
		}
	}
	
	return FF_FAIL;
}

DWORD ofFFGLPlugin::SetParameter(const SetParameterStruct* pParam)
{
	ofFFGLParameter * v = _app->getParameter(pParam->ParameterNumber);
	if(!v)
	{
		//debugPrint("ofFFGLPlugin::SetParameter unknown parameter %d\n",pParam->ParameterNumber);
		return FF_FAIL;
	}
	
	switch(v->getType())
	{
		case PARAM_FLOAT:
		{
			float val =  *((float *)(unsigned)&(pParam->NewParameterValue));
			
			v->setFloat( v->getMin() + val*(v->getMax()-v->getMin()) );
			_app->onParameterChanged(v);
			return FF_SUCCESS;
		}	
		
		case PARAM_STRING:
		{
			char * str = (char*)(pParam->NewParameterValue);
			v->setString(str);
			_app->onParameterChanged(v);

			return FF_SUCCESS;
		}
		
		default:
		{
			//debugPrint("ofFFGLPlugin::SetParameter  unknown parameter type for %s\n",v->getName().str);
			return FF_FAIL;
		}
	}
	
	return FF_FAIL;
}

char*	ofFFGLPlugin::GetParameterDisplay(DWORD dwIndex)
{
	ofFFGLParameter * v = _app->getParameter(dwIndex);
	
	if(!v)
	{
		printf("No param!");
		return 0;
	}
	
	switch( v->getType() )
	{
		case PARAM_FLOAT:
			sprintf(_paramDisplay,"%g",v->getFloat());
			printf("Parameter %s\n",_paramDisplay);
			return _paramDisplay;
			
		case PARAM_BOOL:
			sprintf(_paramDisplay,"%d",v->getBool());
			return _paramDisplay;
			
		case PARAM_STRING:
			return (char*)v->getString();
	}
	
	return 0;
	
}

	
