#include "glow.h"
#include "ofVbo.h"

#include <random>

Glow::Glow() {
}

Glow::Glow( const ofVec3f &pos, const ofVec3f &vel, float radius, float lifespan ) {
    
    billboard.setUsage( GL_DYNAMIC_DRAW );
    //billboard.usingNormals();
    
    billboard.setMode(OF_PRIMITIVE_POINTS);
    
    billboard.getNormals().resize(1,ofVec3f(0));
    billboard.getVertices().resize(1);
    
    mPos		= pos;
    mVel		= vel;
    mRadiusDest	= radius * 0.75;
    mRadius		= radius;// * 1.25;
    mAge		= 0.0f;
    mLifespan	= lifespan;
    mIsDead		= false;
    mRot		= ofRandom( 360.0f );
    mRotVel		= ofRandom( -10.0f, 10.0f );
    
    if( ofRandomf() < 0.01f ){
        mRadius *= 2.0f;
        mLifespan *= 1.3f;
        mRotVel	= randPosNegFloat( 1.0f, 3.0f );
    }
    
    //billboard.o
    billboard.getVertices()[0].set(mVel);
    billboard.setVertex(0, mPos);
}

void Glow::update( float dt )
{
    mRot	+= mRotVel * 0.10;

    mPos	+= mVel * dt;
    billboard.setVertex(0, mPos);
    
    mVel	-= mVel * 0.02 * dt;
    billboard.getVertices()[0] -= mVel;
    
    mAge	+= dt;
    mAgePer  = 1.0f - mAge/mLifespan;
        //billboard.addNormal(const ofVec3f &n);
    //billboard.makeRotate(mRot, ofVec3f(1, 0, 0));
    //billboard.setNormal(0, <#const ofVec3f &n#>)
    //mRadius *= 0.99f;
    mRadius -= ( mRadius - mRadiusDest ) * 0.1f * dt;
    
    if( mAge > mLifespan ){// || mRadius < 1.0f ){
        mIsDead = true;
        //				mRadius = 1.0f;
    }
}

void Glow::draw( ) {
    billboard.draw();
}

float Glow::randPosNegFloat( float a, float b )
{
    if( getRandomBoolean() )
        return ofRandom( a, b );
    else
        return -ofRandom( a, b );
}

bool Glow::getRandomBoolean()
{
    return ofRandomf() < 0.5;
}

