#include "VideoResource.h"

//---------------------------------------------------------------------------

void VideoResource::setup()
{
    // init webcam and face tracker
    cam.initGrabber(640, 480);
    tracker.setup();
}

//---------------------------------------------------------------------------

void VideoResource::update()
{
    // update webcam capture and face tracking
    cam.update();
    if (cam.isFrameNew())
    {
        tracker.update(toCv(cam));
    }
    
    // get polyline and bounding box of outer mouth
    mouthOutPt = tracker.getImageFeature(ofxFaceTracker::OUTER_MOUTH);
    mouthBx = mouthOutPt.getBoundingBox();
    
    // get polyline of inner mouth
    mouthInPt = tracker.getImageFeature(ofxFaceTracker::INNER_MOUTH);
}

//---------------------------------------------------------------------------

void VideoResource::draw()
{
    //cam.draw(0, 0);
    //tracker.draw();
    
    // crop pixels of mouth
    ofPixels pixels;
    cam.getTextureReference().readToPixels(pixels);
    pixels.crop(mouthBx.x - MARGINX, mouthBx.y - MARGINY,
                mouthBx.width + MARGINX * 2, mouthBx.height + MARGINY * 1.5);
    
    // create image out of cropped pixels
    ofImage videoImg;
    videoImg.setFromPixels(pixels);
    
    // draw cropped image
    videoImg.draw(mouthBx.x - MARGINX, mouthBx.y - MARGINY);
}
