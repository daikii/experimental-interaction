#pragma once

#ifndef mouthTV_VideoResource_h
#define mouthTV_VideoResource_h

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxFaceTracker.h"

using namespace ofxCv;

// extra margin for cropping mouth
#define MARGINX 20
#define MARGINY 10


class VideoResource
{
public:
    void setup();
    void update();
    void draw();
    
    ofVideoGrabber cam;
    ofxFaceTracker tracker;
    
    ofPolyline mouthOutPt;
    ofRectangle mouthBx;
    ofPolyline mouthInPt;
};

#endif
