#pragma once

#include "ofMain.h"

struct Triangle {
	ofIndexType A;
	ofIndexType B;
	ofIndexType C;
};


class testApp : public ofBaseApp{

	map<ofIndexType, vector<Triangle> > spikeTriangles;	// index will be the spike.

	
	ofEasyCam	mCamMainCam;
	ofShader*   mShdPhong;
	ofLight		mLigDirectional;
	ofMaterial	mMatMainMaterial;
	
	ofMesh		mMshBucky;
	
	bool		isShaderDirty;
	bool		shouldRenderNormals;
	bool		shouldUseFlatShading;

	float		mFltSpikeScaleFactor;
	
	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
};
