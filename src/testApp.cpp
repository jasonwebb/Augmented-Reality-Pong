#include "testApp.h"


/****************************************************************
 * Setup the application
 ****************************************************************/
void testApp::setup() {

    // Set up the oF environment
    ofSetVerticalSync(true);
    //ofHideCursor();

    // Set up the current mode to NORMAL mode
    CURRENT_MODE = NORMAL_MODE;

    // Initialize all of the various modes to be ready for use
    homeScreen.setup();
    normalMode.setup();
    attackMode.setup();
    retroMode.setup();

    // Initialize up the OpenCV variables
    bLearnBackground = false;
    minBlobSize = 40;
    maxBlobSize = (320*240)/2;
    numBlobs = 2;
    VERBOSE = true;

    // Initialize and set up the camera
    #ifdef _USE_LIVE_VIDEO
        vidGrabber.setDeviceID(1);
        vidGrabber.setVerbose(true);
        vidGrabber.initGrabber(320,240);
        colorImg.allocate(320,240);
        grayImage.allocate(320,240);
        grayBg.allocate(320,240);
        grayDiff.allocate(320,240);
        bLearnBackground = true;
        threshold = 55;
    #endif

}

/****************************************************************
 * Update all of the program variables
 ****************************************************************/
void testApp::update() {

    ofBackground(0,0,0);

    // Process the camera data
    #ifdef _USE_LIVE_VIDEO
        bool bNewFrame = false;
        vidGrabber.grabFrame();
        bNewFrame = vidGrabber.isFrameNew();

        // When there is a new frame from the video grabber ...
        if (bNewFrame) {

            // Obtain the pixels
            colorImg.setFromPixels(vidGrabber.getPixels(), 320,240);
            grayImage = colorImg;

            // If needed, save the current frame as the background for future subtraction
            if (bLearnBackground == true) {
                grayBg = grayImage;
                bLearnBackground = false;
            }

            // Subtract the background, then use threshold function
            grayDiff.absDiff(grayBg, grayImage);
            grayDiff.threshold(threshold);

            // Analyze frame for blobs using our variables
            contourFinder.findContours(grayDiff, minBlobSize, maxBlobSize, numBlobs, true);	// find holes
        }
    #endif

    // If more info is requested ...
    if(VERBOSE)
        printf("Threshold: %i   \t Min blob size: %i    \t Max blob size: %i \n\r", threshold, minBlobSize, maxBlobSize);


    // Hand over focus to appropriate mode through appropriate objects
    switch(CURRENT_MODE) {
        case HOME_SCREEN:
            homeScreen.update();

            if( homeScreen.getChosenMode() != -1 )
                CURRENT_MODE = homeScreen.getChosenMode();

            break;
        case NORMAL_MODE:
            normalMode.update();
            break;
        case ATTACK_MODE:
            attackMode.update();
            break;
        case RETRO_MODE:
            retroMode.update();
            break;
    }
}


/***************************************************************
 * Draw everything to the screen
 ***************************************************************/
