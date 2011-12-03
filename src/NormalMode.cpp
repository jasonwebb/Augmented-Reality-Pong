#include "NormalMode.h"
#include "ofxBox2d.h"

using namespace MSA;

/*****************************************************************
 * Initialize the mode
 *****************************************************************/
void NormalMode::setup() {
    cout << "Setting up NORMAL mode ... ";

    // Set up the Box2d environment
    box2d.init();
    box2d.setGravity(0,0);
    box2d.setFPS(30);

    // Setup gameplay variables
    gameOver = false;
    MAX_SCORE = 5;
    INITIAL_SPEED = 3;

    // Setup the top and bottom walls in Box2d
    upperWall.setup(box2d.getWorld(), 0, 0, ofGetWidth(), 10,false);
    lowerWall.setup(box2d.getWorld(), 0, ofGetHeight() - 10, ofGetWidth(), ofGetHeight(),false);

    // Initialize names of sprites
    strLeftGoal = "leftGoal";
    strRightGoal = "rightGoal";
    strBall = "baller";

    // Start listening for collisions
    ofxBox2dContactListener* listener = new pongContactListener(this);
    box2d.setContactListener(listener);

    // Load the sounds
    hitLeft1.loadSound("sounds/normal/hitLeft1.wav");
    hitRight1.loadSound("sounds/normal/hitRight1.wav");

    // Load the font
    pongFont.loadFont("clubland.ttf", 72, true, true, true);

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

    // Spawn a new ball and get the game started!
    newBall(true);

    cout << "READY" << endl;

    // Set up MSA objects and variables
    fluidSolver.setup(100, 100);
    fluidSolver.enableRGB(true).setFadeSpeed(0.002).setDeltaT(0.5).setVisc(0.00015).setColorDiffusion(0);
    fluidDrawer.setup( &fluidSolver );

    fluidSolver.viscocity = .00015;
    fluidSolver.colorDiffusion = .0000015;
    fluidSolver.fadeSpeed = .002;
    fluidSolver.deltaT = .1;
    fluidSolver.doVorticityConfinement = false;

    fluidCellsX = 150;

    pMouse = getWindowCenter();
    resizeFluid = true;

    currentTime = 0;
    targetTime = 0;
    timerCount = 4000;
    timing = false;

    drawFluid = true;
    drawParticles = false;


    // Modify the OF environment for this mode
    ofSetFrameRate(60);
    ofEnableAlphaBlending();
    ofSetBackgroundAuto(true);

}

//MSA FLUID
void NormalMode::fadeToColor(float r, float g, float b, float speed) {
    glColor4f(r, g, b, speed);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
}

//MSA FLUID
void NormalMode::addToFluid( Vec2f pos, Vec2f vel, bool addColor, bool addForce ) {

    // Balance the x and y components of speed with the screen aspect ratio
    float speed = vel.x * vel.x  + vel.y * vel.y * getWindowAspectRatio() * getWindowAspectRatio();

    if(speed > 0) {
        pos.x = constrain(pos.x, 0.0f, 1.0f);
        pos.y = constrain(pos.y, 0.0f, 1.0f);

        const float colorMult = 100;
        const float velocityMult = 30;

        int index = fluidSolver.getIndexForPos(pos);

        if(addColor) {
            Color drawColor( CM_HSV, ( getElapsedFrames() % 360 ) / 360.0f, 1, 1 );
            fluidSolver.addColorAtIndex(index, drawColor * colorMult);
        }

        if(addForce)
            fluidSolver.addForceAtIndex(index, vel * velocityMult);

    }
}


/*****************************************************************
 * Update all variables
 *****************************************************************/
