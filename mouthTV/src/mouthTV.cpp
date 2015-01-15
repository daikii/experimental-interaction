#include "mouthTV.h"

using namespace ofxCv;

//---------------------------------------------------------------------------

void mouthTV::setup()
{
    // init display
    ofBackground(0);
    ofSetLineWidth(2);
    
    // frame change responds accordingly
    ofSetVerticalSync(true);
    
    // webcam and face tracker setup
    video.setup();

    // shader and fbo setup for masking video
    mask.setup();
}

//---------------------------------------------------------------------------

void mouthTV::update()
{
    // webcam and face tracker update
    video.update();
    
    // variable updates
    jaw = video.tracker.getGesture(ofxFaceTracker::JAW_OPENNESS);
    mouthX = video.mouthBx.x;
    mouthY = video.mouthBx.y;
    // get vertex points of the inner mouth contour
    points = video.mouthInPt.getVertices();
    
    // shader and fbo masking
    mask.update(jaw, mouthX, mouthY, points, THRESH);
}

//---------------------------------------------------------------------------

void mouthTV::draw()
{
    // display webcam frames and tracking contour
    video.draw();
    
    // fps on top-left
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);

    // execute masking when mouth open
    if (jaw > THRESH)
    {
        mask.draw();
    }

    //ofLog() << video.tracker.getGesture(ofxFaceTracker::JAW_OPENNESS);
}

//---------------------------------------------------------------------------

void mouthTV::keyPressed(int key)
{
    // reset face tracking
	if (key == 'r')
    {
		video.tracker.reset();
	}
}
