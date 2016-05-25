#include "ofApp.h"

#include <iostream>
#include <string>     // std::string, std::stoi
#include <stdlib.h>
#include "ofxEasing.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    ofBackground(0, 0, 0);
    // SERIAL
    
    std::vector<ofx::IO::SerialDeviceInfo> devicesInfo = ofx::IO::SerialDeviceUtils::listDevices();
    
    
    ofLogNotice("ofApp::setup") << "Connected Devices: ";
    for (std::size_t i = 0; i < devicesInfo.size(); ++i)
    {
        ofLogNotice("ofApp::setup") << "\t" << devicesInfo[i];
    }
    
    // connect to 1st device on list
    device.setup(devicesInfo[0], 9600);
    cout << "connecting to: " << devicesInfo[0] << "\n";
    // listen to arduino
    device.registerAllEvents(this);
    
    // TELESCOPE INSTRUMENTS
    // INSTRUMENTS
    newInstrument = 1;
    oldInstrument = 0;
    
    // images
    backgroundImage.load("background.png");
    coordinateImage.load("coords-02.png");
    
    backgroundFadeOverNew.load("bg-01.png");
    backgroundFadeOverOld.load("bg-02.png");
    
    drawImage = backgroundImage;
    drawItem = 0;
    
    initTime = 0;
    bgAlpha = 0;
    
    movVisibleLight.load("02_vis_light.mp4");
    movVisibleLight.setLoopState(OF_LOOP_NONE);
    
    movSpectrum.load("03_spectrum.mp4");
    movSpectrum.setLoopState(OF_LOOP_NONE);
    
    movLightCurve.load("04_lightCurve.mp4");
    movSpectrum.setLoopState(OF_LOOP_NONE);
    
    movLifecycle.load("05_lifecycle.mp4");
    movSpectrum.setLoopState(OF_LOOP_NONE);
    
    
    // FBO
    initFbo();
    
    // STAR
    showGradient = true;
    showSphere = true;
    showCorona = true;
    showNebulas = true;
    showGlow = true;
    

    // GL_REPEAT for texture wrap only works with NON-ARB textures //
    ofDisableArbTex();
    ofEnableAlphaBlending();
    
    icoSphere.set(0.5, 4);
    icoSphere.setPosition(0, 0, 0);
    
    plane.set( 1, 1 );
    plane.setPosition(0, 0, 0);
    plane.rotate(180, 1.0, 0.0, 0.0);
    
    camera.setFarClip(20000);
    
#ifdef TARGET_OPENGLES
    shader.load("Sphere.vert","Sphere.frag");
    shaderPlane.load("gradient.vert","gradient.frag");
    coronaShader.load("corona.vert","corona.frag");
    nebulaShader.load("nebula.vert","nebula.frag");
    glowShader.load("glow.vert","glow.frag");
#else
    if(ofIsGLProgrammableRenderer()){
        shader.load("Sphere.vert", "Sphere.frag");
        shaderPlane.load("Sphere.vert","Sphere.frag");
        coronaShader.load("corona.vert","corona.frag");
        nebulaShader.load("nebula.vert","nebula.frag");
        glowShader.load("glow.vert","glow.frag");
    }else{
        shader.load("Sphere.vert", "Sphere.frag");
        shaderPlane.load("gradient.vert","gradient.frag");
        coronaShader.load("corona.vert","corona.frag");
        nebulaShader.load("nebula.vert","nebula.frag");
        glowShader.load("glow.vert","glow.frag");
    }
