#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    
    //ofBackground(30, 30, 30);
    // GL_REPEAT for texture wrap only works with NON-ARB textures //
    ofDisableArbTex();
    ofEnableAlphaBlending();
    
    icoSphere.set(1, 4);
    icoSphere.setPosition(0, 0, 0);
    
    plane.set( 1, 1 );
    plane.setPosition(0, 0, 0);
    
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
    
    gui.setup(); // most of the time you don't need a name
    
    gui.add(solarRadius.setup("radius", 150, 1, 200));
    
    gui.add(color.setup("color",
                        ofFloatColor(0.92, 0.82, 0.34),
                        ofFloatColor(0.0,0.0),
                        ofFloatColor(1.0,1.0)));
    
    gui.add(clmpMin.setup("clamp Min", 0.0, 0.0, 1.0));
    gui.add(clmpMax.setup("clamp Max", 0.585, 0.0, 1.0)); // 0.85
    gui.add(dtt.setup("dot mult", 0.495, 0.0, 1.0));      //  0.75
    
    gui.add(coronaRadius.setup("coronaR", 3.975, 1, 10));
    gui.add(specSpalva.setup("specSplv", 0.45, 0, 1.0));
    gui.add(nebulaRadius.setup("nebulaR", 100, 0, 200));
    
    gui.add(randomas.setup("randomas", 134, 0, 400));
    //gui.add(circleResolution.setup("circle res", 5, 3, 90));
    //gui.add(twoCircles.setup("two circles"));
    //gui.add(ringButton.setup("ring"));
    //gui.add(screenSize.setup("screen size", ofToString(ofGetWidth())+"x"+ofToString(ofGetHeight())));
    
    
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
    
    // OSC
    // listen on the given port
    cout << "listening for osc messages on port " << PORT << "\n";
    receiver.setup(PORT);
    filter = 0;
    
    ofTrueTypeFont::setGlobalDpi(72);
    gothamBold.load("fonts/Gotham-Bold.otf",40, true, true);
    gothamBold.setLineHeight(46.0f);
    gothamBold.setLetterSpacing(1.037);
    
    gothamLight.load("fonts/Gotham-Light.otf",40, true, true);
    gothamLight.setLineHeight(46.0f);
    gothamLight.setLetterSpacing(1.037);
    
    xSTR = "0";
    ySTR = "0";
    zSTR = "0";
    
    //SOUND
    lightCurveAudio.load("sound/007_Kepler_Star_KIC12268220C Light_Curve_Waves_to_Sound.wav");
    lightCurveAudio.setVolume(0.75f);
    lightCurveAudio.setMultiPlay(false);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    // update the sound playing system:
    ofSoundUpdate();
    
    // OSC
    checkOSC();
    
    //plane.set( solarRadius*30, solarRadius*30 );
    //ofVec3f haha = camera.getOrientationEuler();
    //float haha = camera.getAspectRatio();
    //bool haha = camera.getForceAspectRatio();
    //float haha = camera.getFov();
    //cout << haha << "\n";
    
    // ROOM
    //mRoom.update();
    updateTime();
    // STAR
    //mStar.update( mRoom.getTimeDelta() );
    
    // CONTROLLER
    if( getTick() ){
        // ADD GLOWS
        int numGlowsToSpawn = 3;
        mController.addGlows( solarRadius, numGlowsToSpawn );
        
        // ADD NEBULAS
        int numNebulasToSpawn = 4;
        //if( mRoom.isPowerOn() ) numNebulasToSpawn = (int)( 8 );//* mStar.mRadiusMulti );
        
        mController.addNebulas( solarRadius, /*mStar,*/ numNebulasToSpawn );
        
        //!!!!!!!!!!!!!!!!!!! REMEMBER TO TURN ON OTHER THINGS IN CONTROLLER UPDATE
        // ADD DUSTS
        //int numDustsToSpawn = 500;
        //mController.addDusts( mStar, numDustsToSpawn );
    }
    
