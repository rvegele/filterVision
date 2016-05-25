#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    
    //ofBackground(30, 30, 30);
    // GL_REPEAT for texture wrap only works with NON-ARB textures //
    ofDisableArbTex();
    ofEnableAlphaBlending();
    
    icoSphere.set(0.5, 4);
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
    
    composition01Image.load("comp-02.jpg");
    composition01Image.resize(composition01Image.getWidth()*0.5, composition01Image.getHeight()*0.5);
    
    composition02Image.load("comp-04.jpg");
    composition02Image.resize(composition02Image.getWidth()*0.5, composition02Image.getHeight()*0.5);
    
    compositionDepth = 0;
    

    gui.setup(); // most of the time you don't need a name
    
    starRadiusGuiDest = 300.25;
    gui.add(solarRadius.setup("radius", starRadiusGuiDest, 1, 400));
    
    gui.add(color.setup("color",
                        ofFloatColor(0.92, 0.82, 0.34),
                        ofFloatColor(0.0,0.0),
                        ofFloatColor(1.0,1.0)));
    
    gui.add(clmpMin.setup("clamp Min", 0.0, 0.0, 1.0));
    gui.add(clmpMax.setup("clamp Max", 0.75, 0.0, 1.0)); // 0.585
    gui.add(dtt.setup("dot mult", 0.200, 0.0, 1.0));      //  0.495
    
    coronaRadiusDest = 1.95;
    gui.add(coronaRadius.setup("coronaR", 1.95, 1, 10));
    
    specSplvDest = 0.55;
    gui.add(specSpalva.setup("specSplv", 0.55, 0, 1.0));

    nebulaRadiusDest = 1.0;
    gui.add(nebulaRadius.setup("nebulaR", 1.0, 0, 5.0));
    
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
    
    numGlowsToSpawn = 3;
    numNebulasToSpawn = 5;
    
    // VIDEO RECORDING //
    snapCounter = 0;
    bSnapshot = false;
    phase = 0;
    
    // PIXELATE IMAGE
    //pixelateImage = 80;    // argument is resulting pixel size
}

//--------------------------------------------------------------
void ofApp::update(){
  
    updateTime();
    
    // CONTROLLER
    if( getTick() ){
        // ADD GLOWS
        numGlowsToSpawn = 3;
        mController.addGlows( solarRadius*nebulaRadius*0.5, numGlowsToSpawn );
        
        // ADD NEBULAS
        numNebulasToSpawn = 5;
        //if( mRoom.isPowerOn() ) numNebulasToSpawn = (int)( 8 );//* mStar.mRadiusMulti );
        
        mController.addNebulas( solarRadius*nebulaRadius*0.5, /*mStar,*/ numNebulasToSpawn );
        
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
    
    visionFilterPart();
    // DEBUG - FPS
    ofSetColor(225);
    //ofFill();
    if (drawGui) {
        ofDrawBitmapString(ofToString(ofGetFrameRate()), ofPoint(ofGetWindowWidth() - 70, 20));
    }

    //ofDisableDepthTest();
    
    // DEBUG - FPS
    ofSetColor(225);
    //ofFill();
    ofDrawBitmapString(ofToString(ofGetFrameRate()), ofPoint(ofGetWindowWidth() - 70, 20));
    
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

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    switch( key ) {
        //case '0':	showGradient = !showGradient; break;
        case 'x':	bSnapshot = true; break;
            
        case '1':	showSphere = !showSphere; break;
        case '2':	showGradient = !showGradient; break;
        case '3':	showCorona = !showCorona; break;
        case '4':	showNebulas = !showNebulas; break;
        case '5':	showGlow = !showGlow; break;
            
        case '6':	showGlow = true; showGradient = true; showCorona = true; showNebulas = true; showSphere = true; break;
            
        case '0':	showGlow = true; showGradient = true; showCorona = true; showNebulas = true; showSphere = true;
            numGlowsToSpawn = 0;
            numNebulasToSpawn = 0;
            break;
            
        case 'g':   drawGui = !drawGui; break;
        case 'b':	blending = !blending; break;
            
        case 'R':
            // code
            starRadiusGuiDest = 100.25;
            coronaRadiusDest = 1.90;
            specSplvDest = 0.225;
            nebulaRadiusDest = 0.875;
            numGlowsToSpawn = 3;
            numNebulasToSpawn = 5;
            break;
        
        case 'Y':
            starRadiusGuiDest = 300.25;
            coronaRadiusDest = 1.95;
            specSplvDest = 0.55;
            nebulaRadiusDest = 1.0;
            numGlowsToSpawn = 4;
            numNebulasToSpawn = 8;
        break;
            
        case 'B':
            starRadiusGuiDest = 400.25;
            coronaRadiusDest = 1.95;
            specSplvDest = 0.91;
            nebulaRadiusDest = 1.0;
            numGlowsToSpawn = 15;
            numNebulasToSpawn = 15;
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
    ofPushMatrix();
    billboardBegin();
    
    ofSetColor(1.0, 1.0, 1.0, 1.0);
    coronaShader.begin();
    coronaShader.setUniformTexture("tex0", textureCorona.getTexture(), 0);
    coronaShader.setUniformTexture("tex1", textureSpectrum.getTexture(), 1);

    coronaShader.setUniform1f("radius", solarRadius);
    coronaShader.setUniform1f("coronaRadius", coronaRadius);
    coronaShader.setUniform4f("color", color);
    
    coronaShader.setUniform1f("specSpalva", specSpalva);

    ofDrawPlane(0, 0, 0, 1, 1);
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

void ofApp::visionFilterPart(){
    
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
        glDisable(GL_DEPTH_TEST);
    }
    
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
    camera.end();
    

    if (drawGui) {
        gui.draw();
    }

}
/*
void ofApp::pixelateImage(int pxSize) {
    
    // use ratio of height/width...
    float ratio;
    if (ofGetWindowWidth() < ofGetWindowHeight()) {
        ratio = ofGetWindowHeight()/ofGetWindowWidth();
    }
    else {
        ratio = ofGetWindowWidth()/ofGetWindowHeight();
    }
    
    // ... to set pixel height
    int pxH = int(pxSize * ratio);

    for (int x=0; x<ofGetWindowWidth(); x+=pxSize) {
        for (int y=0; y<ofGetWindowHeight(); y+=pxH) {
            //fill(p.get(x, y));
            //rect(x, y, pxSize, pxH);
        }
    }
}
*/