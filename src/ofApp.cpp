#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0);
    ofDisableAlphaBlending();
    ofEnableArbTex();

    
    cap.setup(ofGetWidth(), ofGetHeight());
    video.load("goki2.mov");
    video.play();
    
    diffImage.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);
    diff = cv::Mat(diffImage.getHeight(), diffImage.getWidth(), CV_MAKETYPE(CV_8UC3, diffImage.getPixels().getNumChannels()), diffImage.getPixels().getData(), 0);
    
    render.load("shaders/render");
    updatePos.load("","shaders/update.frag");
    
    
    particles.setMode(OF_PRIMITIVE_POINTS);
    for(int j=0;j<ofGetHeight();j++){
        for(int i=0;i<ofGetWidth();i++){
                particles.addVertex(ofVec3f(0,0,0));
                particles.addTexCoord(ofVec2f(i, j));
                particles.addColor(ofFloatColor(1.0, 1.0, 1.0, 1.0));
        }
    }
    
    pingPong.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F, 2);
    float * posAndAge = new float[ofGetWidth() * ofGetHeight() * 4];
    for (int x = 0; x < ofGetWidth(); x++){
        for (int y = 0; y < ofGetHeight(); y++){
            int i = ofGetWidth() * y + x;
            posAndAge[i*4 + 0] = 0;
            posAndAge[i*4 + 1] = 0;
            posAndAge[i*4 + 2] = 0;
            posAndAge[i*4 + 3] = 0;
        }
    }
    pingPong.src->getTextureReference(0).loadData(posAndAge, ofGetWidth(), ofGetHeight(), GL_RGBA);
    delete [] posAndAge;
    
    
    float * velAndMaxAge = new float[ofGetWidth() * ofGetHeight() * 4];
    for (int x = 0; x < ofGetWidth(); x++){
        for (int y = 0; y < ofGetHeight(); y++){
            int i = ofGetWidth() * y + x;
            velAndMaxAge[i*4 + 0] = 0.0;
            velAndMaxAge[i*4 + 1] = -5.0;
            velAndMaxAge[i*4 + 2] = 0.0;
            velAndMaxAge[i*4 + 3] = ofRandom(10,50);
        }
    }
    pingPong.src->getTextureReference(1).loadData(velAndMaxAge, ofGetWidth(), ofGetHeight(), GL_RGBA);
    delete [] velAndMaxAge;

    fbo.allocate(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::update(){
    float time = ofGetElapsedTimef();

    cap.update();
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
    
    updatePos.begin();
    updatePos.setUniformTexture("u_posAndAgeTex", pingPong.src->getTextureReference(0), 0);
    updatePos.setUniformTexture("u_velAndMaxAgeTex", pingPong.src->getTextureReference(1), 1);
    updatePos.setUniformTexture("u_diffTex", diffImage.getTexture(), 2);
    updatePos.setUniform1f("u_time", time);
    pingPong.src->draw(0, 0);
    updatePos.end();
    
    pingPong.dst->end();
    pingPong.swap();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(b_video){
        video.draw(0, 0);
    }
    else{
        cap.draw(0, 0);
    }
    
    ofEnableAlphaBlending();
    render.begin();
    render.setUniformTexture("u_posAndAgeTex", pingPong.src->getTextureReference(0), 0);
    updatePos.setUniformTexture("u_velAndMaxAgeTex", pingPong.src->getTextureReference(1), 1);
    particles.draw();

    render.end();
    ofDisableAlphaBlending();
    if(texDraw){
        diffImage.draw(0, 0);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 't'){
        texDraw = !texDraw;
    }
    if(key == 'v'){
        b_video = !b_video;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
