#include "HomeScreen.h"

void HomeScreen::setup() {
    cout << "Setting up HOME SCREEN ... ";

    // Load the sprites
    normalModeOff.loadImage("images/mode-buttons/normal-mode-off.png");
    normalModeOn.loadImage("images/mode-buttons/normal-mode-on.png");

    attackModeOff.loadImage("images/mode-buttons/attack-mode-off.png");
    attackModeOn.loadImage("images/mode-buttons/attack-mode-on.png");

    retroModeOff.loadImage("images/mode-buttons/retro-mode-off.png");
    retroModeOn.loadImage("images/mode-buttons/retro-mode-on.png");

    upperWallGraphic.loadImage("images/normal/upper-wall.png");
    lowerWallGraphic.loadImage("images/normal/lower-wall.png");

    // Position the sprites
    normalModeX = ofGetWidth()/2 - normalModeOff.width - normalModeOff.width/3;
    normalModeY = ofGetHeight()/2 - normalModeOff.height;

    attackModeX = ofGetWidth()/2 + attackModeOff.width/3;
    attackModeY = ofGetHeight()/2 - attackModeOff.height;

    retroModeX = ofGetWidth()/2 - retroModeOff.width/2;
    retroModeY = ofGetHeight()/2;

    // Make ON states transparent
    normalOpacity = 0;
    attackOpacity = 0;
    retroOpacity = 0;

    // None of the buttons are being pressed
    NORMAL_OVER = false;
    ATTACK_OVER = false;
    RETRO_OVER = false;

    // Mode selection
    CHOSEN_MODE = -1;

    // Fading variables
    FADE_IN_RATE = 5;
    FADE_OUT_RATE = 5;

    cout << "READY" << endl;

}


void HomeScreen::update() {

    // Normal Mode button fade in/out
    if( NORMAL_OVER ) {
        if(normalOpacity < TRIGGER_POINT) {
            normalOpacity += FADE_IN_RATE;
        } else {
            CHOSEN_MODE = NORMAL_MODE;
            cout << "Entering NORMAL MODE ..." << endl;
        }
    } else {
        if(normalOpacity > 0)
            normalOpacity -= FADE_OUT_RATE;
    }


    // Attack Mode button fade in/out
    if( ATTACK_OVER ) {
        if(attackOpacity < TRIGGER_POINT) {
            attackOpacity += FADE_IN_RATE;
        } else {
            CHOSEN_MODE = ATTACK_MODE;
            cout << "Entering ATTACK MODE ..." << endl;
        }
    } else {
        if(attackOpacity > 0)
            attackOpacity -= FADE_OUT_RATE;
    }


    // Attack Mode button fade in/out
    if( RETRO_OVER ) {
        if(retroOpacity < TRIGGER_POINT) {
            retroOpacity += FADE_IN_RATE;
        } else {
            CHOSEN_MODE = RETRO_MODE;
            cout << "Entering RETRO MODE ..." << endl;
        }
    } else {
        if(retroOpacity > 0)
            retroOpacity -= FADE_OUT_RATE;
    }
}


void HomeScreen::draw() {

    // Draw all of the OFF states
    normalModeOff.draw( normalModeX, normalModeY );
    attackModeOff.draw( attackModeX, attackModeY );
    retroModeOff.draw( retroModeX, retroModeY );

    // Normal Mode ON
    ofSetColor(255, 255, 255, normalOpacity);
    normalModeOn.draw( normalModeX, normalModeY );
    ofSetColor(255,255,255,255);

    // Attack Mode ON
    ofSetColor(255, 255, 255, attackOpacity);
    attackModeOn.draw( attackModeX, attackModeY );
    ofSetColor(255,255,255,255);

    // Retro Mode ON
    ofSetColor(255, 255, 255, retroOpacity);
    retroModeOn.draw( retroModeX, retroModeY );
    ofSetColor(255,255,255,255);

}

void HomeScreen::mousePressed(int x, int y, int button) {

    // Normal Mode button being pressed
    if( (x > normalModeX) && (x < normalModeX + normalModeOn.width) && (y > normalModeY) && (y < normalModeY + normalModeOn.height) ) {
        NORMAL_OVER = true;
        normalOpacity += 3;
    } else {
        NORMAL_OVER = false;
    }

    // Attack Mode button being pressed
    if( (x > attackModeX) && (x < attackModeX + attackModeOn.width) && (y > attackModeY) && (y < attackModeY + attackModeOn.height) ) {
        ATTACK_OVER = true;
        attackOpacity += 3;
    } else {
        ATTACK_OVER = false;
    }

    // Retro Mode button being pressed
    if( (x > retroModeX) && (x < retroModeX + retroModeOn.width) && (y > retroModeY) && (y < retroModeY + retroModeOn.height) ) {
        RETRO_OVER = true;
        retroOpacity += 3;
    } else {
        RETRO_OVER = false;
    }

}

void HomeScreen::mouseReleased(int x, int y, int button) {
    // Turn off all the buttons
    NORMAL_OVER = false;
    ATTACK_OVER = false;
    RETRO_OVER = false;
}

int HomeScreen::getChosenMode() {
    return CHOSEN_MODE;
}
