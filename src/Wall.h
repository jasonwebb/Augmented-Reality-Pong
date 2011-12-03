#include "ofMain.h"
#include "ofxBox2d.h"

/* ===============================================================
  Walls (upper and lower)
 ================================================================= */
class Wall : public ofxBox2dRect {
public:
    Wall() {}

    ofColor colour;

    void draw() {

        b2Shape* s = body->GetShapeList();
        const b2XForm& xf = body->GetXForm();
        b2PolygonShape* poly = (b2PolygonShape*)s;
        int count = poly->GetVertexCount();
        const b2Vec2* localVertices = poly->GetVertices();
        b2Assert(count <= b2_maxPolygonVertices);
        b2Vec2 verts[b2_maxPolygonVertices];

        for(int32 i=0; i<count; ++i) {
            verts[i] = b2Mul(xf, localVertices[i]);
        }

        ofSetColor(colour.r, colour.g, colour.b, 0);
        ofFill();
        ofBeginShape();

        for (int32 i = 0; i <count; i++) {
            ofVertex(verts[i].x*OFX_BOX2D_SCALE, verts[i].y*OFX_BOX2D_SCALE);
        }

        ofEndShape();
        ofEndShape(true);
    }
};
