#include "ofApp.h"
#include "ofxOsc.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // disable rect textures, use 2D instead (normalised coordinates)
    ofDisableArbTex();
    
    ofSetFrameRate(30);
    
    // osc receiver setup
    receiver.setup(57120);
    
    // load shader into project
    shader.load("shaderVert.c", "shaderFrag.c");
    
    // allocate the framebuffer
    mainFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA, 4);
    camFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA, 4); // fbo.allocate(width, height, color_mode, num_samples)
    wifiFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA, 4);
    
    // list of webcams to grab
    ipcams = ofx::Video::IpVideoGrabberSettings::fromFile("streams.json");
    
    reloadCameras();
}

ofx::Video::IpVideoGrabberSettings& ofApp::getNextCamera() {
    nextCamera = (nextCamera + 1) % ipcams.size();
    return ipcams[nextCamera];
}

void ofApp::reloadCameras() {
    grabbers.clear();
    
    for (std::size_t i = 0; i < numCameras; ++i) {
        auto c = std::make_shared<ofx::Video::IPVideoGrabber>();
        auto& settings = getNextCamera();
        c -> setURI(settings.getURL());
        c -> connect();
        
        grabbers.push_back(c);
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    for (auto& grabber : grabbers) {
        grabber -> update();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0, 0, 0);
    
    ofSetHexColor(0xFFFFFF);
    
    // camera related
    std::size_t row = 0;
    std::size_t col = 0;
    
    float x = 0;
    float y = 0;
    
    float w = ofGetWidth() / numCameraColumns;
    float h = ofGetHeight() / numCameraRows;
    
    // osc related
    ofxOscMessage m;
    string message;
    
    while (receiver.hasWaitingMessages()) {
        receiver.getNextMessage(m);
        message = m.getArgAsString(0);
        messages.push_back(message);
        if (messages.size() > 20) {
            messages.pop_front();
        }
    }
    
    // start rendering into the framebuffer - cameras
    camFbo.begin();
    
    // clear buffer in case of artefacts
    ofClear(255, 255, 255, 0);
    
    // draw the cameras in row and col
    for (std::size_t i = 0; i < grabbers.size(); ++i) {
        x = col * w;
        y = row * h;
        
        row = (row + 1) % numCameraRows;
        
        if (row == 0) {
            col = (col + 1) % numCameraColumns;
        }
        
        ofPushMatrix();
        ofTranslate(x, y);
        ofSetColor(255, 255, 255, 255);
        grabbers[i] -> draw(0, 0, w, h);
        
        ofPopMatrix();
    }
    
    camFbo.end();
    
    wifiFbo.begin();
    
    // clear buffer in case of artefacts
    ofClear(255, 255, 255, 0);
    
    while (receiver.hasWaitingMessages()) {
        receiver.getNextMessage(m);
        message = m.getArgAsString(0);
        messages.push_back(message);
        if (messages.size() > 20) {
            messages.pop_front();
        }
    }
    
    ofPushMatrix();
    ofTranslate(ofGetWidth()/3, ofGetHeight()/3);
    for(int i = 0; i < messages.size(); i++) {
        ofDrawBitmapString(messages[i], 0, i * 20);
    }
    ofPopMatrix();
    
    wifiFbo.end();
    
    // start the shader
    shader.begin();
    
    shader.setUniformTexture("camTex", camFbo.getTexture(0), 1); // bind texture to buffer 1
    shader.setUniformTexture("wifiTex", wifiFbo.getTexture(0), 2); // bind texture to buffer 2
    
    // setting up uniforms
    float time = ofGetElapsedTimef();
    shader.setUniform1f("time", time);

    int resolution[2] = {ofGetWidth(), ofGetHeight()};
    shader.setUniform3iv("resolution", resolution);
    
    // blend the two fbos together
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
//    camFbo.draw(0, 0);
//    wifiFbo.draw(0, 0);
//    glDisable(GL_BLEND);
    
    mainFbo.draw(0, 0); // draw the mainFBO combining the fbos
    
    shader.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
