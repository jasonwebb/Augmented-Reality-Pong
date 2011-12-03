#include "Wall.h"
#include "Paddle.h"
#include "Ball.h"
#include "Brick.h"

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxBox2d.h"


class AttackMode {
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
    Brick b1;
    Brick bricks[3][20];

    const static int COLS = 3;
    const static int ROWS = 10;

    // Custom sprites
    ofImage leftPaddleGraphic;
    ofImage rightPaddleGraphic;
    ofImage gameBallGraphic;
    ofImage upperWallGraphic;
    ofImage lowerWallGraphic;

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

};

