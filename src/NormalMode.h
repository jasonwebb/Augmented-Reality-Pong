#import "Wall.h"
#import "Paddle.h"
#import "Ball.h"

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxBox2d.h"

#include "MSAFluid.h"
#include "MSATimer.h"

using namespace MSA;

class NormalMode
{
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
    void newMatch();

    // Interactivity
    void mouseMoved(int x, int y);

    // Player Left coordinates
    int playerLeftX;
    int playerLeftY;

    // Player Right coordinates
    int playerRightX;
    int playerRightY;

    // Player scores
    char leftScore;
    char rightScore;

    string leftEndMessage;
    string rightEndMessage;

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
    ofSoundPlayer  hitLeft1;
    ofSoundPlayer  hitRight1;

    // Font
    ofTrueTypeFont pongFont;

    // MSAFluid
    void fadeToColor(float r, float g, float b, float speed);
    void addToFluid(Vec2f pos, Vec2f vel, bool addColor, bool addForce);

    int	fluidCellsX;
    bool resizeFluid;
    bool drawFluid;
    bool drawParticles;

    MSA::FluidSolver	fluidSolver;
    MSA::FluidDrawerGl	fluidDrawer;

    MSA::Vec2f pMouse;

    int currentTime;
    int targetTime;
    int timerCount;
    bool timing;

};

/***************************************************************************
  Contact listener for Box2d
****************************************************************************/
class pongContactListener : public ofxBox2dContactListener {
    public:
        NormalMode* game;

        pongContactListener(NormalMode* obj) {
            game = obj;
        }

        void Add(const b2ContactPoint* point) {

            b2Vec2 p = point->position;
            p *= OFX_BOX2D_SCALE;

            b2Shape* shape1 = point->shape1;
            b2Shape* shape2 = point->shape2;

            // Player 1 scores = right goal was involved in a collision
            if(shape1->GetBody()->GetUserData() == game->strRightGoal || shape2->GetBody()->GetUserData() == game->strRightGoal) {
                if(shape1->GetBody()->GetUserData() == game->strBall || shape2->GetBody()->GetUserData() == game->strBall) {
                    game->volleyCount++;
                    game->hitLeft1.play();
                }
            }

            // Player 2 scores = left goal was involved in a collision
            else if (shape1->GetBody()->GetUserData() == game->strLeftGoal || shape2->GetBody()->GetUserData() == game->strLeftGoal) {
                if(shape1->GetBody()->GetUserData() == game->strBall || shape2->GetBody()->GetUserData() == game->strBall) {
                    game->volleyCount++;
                    game->hitRight1.play();
                }
            }

            // Increase velocity of ball every 5 volleys
            if(game->volleyCount % 5 == 1 && game->volleyCount > 5) {
                ofPoint currentVeloc = game->gameBall.getVelocity();
                float addedVelocity = copysign(1, currentVeloc.x);
                game->gameBall.setVelocity(currentVeloc.x + addedVelocity, currentVeloc.y);
            }
        }
};