#endif
    
    textureGradient.load("grid.png");
    textureCorona.load("corona.png");
    textureSpectrum.load("spectrum.png");
    textureNebula.load("nebula.png");
    textureSmallGrid.load("smallGrid.png");
    textureGlow.load("glow.png");
    
    composition01Image.load("comp-02.jpg");
    composition01Image.resize(composition01Image.getWidth()*0.5, composition01Image.getHeight()*0.5);
    
    composition02Image.load("comp-04.jpg");
    composition02Image.resize(composition02Image.getWidth()*0.5, composition02Image.getHeight()*0.5);
    
    compositionDepth = 0;
    

    gui.setup(); // most of the time you don't need a name
    /*
    gui.add(color.setup("color",
                        ofFloatColor(0.92, 0.82, 0.34),
                        ofFloatColor(0.0,0.0),
                        ofFloatColor(1.0,1.0)));
    */
    gui.add(solarRadius.setup("radius", 100.25, 1, 400));
    gui.add(clmpMin.setup("clamp Min", 0.0, 0.0, 1.0));
    gui.add(clmpMax.setup("clamp Max", 0.75, 0.0, 1.0)); // 0.585
    gui.add(dtt.setup("dot mult", 0.200, 0.0, 1.0));      //  0.495
    gui.add(coronaRadius.setup("coronaR", 1.90, 1, 10));
    gui.add(specSpalva.setup("specSplv", 0.255, 0, 1.0));
    gui.add(nebulaRadius.setup("nebulaR", 0.875, 0, 5.0));
    gui.add(randomas.setup("randomas", 134, 0, 400));
    gui.add(numGlowsToSpawn.setup("glows", 3, 0, 50));
    gui.add(numNebulasToSpawn.setup("nebulaz", 5, 0, 50));
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    
    coronaFBO.allocate(500, 500);
    
    camera.setFov(22);
    
    // TIME
    mTick = false;
    mTime = ofGetElapsedTimef();
    mTimeElapsed = 0.0f;
    mTimeMulti = 60.0f;
    mTimer = 0.0f;
    
    numGlowsToSpawn = 3;
    numNebulasToSpawn = 5;
    
    // VIDEO RECORDING //
    snapCounter = 0;
    bSnapshot = false;
    phase = 0;
    
    // PIXELATE IMAGE
    //pixelateImage = 80;    // argument is resulting pixel size
    numGlowsToSpawn = 3;
    numNebulasToSpawn = 5;
    blending = true;
}
void ofApp::exit() {
    device.unregisterAllEvents(this);
}
//--------------------------------------------------------------
void ofApp::update(){
    
    // UPDATE STAR
    updateTime();
    tick();
    mController.update( getTimeDelta() );
    
}
void ofApp::onSerialBuffer(const ofx::IO::SerialBufferEventArgs& args)
{
    std::string message = args.getBuffer().toString();
    cout << "new message: " << message << "\n";
    
    vector<string> input = ofSplitString(message, ",");
    
    if(input.size() >= 1) {
        
        std::string knob = input.at(0);
        int         val  = std::stoi (input.at(1),nullptr,0);
        
        //cout << "decoded message: " << "knob " << knob << " | value " << val << "\n";
        
        // change of the instrument and register time of change
        if (knob == "ins") {
            newSelectionTime = ofGetElapsedTimeMillis();
            newInstrument = val;
        }
        
        // if we're on coordinates and tweaking encoders
        if ( newInstrument == 1 && (knob == "enc1" || knob == "enc2" || knob == "enc3") ) {
            if      (knob == "enc1") {
                if (val == 1) {
                    coordX ++;
                } else {
                    coordX--;
                }
                //cout << "X: " << coordX << "\n";
            }
            else if (knob == "enc2") {
                if (val == 1) {
                    coordY++;
                } else {
                    coordY--;
                }
                //cout << "Y: " << coordY << "\n";
            }
            else if (knob == "enc3") {
                if (val == 1) {
                    coordZ++;
                } else {
                    coordZ--;
                }
                //cout << "Z: " << coordZ << "\n";
            }
        }
        
    }
}

