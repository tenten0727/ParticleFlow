#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0);
    ofDisableAlphaBlending();
    ofEnableArbTex();
    
    //recorder.setup(glm::vec2(WIDTH,HEIGHT));
    //  recorder.isRecording = !recorder.isRecording;
    
    //    cap.setup(WIDTH, HEIGHT);
    video.load("gokite1.mov");
    video.play();
    
    diffImage.allocate(WIDTH, HEIGHT, OF_IMAGE_COLOR);
    diff = cv::Mat(diffImage.getHeight(), diffImage.getWidth(), CV_MAKETYPE(CV_8UC3, diffImage.getPixels().getNumChannels()), diffImage.getPixels().getData(), 0);
    
    render.load("shaders/render");
    updatePos.load("","shaders/update.frag");
    
    particles.setMode(OF_PRIMITIVE_POINTS);
    for(int j=0;j<HEIGHT;j++){
        for(int i=0;i<WIDTH;i++){
            particles.addVertex(ofVec3f(0,0,0));
            particles.addTexCoord(ofVec2f(i, j));
            int rand = ofRandom(3);
            switch (rand) {
                case 0:
                    particles.addColor(ofFloatColor(0.1, 0.1, 0.1, 0.7));
                    break;
                case 1:
                    particles.addColor(ofFloatColor(0.2, 0.2, 0.7, 0.7));
                    break;
                case 2:
                    particles.addColor(ofFloatColor(ofRandomf()*0.7, ofRandomf()*0.7, ofRandomf()*0.7, 0.7));
                    break;
                default:
                    break;
            }
        }
    }
    
    pingPong.allocate(WIDTH, HEIGHT, GL_RGBA16F, 2);
    float * posAndAge = new float[WIDTH * HEIGHT * 4];
    for (int x = 0; x < WIDTH; x++){
        for (int y = 0; y < HEIGHT; y++){
            int i = WIDTH * y + x;
            posAndAge[i*4 + 0] = 0;
            posAndAge[i*4 + 1] = 0;
            posAndAge[i*4 + 2] = 0;
            posAndAge[i*4 + 3] = 0;
        }
    }
    pingPong.src->getTexture(0).loadData(posAndAge, WIDTH, HEIGHT, GL_RGBA);
    delete [] posAndAge;
    
    
    float * velAndMaxAge = new float[WIDTH * HEIGHT * 4];
    for (int x = 0; x < WIDTH; x++){
        for (int y = 0; y < HEIGHT; y++){
            int i = WIDTH * y + x;
            velAndMaxAge[i*4 + 0] = 0.0;
            velAndMaxAge[i*4 + 1] = -5.0;
            velAndMaxAge[i*4 + 2] = 0.0;
            velAndMaxAge[i*4 + 3] = ofRandom(10,50);
        }
    }
    pingPong.src->getTexture(1).loadData(velAndMaxAge, WIDTH, HEIGHT, GL_RGBA);
    delete [] velAndMaxAge;
    
    fbo.allocate(WIDTH, HEIGHT,GL_RGBA);
    server.setName("ParticleFlow");
    
    ofxSubscribeOsc(8001, "/recorded", [=](){
        isFrameRecorded = true;
    });
    
}

void ofApp::sceneUpdate(){
    video.update();
    ofImage src;
    if(b_video){
        src.setFromPixels(video.getPixels());
    }
    else{
        src.setFromPixels(cap.getPixels());
    }
    
    frame = ofxCv::toCv(src);
    if(ofGetFrameNum() > 2){
        diff = prevFrame - frame;
    }
    frame.copyTo(prevFrame);
    diffImage.update();
    
    pingPong.dst->begin();
    ofClear(0,255);
    updatePos.begin();
    updatePos.setUniformTexture("u_posAndAgeTex", pingPong.src->getTexture(0), 0);
    updatePos.setUniformTexture("u_velAndMaxAgeTex", pingPong.src->getTexture(1), 1);
    updatePos.setUniformTexture("u_diffTex", diffImage.getTexture(), 2);
    updatePos.setUniform1f("u_time", time);
    pingPong.src->draw(0, 0,WIDTH,HEIGHT);
    updatePos.end();
    
    pingPong.dst->end();
    pingPong.swap();
    
    fbo.begin();
    ofClear(0);
    video.draw(0, 0, WIDTH, HEIGHT);
    render.begin();
    render.setUniformTexture("u_posAndAgeTex", pingPong.src->getTexture(0), 0);
    updatePos.setUniformTexture("u_velAndMaxAgeTex", pingPong.src->getTexture(1), 1);
    updatePos.setUniform1f("u_time", time);
    particles.draw();
    render.end();
    fbo.end();
}


//--------------------------------------------------------------
void ofApp::update(){
    time = currentFrameNum/60.0;
    
    if(!isFrameSent&&!isFrameRecorded){
        sceneUpdate();
        ofxPublishOsc("localhost", 8000, "/update", true);
        isFrameSent = true;
    }
    if(isFrameSent&&!isFrameRecorded){
        //フレーム送ったけど録画されたかわからないので待機
    }
    if(isFrameSent&&isFrameRecorded){
        isFrameSent = false;
        isFrameRecorded = false;
        // リセット
    }
    
    
    //  ofSetWindowTitle(ofToString(ofGetFrameNum()));
    
    // cap.update();
    
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    fbo.draw(0, 0);
    //recorder.record(fbo);
    server.publishTexture(&fbo.getTexture());
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'a'){
        // recorder.isRecording = !recorder.isRecording;
    }
}