//!!!!!!!!!!!!!!!!!!! REMEMBER TO TURN ON OTHER THINGS IN CONTROLLER UPDATE
        mController.update( getTimeDelta() );
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
#ifdef DEBUG
    
        ofClear ( 0, 0, 0 );
        
        glDisable(GL_CULL_FACE);
        ofBackgroundGradient(ofColor(30, 30, 30), ofColor(0, 0, 0), OF_GRADIENT_CIRCULAR);
        glEnable(GL_CULL_FACE);
        
        camera.begin();
        //ofDrawAxis(1000);
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
            
            //gl::disableDepthWrite();
            //glDepthMask(false);
            glDisable(GL_DEPTH_TEST);
            
        }
        //ofEnableBlendMode(OF_BLENDMODE_ADD);
        //glEnable(GL_BLEND);
        
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
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
 //   ofEnableBlendMode(OF_BLENDMODE_ADD);
    
            //glDisable(GL_BLEND);
        camera.end();
        gui.draw();
        

    // DEBUG - FPS
    ofSetColor(225);
    //ofFill();
    ofDrawBitmapString(ofToString(ofGetFrameRate()), ofPoint(ofGetWindowWidth() - 70, 20));
#else
    
    //ofBackgroundGradient(ofColor::green, ofColor::gray);
    //ofEnableBlendMode(OF_BLENDMODE_ADD);
    //ofEnableDepthTest();
    
    
    // FILTER BLANK SCREEN BEGIN
        if (filter == 0) {
            glDisable(GL_CULL_FACE);
            ofBackgroundGradient(ofColor(30, 30, 30), ofColor(0, 0, 0), OF_GRADIENT_CIRCULAR);
            glEnable(GL_CULL_FACE);
            /*
            ofBackground(30, 30, 30);
            ofSetColor(225);
            ofFill();
            ofDrawBitmapString("Use OSC 1-2-3-4 for filter demo", 50, 50);
            */
        }
    // FILTER BLANK SCREEN END
    
    // --------------------------------------- //
    
    // FILTER COORDINATES BEGIN
        if (filter == 1) {
            //ofBackground(255, 0, 0);
            glDisable(GL_CULL_FACE);
            ofBackgroundGradient(ofColor(30, 30, 30), ofColor(0, 0, 0), OF_GRADIENT_CIRCULAR);
            glEnable(GL_CULL_FACE);
            
            ofSetColor(225);
            
            string step1 = "1. SET GALACTIC COORDINATES";
            ofRectangle rect = gothamBold.getStringBoundingBox(step1, 0,0);
            gothamBold.drawString(step1, ofGetWindowWidth()/2-rect.width/2, ofGetWindowHeight()/2-((rect.height / 2) + 200));
            
           
            string xSTR1 = "X";
            rect = gothamLight.getStringBoundingBox(xSTR1, 0,0);
            gothamLight.drawString(xSTR1, (ofGetWindowWidth()/2-rect.width/2) - 300, ofGetWindowHeight()/2+(rect.height / 2)-115);
            
            string ySTR1 = "Y";
            rect = gothamLight.getStringBoundingBox(ySTR1, 0,0);
            gothamLight.drawString(ySTR1, (ofGetWindowWidth()/2-rect.width/2) , ofGetWindowHeight()/2+(rect.height / 2)-115);
            
            string zSTR1 = "Z";
            rect = gothamLight.getStringBoundingBox(zSTR1, 0,0);
            gothamLight.drawString(zSTR1, (ofGetWindowWidth()/2-rect.width/2) + 300, ofGetWindowHeight()/2+(rect.height / 2)-115);
            
    
            //xSTR = "+12.675";
            rect = gothamBold.getStringBoundingBox(xSTR, 0,0);
            gothamBold.drawString(xSTR, (ofGetWindowWidth()/2-rect.width/2) - 300, ofGetWindowHeight()/2+(rect.height / 2));
            
            ofDrawLine(ofGetWindowWidth()/2-151, ofGetWindowHeight()/2-50, ofGetWindowWidth()/2-151, ofGetWindowHeight()/2+50);
            
            //string ySTR = "-5.224";
            rect = gothamBold.getStringBoundingBox(ySTR, 0,0);
            gothamBold.drawString(ySTR, (ofGetWindowWidth()/2-rect.width/2) , ofGetWindowHeight()/2+(rect.height / 2));
            
            ofDrawLine(ofGetWindowWidth()/2+151, ofGetWindowHeight()/2-50, ofGetWindowWidth()/2+151, ofGetWindowHeight()/2+50);
            
            //string zSTR = "+1.34";
            rect = gothamBold.getStringBoundingBox(zSTR, 0,0);
            gothamBold.drawString(zSTR, (ofGetWindowWidth()/2-rect.width/2) + 300, ofGetWindowHeight()/2+(rect.height / 2));
            
            string step2 = "2. INSERT FILTER";
            rect = gothamBold.getStringBoundingBox(step2, 0,0);
            gothamBold.drawString(step2, ofGetWindowWidth()/2-rect.width/2, ofGetWindowHeight()/2+((rect.height / 2) + 200));

        }
    // FILTER COORDINATES END
    
    // --------------------------------------- //
    
    // FILTER VISION BEGIN
        if (filter == 2) {
            ofClear ( 0, 0, 0 );
            
            glDisable(GL_CULL_FACE);
            ofBackgroundGradient(ofColor(30, 30, 30), ofColor(0, 0, 0), OF_GRADIENT_CIRCULAR);
            glEnable(GL_CULL_FACE);
            
            camera.begin();
            //ofDrawAxis(1000);
            // GRADIENT PLANE
            if (showGradient) {
                drawGradient();
            }
            // SUN SPHERE
            if (showSphere) {
                drawSphere();
            }
            // SUN CORONA
            if (showCorona) {
                drawCorona();
            }
            // SUN NEBULAS
            if (showNebulas){
                drawNebulas();
            }
            
            camera.end();
            gui.draw();
            
        }
    // FILTER VISION END
    
    // --------------------------------------- //
    
    // FILTER XRAY SOUND BEGIN
    if (filter == 3) {
        glDisable(GL_CULL_FACE);
        ofBackgroundGradient(ofColor(30, 30, 30), ofColor(0, 0, 0), OF_GRADIENT_CIRCULAR);
        glEnable(GL_CULL_FACE);
        if (lightCurveAudio.isPlaying()) {
            
        } else { lightCurveAudio.play(); }
    }
    // FILTER XRAY SOUND END
    
    // --------------------------------------- //
    
    // FILTER COMPOSITION BEGIN
    if (filter == 4) {
    }
    // FILTER COMPOSITION END
    
    // --------------------------------------- //
    
    //ofDisableDepthTest();
    
    // DEBUG - FPS
    ofSetColor(225);
    //ofFill();
    ofDrawBitmapString(ofToString(ofGetFrameRate()), ofPoint(ofGetWindowWidth() - 70, 20));
    
