#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxBox2d.h"

#include "HomeScreen.h"
#include "NormalMode.h"
#include "AttackMode.h"
#include "RetroMode.h"

#define _USE_LIVE_VIDEO


/***************************************************************************
  Main application definition
****************************************************************************/
class testApp : public ofBaseApp {
public:

    // General functions
    void setup();
    void update();
    void draw();

    // Interactivity functions
    void keyPressed  (int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);

    // Computer vision variables
    ofVideoGrabber 		    vidGrabber;
    ofxCvColorImage		    colorImg;
    ofxCvGrayscaleImage 	grayImage;
    ofxCvGrayscaleImage 	grayBg;
    ofxCvGrayscaleImage 	grayDiff;
    ofxCvContourFinder 	    contourFinder;
    int 				    threshold;
    bool				    bLearnBackground;
    int                     minBlobSize;
    int                     maxBlobSize;
    int                     numBlobs;
    bool                    VERBOSE;

    // Mode management
    int CURRENT_MODE;
    const static int HOME_SCREEN = 0;
    const static int NORMAL_MODE = 1;
    const static int RETRO_MODE  = 2;
    const static int ATTACK_MODE = 3;

    // Mode objects
    HomeScreen homeScreen;
    NormalMode normalMode;
    AttackMode attackMode;
    RetroMode  retroMode;

};

#endif;
