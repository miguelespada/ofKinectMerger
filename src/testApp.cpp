#include "testApp.h"
#include "ofxSimpleGuiToo.h"


bool keys[255];
float camZoom, camPosX, camPosY, camRotX, camRotY;
int pMouseX, pMouseY;
ofVec3f centroid;

ofxSimpleGuiTitle *status;

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
    
    mode = &calibrate;
   // mode = &snapshot;
    mode->setup();
    
    
    status = &gui.addTitle("STATUS");
    
    gui.addSlider("Zoom", camZoom, -20000, 10000).setSmoothing(0.9);
    gui.addSlider("camPosX", camPosX, -10000, 10000).setSmoothing(0.9);;
    gui.addSlider("camPosY", camPosY, -10000, 10000).setSmoothing(0.9);;
    gui.addSlider("camRotX", camRotX, 0, 360).setSmoothing(0.9);;
    gui.addSlider("camRotY", camRotY, 0, 360).setSmoothing(0.9);;
    gui.addToggle("Info", bInfo);
    gui.show();
    

}

//--------------------------------------------------------------
void testApp::update(){
    char msg[1024];
    
    mode->continousKeyPress(keys);

    mode->getStatus(msg);
    status->setName(msg);
    status->setSize(400, 120);
    mode->update();
  
}

//--------------------------------------------------------------
void testApp::draw(){
    centroid = mode->getCentroid();
    
	cam.setPosition(centroid + ofVec3f(0, 0,-5000));
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
    
	if(bInfo) gui.draw();
}

void testApp::keyPressed(int key){
    mode->keyPressed(key);
    
    switch (key ) {
        case 'i':
            bInfo = !bInfo;
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
    if(keys['1']) camZoom +=  (mouseX - pMouseX);
    if(keys['2']) {
        camPosX +=  (mouseX - pMouseX);
        camPosY +=  (mouseY - pMouseY);
    }
    if(keys['3']) {
        camRotY +=  (mouseX - pMouseX) / 10.;
        camRotX +=  (mouseY - pMouseY) / 10.;
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