#endif

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    switch( key ) {
        //case '0':	showGradient = !showGradient; break;
        case '1':	showSphere = !showSphere; break;
        case '2':	showGradient = !showGradient; break;
        case '3':	showCorona = !showCorona; break;
        case '4':	showNebulas = !showNebulas; break;
        case '5':	showGlow = !showGlow; break;
        
        case 'b':	blending = !blending; break;
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
    
    //plane.setPosition(0, 0, -1);
    //plane.set(solarRadius * 25, solarRadius * 25, 2, 2);
    //plane.draw(solarRadius * 25);
    ofDrawPlane(0, 0, -1, solarRadius * 25, solarRadius * 25);
    
    // ofDrawPlane(0, 0 , coronaRadius*5, coronaRadius*5);
    //plane.draw();
    //plane.restoreTransformGL();
    ofPopMatrix();
    billboardEnd();
    
    textureGradient.getTexture().unbind();
    shaderPlane.end();
   
}
void ofApp::drawCorona()
{
    //if( mBillboard ){
    //    gl::pushMatrices();
    //    gl::rotate( mSpringCam.getCam().getOrientation() );
    //}
    //gl::color( ColorA( 1, 1, 1, 1 ) );
    /*
    mCoronaTex.bind();
    mSpectrumTex.bind( 1 );
    
    mCoronaShader.bind();
    
    mCoronaShader.uniform( "coronaTex", 0 );
    mCoronaShader.uniform( "spectrumTex", 1 );
    
    mCoronaShader.uniform( "starColor", mStar.mColor );
    mCoronaShader.uniform( "power", mRoom.getPower() );
    gl::drawSolidRect( Rectf( -radius, -radius, radius, radius ) );
    mCoronaShader.unbind();
    
    if( mBillboard ) gl::popMatrices();
    */
    
    //gl::rotate();
    //gl::rotate( camera.getOrientationEuler() );
    //gl::rotate( camera.getOrientationQuat());
    
    ofPushMatrix();
    billboardBegin();
    //ofTranslate(ofGetWidth()/2, ofGetHeight()/2, zoom);
    //ofQuaternion kamera = camera.getOrientationQuat();
    //ofVec3f kamera2 = kamera.asVec3();
    //ofRotateX(-1 * kamera2.x);
    //ofRotateY(-1 * kamera2.y);
    //ofRotateZ(-1 * kamera2.z);
    //ofRotate(camera, kamera2.x, 0, 0);
    //ofRotate(camera, 0, kamera2.y, 0);
    //ofRotate(camera, 0, 0, kamera2.z);
    
        ofSetColor(1.0, 1.0, 1.0, 1.0);
    
        float radiuss = solarRadius * coronaRadius;
        //float colorr = gui.getParameter().color;
        //float starColor = 0.9;
        //textureCorona.getTexture().bind(1);
        //textureSpectrum.getTexture().bind(1);
    
        //coronaShader.setUniformTexture("coronaTex", textureCorona.getTexture(), 1);
        //coronaShader.setUniformTexture("spectrumTex", textureSpectrum.getTexture(), 2);
    
    
    
        //shader.setUniformTexture("tex2", movie.getTextureReference(), 3);
        //shader.setUniformTexture("imageMask", maskFbo.getTextureReference(), 4);
    

    
        coronaShader.begin();
        //coronaShader.setUniform1f("corona", 1);
        coronaShader.setUniformTexture("tex0", textureCorona.getTexture(), 0);
        coronaShader.setUniformTexture("tex1", textureSpectrum.getTexture(), 1);

        coronaShader.setUniform1f("radius", solarRadius);
        coronaShader.setUniform1f("coronaRadius", coronaRadius);
        coronaShader.setUniform4f("color", color);
    
        coronaShader.setUniform1f("specSpalva", specSpalva);
        // -radius, -radius, radius, radius // -60, -60, 60, 60
        //ofSetPlaneResolution(2, 2);
    
        ofDrawPlane(0, 0, 0, 1, 1);
        //ofDrawPlane(0, 0 , coronaRadius, coronaRadius);
    
    billboardEnd();
    ofPopMatrix();
    
        coronaShader.end();
    
        //textureCorona.getTexture().unbind();

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
    icoSphere.draw();
    shader.end();
    
}

