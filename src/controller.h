#pragma once
#include "ofMain.h"
#include "nebula.h"
#include "glow.h"

#include <vector>

class Controller {
public:
    //struct DustVertex {
    //    ci::Vec3f vertex;
    //    ci::Vec4f color;
    //};
    
    //struct Dust {
    //    Dust( const ci::Vec3f &pos, const ci::Vec3f &vel )
    //    : mPos( pos ), mVel( vel )
    //    {
    //        mPosInit		= mPos;
    //        mLifespanInit	= ci::Rand::randFloat( 5.0f, 12.0f );
    //        mLifespan		= mLifespanInit;
    //        mInvLifespan	= 0.1f;
    //        mAge			= 0.0f;
    //        mCol			= ci::ColorA( 1.0f, 1.0f, 1.0f, 1.0f );
    //    }
    //
    //    void update( float dt ){
    //        mPos	+= mVel * dt;
    //        mAge	+= dt;
    //
    //        if( mAge > mLifespan ){
    //            mLifespan += 10.0f;
    //            mPos = mPosInit;
    //        }
    //        mAgePer = 1.0 - ( mAge - mLifespan ) * mInvLifespan;
    //
    //        mCol	= ci::ColorA( 1.0f, 1.0f, 1.0f, mAgePer * 0.15f );
    //    }
    //
    //    ci::Vec3f mPosInit, mPos, mVel;
    //    ci::ColorA mCol;
    //    float mAge, mLifespanInit, mLifespan, mInvLifespan, mAgePer;
    //    bool mIsDead;
    //};
    
    Controller();
    void update( float dt );
    
    //void updateDusts( float dt );
    void drawGlows( ofShader *shader/*, const ofVec3f &right, const ci::Vec3f &up */);
    void drawNebulas( ofShader *shader/*, const ofVec3f &right, const ofVec3f &up*/);
    //void drawDusts();
    
    void addGlows( float solarRadius,/*const Star &star, float power, */int amt );
    void addNebulas( float solarRadius,/*const Star &star,*/ int amt );
    //void addDusts( const Star &star, int amt );
    
    // CANIS MAJORIS
    //void addCMNebulas( const ci::Vec3f &starPos, float starRadius, float radiusMulti, int amt );
    //void addCMGlows( const ci::Vec3f &starPos, float starRadius, int amt );
    
    std::vector<Glow>	mGlows;
    std::vector<Nebula>	mNebulas;
    //std::vector<Dust>	mDusts;
    //int					mTotalDustVerts;
    //int					mPrevTotalDustVerts;
    //DustVertex			*mDustVerts;
    
    ofVec3f randVec3f();
};