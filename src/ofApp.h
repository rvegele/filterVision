#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "controller.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        ofIcoSpherePrimitive icoSphere;
        ofPlanePrimitive plane;
        ofImage textureGradient;
    
        bool mRenderSphere;
    
        ofShader shader;
        ofShader shaderPlane;
        ofEasyCam camera;
		
        ofxPanel gui;
    
        ofxFloatSlider solarRadius;
        ofxFloatColorSlider color;
    
        ofxFloatSlider clmpMin;
        ofxFloatSlider clmpMax;
        ofxFloatSlider dtt;
    
        ofxFloatSlider coronaRadius;
    
        ofxFloatSlider specSpalva;
        ofxFloatSlider nebulaRadius;
    
        ofxFloatSlider randomas;
        //ofxVec2Slider center;
        //ofxIntSlider circleResolution;
    
        //ofxToggle filled;
        //ofxButton twoCircles;
        //ofxButton ringButton;
        //ofxLabel screenSize;
    
        float colorz;
    
        void billboardBegin();
        void billboardEnd();
    
        bool showGradient;
        void drawGradient();
    
        bool showSphere;
        void drawSphere();
    
        bool showCorona;
        void drawCorona();
    
        bool showNebulas;
        void drawNebulas();
        ofShader nebulaShader;
        ofImage textureNebula;
        ofImage textureSmallGrid;
    
        bool blending; // additive blending mode
    
        ofShader coronaShader;
        ofImage textureCorona;
    
        ofImage textureSpectrum;
    
        ofFbo coronaFBO;
    
        // TIME
        float			mTime;				// Time elapsed in real world seconds
        float			mTimeElapsed;		// Time elapsed in simulation seconds
    	float			mTimeMulti;			// Speed at which time passes
    	float			mTimeAdjusted;		// Amount of time passed between last frame and current frame
        float			mTimer;				// A resetting counter for determining if a Tick has occured
        bool			mTick;				// Tick (aka step) for triggering discrete events
    
        bool getTick();
        void updateTime();
        float		getTimeDelta();
    
        // CONTROLLER
        Controller mController;
    
        ofVboMesh billboard;
};
