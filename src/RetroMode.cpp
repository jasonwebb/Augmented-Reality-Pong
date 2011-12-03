#include "RetroMode.h"
#include "ofxBox2d.h"

/*****************************************************************
 * Initialize the mode
 *****************************************************************/
void RetroMode::setup() {
    cout << "Setting up RETRO mode ... ";

    // Set up the Box2d environment
    box2d.init();
    box2d.setGravity(0,0);
    box2d.setFPS(30);

    gameOver = false;
    ACTIVE = true;

    MAX_SCORE = 5;
    INITIAL_SPEED = 3;

    // Setup the top and bottom walls in Box2d
    upperWall.setup(box2d.getWorld(), 0, 0, ofGetWidth(), 10,false);
    lowerWall.setup(box2d.getWorld(), 0, ofGetHeight() - 10, ofGetWidth(), ofGetHeight(),false);

    strLeftGoal = "leftGoal";
    strRightGoal = "rightGoal";
    strBall = "baller";

    // Start listening for collisions
    //ofxBox2dContactListener* listener = new pongContactListener(this);
    //box2d.setContactListener(listener);

    // Load the sounds
    hitLeft.loadSound("sounds/retro/hitPaddle.wav");
    hitRight.loadSound("sounds/retro/hitPaddle.wav");
    hitWall.loadSound("sounds/retro/hitWall.wav");

    // Load the sprites
    leftPaddleGraphic.loadImage("images/retro/paddle.png");
    rightPaddleGraphic.loadImage("images/retro/paddle.png");
    gameBallGraphic.loadImage("images/retro/ball.png");

    // Set up the left paddle
    leftPaddle.setPhysics(1000.0, 1.0, 1.0);
    leftPaddle.setup(box2d.getWorld(), 0, 0, 32, 155,false);
    leftPaddle.body->SetUserData( strLeftGoal );

    // Set up the right paddle
    rightPaddle.setPhysics(1000.0, 1.0, 1.0);
    rightPaddle.setup(box2d.getWorld(), 0, 0, 32, 155,false);
    rightPaddle.body->SetUserData( strRightGoal );

    // Setup the game ball
    gameBall.setPhysics(1, 1, 0);
    gameBall.setup(box2d.getWorld(), ofGetWidth()/2, ofGetHeight()/2, 35);
    gameBall.body->SetUserData( strBall );

    // Spawn a new ball and get the game started!
    newBall(true);

    cout << "READY" << endl;
}


/*****************************************************************
 * Update all variables
 *****************************************************************/
void RetroMode::update() {
    if(ACTIVE) {
        box2d.update();

        // Move the left and right paddles to the mouse
        leftPaddle.moveTo(100, playerLeftY);
        rightPaddle.moveTo(ofGetWidth() - 100, playerRightY);

        // Game ball goes off left edge of screen
        if(gameBall.getPosition().x < 0) {
            if(gameOver != true) {
                newBall(true);
                leftScore++;
            }
        }

        // Game ball goes off right edge of screen
        if(gameBall.getPosition().x > ofGetWidth()) {
            if(gameOver != true) {
                newBall(false);
                rightScore++;
            }
        }

        // If the left player's score hit the max, they win
        if(leftScore == MAX_SCORE) {
            winner = "left";
            endGame(winner);
        }

        // If the right player's score hit the max, they win
        if(rightScore == MAX_SCORE) {
            winner = "right";
            endGame(winner);
        }
    }

}


/*****************************************************************
 * Draw everything to the screen
 *****************************************************************/
