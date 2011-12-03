#include "AttackMode.h"
#include "ofxBox2d.h"

/*****************************************************************
 * Initialize the mode
 *****************************************************************/
void AttackMode::setup() {
    cout << "Setting up ATTACK mode ... ";

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
    hitLeft.loadSound("sounds/normal/hitLeft1.wav");
    hitRight.loadSound("sounds/normal/hitRight1.wav");

    // Load the sprites
    leftPaddleGraphic.loadImage("images/normal/left-paddle.png");
    rightPaddleGraphic.loadImage("images/normal/right-paddle.png");
    gameBallGraphic.loadImage("images/normal/ball.png");
    upperWallGraphic.loadImage("images/normal/upper-wall.png");
    lowerWallGraphic.loadImage("images/normal/lower-wall.png");

    // Resize the wall sprites
    upperWallGraphic.resize( ofGetWidth(), upperWallGraphic.height );
    lowerWallGraphic.resize( ofGetWidth(), lowerWallGraphic.height );

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
    gameBall.setup(box2d.getWorld(), ofGetWidth()/2, ofGetHeight()/2, 40);
    gameBall.body->SetUserData( strBall );

    // Set up all the bricks
    b1.colour.r = ofRandom(0,255);
    b1.colour.g = ofRandom(0,255);
    b1.colour.b = ofRandom(0,255);
    b1.setPhysics(10000.0, 1.0, 1.0);
    b1.setup(box2d.getWorld(), 0, 0, 25, 100,false);
    b1.moveTo( ofGetWidth()/2-100, 100 );

    for(int i=0; i<3; i++) {
        for(int j=0; j<10; j++) {
            bricks[i][j].colour.r = ofRandom(0,255);
            bricks[i][j].colour.g = ofRandom(0,255);
            bricks[i][j].colour.b = ofRandom(0,255);
            bricks[i][j].setPhysics(10000.0, 1.0, 1.0);
            bricks[i][j].setup(box2d.getWorld(), 0, 0, 25, 100,false);
            bricks[i][j].moveTo( ofGetWidth()/2 - 100*i, 100*j );
        }
    }

    // Spawn a new ball and get the game started!
    newBall(true);

    cout << "READY" << endl;
}


/*****************************************************************
 * Update all variables
 *****************************************************************/
void AttackMode::update() {
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
void AttackMode::draw() {
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
    upperWall.draw();
    lowerWall.draw();
    leftPaddle.draw();
    rightPaddle.draw();
    gameBall.draw();

    // Draw bricks
    b1.draw();

    // Draw all of the sprites
    upperWallGraphic.draw(0,0);
    lowerWallGraphic.draw(0,ofGetHeight() - lowerWallGraphic.height);
    leftPaddleGraphic.draw( 27, playerLeftY - leftPaddleGraphic.height/2);
    rightPaddleGraphic.draw( ofGetWidth() - 172, playerRightY - rightPaddleGraphic.height/2);
    gameBallGraphic.draw( (gameBall.body->GetPosition().x * 30) - gameBallGraphic.width/2, (gameBall.body->GetPosition().y * 30) - gameBallGraphic.height/2);

}


/*****************************************************************
 * Start gameplay
 *****************************************************************/
void AttackMode::start() {
    cout << "Starting NORMAL mode ..." << endl;

    ACTIVE = true;
}


/*****************************************************************
 * Stop gameplay
 *****************************************************************/
void AttackMode::stop() {
    cout << "Stopping NORMAL mode ..." << endl;

    ACTIVE = false;
}


/*****************************************************************
 * Generate a new ball
 *****************************************************************/
void AttackMode::newBall(bool ballDirection) {

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
void AttackMode::endGame(string winner) {
    gameOver = true;
}


/*****************************************************************
 * Max score getter / setter
 *****************************************************************/
int AttackMode::getMaxScore() {
    return MAX_SCORE;
}

void AttackMode::setMaxScore(int score) {
    MAX_SCORE = score;
}


/*****************************************************************
 * Initial speed getter / setter
 *****************************************************************/
int AttackMode::getInitialSpeed() {
    return INITIAL_SPEED;
}

void AttackMode::setInitialSpeed(int speed) {
    INITIAL_SPEED = speed;
}


/****************************************************************
 * Mouse interactivity
 ****************************************************************/
void AttackMode::mouseMoved(int x, int y ) {
    #ifdef _USE_LIVE_VIDEO

    #else
        playerLeftY = y;
        playerRightY = y;
    #endif
}


