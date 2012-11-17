//
//  calibratorModeCalibrate.h
//  kinectCalibrator
//
//  Created by miguel valero espada on 11/16/12.
//
//
#include <iterator>
#include <iostream>
#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "calibratorMode.h"
#include "kinectData.h"

class calibratorModeCalibrate : public calibratorMode {
    private:
    
    kinectData kinect0, kinect1;
    ofMatrix4x4 M0, M1;
    
    ofxXmlSettings MLP;
    ofxXmlSettings XML;
    
    string k0File, k1File, matrixFile, path, seq0Folder, seq1Folder;
    bool bSeq, bAutoplay, bNewFrame;
    int frame, nFrames;
    
    ofMatrix4x4 parseMatrix(string s){
        // If possible, always prefer std::vector to naked array
        std::vector<float> v;
        
        // Build an istream that holds the input string
        std::istringstream iss(s);
        
        // Iterate over the istream, using >> to grab floats
        // and push_back to store them in the vector
        std::copy(std::istream_iterator<float>(iss),
                  std::istream_iterator<float>(),
                  std::back_inserter(v));
        
        return ofMatrix4x4(v[0], v[1], v[2], v[3],
                           v[4], v[5], v[6], v[7],
                           v[8], v[9], v[10], v[11],
                           v[12], v[13], v[14], v[15]);
        
    }
    void openFile(const char* msg, string &s, string &path){
        ofFileDialogResult result = ofSystemLoadDialog(msg);
        string aux = result.getName();
        s = result.getPath();
        path = result.getPath();
        path.erase(path.size() - aux.size(), path.size() - 1);
    }
    void getMatrixFile(){
        
        openFile("MeshLab mlp file", matrixFile, path);
        
        if(matrixFile.size() > 0){
            XML.setValue("MATRIX_FILE", matrixFile);
            XML.setValue("MATRIX_PATH", path);
            XML.saveFile("xmlSettings.xml");
        }
    }
    void getSequenceFolder(string &folder, int &numFrames, bool bGetNFrames = false){
        string root;
        string seqPath;
        openFile("Select sequence 0 _###.obj", root, seqPath);
        int pos = root.find_last_of('_');
        root.erase(pos + 1, root.size() - pos);
        folder = root;
        if(bGetNFrames){
            ofDirectory dir(seqPath );
            dir.allowExt("obj");
            dir.listDir();
            numFrames = dir.numFiles();
        }
    }
    void openSequenceFiles(){
        
        getSequenceFolder(seq0Folder, nFrames, true);
        frame = 0;
        getSequenceFolder(seq1Folder, nFrames);
    
        if(seq0Folder.size() > 0) XML.setValue("SEQ_0", seq0Folder);
        if(seq1Folder.size() > 0) XML.setValue("SEQ_1", seq1Folder);
        if(nFrames > 0) XML.setValue("N_FRAMES", nFrames);
        XML.saveFile("xmlSettings.xml");
        
    }

  
public:
    void setup(){
        bSeq = false;
    
        cout << "loading " << "xmlSettings.xml" << endl;
        
        if( XML.loadFile(ofToDataPath("xmlSettings.xml")) ){
            cout << "settings loaded!" << endl;
        }else{
            cout << "unable to load " << "xmlSettings.xml" << " check data/ folder" << endl;
        }
        
        matrixFile= XML.getValue("MATRIX_FILE", "");
        path= XML.getValue("MATRIX_PATH", "");
        
        seq0Folder = XML.getValue("SEQ_0", "");
        seq1Folder = XML.getValue("SEQ_1", "");
        nFrames = XML.getValue("N_FRAMES", 0);
        
        openMeshLabProject();
            
    }
    void resetMatrixFile(){matrixFile = "";}
    
       
    void openMeshLabProject(){
        if (!matrixFile.compare("")) return;
        
        if( MLP.loadFile(ofToDataPath(matrixFile)) ){
            cout << "aligment loaded!" << endl;
        }else{
            cout << "unable to load " << matrixFile << " check data/ folder" << endl;
            return;
        }
        
        MLP.pushTag("MeshLabProject");
        MLP.pushTag("MeshGroup");
        
        k0File =  MLP.getAttribute("MLMesh", "filename", "", 0);
        k1File = MLP.getAttribute("MLMesh", "filename", "", 1);
        
        string s0 = MLP.getValue("MLMesh:MLMatrix44", "", 0);
        string s1 =  MLP.getValue("MLMesh:MLMatrix44", "", 1);
        
        if(s0.size() == 0 || s1.size() == 0){
            cout << "Incorrect matrix file format " << endl;
            return;
        }
        
        M0 = parseMatrix(s0);
        M1 = parseMatrix(s1);
        
        cout << "Loading matrix... " <<  MLP.getAttribute("MLMesh","label","", 0)
        << "\n" << M0 << endl;
        cout << "Loading matrix... " << MLP.getAttribute("MLMesh","label","", 1)
        << "\n" << M1 << endl;
        
     //   ofxObjLoader::loadWithMatrix(path + k0File, calib0, false, M0);
     //   ofxObjLoader::loadWithMatrix(path + k1File, calib1, false, M1);
        
        kinect0.load(path + k0File, M0);
        kinect1.load(path + k1File, M1);
    }
    
