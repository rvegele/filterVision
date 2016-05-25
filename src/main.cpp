//========================================================================
#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    //ofSetLogLevel(OF_LOG_VERBOSE);
    int windowWidth = 200;
    int windowHeight = 200;
    
#ifdef TARGET_OPENGLES
    ofGLESWindowSettings settings;
    settings.width = windowWidth;
    settings.height = windowHeight;
    settings.setGLESVersion(2);
    ofCreateWindow(settings);
#else
    ofSetupOpenGL(windowWidth, windowHeight, OF_WINDOW);
#endif
    
    //ofSetupOpenGL(800,600,OF_WINDOW);			// <-------- setup the GL context
    
    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    ofRunApp(new ofApp());
}


