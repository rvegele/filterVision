#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "controller.h"

#define DEBUG

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
    
        ofEasyCam camera;
    
        ofIcoSpherePrimitive icoSphere;
        ofPlanePrimitive plane;
    
        ofImage textureGradient;
    
        bool mRenderSphere;
    
        ofShader shader;
        ofShader shaderPlane;
    
    
		
        bool drawGui;
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
    
        //float colorz;
    
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
    
        ofShader glowShader;
        ofImage textureGlow;
        void drawGlow();
        bool showGlow;

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
    
    
        //ofSoundPlayer  lightCurveAudio;
    
    void visionFilterPart();
    
    ofImage composition01Image;
    ofImage composition02Image;
    int compositionDepth;
    
    // star settings

    float starRadiusGuiDest;
    float coronaRadiusDest;
    float specSplvDest;
    float nebulaRadiusDest;
    
    void updateGUI();
    
    int numGlowsToSpawn;
    int numNebulasToSpawn;
    

    // VIDEO RECORDER
    int 				snapCounter;
    string 				snapString;
    ofImage 			img;
    bool 				bSnapshot;
    float 				phase;
    
    // PIXELATE
    //int pixelateImage;    // argument is resulting pixel size
    //void pixelateImage(int pxSize);
};
