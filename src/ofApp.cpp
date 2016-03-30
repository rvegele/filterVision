#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    
    ofBackground(30, 30, 30);
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
#else
    if(ofIsGLProgrammableRenderer()){
        shader.load("Sphere.vert", "Sphere.frag");
        shaderPlane.load("Sphere.vert","Sphere.frag");
        coronaShader.load("corona.vert","corona.frag");
        nebulaShader.load("nebula.vert","nebula.frag");
    }else{
        shader.load("Sphere.vert", "Sphere.frag");
        shaderPlane.load("gradient.vert","gradient.frag");
        coronaShader.load("corona.vert","corona.frag");
        nebulaShader.load("nebula.vert","nebula.frag");
    }
#endif
    
    textureGradient.load("grid.png");
    textureCorona.load("corona.png");
    textureSpectrum.load("spectrum.png");
    textureNebula.load("nebula.png");
    textureSmallGrid.load("smallGrid.png");
    
    
    gui.setup(); // most of the time you don't need a name
    
    gui.add(solarRadius.setup("radius", 150, 1, 200));
    gui.add(color.setup("color",
                        ofFloatColor(0.92, 0.82, 0.34),
                        ofFloatColor(0.0,0.0),
                        ofFloatColor(1.0,1.0)));
    gui.add(clmpMin.setup("clamp Min", 0.0, 0.0, 1.0));
    gui.add(clmpMax.setup("clamp Max", 0.85, 0.0, 1.0));
    gui.add(dtt.setup("dot mult", 0.75, 0.0, 1.0));
    
    gui.add(coronaRadius.setup("coronaR", 3.975, 1, 10));
    gui.add(specSpalva.setup("specSplv", 0.65, 0, 1.0));
    gui.add(nebulaRadius.setup("nebulaR", 100, 0, 200));
    
    gui.add(randomas.setup("randomas", 134, 0, 200));
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
}

//--------------------------------------------------------------
void ofApp::update(){
    
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
        //int numGlowsToSpawn = 4;
        //if( mRoom.isPowerOn() ) numGlowsToSpawn = (int)( 24 );//* mStar.mRadiusMulti );
        //mController.addGlows( mStar, mRoom.getPower(), numGlowsToSpawn );
        
        // ADD NEBULAS
        int numNebulasToSpawn = 18;
        //if( mRoom.isPowerOn() ) numNebulasToSpawn = (int)( 8 );//* mStar.mRadiusMulti );
        
        mController.addNebulas( solarRadius, /*mStar,*/ numNebulasToSpawn );
        
        // ADD DUSTS
        //int numDustsToSpawn = 500;
        //mController.addDusts( mStar, numDustsToSpawn );
    }
    
        mController.update( getTimeDelta() );
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //ofEnableBlendMode(OF_BLENDMODE_ADD);
    //ofEnableDepthTest();
    
    
    
    // VISION FILTER BEGIN
        if (filter == 2) {
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
    // VISION FILTER END
    
    
    
    //ofDisableDepthTest();
    
    // DEBUG - FPS
    ofSetColor(225);
    ofFill();
    ofDrawBitmapString(ofToString(ofGetFrameRate()), ofPoint(ofGetWindowWidth() - 70, 20));
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
    
        nebulaShader.begin();
    
        nebulaShader.setUniformTexture("tex0", textureNebula.getTexture(), 0);
        nebulaShader.setUniformTexture("tex1", textureSpectrum.getTexture(), 1);
        nebulaShader.setUniformTexture("tex2", textureSmallGrid.getTexture(), 2);
    
        nebulaShader.setUniform1f("specSpalva", specSpalva);
        nebulaShader.setUniform1f("radius", solarRadius/* * 0.025*/);
        nebulaShader.setUniform1f("nebulaRadius", nebulaRadius);
        nebulaShader.setUniform1f("randomas", randomas);
        //Vec3f right = Vec3f::xAxis();
        //Vec3f up	= Vec3f::yAxis();
        //if( mBillboard ) mSpringCam.mCam.getBillboardVectors( &right, &up );
    
        ofEnablePointSprites();
        mController.drawNebulas( &nebulaShader/*, right, up*/ );
        ofDisablePointSprites();
        //mNebulaShader.unbind();
    
        //ofDrawPlane(0, 0, 0, 100, 100);
    
    
        nebulaShader.end();
    
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
            if (filterCoordinates)      { filter = 1; }
            else if (filterVisible)      { filter = 2; }
            else if (filterXraySound)   { filter = 3; }
            else if (filterComposition) { filter = 4; }
            else                        { filter = 0; }
            cout << "Filter: " << filter << "\n";
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