void NormalMode::update() {
    box2d.update();

    // Move the left and right paddles to the mouse
    leftPaddle.moveTo(100, playerLeftY);
    rightPaddle.moveTo(ofGetWidth() - 100, playerRightY);

    // Game ball goes off left edge of screen
    if(gameBall.getPosition().x < 0) {
        timing = true;

        if(gameOver != true) {
            currentTime = ofGetElapsedTimeMillis();

            if(currentTime >= targetTime) {
                targetTime = currentTime + 1000;
                timerCount-=1000;
            }

            if(timerCount <= 0) {
                newBall(true);
                rightScore++;
                timerCount = 4000;
                timing = false;
            }
        }
    }

    // Game ball goes off right edge of screen
    if(gameBall.getPosition().x > ofGetWidth()) {
        timing = true;

        if(gameOver != true) {
            currentTime = ofGetElapsedTimeMillis();

            if(currentTime >= targetTime) {
                targetTime = currentTime + 1000;
                timerCount-=1000;
            }

            if(timerCount <= 0) {
                newBall(false);
                leftScore++;
                timerCount = 4000;
                timing = false;
            }
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

    // Use game ball position to update MSA fluid solver
    Vec2f eventPos = Vec2f(gameBall.getPosition().x, gameBall.getPosition().y);
    Vec2f mouseNorm = Vec2f( eventPos) / getWindowSize();
    Vec2f mouseVel = Vec2f( eventPos - pMouse ) / getWindowSize();
    addToFluid( mouseNorm, mouseVel, true, true );
    pMouse = eventPos;

    if(resizeFluid) {
        fluidSolver.setSize(fluidCellsX, fluidCellsX / getWindowAspectRatio() );
        fluidDrawer.setup(&fluidSolver);
        resizeFluid = false;
    }

    fluidSolver.update();

}


/*****************************************************************
 * Draw everything to the screen
 *****************************************************************/
void NormalMode::draw()
{
    // Daw the MSA fluid solver
    if( drawFluid ) {
        glColor3f(1, 1, 1);
        fluidDrawer.draw(0, 0, getWindowWidth(), getWindowHeight());
    } else {
        if(getElapsedFrames()%5==0) fadeToColor( 0, 0, 0, 0.1f );
    }

    // Draw half-court line
    ofRect(ofGetWidth()/2 - 1, 0, 1, ofGetHeight());

    // Draw all the physics objects
    upperWall.draw();
    lowerWall.draw();
    leftPaddle.draw();
    rightPaddle.draw();
    gameBall.draw();

    // Draw all of the sprites
    ofEnableAlphaBlending();
    upperWallGraphic.draw(0,0);
    lowerWallGraphic.draw( 0, ofGetHeight() - lowerWallGraphic.height);
    leftPaddleGraphic.draw( 27, playerLeftY - leftPaddleGraphic.height/2);
    rightPaddleGraphic.draw( ofGetWidth() - 172, playerRightY - rightPaddleGraphic.height/2);

    // Draw Countdown Timer
    if(timing == true)
        pongFont.drawString(ofToString(timerCount/1000) , ofGetHeight()/2, ofGetWidth()/2);

    // If game is over ...
    if(gameOver == true) {
        // Player 1 end message
        glPushMatrix();
        glTranslatef( 135, playerLeftY+20, 0.0 );
        glRotatef( 90, 0, 0, 1 );
        pongFont.drawString(leftEndMessage, 0, 0);
        glPopMatrix();

        // Player 2 end message
        glPushMatrix();
        glTranslatef( ofGetWidth()-140, playerRightY-20, 0.0 );
        glRotatef( -90, 0, 0, 1 );
        pongFont.drawString(rightEndMessage, 0, -pongFont.getStringBoundingBox(rightEndMessage,0,0).width/2);
        glPopMatrix();

    } else {
        // Player 1 score
        glPushMatrix();
        glTranslatef( 135, playerLeftY+20, 0.0 );
        glRotatef( 90, 0, 0, 1 );
        pongFont.drawString(ofToString(leftScore), 0, 0);
        glPopMatrix();

        // Player 2 score
        glPushMatrix();
        glTranslatef( ofGetWidth()-140, playerRightY-20, 0.0 );
        glRotatef( -90, 0, 0, 1 );
        pongFont.drawString(ofToString(rightScore), 0, 0);
        glPopMatrix();

    }

    // Draw the game ball
    gameBallGraphic.draw( (gameBall.body->GetPosition().x * 30) - gameBallGraphic.width/2, (gameBall.body->GetPosition().y * 30) - gameBallGraphic.height/2);
    ofSetColor(255,255,255);

    ofDisableAlphaBlending();
}


/*****************************************************************
 * Start gameplay
 *****************************************************************/
void NormalMode::start() {}
    cout << "Starting NORMAL mode ..." << endl;
}


/*****************************************************************
 * Stop gameplay
 *****************************************************************/
void NormalMode::stop() {
    cout << "Stopping NORMAL mode ..." << endl;
}


/*****************************************************************
 * Generate a new ball
 *****************************************************************/
void NormalMode::newBall(bool ballDirection) {
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
void NormalMode::endGame(string winner) {
    gameOver = true;

    if(winner == "left") {
        leftEndMessage = "Winner!";
        rightEndMessage = "Loser";
    } else {
        leftEndMessage = "Loser";
        rightEndMessage = "Winner!";
    }

}

/****************************************************************
* Start a new match
*****************************************************************/
void NormalMode::newMatch() {
    // Reset the scores
    leftScore = 0;
    rightScore = 0;
    timerCount = 0;

    // Spawn a new ball
    newBall(false);
}


/*****************************************************************
 * Max score getter / setter
 *****************************************************************/
int NormalMode::getMaxScore() {
    return MAX_SCORE;
}

void NormalMode::setMaxScore(int score) {
    MAX_SCORE = score;
}


/*****************************************************************
 * Initial speed getter / setter
 *****************************************************************/
int NormalMode::getInitialSpeed() {
    return INITIAL_SPEED;
}

void NormalMode::setInitialSpeed(int speed) {
    INITIAL_SPEED = speed;
}


/****************************************************************
 * Mouse interactivity
 ****************************************************************/
void NormalMode::mouseMoved(int x, int y ) {
    #ifdef _USE_LIVE_VIDEO

    #else
        //playerLeftY = y;
        //playerRightY = y;
    #endif
}

