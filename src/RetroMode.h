#import "Wall.h"
#import "Paddle.h"
#import "Ball.h"

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxBox2d.h"

class RetroMode {
public:

    // Application functions
    void setup();
    void update();
    void draw();

    // Helper functions
    void start();
    void stop();
    int getMaxScore();
    void setMaxScore(int score);
    int getInitialSpeed();
    void setInitialSpeed(int speed);

    // Gameplay functions
    void newBall( bool ballDirection );
    void endGame(string winner);

    // Interactivity
    void mouseMoved(int x, int y);

    // Player Left coordinates
    int playerLeftX;
    int playerLeftY;

    // Player Right coordinates
    int playerRightX;
    int playerRightY;

    // Player scores
    int leftScore;
    int rightScore;

    // Endgame information
    bool gameOver;
    string winner;

    // Box2d
    ofxBox2d box2d;
    ofxBox2dCircle circle;

    // Custom objects
    Ball gameBall;
    Paddle leftPaddle;
    Paddle rightPaddle;
    Wall upperWall;
    Wall lowerWall;

    // Custom sprites
    ofImage leftPaddleGraphic;
    ofImage rightPaddleGraphic;
    ofImage gameBallGraphic;

    // Identification strings
    char* strLeftGoal;
    char* strRightGoal;
    char* strBall;

    // Gameplay variables
    int volleyCount;
    bool ACTIVE;
    int INITIAL_SPEED;
    int MAX_SCORE;

    // Sound effects
    ofSoundPlayer  hitLeft;
    ofSoundPlayer  hitRight;
    ofSoundPlayer  hitWall;

};
