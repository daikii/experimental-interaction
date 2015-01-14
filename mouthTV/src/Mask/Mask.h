#pragma once

#ifndef mouthTV_Mask_h
#define mouthTV_Mask_h

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxFaceTracker.h"

#include "VideoResource.h"


class Mask
{
public:
    void setup();
    void update(float jaw, float mouthX, float mouthY, vector<ofPoint> points, float thresh);
    void draw();
    
private:
    ofVideoPlayer sandVid;
    ofVideoPlayer colorVid;
    
    ofShader shader;
    ofFbo fbo;
    ofFbo maskFbo;
    
    bool isOpen = false;
    bool isColor = false;
    
    // timer
    double timeSt;
    double timeCu;
    double timeDiff;
};

#endif