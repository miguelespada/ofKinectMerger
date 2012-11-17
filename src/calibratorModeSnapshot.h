//
//  calibratorModeSnapshot.h
//  kinectCalibrator
//
//  Created by miguel valero espada on 11/16/12.
//
//


//
//  calibratorModeCalibrate.h
//  kinectCalibrator
//

#include "testApp.h"
#include "calibratorMode.h"
//#include "ofxOpenNI.h"


class calibratorModeSnapshot : public calibratorMode {
private:
    
	//ofxOpenNI openNIDevice;
    public:
    
    void setup(){
      /*  openNIDevice.setup();
        openNIDevice.addImageGenerator();
        openNIDevice.addDepthGenerator();
        openNIDevice.setRegister(true);
        openNIDevice.setMirror(true);
        openNIDevice.addUserGenerator();
        openNIDevice.setMaxNumUsers(3);
        openNIDevice.start();
        
        // set properties for all user masks and point clouds
        //openNIDevice.setUseMaskPixelsAllUsers(true); // if you just want pixels, use this set to true
        openNIDevice.setUseMaskTextureAllUsers(true); // this turns on mask pixels internally AND creates mask textures efficiently
        openNIDevice.setUsePointCloudsAllUsers(true);
        openNIDevice.setPointCloudDrawSizeAllUsers(1); // size of each 'point' in the point cloud
        openNIDevice.setPointCloudResolutionAllUsers(2); // resolution of the mesh created for the point cloud eg., this will use every second depth pixel
       */
    }
    void update(){
        //openNIDevice.update();
        
    }
    void draw(){
               
      /*  ofPushMatrix();
        // use a blend mode so we can see 'through' the mask(s)
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        
        // get number of current users
        int numUsers = openNIDevice.getNumTrackedUsers();
        
        // iterate through users
        for (int i = 0; i < numUsers; i++){
            
            // get a reference to this user
            ofxOpenNIUser & user = openNIDevice.getTrackedUser(i);
            
            // draw the mask texture for this user
            user.drawMask();
            
            // you can also access the pixel and texture references individually:
            
            // TEXTURE REFERENCE
            //ofTexture & tex = user.getMaskTextureReference();
            // do something with texture...
            
            // PIXEL REFERENCE
            //ofPixels & pix = user.getMaskPixels();
            // do something with the pixels...
            
            // and point clouds:
            
            ofPushMatrix();
            // move it a bit more central
            ofTranslate(320, 240, 10);
            user.drawPointCloud();
            
            // you can also access the mesh:
            
            // MESH REFERENCE
            //ofMesh & mesh = user.getPointCloud();
            // do something with the point cloud mesh
            
            ofPopMatrix();
            
        }
        
        ofDisableBlendMode();
        ofPopMatrix();
              */
    }
    ofVec3f getCentroid(){
    }
    void stop(){
        
    }
    void keyPressed(int key){
        
    }
    
};


