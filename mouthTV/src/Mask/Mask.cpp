#include "Mask.h"

//---------------------------------------------------------------------------

void Mask::setup()
{
    // load movie file
    sandVid.loadMovie("videos/sandstorm.mp4");
    colorVid.loadMovie("videos/colortest.mp4");
    
    // Uncomment this to show movies with alpha channels
    //sampleVid.setPixelFormat(OF_PIXELS_RGBA);
    
    // fbo setting
    maskFbo.allocate();
    fbo.allocate();
    
    // load shader
#ifdef TARGET_OPENGLES
    shader.load("shaders_gles/alphamask.vert","shaders_gles/alphamask.frag");
#else
    if (ofGetGLProgrammableRenderer())
    {
        string vertex = "#version 150\n\
        \n\
        uniform mat4 projectionMatrix;\n\
        uniform mat4 modelViewMatrix;\n\
        uniform mat4 modelViewProjectionMatrix;\n\
        \n\
        \n\
        in vec4  position;\n\
        in vec2  texcoord;\n\
        \n\
        out vec2 texCoordVarying;\n\
        \n\
        void main()\n\
        {\n\
        texCoordVarying = texcoord;\
        gl_Position = modelViewProjectionMatrix * position;\n\
        }";
        string fragment = "#version 150\n\
        \n\
        uniform sampler2DRect tex0;\
        uniform sampler2DRect maskTex;\
        in vec2 texCoordVarying;\n\
        \
        out vec4 fragColor;\n\
        void main (void){\
        vec2 pos = texCoordVarying;\
        \
        vec3 src = texture(tex0, pos).rgb;\
        float mask = texture(maskTex, pos).r;\
        \
        fragColor = vec4( src , mask);\
        }";
        shader.setupShaderFromSource(GL_VERTEX_SHADER, vertex);
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragment);
        shader.bindDefaults();
        shader.linkProgram();
    }
    else
    {
        string shaderProgram = "#version 120\n \
        #extension GL_ARB_texture_rectangle : enable\n \
        \
        uniform sampler2DRect tex0;\
        uniform sampler2DRect maskTex;\
        \
        void main (void){\
        vec2 pos = gl_TexCoord[0].st;\
        \
        vec3 src = texture2DRect(tex0, pos).rgb;\
        float mask = texture2DRect(maskTex, pos).r;\
        \
        gl_FragColor = vec4( src , mask);\
        }";
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderProgram);
        shader.linkProgram();
    }
#endif
    
    // refresh
    maskFbo.begin();
    ofClear(0);
    maskFbo.end();
    
    // refresh
    fbo.begin();
    ofClear(0);
    fbo.end();
}

//---------------------------------------------------------------------------

void Mask::update(float jaw, float mouthX,
                  float mouthY, vector<ofPoint> points, float thresh)
{
    // mouth open, play movie file
    if (jaw > thresh)
    {
        // set timer
        if (!isOpen)
        {
            timeSt = ofGetElapsedTimeMillis();
            isOpen = true;
            sandVid.play();
        }
        
        // timer update
        timeCu = ofGetElapsedTimeMillis();
        timeDiff = timeCu - timeSt;
        
        if (timeDiff < 5000)
        {
            // update video file play
            sandVid.update();
        }
        else
        {
            // change video file
            if (!isColor)
            {
                isColor = true;
                sandVid.stop();
                colorVid.play();
            }
            
            // update video file play
            colorVid.update();
        }
        
        // save contour to fbo
        maskFbo.begin();
        ofClear(0);
        ofPushMatrix();
        ofTranslate(-mouthX + MARGINX, -mouthY + MARGINY);
        // get vertex points of the inner mouth contour and create shape
        ofBeginShape();
        for (int i = 0; i < points.size(); i++)
        {
            if (i == 0)
            {
                ofCurveVertex(points[0].x, points[0].y);
                ofCurveVertex(points[0].x, points[0].y);
            }
            else if (i == points.size() - 1)
            {
                ofCurveVertex(points[i].x, points[i].y);
                ofCurveVertex(points[0].x, points[0].y);
                ofCurveVertex(points[0].x, points[0].y);
            }
            else
            {
                ofCurveVertex(points[i].x, points[i].y);
            }
        }
        ofEndShape();
        ofPopMatrix();
        maskFbo.end();
        
        // mask video with the contour
        fbo.begin();
        ofClear(0);
        shader.begin();
        shader.setUniformTexture("maskTex", maskFbo.getTextureReference(), 1);
        shader.setUniform1f("tex0", 0);
        ofEnableAlphaBlending();
        if (!isColor)
        {
            sandVid.draw(mouthX - MARGINX, mouthY - MARGINY);
        }
        else
        {
            colorVid.draw(mouthX - MARGINX, mouthY - MARGINY);
        }
        shader.end();
        fbo.end();
    }
    else
    {
        // reset when mouth closed
        isOpen = false;
        isColor = false;
        ofResetElapsedTimeCounter();
        sandVid.stop();
        colorVid.stop();
    }

}

//---------------------------------------------------------------------------

void Mask::draw()
{
    fbo.draw(0, 0);
}