void ofApp::onSerialError(const ofx::IO::SerialBufferErrorEventArgs& args)
{
    // Errors and their corresponding buffer (if any) will show up here.
    //SerialMessage message(args.getBuffer().toString(),
    //                      args.getException().displayText(),
    //                      500);
    //serialMessages.push_back(message);
    //cout << args.getBuffer().toString() << "\n";
}
//--------------------------------------------------------------
void ofApp::draw(){
    // DRAW STAR

    //starFbo.draw(0, 0);
    
    //glEnable(GL_BLEND);
    //
    
    //ofClear(0, 0, 0, 0);
    //glDisable(GL_CULL_FACE);
    
    ofEnableAlphaBlending();
    //ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    //starFbo.draw(0, 0);
    // reset blending mode
    //ofEnableAlphaBlending();
    
    //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    //glDisable(GL_BLEND);
    
    //ofDisableBlendMode();
    // this is where the FBO comes from
    
    //glEnable(GL_BLEND);
    //glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
    
    starFbo.begin();
        ofPushMatrix();
        ofTranslate( 1080 /2, 1293 /2) ;
        visionFilter();
        ofPopMatrix();
    starFbo.end();
    // UPDATE STAR ENDS
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    starFbo.draw(0, 0);
    
    //glDisable(GL_BLEND);
    // FOR SCREENSHOT
    //ofDisableDepthTest();
    // for saving frames
    /*
    if (bSnapshot == true){
        // grab a rectangle at 200,200, width and height of 300,180
        img.grabScreen(0,0,ofGetWindowWidth(),ofGetWindowHeight());
        string fileName = "screengrab/snapshot_"+ofToString(10000+snapCounter)+".png";
        img.save(fileName);
        //snapString = "saved " + fileName;
        snapCounter++;
        bSnapshot = false;
    }
    */
    
    ofSetColor(225);
    if (drawGui) {
        gui.draw();
        ofDrawBitmapString(ofToString(ofGetFrameRate()), ofPoint(ofGetWindowWidth() - 70, 20));
    }
    ofDisableBlendMode();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == '1') {
        oldInstrument = 1;
        newInstrument = 1;
        cout << "Set instrument: " << oldInstrument << "\n";
        initTime = ofGetElapsedTimef();
        step = 1;
    }
    
    if (key == '2') {
        oldInstrument = 2;
        newInstrument = 2;
        cout << "Set instrument: " << oldInstrument << "\n";
        initTime = ofGetElapsedTimef();
        step = 1;
    }
    
    if (key == '3') {
        oldInstrument = 3;
        newInstrument = 3;
        cout << "Set instrument: " << oldInstrument << "\n";
        initTime = ofGetElapsedTimef();
        step = 1;
    }
    
    if (key == '4') {
        oldInstrument = 4;
        newInstrument = 4;
        cout << "Set instrument: " << oldInstrument << "\n";
        initTime = ofGetElapsedTimef();
        step = 1;
    }
    if (key == '5') {
        oldInstrument = 5;
        newInstrument = 5;
        cout << "Set instrument: " << oldInstrument << "\n";
        initTime = ofGetElapsedTimef();
        step = 1;
    }
    
    switch( key ) {
        //case '0':	showGradient = !showGradient; break;
        //case 'x':	bSnapshot = true; break;
            
        //case '1':	showSphere = !showSphere; break;
        //case '2':	showGradient = !showGradient; break;
        //case '3':	showCorona = !showCorona; break;
        //case '4':	showNebulas = !showNebulas; break;
        //case '5':	showGlow = !showGlow; break;
            
        case '0':	showGlow = !showGlow; showGradient = !showGradient; showCorona = !showCorona; showNebulas = !showNebulas; showSphere = !showSphere; break;
            
        //case '0':	showGlow = true; showGradient = true; showCorona = true; showNebulas = true; showSphere = true;
        //    numGlowsToSpawn = 0;
        //    numNebulasToSpawn = 0;
        //    break;
            
        case 'g':   drawGui = !drawGui; break;
        case 'b':	blending = !blending; break;
            
        case 'R':
            solarRadius = 100.25;
            coronaRadius = 1.90;
            specSpalva = 0.225;
            nebulaRadius = 0.875;
            numGlowsToSpawn = 3;
            numNebulasToSpawn = 5;
            clmpMin = 0.0;
            clmpMax = 0.75; // 0.585
            dtt = 0.200; //  0.495
            break;
        case 'Y':
            solarRadius = 300.25;
            coronaRadius = 1.95;
            specSpalva = 0.55;
            nebulaRadius = 1.0;
            numGlowsToSpawn = 10;
            numNebulasToSpawn = 6;
            clmpMin = 0.0;
            clmpMax = 1.00; // 0.585
            dtt = 0.735; //  0.495

        break;
            
        case 'B':
            solarRadius = 400.25;
            coronaRadius = 1.95;
            specSpalva = 0.91;
            nebulaRadius = 1.0;
            numGlowsToSpawn = 3;
            numNebulasToSpawn = 15;
            clmpMin = 0.0;
            clmpMax = 0.75; // 0.585
            dtt = 0.115; //  0.495
            break;
            
        case 'f':
            ofToggleFullscreen();
        break;
            
        default: break;
    }

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
cout << "width: " << ofGetWindowWidth() << " | height: " << ofGetWindowHeight() << "\n";
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::billboardBegin() {
    
    float modelview[16];
    int i,j;
    
    // save the current modelview matrix
    glPushMatrix();
    
    // get the current modelview matrix
    glGetFloatv(GL_MODELVIEW_MATRIX , modelview);
    
    // undo all rotations
    // beware all scaling is lost as well
    for( i=0; i<3; i++ )
        for( j=0; j<3; j++ ) {
            if ( i==j )
                modelview[i*4+j] = 1.0;
            else
                modelview[i*4+j] = 0.0;
        }
    
    // set the modelview with no rotations
    glLoadMatrixf(modelview);
}