void testApp::draw() {

    // Ask the currently active mode object to draw it's objects first
    switch(CURRENT_MODE) {
        case HOME_SCREEN:
            homeScreen.draw();
            break;
        case NORMAL_MODE:
            normalMode.draw();
            break;
        case ATTACK_MODE:
            attackMode.draw();
            break;
        case RETRO_MODE:
            retroMode.draw();
            break;
    }


    #ifdef _USE_LIVE_VIDEO
        // If asked, draw both the gray-scale live image, and the blobs from contourFinder
        if(VERBOSE) {
            grayImage.draw(0,0);
            contourFinder.draw(0,0);
        }

        // If contourFinder finds at least two blobs, then two players are ready for play
        if(contourFinder.nBlobs >= 2) {

            // Infer each player's positions based on the blobs found
            for(int i=0; i<2; i++) {
                if(contourFinder.blobs[i].centroid.x/320 * ofGetScreenWidth() < ofGetScreenWidth()/2)
                    normalMode.playerLeftY = (contourFinder.blobs[i].centroid.y)/240 * ofGetScreenHeight();
                else
                    normalMode.playerRightY = (contourFinder.blobs[i].centroid.y)/240 * ofGetScreenHeight();
            }
        }

        /*
        if(contourFinder.nBlobs >= 2) {
            int y1 = (contourFinder.blobs[0].centroid.y)/240 * ofGetScreenHeight();
            int y2 = (contourFinder.blobs[1].centroid.y)/240 * ofGetScreenHeight();

            int x1 = (contourFinder.blobs[0].centroid.x)/320 * ofGetScreenWidth();
            int x2 = (contourFinder.blobs[1].centroid.x)/320 * ofGetScreenWidth();

            if(x1 < ofGetScreenWidth()/2)
                normalMode.playerLeftY = y1;

            if(x2 > ofGetScreenWidth()/2)
                normalMode.playerRightY = y2;


            // The first blob will be player 1
            //normalMode.playerLeftX = contourFinder.blobs[0].centroid.x;
            //normalMode.playerLeftY = (contourFinder.blobs[0].centroid.y)/240 * ofGetScreenHeight();

            // The second blob will be player 2
            //normalMode.playerRightX = contourFinder.blobs[1].centroid.x;
            //normalMode.playerRightY = (contourFinder.blobs[1].centroid.y)/240 * ofGetScreenHeight();


            // Draw left paddle
            ofFill();
            ofSetColor(0xff0000);
            //ofRect(ofMap(playerLeftX, 0, 320, 0, ofGetScreenWidth()), ofMap(playerLeftY, 0, 240, 0, ofGetScreenHeight()), 20,160);

            // Draw right paddle
            ofSetColor(0x0000ff);
            //ofRect(ofMap(playerRightX, 0, 320, 0, ofGetScreenWidth()), ofMap(playerRightY, 0, 240, 0, ofGetScreenHeight()), 20,160);
        }*/

        if(VERBOSE) {
            // Draw informational message for camera
            ofSetColor(0xffffff);
            char reportStr[1024];
            sprintf(reportStr, "bg subtraction and blob detection\npress ' ' to capture bg\nthreshold %i (press: +/-)\nnum blobs found %i, fps: %f", threshold, contourFinder.nBlobs, ofGetFrameRate());
            ofDrawBitmapString(reportStr, 20, 600);
        }
    #endif

/*    //Define Reports String and set its color
    char reportString[1024];
    ofSetColor(0xff0000);

    // Draw Player 1's score
    sprintf(reportString, "Player 1 Score is: %i", leftScore);
    ofDrawBitmapString(reportString, 100, 100);

    // Draw Player 2's score
    sprintf(reportString, "Player 2 Score is: %i", rightScore);
    ofDrawBitmapString(reportString, 100, 120);

    // Draw Volley Count
    sprintf(reportString, "The Volley Count is: %i", volleyCount);
    ofDrawBitmapString(reportString, 100, 140);
*/

}


//--------------------------------------------------------------
// Keyboard controls:
// [space] = save current frame as background for subtraction
// + = increase threshold for video
// - = decrease threshold for video
// i = increase minimum blob size
// u = decrease minimum blob size
// k = increase maximum blob size
// j = decrease maximum blob size
// v = toggle VERBOSE mode
// r = start a new match
//--------------------------------------------------------------
void testApp::keyPressed  (int key) {
    switch (key) {
        case ' ':
            bLearnBackground = true;
            break;
        case '+':
            threshold ++;
            if (threshold > 255) threshold = 255;
            break;
        case '-':
            threshold --;
            if (threshold < 0) threshold = 0;
            break;
        case 'u':
            minBlobSize--;
            break;
        case 'i':
            minBlobSize++;
            break;
        case 'j':
            maxBlobSize--;
            break;
        case 'k':
            maxBlobSize++;
            break;
        case 'v':
            VERBOSE = !VERBOSE;
            break;
        case 'r':
            normalMode.newMatch();
            break;
    }
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {

    // Trickle down focus to appropriate mode
    switch(CURRENT_MODE) {
        case NORMAL_MODE:
            normalMode.mouseMoved(x, y);
            break;
        case ATTACK_MODE:
            attackMode.mouseMoved(x, y);
            break;
        case RETRO_MODE:
            retroMode.mouseMoved(x, y);
            break;
    }
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {

    // Trickle down the action
    switch(CURRENT_MODE) {
        case HOME_SCREEN:
            homeScreen.mousePressed(x,y,button);
            break;
        case NORMAL_MODE:
            normalMode.mousePressed(x,y,button);
            break;
        case ATTACK_MODE:
            attackMode.mousePressed(x,y,button);
            break;
        case RETRO_MODE:
            retroMode.mousePressed(x,y,button);
            break;
    }

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {

    // Trickle down the action
    switch(CURRENT_MODE) {
        case HOME_SCREEN:
            homeScreen.mouseReleased(x,y,button);
            break;
        case NORMAL_MODE:
            normalMode.mouseReleased(x,y,button);
            break;
        case ATTACK_MODE:
            attackMode.mouseReleased(x,y,button);
            break;
        case RETRO_MODE:
            retroMode.mouseReleased(x,y,button);
            break;
    }

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h) {}

