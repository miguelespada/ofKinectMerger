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
#include "ofxTimeMeasurements.h"

#ifndef kinectCalibrator_kinectData_h
#define kinectCalibrator_kinectData_h



class kinectData{
    ofMesh points;
    public:
    void load(string fileName, ofMatrix4x4 M){
        TIME_SAMPLE_START("read");
        points.clear();
        string line;
        bool bParsing = false;
        int propertyCount = 0;
        ifstream file (fileName.c_str());
        if (file.is_open())
        {
            while ( file.good() )
            {
                getline (file,line);
                if(!bParsing){
                    vector<string> tokens;
                    tokens = ofSplitString(line, " ");
                    if(!tokens[0].compare("property")){
                        propertyCount ++;
                        continue;
                    }
                    if(!line.compare("end_header")) {
                        bParsing = true;
                        cout << "Start parsing!" << endl;
                        continue;
                    }
                }
                
                if(bParsing && propertyCount == 3){
                    ofVec3f p;
                    sscanf(line.c_str(), "%f  %f  %f", &p.x, &p.y, &p.z);
                    points.addVertex(M.postMult(p));
                }
                
                if(bParsing && propertyCount == 4){
                    ofVec3f p;
                    int userId;
                    sscanf(line.c_str(), "%f  %f  %f %d", &p.x, &p.y, &p.z, &userId);
                    points.addVertex(M.postMult(p));
                }
            }
            file.close();
            cout << "Loaded " << points.getNumVertices() << " vertices in " << fileName << endl;
        }
        
        else {
            cout << "Unable to open file: " << fileName << endl;
        }
        
        TIME_SAMPLE_STOP("read");
        TIME_SAMPLE_PRINT("read");
        
    }
    void draw(){
        points.drawVertices();
    }
    int getNumVertices(){
        return points.getNumVertices();
    }
    ofVec3f getCentroid(){
        return points.getCentroid();
    }
    void saveVertices(ofstream *o){
        for(int i = 0; i < points.getNumVertices(); i++){
            ofPoint p = points.getVertex(i);
            *o << p.x << "  " << p.y << "  " << p.z << endl;
        }
        
    }
};
#endif
