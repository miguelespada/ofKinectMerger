#include "testApp.h"
#include "ofxSimpleGuiToo.h"


bool keys[255];
float camZoom, camPosX, camPosY, camRotX, camRotY;
int pMouseX, pMouseY;
ofVec3f centroid;

ofxSimpleGuiTitle *status;
bool bLoadMLP, bLoadSeq, bSimulation, bSwap, bNext, bPrev;

void drawAxes(ofVec3f center){
    ofPushMatrix();
    ofTranslate(center);
    ofPushStyle();
    ofSetColor(255, 0, 0);
    ofLine(0, 0, 0, 200, 0, 0);
    
    ofSetColor(0, 255, 0);
    ofLine(0, 0, 0, 0, 200, 0);
    
    ofSetColor(0, 0, 255);
    ofLine(0, 0, 0, 0, 0, 200);
    ofPopStyle();
    ofPopMatrix();
}

void pivot(ofVec3f center, float aX, float aY, float aZ){
    ofTranslate(center);
    ofRotateX(aX);
    ofRotateY(aY);
    ofRotateZ(aZ);
    ofTranslate(-center);
}

//--------------------------------------------------------------
void testApp::setup(){
    
    ofBackground(0);
    ofSetFrameRate(60);
    
    mode = new calibratorModeCalibrate();
    mode->setup();
    gui.setup();
	gui.addTitle("KINECT MERGER \n[i] hide controls");
    status = &gui.addTitle("STATUS");
    gui.addButton("Load MeshLab File", bLoadMLP);
    gui.addButton("Load PLY sequences", bLoadSeq);
    gui.addButton("Simulation/MeshLab", bSimulation);
    gui.addSlider("Zoom", camZoom, -5000, 5000).setSmoothing(0.9);
    gui.addSlider("camPosX", camPosX, -200, 200).setSmoothing(0.9);;
    gui.addSlider("camPosY", camPosY, -200, 200).setSmoothing(0.9);;
    gui.addSlider("camRotX", camRotX, 0, 360).setSmoothing(0.9);;
    gui.addSlider("camRotY", camRotY, 0, 360).setSmoothing(0.9);;
    gui.addToggle("Merge", mode->bSaving);
    gui.addButton("Swap matrix", bSwap);
    gui.addToggle("Play", mode->bAutoplay);
    gui.addButton("Next Frame", bNext);
    gui.addButton("Prev Frame", bPrev);
    
	gui.loadFromXML();
    gui.show();
}

//--------------------------------------------------------------
void testApp::update(){
    char msg[1024];

    mode->getStatus(msg);
    status->setName(msg);
    status->setSize(300, 100);
    mode->update();
    if(bLoadMLP){
        bLoadMLP = false;
        mode->openMLP();
    }
    if(bLoadSeq){
        bLoadSeq = false;
        mode->openSequenceFolders();
    }
    if(bSwap){
        bSwap = false;
        mode->toogleSwapMatrix();
    }
    if(bNext){
        bNext = false;
        mode->nextFrame();
    }
    if(bPrev){
        bPrev = false;
        mode->prevFrame();
    }
  
}

//--------------------------------------------------------------
void testApp::draw(){
    centroid = ofVec3f(0, 0, 4000);
    
	cam.setPosition(ofVec3f(0, 0,-2000));
    cam.lookAt(centroid, ofVec3f(0,1,0));
    cam.setFarClip(50000);
    
    cam.begin();
    
    ofPushMatrix();
    ofTranslate(camPosX, camPosY, camZoom);
    
    pivot(centroid, camRotX, camRotY, 0);
    drawAxes(centroid);
    
    mode->draw();
    
    ofPopMatrix();

    cam.end();
    
    gui.draw();
    
}

void testApp::keyPressed(int key){
    
    switch (key ) {
        case 'i':
            gui.toggleDraw();
            break;  
        default:
            break;
    }
    if(key > 0 && key <255)
        keys[key] = true;
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
   
    if(key > 0 && key <255)
        keys[key] = false;
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    if(keys['1']) camZoom -=  (mouseX - pMouseX) * 3;
    if(keys['2']) {
        camPosX -=  (mouseX - pMouseX) *2;
        camPosY -=  (mouseY - pMouseY) *2;
    }
    if(keys['3']) {
        camRotY -=  (mouseX - pMouseX) / 10.;
        camRotX -=  (mouseY - pMouseY) / 10.;
    }
    
    pMouseX = mouseX;
    pMouseY = mouseY;

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    pMouseX = mouseX;
    pMouseY = mouseY;

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}