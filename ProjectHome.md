### Details ###

This is a openFrameworks addon for creating FFGL (FreeFrameGL 1.5) plugins,
and to easily convert openFrameworks apps to FFGL plugins.


### To create your own ofFFGLPlugin project: ###

Download and unzip openFrameworks v0.061 for Visual Studio or Xcode
http://www.openframeworks.cc/download

Download and unzip ofxFFGLPluginAddon
http://code.google.com/p/ofxffglplugin/downloads/list

Copy the /addons/ofxFFGLPlugin directory to the /addons directory of openFrameworks.

Copy the /apps/addonsExamples to the /apps/addonsExamples of openFrameworks.

Open the project file for one of the examples.

In main.cpp:
  * In the DEFINE\_FFGL\_PLUGIN macro
> change testApp with your App class name, if it is different.
> And set min and max input textures.

  * Customize the CFFGLPluginInfo struct:
```
static CFFGLPluginInfo PluginInfo (
	createPluginInstance,	// * Default creation method ( created with the DEFINE_FFGL_PLUGIN macro )
	
	"OFEX",			// Plugin unique ID. This must be different for every plugin and 4 characters long.
	"OFTest1",		// Plugin name
	
	1,			// * API major version number
	500,			// * API minor version number
	1,			// Plugin major version number
	000,			// Plugin minor version number
	
	FF_SOURCE,		// Plugin type FF_SOURCE or FF_EFFECT ( filters ) 
	"Test OpenFrameworks Plugin",		// Plugin description
	"by Daniel Berio - www.enist.org"	// About
	
	,0					// *
	,NULL					// *
);
```

_It is important that every plugin has a 4 character identifier. This must be different for every FFGL plugin._


The App class MUST be derived from ofFFGLApp.
If you want to convert an existing openFrameworks app to FFGL, simply change the derivation from ofBaseApp to ofFFGLApp.


To add custom parameters go to the constructor of the class, and use the custom functions in ofFFGLApp:

```
addFloatParameter( const char * name, float * address, float min = 0.0f, float max = 1.0f );
addBoolParameter( const char * name, bool * address );
addStringParameter( const char * name, char * address );
```

example:

```
	addFloatParameter("test float", &myFloat, 0.0f, 100.0f );
```

the variable myFloat MUST be declared in the class, since we are passing in it's address.

At this point any variable linked with the plugin will be automatically modified by the host.



If you want to do something when a parameter change, you can override the function:

```
virtual void onParameterChanged( ofFFGLParameter * param )
```

you can test the name or the address of the parameter to know which one has changed.

### Installing ###

When compiled the plugin file will generate a **dll** file on Windows and a **bundle** file on Mac, in the _bin_ directory of the project.
To install, copy the file to you host's plugin directory.

### Hosts ###
For an FFGL host, check out Resolume  http://www.resolume.com
In the Video Preferences you can specify the directories where resolume searches for plugins at startup.

For a openFrameworks FreeFrameGL host check out
http://github.com/prisonerjohn/openFrameworks/tree/master/addons/ofxFreeFrame/



## Issues ##

_At the moment XCode 3.0 and Visual Studio Express 2008 projects are present._

To get the Visual Studio Express version to compile it was necessary to make a slight modification to the code in FFGL ( _FFGLPluginInfo.h_ )
so the version included with this addon is not the official FFGL version.

The windows version of openFrameworks needs to be linked with some dll's that will be automatically copyed in the bin folder after compilation.
To get the plugin to work these .dll's must be copyed either into the SYSTEM32 folder or in the same folder as the plugin.

_Mouse and keyboard callbacks are not supported._

_Color parameters are not yet supported, but you can easily use float instead._





http://www.resolume.com

For any info
[mailto:mail@resolume.com](mailto:mail@resolume.com)

For more info in FFGL:
http://freeframe.sourceforge.net/


