#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxBox2d.h"


class HomeScreen {
public:
    // OF main functions
    void setup();
    void update();
    void draw();

    // Helpers
    int getChosenMode();
    int getFadeInRate();
    int getFadeOutRate();
    void setFadeInRate(int rate);
    void setFadeOutRate(int rate);

    // Mouse interactivity
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);

    // Sprites
    ofImage normalModeOff;
    ofImage normalModeOn;
    ofImage attackModeOff;
    ofImage attackModeOn;
    ofImage retroModeOff;
    ofImage retroModeOn;

    ofImage upperWallGraphic;
    ofImage lowerWallGraphic;

    // X/Y coordinates of sprites
    int normalModeX;
    int normalModeY;
    int attackModeX;
    int attackModeY;
    int retroModeX;
    int retroModeY;

    // Opacity of ON states
    int normalOpacity;
    int attackOpacity;
    int retroOpacity;

    // Hit testing for buttons
    bool NORMAL_OVER;
    bool ATTACK_OVER;
    bool RETRO_OVER;

    // Fade variables
    int FADE_IN_RATE;
    int FADE_OUT_RATE;

    // Point at which button is 'selected'
    const static int TRIGGER_POINT = 300;

    // Mode chosen
    int CHOSEN_MODE;
    const static int HOME_SCREEN = 0;
    const static int NORMAL_MODE = 1;
    const static int RETRO_MODE  = 2;
    const static int ATTACK_MODE = 3;

};
