#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    ofSetFrameRate(30);
  //  cam.setDeviceID(0); //video input RPI
     camera.setDeviceID(2); //video input COMPUTER
    /*
    ///MIDI setup
    midiIn.listInPorts();  // print input ports to console
    //if MIDI isn't working check your device number in the console when you first make it change the (0) to whatever your MIDI controller is
    //midiIn.openPort(0);//computer
    midiIn.openPort(1);//Raspberry pi
    midiIn.ignoreTypes(false, false, false);
    //midiIn.addListener(this);
    midiIn.setVerbose(true); // print received messages to the console
    ///
     */
    
    camWidth = 640;
    camHeight = 480;
    reduceWidth = 40;
    reduceHeight = 30;
    
    camera.setup(reduceWidth,reduceHeight);
     camera2.setup(camWidth,camHeight);
    
   
    
    color.allocate(camWidth,camHeight);
    gray.allocate(camWidth,camHeight);
    background.allocate(camWidth,camHeight);
    difference.allocate(camWidth,camHeight);
    
     numPixels =  reduceWidth * reduceHeight;
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    
    camera.update();
    camera2.update();
    
    unsigned char * loadedPixels = camera.getPixels().getData();
    
   
    if(camera.isFrameNew()){
        color.setFromPixels(camera2.getPixels());
        gray = color;
        
        /////
        
        if(ofGetFrameNum() % 60 == 0){
            background = gray;
            learn = false;}
        
        /////
        
        difference.absDiff(background,gray);
        difference.threshold(thresholddiff, invertb);
        
        
        /////////////contour(input,min area, max area, max blobs considered, find holes in blobs (bool), approx (bool)
        contour.findContours(difference, 55, numPixels, 51, true, false);
        
        /////
        
        camera2.getTexture().setAlphaMask(difference.getTexture());
        
        /////
        if(ofGetFrameNum() % 2 == 0 ){
            
        bloke = cvCountNonZero(difference.getCvImage());
        blobnumber = contour.nBlobs;
        
        /////
        
        
        
        for( int i = 0; i < numPixels ;i++)
        {
            R = loadedPixels[ i*3 ]; //R
            G = loadedPixels[ i*3+1 ]; //G
            B = loadedPixels[ i*3+2 ]; //B
            
            avgR += R;
            avgG += G;
            avgB += B;
        }
        
        avgR /= numPixels;
        avgG /= numPixels;
        avgB /= numPixels;
        
        //////
        
        for (int y = 0; y < reduceWidth; ++y)
        {
            for (int x = 0; x < reduceHeight; ++x)
            {
                
                
                ofPixels pix = camera.getPixels();
                ofColor HSB = pix.getColor(x,y);
                
                HSB.getHsb(hue, saturation, brightness);
                hueAngle = HSB.getHueAngle();
                avgLightnessNeg = 255 - HSB.getLightness();
                
                avgHue += hue;
                avgSaturation += saturation;
                avgBrightness += brightness;
                
        
            }
        }
        
        avgHue /= numPixels;
        avgSaturation /= numPixels;
        avgBrightness /= numPixels;
        }
        /////
        
        
        std::cout << "nonzer-boke: " << bloke/1200. << endl; // 0-255
        std::cout << "blob: " << blobnumber*5 << endl; // 0-255
        std::cout << "hueAngle: " << hueAngle << endl; //0-360
        std::cout << "avglightnessNeg: " << avgLightnessNeg << endl; // 0-255
        std::cout << "avgR: " << avgR << endl; // 0-255
        std::cout << "avgG: " << avgG << endl; //0-255
        std::cout << "avgB: " << avgB << endl; //0-255
        std::cout << "avghue: " << avgHue << endl; // 0-255
        std::cout << "avgsaturation: " << avgSaturation << endl; //0-255
        std::cout << "avgbrightness: " << avgBrightness << endl; //0-255
        std::cout << "metamodRGB: " << metamodulationRGB /3 << endl; //0-255
        std::cout << "metamodHSB: " << metamodulationHSB  /3<< endl; //0-255
        std::cout << "mondometa: " << mondometa /3 << endl; //0-255
        
        mapR = ofMap(avgR, 0.0, 255.0, 0.0, 480.);
        mapG = ofMap(avgG, 0.0, 255.0, 0.0, 480.);
        mapB = ofMap(avgB, 0.0, 255.0, 0.0, 480.);
        mapHue = ofMap(avgHue, 0.0, 255.0, 0.0, 720.);
        mapBright = ofMap(avgBrightness, 0.0, 255.0, 0.0, 720.);
        mapSat = ofMap(avgSaturation, 0.0, 255.0, 0.0, 720.);
        metamodulationRGB = mapR+mapG+mapB;
        metamodulationHSB = mapHue+mapBright+mapSat;
        mondometa = metamodulationHSB + metamodulationRGB ;
        
}
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // color.draw(0, 0);
    //contour.draw(0, 0);
    
    
    difference.draw(0, 0, 640, 480);
    camera2.draw(0,0, 640, 480);
    
    
    
    ofPushStyle();
    
    ofSetColor(avgR, avgG, avgB);
    ofDrawRectangle(avgLightnessNeg, metamodulationRGB, mapHue-bloke/1200., mapB);
    
    ofSetColor(avgG, avgB, avgR);
    ofDrawRectangle(mapSat, mapR, (bloke/1200.)+mapBright/2, mapG);
    
    ofSetColor(avgB, avgR, avgG);
    ofDrawRectangle(metamodulationHSB, mapB, mapSat, mapR);
    
    ofSetColor(bloke/1200., metamodulationRGB, mapHue);
    ofDrawRectangle(mapBright, mapG, metamodulationHSB, blobnumber);
    
    ofPopStyle();
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == ' '){
        background = gray;
    }
    
    if(key == '0'){
        invertb = !invertb;
    }
    
    
    switch (key) {
        case OF_KEY_UP:
            thresholddiff++;
            break;
        case OF_KEY_DOWN:
            thresholddiff--;
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------

void ofApp::newMidiMessage(ofxMidiMessage& message){
    
    /*
    //cursor on/off 64 | 1
    if (message.control == 64) {
        if (message.value == 127) {ofShowCursor();}
        if (message.value == 0) {ofHideCursor();}}
    
    
    ///MIDI brush colorize Red 20 CC
    if (message.control==20)
    {if (backColor == true)
    {bgcolor1 = message.value*2;} else
    {brushred=(message.value*2);}}
     
     */
    
}
