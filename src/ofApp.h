#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxMIDI.h"

class ofApp : public ofBaseApp, public ofxMidiListener{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void newMidiMessage(ofxMidiMessage& message);
    
    
    ofxMidiIn midiIn;
    std::vector<ofxMidiMessage> midiMessages;
    std::size_t maxMessages = 10; //< max number of messages to keep track of
    
    
    
    ofVideoGrabber camera;
    ofVideoGrabber camera2;
    ofTexture myTexture;
    unsigned char * invertedVideoData;
    unsigned char * oldPixels;
    unsigned char * loadedPixels;
    
    ofFbo wait;
    
    ofPixels pix;
    ofxCvColorImage color;
    ofxCvGrayscaleImage gray;
    ofxCvGrayscaleImage background;
    ofxCvGrayscaleImage difference;
    ofxCvContourFinder contour;
    
    int thresholddiff = 30;
    bool learn = true;
    int camWidth;
    int camHeight;
    
    int threshold;
    int motioncount;
    
    int  r_totalPixelVal;
    int g_totalPixelVal;
    int b_totalPixelVal;
    int averageval;
    int bloke;
    
    bool invertb = false;
    
    
    float hue = 0.0;
    float saturation = 0.0;
    float brightness = 0.0;
    float avgHue = 0.0;
    float avgSaturation = 0.0;
    float avgBrightness = 0.0;
    
    int numPixels =  camWidth * camHeight;
    
  
    float avgR = 0.0;
    float avgG = 0.0;
    float avgB = 0.0;
    
    float R = 0.0;
    float G = 0.0;
    float B = 0.0;
    float hueAngle = 0.0;
    float avgLightnessNeg = 0.0;
   
    
    float mapR = 0.0;
    float mapG = 0.0;
    float mapB = 0.0;
    
    float mapHue = 0.0;
    float mapBright = 0.0;
    float mapSat = 0.0;
    
    float metamodulationRGB = 0.0;
    float metamodulationHSB = 0.0;
    float mondometa = 0.0;
    int reduceWidth = 0;
    int reduceHeight = 0;
 
    float blobnumber = 0;
    
};
