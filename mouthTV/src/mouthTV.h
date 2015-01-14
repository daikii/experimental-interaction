#pragma once

// addons
#include "ofMain.h"
#include "ofxCv.h"
#include "ofxFaceTracker.h"

// custom class
#include "VideoResource.h"
#include "Mask.h"

#define THRESH 24


class mouthTV : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    
private:
    VideoResource video;
    Mask mask;
    
    float jaw;
    float mouthX;
    float mouthY;
    vector<ofPoint> points;
};
