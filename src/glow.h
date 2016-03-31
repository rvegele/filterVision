#pragma once
#include "ofMain.h"
#include "ofVbo.h"

#define NUM_BILLBOARDS 1

class Glow {
public:
    
    Glow();
    Glow( const ofVec3f &pos, const ofVec3f &vel, float radius, float lifespan );
    void update( float timeDelta );
    void draw( /*const ofVec3f &right, const ofVec3f &up*/ );
    
    ofVec3f	mPos;
    ofVec3f	mVel;
    float		mRot, mRotVel;
    float		mRadius, mRadiusDest;
    float		mAge, mAgePer, mLifespan;
    float		mColor;
    bool		mIsDead;
    
    float randPosNegFloat(float a, float b);
    bool getRandomBoolean() ;
    
    ofVboMesh billboard;
    
    ofVec2f pos[NUM_BILLBOARDS];
    ofVec2f vel[NUM_BILLBOARDS];
    ofVec2f home[NUM_BILLBOARDS];
    float pointSizes[NUM_BILLBOARDS];
    float rotations[NUM_BILLBOARDS];
};