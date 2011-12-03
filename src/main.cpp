#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){

    ofAppGlutWindow window;
	//ofSetupOpenGL(&window, 1920, 1080, OF_FULLSCREEN);    // Desktop
    //ofSetupOpenGL(&window, 1024, 768, OF_WINDOW);         // Projector testing
    ofSetupOpenGL(&window, 1024, 768, OF_FULLSCREEN);       // Projector live
	ofRunApp( new testApp());

}