void ofApp::billboardEnd() {
    
    // restore the previously
    // stored modelview matrix
    glPopMatrix();
}

void ofApp::drawGradient() {
    
    shaderPlane.begin();
    
    textureGradient.getTexture().bind();
    shaderPlane.setUniformTexture("tex1", textureSpectrum.getTexture(), 1);
    
    ofPushMatrix();
    billboardBegin();
    
    shaderPlane.setUniform1f("radius", solarRadius);
    shaderPlane.setUniform4f("color", color);
    //plane.transformGL();
    shaderPlane.setUniform1f("specSpalva", specSpalva);
    
    ofDrawPlane(0, 0, -1, solarRadius * 25, solarRadius * 25);
    //plane.set(solarRadius * 25, solarRadius * 25);
    //plane.setPosition(0, 0, -1);
    //plane.draw();
    //plane.restoreTransformGL();
    ofPopMatrix();
    billboardEnd();
    
    textureGradient.getTexture().unbind();
    shaderPlane.end();
   
}
void ofApp::drawCorona()
{
    ofPushMatrix();
    billboardBegin();
    
    //ofSetColor(1.0, 1.0, 1.0, 1.0);
    coronaShader.begin();
    coronaShader.setUniformTexture("tex0", textureCorona.getTexture(), 0);
    coronaShader.setUniformTexture("tex1", textureSpectrum.getTexture(), 1);

    coronaShader.setUniform1f("radius", solarRadius);
    coronaShader.setUniform1f("coronaRadius", coronaRadius);
    //coronaShader.setUniform4f("color", color);
    
    coronaShader.setUniform1f("specSpalva", specSpalva);

    ofDrawPlane(0, 0, 0, 1, 1);
    //plane.set(1, 1);
    //plane.setPosition(0, 0, 0);
    //plane.draw();
    
    billboardEnd();
    ofPopMatrix();
    
    coronaShader.end();
}

void ofApp::drawSphere() {
    
    shader.begin();
    shader.setUniformTexture("tex1", textureSpectrum.getTexture(), 1);
    shader.setUniform1f("radius", solarRadius/* * 0.025*/);
    shader.setUniform4f("color", color);
    shader.setUniform1f("clmpMin", clmpMin);
    shader.setUniform1f("clmpMax", clmpMax);
    shader.setUniform1f("dtt", dtt);
    
    shader.setUniform1f("specSpalva", specSpalva);
    //ofDrawIcoSphere(0, 0, 0, 1);
    //ofDrawSphere(0, 0, 0, 1);
    //icoSphere.set(0.5, 4);
    icoSphere.draw();
    shader.end();
    
}

void ofApp::drawNebulas() {
    
        nebulaShader.begin();
        nebulaShader.setUniformTexture("tex0", textureNebula.getTexture(), 0);
        nebulaShader.setUniformTexture("tex1", textureSpectrum.getTexture(), 1);
        nebulaShader.setUniformTexture("tex2", textureSmallGrid.getTexture(), 2);
    
        nebulaShader.setUniform1f("specSpalva", specSpalva);
        nebulaShader.setUniform1f("radius", solarRadius/* * 0.025*/);
        nebulaShader.setUniform1f("nebulaRadius", nebulaRadius);
        nebulaShader.setUniform1f("randomas", randomas);
    
        //ofVec3f right = ofVec3f(1, 0, 0);
        //right.normalize();
        //right *= -1;
    
        //ofVec3f up	= ofVec3f(0, 1, 0);
        //up.normalize();
        //up *= -1;
    
        //right.set(camera.getOrientationEuler().x);
        //up.set(camera.getOrientationEuler().y);
    
        ofEnablePointSprites();
        mController.drawNebulas( &nebulaShader/*, right, up*/ );
        ofDisablePointSprites();
    
        nebulaShader.end();
    
        //ofDisableBlendMode();
    
}

