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
#include "kinectData.h"

#define K 2
class calibratorModeCalibrate  {
    private:
    
    kinectData kinect[K];
    ofMatrix4x4 M[K];
    
    ofxXmlSettings MLP;
    ofxXmlSettings XML;
    
    string kFile[K];
    string seqFolder[K];
    string matrixFile, path;
    
    
    ofMatrix4x4 parseMatrix(string s){
        std::vector<float> v;
        std::istringstream iss(s);
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
        string oldMatrixFile = matrixFile;
        string oldPath = path;
        openFile("MeshLab mlp file", matrixFile, path);
        if(matrixFile.size() > 0){
            XML.setValue("MATRIX_FILE", matrixFile);
            XML.setValue("MATRIX_PATH", path);
            XML.saveFile("xmlSettings.xml");
        }
        else{
            matrixFile = oldMatrixFile;
            oldPath = path;
        }
    }
    
    void getSequenceFolder(string &folder, int &numFrames, bool bGetNFrames = false){
        string root;
        string seqPath;
        openFile("Select sequence 0 _###.ply", root, seqPath);
        int pos = root.find_last_of('_');
        root.erase(pos + 1, root.size() - pos);
        folder = root;
        if(bGetNFrames){
            ofDirectory dir(seqPath );
            dir.allowExt("ply");
            dir.listDir();
            numFrames = dir.numFiles();
        }
    }
    void openSequenceFiles(){
        for(int i = 0; i < K; i++){
            string oldSequenceFolder = seqFolder[i];
            int oldNFrames = nFrames;
            
            if(i == 0) getSequenceFolder(seqFolder[i], nFrames, true);
            else getSequenceFolder(seqFolder[i], nFrames, false);
            
            if(seqFolder[i].size() > 0)
                XML.setValue("SEQ_" + ofToString(i), seqFolder[i]);
            else {
                seqFolder[i] = oldSequenceFolder;
                nFrames = oldNFrames;
            }
        }
        if(nFrames > 0) XML.setValue("N_FRAMES", nFrames);
        XML.saveFile("xmlSettings.xml");
    }

  
public:
    
    bool bSeq, bAutoplay, bNewFrame;
    int frame, nFrames;
    bool bSaving = false;
    
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
        
        for(int i = 0; i < K; i++)
            seqFolder[i] = XML.getValue("SEQ_" + ofToString(i), "", 0);
        
        nFrames = XML.getValue("N_FRAMES", 0);
        openMeshLabProject();
            
    }
       
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
        cout << " A " << endl;
        for(int i = 0; i < K; i++){
            kFile[i] =  MLP.getAttribute("MLMesh", "filename", "", i);
            cout << kFile[i] << endl;
            string s = MLP.getValue("MLMesh:MLMatrix44", "", i);
            if(s.size() == 0 ){
                cout << "Incorrect matrix file format " << endl;
                return;
            }
            M[i] = parseMatrix(s);
            
            cout << "Loading matrix... " <<  MLP.getAttribute("MLMesh","label","", i)
            << "\n" << M[i] << endl;
        }
       
        for(int i = 0; i < K; i++)
            kinect[i].load(path + kFile[i], M[i]);
        
    }
    
    void update(){
        if(bSeq){
            if(bNewFrame){
                for(int i = 0; i < K; i++){
                    string fileName = seqFolder[i];
                    fileName.append(ofToString(frame)).append(".ply");
                    kinect[i].load(fileName, M[i]);
                }
                if(bSaving){
                    string fileName = seqFolder[0];
                    fileName += "merged_";
                    fileName.append(ofToString(frame)).append(".ply");
                    cout << "Saving " << fileName << endl;
                    int  n = 0;
                    for(int i = 0; i < K; i++)
                        n += kinect[i].getNumVertices();
                    
                    cout << "Saving " << n << " vertices" << endl;
                    
                    ofstream *f = new ofstream(fileName.c_str());
                    *f << "ply" << endl;
                    *f << "format ascii 1.0" << endl;
                    *f << "comment : created from Kinect merger" << endl;
                    *f << "element vertex " << n << endl;
                    *f << "property float x" << endl;
                    *f << "property float y" << endl;
                    *f << "property float z" << endl;
                    *f << "end_header" << endl;

                    for(int i = 0; i < K; i++)
                        kinect[i].saveVertices(f);
                    
                    f->close();
                    delete f;
                }
            
            }
            bNewFrame = false;
            if(bAutoplay){
                frame = (frame + 1) % nFrames;
                bNewFrame = true;
            }
        }
    }
    void draw(){
        
        for(int i = 0; i < K; i++){
            ofPushStyle();
            switch(i){
                case 0:
                    ofSetHexColor(0xE0D0AA);
                    break;
                case 1:
                    ofSetHexColor(0x8DA893);
                    break;
                case 2:
                    ofSetHexColor(0x1DA813);
                    break;
                case 3:
                    ofSetHexColor(0x0DFF192);
                    break;
                default:
                    ofSetHexColor(0xFFFFFF);
                    break;
                    
                    
            }
            
            kinect[i].draw();
            ofPopStyle();
        }
        
    }
   

    void nextFrame(){
        frame = (frame + 1) % nFrames;
        bNewFrame = true;
    }
    void prevFrame(){
        frame = (frame + nFrames - 1) % nFrames;
        bNewFrame = true;
        
    }
    void swapMatrix(){
        ofMatrix4x4 aux;
        aux = M[0];
        M[0] = M[1];
        M[1] = aux;
        bNewFrame = true;
        
        cout << "SWAP MATRIX" << endl;
        cout << "M0: \n" << M[0] << endl;
        cout << "M1: \n" << M[1] << endl;
        
    }
    
    void openSequenceFolders(){
        openSequenceFiles();
    }
    void openMLP(){
        getMatrixFile();
        openMeshLabProject();
    }
    void toggleSimulation(){
        bSeq = !bSeq;
        frame = 0;
        bNewFrame = true;
        
    }
    
    void toogleSwapMatrix(){
        swapMatrix();
    }
 
    void getStatus(char *str){
        string folders = "";
        for(int i = 0; i < K; i++){
            folders += seqFolder[i].c_str();
            folders += "\n";
        }
        if(!bSeq)
            sprintf(str, "MESHLAB VIEW:\nMatrix: %s\n%s", matrixFile.c_str(), folders.c_str());
        else{
            sprintf(str, "SIMULATION:\nMatrix: %s\n%s\nframe %d N frames %d", matrixFile.c_str(), folders.c_str(), frame, nFrames);
        }
    }
    
};


