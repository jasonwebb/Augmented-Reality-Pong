/* ============================================================
  Game ball
 ============================================================== */
class Ball : public ofxBox2dCircle {
public:
    // Constructor
    Ball() {}

    // Color of ball (tint)
    ofColor color;

    // Override Box2dCircle's draw method to draw a simple circle instead
    void draw() {
        float radius = getRadius();

        glPushMatrix();
            glTranslatef(getPosition().x, getPosition().y, 0);

            ofSetColor(color.r, color.g, color.b);
            ofFill();
            ofCircle(0, 0, radius);
        glPopMatrix();

    }
};
