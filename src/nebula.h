#pragma once
#include "ofMain.h"
#include "ofVbo.h"

#define NUM_BILLBOARDS 1

class Nebula {

public:
    Nebula();
    Nebula( const ofVec3f &pos, const ofVec3f &vel, float radius, float lifespan );
    
    void update( float timeDelta );
    void draw( /*const ofVec3f &right, const ofVec3f &up*/ );
    
    ofVec3f	mPos, mVel;
    
    float		mRadius, mRadiusDest, mRadiusMulti;
    float		mRot;
    float		mAge, mAgePer;
    float		mLifespan;
    bool		mIsDead;
    
    ofVboMesh billboard;
    
    ofVec2f pos[NUM_BILLBOARDS];
    ofVec2f vel[NUM_BILLBOARDS];
    ofVec2f home[NUM_BILLBOARDS];
    float pointSizes[NUM_BILLBOARDS];
    float rotations[NUM_BILLBOARDS];
};