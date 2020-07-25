#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"
#include "ofxGui.h"
#include "pingPongBuffer.h"
#include "ofxSequenceRecorder.h"
#include "ofxSyphon.h"
#include "ofxPubSubOsc.h"



#define WIDTH 1920
#define HEIGHT 1080

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    
private:
    cv::Mat prevFrame, frame, diff;
    ofImage diffImage;
    ofFbo fbo;
    
    ofVideoGrabber cap;
    ofVideoPlayer video;
    
    ofShader render, updatePos;
    ofVboMesh particles;
    pingPongBuffer pingPong;
    
    bool texDraw = false;
    bool b_video = true;
    
    float time;
    
    ofxSequenceRecorder recorder;
    ofxSyphonServer server;
    
};
