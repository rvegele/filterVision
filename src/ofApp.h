#pragma once

#include "ofMain.h"
#include "ofxSerial.h"
#include <vector>

#include "ofxGui.h"
#include "controller.h"

class SerialMessage {
public:
    SerialMessage(): fade(0) {
    }
    
    SerialMessage(const std::string& _message,
                  const std::string& _exception,
                  int _fade):
    message(_message),
    exception(_exception),
    fade(_fade)
    {
    }
    
    std::string message;
    std::string exception;
    int fade;
};

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
    
        void exit();
        void onSerialBuffer(const ofx::IO::SerialBufferEventArgs& args);
        void onSerialError(const ofx::IO::SerialBufferErrorEventArgs& args);
        ofx::IO::BufferedSerialDevice device;
        std::vector<SerialMessage> serialMessages;
    
        // instruments
        int newInstrument;
        int oldInstrument;
        long long newSelectionTime;
    
        bool newStar = true;
    
        // coordinates
        int coordX;
        int coordY;
        int coordZ;
    
        // images
        ofImage backgroundImage;
        ofImage coordinateImage;
        ofImage drawImage;
        int drawItem;
    
        ofImage backgroundFadeOverNew;
        ofImage backgroundFadeOverOld;
    
        // for fading
        float initTime;
        float bgAlpha;
        float endTime;
    
        int step;
    
        // VIDEOS
        ofVideoPlayer movVisibleLight;
        ofFbo fboVisibleLight;
    
        ofVideoPlayer movSpectrum;
        ofFbo fboSpectrum;
    
    
        ofVideoPlayer movLightCurve;
        ofVideoPlayer movLifecycle;
    
        // STAR FBO
        ofFbo starFbo; // with alpha
        void initFbo();
        // STAR STUFF
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
        ofxIntSlider numGlowsToSpawn;
        ofxIntSlider numNebulasToSpawn;
    
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
        void tick();
        void updateTime();
        float		getTimeDelta();
    
        // CONTROLLER
        Controller mController;
        ofVboMesh billboard;
    
        //ofSoundPlayer  lightCurveAudio;
    
    void visionFilter();
    
    ofImage composition01Image;
    ofImage composition02Image;
    int compositionDepth;
    
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