void RetroMode::draw() {
    // Draw half-court line
    ofRect(ofGetWidth()/2 - 1, 0, 1, ofGetHeight());

    // If the game is over, highlight the winner's side
    if(gameOver == true) {
        // Change color to red
        ofSetColor(0xff0000);
        ofFill();

        // Draw pretty rect over winner's side
        if(winner == "left")
            ofRect(0, 0, ofGetWidth()/2, ofGetHeight());
        else
            ofRect(ofGetWidth()/2, 0, ofGetWidth()/2, ofGetHeight());
    }

    // Draw all the physics objects
    leftPaddle.draw();
    rightPaddle.draw();
    gameBall.draw();

    // Draw all of the sprites
    leftPaddleGraphic.draw( 90, playerLeftY - leftPaddleGraphic.height/2);
    rightPaddleGraphic.draw( ofGetWidth() - 107, playerRightY - rightPaddleGraphic.height/2);
    gameBallGraphic.draw( (gameBall.body->GetPosition().x * 30) - gameBallGraphic.width/2, (gameBall.body->GetPosition().y * 30) - gameBallGraphic.height/2);

}


/*****************************************************************
 * Start gameplay
 *****************************************************************/
void RetroMode::start() {
    cout << "Starting NORMAL mode ..." << endl;

    ACTIVE = true;
}


/*****************************************************************
 * Stop gameplay
 *****************************************************************/
void RetroMode::stop() {
    cout << "Stopping NORMAL mode ..." << endl;

    ACTIVE = false;
}


/*****************************************************************
 * Generate a new ball
 *****************************************************************/
void RetroMode::newBall(bool ballDirection) {

    //Reset volleyCount
    volleyCount = 0;

    // Put the ball in the middle of the screen
    gameBall.moveTo(ofGetWidth()/2, ofGetHeight()/2);

    // Determine the direction of the ball
    // - based on who has possession
    float sgn;
    if(ballDirection == false)
        sgn = -1;
    else
        sgn = 1;

    // Come up with new velocity for ball
    float vx = copysign( INITIAL_SPEED, sgn);
    sgn = ofRandom(-1, 1);
    float vy = copysign( INITIAL_SPEED, sgn);

    // Put the ball in motion
    gameBall.setVelocity(vx, vy);
}



/*****************************************************************
 * End the game
 *****************************************************************/
void RetroMode::endGame(string winner) {
    gameOver = true;
}


/*****************************************************************
 * Max score getter / setter
 *****************************************************************/
int RetroMode::getMaxScore() {
    return MAX_SCORE;
}

void RetroMode::setMaxScore(int score) {
    MAX_SCORE = score;
}


/*****************************************************************
 * Initial speed getter / setter
 *****************************************************************/
int RetroMode::getInitialSpeed() {
    return INITIAL_SPEED;
}

void RetroMode::setInitialSpeed(int speed) {
    INITIAL_SPEED = speed;
}


/****************************************************************
 * Mouse interactivity
 ****************************************************************/
void RetroMode::mouseMoved(int x, int y ) {
    #ifdef _USE_LIVE_VIDEO

    #else
        playerLeftY = y;
        playerRightY = y;
    #endif
}

    /***************************************************************************
      Contact listener for Box2d
    ****************************************************************************/
    class pongContactListener : public ofxBox2dContactListener {
    public:
        RetroMode* game;

        pongContactListener(RetroMode* obj) {
            game = obj;
        }

        void Add(const b2ContactPoint* point) {

            b2Vec2 p = point->position;
            p *= OFX_BOX2D_SCALE;

            b2Shape* shape1 = point->shape1;
            b2Shape* shape2 = point->shape2;
                    cout << "hello" << endl;

            // Player 1 scores = right goal was involved in a collision
            if(shape1->GetBody()->GetUserData() == game->strRightGoal || shape2->GetBody()->GetUserData() == game->strRightGoal) {
                if(shape1->GetBody()->GetUserData() == game->strBall || shape2->GetBody()->GetUserData() == game->strBall) {
                    game->volleyCount++;
                    game->hitLeft.play();
                    cout << "hello" << endl;
                }
            }

            // Player 2 scores = left goal was involved in a collision
            else if (shape1->GetBody()->GetUserData() == game->strLeftGoal || shape2->GetBody()->GetUserData() == game->strLeftGoal) {
                if(shape1->GetBody()->GetUserData() == game->strBall || shape2->GetBody()->GetUserData() == game->strBall) {
                    game->volleyCount++;
                    game->hitRight.play();
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