    void update(){
        if(bSeq){
            if(bNewFrame){
                string fileName = seq0Folder;
                fileName.append(ofToString(frame)).append(".obj");
                kinect0.load(fileName, M0);
            
                fileName = seq1Folder;
                fileName.append(ofToString(frame)).append(".obj");
                kinect1.load(fileName, M1);
            }
            bNewFrame = false;
            if(bAutoplay){
                frame = (frame + 1) % nFrames;
                bNewFrame = true;
            }
        }
    }
    void draw(){
            ofPushStyle();
            ofSetHexColor(0xE0D0AA);
            kinect0.draw();
            ofSetHexColor(0x8DA893);
            kinect1.draw();
            ofPopStyle();
        
    }
    ofVec3f getCentroid(){
        return kinect0.getCentroid();
    }
    void stop(){
    
    }
    void continousKeyPress(bool keys[255]){
        if(keys['a']){
            frame = (frame + 1) % nFrames;
            bNewFrame = true;
        }
        if(keys['z']){
            frame = (frame + nFrames - 1) % nFrames;
            bNewFrame = true;
        }
        
    }
    void swapMatrix(){
        ofMatrix4x4 aux;
        aux = M0;
        M0 = M1;
        M1 = aux;
        bNewFrame = true;
        
        cout << "SWAP MATRIX" << endl;
        cout << "M0: \n" << M0 << endl;
        cout << "M1: \n" << M1 << endl;
        
    }
    void keyPressed(int key){
        switch (key ) {
            case 'm':
                getMatrixFile();
                openMeshLabProject();
                break;
            case 'l':
                seq0Folder = "";
                seq1Folder = "";
                openSequenceFiles();
                break;
            case 'p':
                bSeq = !bSeq;
                frame = 0;
                break;
            case 'r':
                bAutoplay = !bAutoplay;
                break;
            case 'w':
                swapMatrix();
                break;
        
            default:
                break;
        }
        
    }
    void getStatus(char *str){
        if(!bSeq)
            sprintf(str, "CALIBRATE:\nMatrix: %s\n seq 0: %s\n seq 1: %s\n[m] to load meshlib file\n[l] to load sequence folders\n\n[p] to play sequence", matrixFile.c_str(), seq0Folder.c_str(), seq1Folder.c_str());
        else
            sprintf(str, "PLAYER SEQUENCE:\nMatrix: %s\n seq 0: %s\n seq 1: %s\n[a] next frame [z] prev frame\n[r] autoplay\nframe %d nFrames %d\n[w] swap matrices [p] stop sequence", matrixFile.c_str(), seq0Folder.c_str(), seq1Folder.c_str(), frame, nFrames);
            
    }
    
};