void ofApp::drawNebulas() {
    
    
        //ofEnableBlendMode(OF_BLENDMODE_ADD);
    
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

void ofApp::checkOSC() {
    
    while(receiver.hasWaitingMessages()){    // check for waiting messages
        ofxOscMessage m;                     // get the next message
        receiver.getNextMessage(m);
        
        // this part happens twice, because we receive ON and then OFF straight after
        if(m.getAddress() == "/1/push1"){
            //cout << m.getArgAsFloat(0) << "\n";
            if (m.getArgAsFloat(0) == 1 ) {
                filterCoordinates = !filterCoordinates;
                //cout << filterCoordinates << "\n";
                filterVisible =         false;
                filterXraySound =       false;
                filterComposition =     false;
            }
        }
        
        // this part happens twice, because we receive ON and then OFF straight after
        if(m.getAddress() == "/1/push2"){
            //cout << m.getArgAsFloat(0) << "\n";
            if (m.getArgAsFloat(0) == 1 ) {
                filterVisible = !filterVisible;
                //cout << filterVisible << "\n";
                filterCoordinates =     false;
                filterXraySound =       false;
                filterComposition =     false;
            }
        }
        
        // this part happens twice, because we receive ON and then OFF straight after
        if(m.getAddress() == "/1/push3"){
            //cout << m.getArgAsFloat(0) << "\n";
            if (m.getArgAsFloat(0) == 1 ) {
                filterXraySound = !filterXraySound;
                //cout << filterXraySound << "\n";
                filterCoordinates =     false;
                filterVisible =         false;
                filterComposition =     false;
            }
        }
        
        // this part happens twice, because we receive ON and then OFF straight after
        if(m.getAddress() == "/1/push4"){
            //cout << m.getArgAsFloat(0) << "\n";
            if (m.getArgAsFloat(0) == 1 ) {
                filterComposition = !filterComposition;
                //cout << filterComposition << "\n";
                filterCoordinates =     false;
                filterVisible =         false;
                filterXraySound =       false;
            }
        }
        
        if (m.getArgAsFloat(0) != 0 ) {
        cout << "COORD: " << filterCoordinates << " | VIS: " << filterVisible << " | SOUND: " << filterXraySound << " | COMPOSITION: " << filterComposition << "\n";
            if      (filterCoordinates) { filter = 1; }
            else if (filterVisible)     { filter = 2; }
            else if (filterXraySound)   { filter = 3; }
            else if (filterComposition) { filter = 4; }
            else                        { filter = 1; }
            cout << "Filter: " << filter << "\n";
        }
        
        if(m.getAddress() == "/1/fader1"){
            //cout << m.getArgAsFloat(0) << "\n";
            xSTR = ofToString(roundf(m.getArgAsFloat(0) * 10) / 10);
        }
        
        if(m.getAddress() == "/1/fader2"){
            //cout << m.getArgAsFloat(0) << "\n";
            ySTR = ofToString(roundf(m.getArgAsFloat(0) * 10) / 10);
        }
    
        if(m.getAddress() == "/1/fader3"){
            //cout << m.getArgAsFloat(0) << "\n";
            zSTR = ofToString(roundf(m.getArgAsFloat(0) * 10) / 10);
        }
        /*
           else {
            // unrecognized message: display on the bottom of the screen
            string msg_string;
            msg_string = m.getAddress();
            msg_string += ": ";
            for(int i = 0; i < m.getNumArgs(); i++){
                // get the argument type
                msg_string += m.getArgTypeName(i);
                msg_string += ":";
                // display the argument - make sure we get the right type
                if(m.getArgType(i) == OFXOSC_TYPE_INT32){
                    msg_string += ofToString(m.getArgAsInt32(i));
                }
                else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
                    msg_string += ofToString(m.getArgAsFloat(i));
                }
                else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
                    msg_string += m.getArgAsString(i);
                }
                else{
                    msg_string += "unknown";
                }
            }
         
            // add to the list of strings to display
            //msg_strings[current_msg_string] = msg_string;
            //timers[current_msg_string] = ofGetElapsedTimef() + 5.0f;
            //current_msg_string = (current_msg_string + 1) % NUM_MSG_STRINGS;
            // clear the next line
            //msg_strings[current_msg_string] = "";
        }*/
        
    }
}