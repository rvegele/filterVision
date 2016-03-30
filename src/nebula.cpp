#include "nebula.h"
#include "ofVbo.h"

Nebula::Nebula() {

}

Nebula::Nebula( const ofVec3f &pos, const ofVec3f &vel, float radius, float lifespan )
{
    billboard.setUsage( GL_DYNAMIC_DRAW );
    billboard.setMode(OF_PRIMITIVE_POINTS);
    
    billboard.getNormals().resize(1,ofVec3f(0));
    billboard.getVertices().resize(1);
    
    mPos			= pos;
    mVel			= vel;
    mRadiusDest		= radius;
    mRadius			= radius;
    mAge			= 0.0f;
    mLifespan		= ofRandom( lifespan * 0.5f, lifespan );
    mRadiusMulti	= 3.0f;
    mRot			= ofRandom( 360.0f );
    mAgePer			= 0.0f;
    mIsDead			= false;
    
    billboard.getVertices()[0].set(mVel);
    billboard.setVertex(0, mPos);
    
    //billboard.setTexCoord(0, ofVec2f(ofRandom(0, 100), ofRandom(0, 100))4);
    //billboard.setNormal(0, ofVec3f( ofRandom(0, 100), ofRandom(0, 100), ofRandom(0, 100)));
    //billboard.setNormal(0,ofVec3f(0,0,mRot));
    
    //billboard.getVertices().resize(NUM_BILLBOARDS);
    //billboard.getNormals().resize(NUM_BILLBOARDS,ofVec3f(0));
    
    //billboard.getVertices().set(mPos);
    //billboard.getVertices().set(ofRandom(-500, 500),
    //                                ofRandom(-500, 500),
    //                                ofRandom(-500, 500));
    //billboard.setVertexData(mPos, 1, GL_DYNAMIC_DRAW);
    //billboard.setVertexData(mPos, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);
}

void Nebula::update( float dt )
{
    mPos	+= mVel * dt;
    billboard.setVertex(0, mPos);
    
    mVel	-= mVel * 0.01 * dt;
    billboard.getVertices()[0] -= mVel;
    
    mRadiusDest += mRadiusMulti * dt;
    mRadiusMulti -= mRadiusMulti * 0.03f * dt;
    mRadius -= ( mRadius - mRadiusDest ) * 0.1f * dt;
    
    //billboard.setNormal(0, ofVec3f(mRadius,0,0));
    
    mAge += dt;
    mAgePer = 1.0f - mAge/mLifespan;//sin( ( mAge/mLifespan ) * M_PI );
    
    if( mAge > mLifespan ){
        mIsDead = true;
    }
}

void Nebula::draw( /*const ofVec3f &right, const ofVec3f &up*/ ) {
    ofPushMatrix();
    ofRotate(mRot);
    billboard.draw();
    //ofDrawPlane(mPos.x, mPos.y, mPos.z, mRadius, mRadius);
    //gl::drawBillboard( mPos, ofVec2f( mRadius, mRadius ), mRot, right, up );
    ofPopMatrix();
}