void ofApp::drawGlow() {
    
    //ofEnableBlendMode(OF_BLENDMODE_ADD);
    glowShader.begin();
    glowShader.setUniformTexture("tex0", textureGlow.getTexture(), 0);
    glowShader.setUniformTexture("tex1", textureSpectrum.getTexture(), 1);
    glowShader.setUniformTexture("tex2", textureSmallGrid.getTexture(), 2);
    
    glowShader.setUniform1f("specSpalva", specSpalva);
    glowShader.setUniform1f("radius", solarRadius/* * 0.025*/);
    glowShader.setUniform1f("nebulaRadius", nebulaRadius);
    glowShader.setUniform1f("randomas", randomas);
    
    //ofVec3f right = ofVec3f(1, 0, 0);
    //right.normalize();
    //right *= -1;
    
    //ofVec3f up	= ofVec3f(0, 1, 0);
    //up.normalize();
    //up *= -1;
    
    //right.set(camera.getOrientationEuler().x);
    //up.set(camera.getOrientationEuler().y);
    
    ofEnablePointSprites();
    mController.drawGlows( &glowShader/*, right, up*/ );
    ofDisablePointSprites();
    
    
    
    glowShader.end();
    //ofDisableBlendMode();
    //ofEnableBlendMode(OF_BLENDMODE_ALPHA);
}


bool ofApp::getTick() {
    return mTick;
}
float ofApp::getTimeDelta()
{
    return mTimeAdjusted;
}
void ofApp::updateTime()
{
    float prevTime	= mTime;
    mTime			= ofGetElapsedTimef();
    float dt		= mTime - prevTime;
    mTimeAdjusted	= dt * mTimeMulti;
    mTimeElapsed	+= mTimeAdjusted;
    
    mTimer += mTimeAdjusted;
    mTick = false;
    if( mTimer > 1.0f ){
        mTick = true;
        mTimer = 0.0f;
    }
}

void ofApp::visionFilter(){
    
    
    ofClear(255, 255, 255, 0);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    //ofClear ( 0, 0, 0 );
    //glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    ofBackground(0, 0, 0);
    
    //glDisable(GL_CULL_FACE);
    // ofBackground(0,0,0);
    //ofBackgroundGradient(ofColor(30, 30, 30), ofColor(0, 0, 0), OF_GRADIENT_CIRCULAR);
    //glEnable(GL_CULL_FACE);
    
    //camera.begin();
    //ofDrawAxis(1000);
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    // GRADIENT PLANE
    if (showGradient) {
        drawGradient();
    }

    // SUN SPHERE
    if (showSphere) {
        //glEnable( GL_TEXTURE_2D );
        glEnable(GL_DEPTH_TEST);
        glDepthMask(true);
        drawSphere();
        glDisable(GL_DEPTH_TEST);
    }
    ofDisableBlendMode();
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    // SUN CORONA
    if (showCorona) {
        drawCorona();
    }
    // SUN NEBULAS
    if (showNebulas){
        drawNebulas();
    }
    // SUN GLOW
    if (showGlow){
        drawGlow();
    }

    ofDisableBlendMode();
    //camera.end();
    //ofClearAlpha();
}

void ofApp::tick() {
    // CONTROLLER
    if( getTick() ){
        // ADD GLOWS
        //numGlowsToSpawn = 3;
        mController.addGlows( solarRadius*nebulaRadius*0.5, numGlowsToSpawn );
        
        // ADD NEBULAS
        //numNebulasToSpawn = 5;
        //if( mRoom.isPowerOn() ) numNebulasToSpawn = (int)( 8 );//* mStar.mRadiusMulti );
        
        mController.addNebulas( solarRadius*nebulaRadius*0.5, /*mStar,*/ numNebulasToSpawn );
        
        //!!!!!!!!!!!!!!!!!!! REMEMBER TO TURN ON OTHER THINGS IN CONTROLLER UPDATE
        // ADD DUSTS
        //int numDustsToSpawn = 500;
        //mController.addDusts( mStar, numDustsToSpawn );
    }

}
void ofApp::initFbo() {
    //starFbo.allocate(1080, 1293, GL_RGBA32F_ARB);
    //starFbo.begin();
    //ofClear(255,255,255, 0);
    //starFbo.end();
    
    ofFbo::Settings s;
    s.width = 1080;//ofGetWindowWidth();
    s.height = 1293;//ofGetWindowHeight();
    s.internalformat    = GL_RGBA;//GL_RGBA32F_ARB;
    starFbo.allocate(s);
    starFbo.begin();
        ofClear(0, 0, 0, 0);
    starFbo.end();
}