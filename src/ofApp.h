#pragma once

#include "ofMain.h"
#include "IPVideoGrabber.h"
#include "ofxOsc.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
        ofShader shader;
        ofFbo mainFbo;
        ofFbo camFbo;
        ofFbo wifiFbo;
    
        ofxOscReceiver receiver;
        deque<string> messages;

        std::vector<std::shared_ptr<ofx::Video::IPVideoGrabber>> grabbers;
    
        void reloadCameras();
        ofx::Video::IpVideoGrabberSettings& getNextCamera();
        std::vector<ofx::Video::IpVideoGrabberSettings> ipcams;
        std::size_t nextCamera = 0;
    
        std::size_t numCameras = 9;
        std::size_t numCameraRows = 3;
        std::size_t numCameraColumns = 3;
    
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
};
