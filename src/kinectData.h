//
//  kinectData.h
//  kinectCalibrator
//
//  Created by miguel valero espada on 11/17/12.
//
//
#include "ofMain.h"
#include <iostream>
#include <fstream>

#ifndef kinectCalibrator_kinectData_h
#define kinectCalibrator_kinectData_h



class kinectData{
    ofMesh points;
    public:
    void load(string fileName, ofMatrix4x4 M){
        points.clear();
        string line;
        
        ifstream file (fileName.c_str());
        if (file.is_open())
        {
            while ( file.good() )
            {
                getline (file,line);
                vector<string> result = ofSplitString(line, " ");
                if(result.size() > 0){
                    if(!result[0].compare("v")){
                        if(result.size() == 4){ //Standar obj f¡le
                            ofVec3f p (ofToFloat(result[1]), ofToFloat(result[2]), ofToFloat(result[3]));
                            points.addVertex(M.postMult(p));
                        }
                        if(result.size() == 5){ //Enriched obj file
                            ofVec3f p (ofToFloat(result[2]), ofToFloat(result[3]), ofToFloat(result[4]));
                            points.addVertex(M.postMult(p));
                            
                        }
                    }
                }
            }
            file.close();
            cout << "Loaded " << points.getNumVertices() << " vertices in " << fileName << endl;
        }
        
        else {
            cout << "Unable to open file" << fileName << endl;
        }
    }
    void draw(){
        /*
        ofColor userColors[] = {ofColor(157, 54, 44),
            ofColor(255, 181, 105),
            ofColor(155, 196, 128),
            ofColor(71, 116, 102),
            ofColor(140, 80, 46)};
         */
        points.drawVertices();
    }
    ofVec3f getCentroid(){
        return points.getCentroid();
    }
};
#endif
