#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	ofSetVerticalSync(true);
	ofSetFrameRate(60);

	isShaderDirty = true;
	
	mLigDirectional.setup();
	mLigDirectional.setDirectional();
	
	mCamMainCam.setupPerspective(false);
	mLigDirectional.setAmbientColor(ofColor::fromHsb(0, 0, 200));
	mLigDirectional.setDiffuseColor(ofColor::fromHsb(120, 120, 128));
	mLigDirectional.setSpecularColor(ofColor(255,255,255));
	
	
	mMatMainMaterial.setDiffuseColor(ofColor(0,0,0));
	mMatMainMaterial.setSpecularColor(ofColor(200,200,200));
	mMatMainMaterial.setShininess(25.0f);
	
	mCamMainCam.setDistance(200);
	
	mFltSpikeScaleFactor = 1.0f;
	
	shouldRenderNormals = false;
	shouldUseFlatShading = true;

	
	// load geometry into mesh
	
	mMshBucky.load("bucky.ply");
	
//	ofFile is("bucky.tri", ofFile::ReadOnly);
//	
//	string line;
//	string error;
//	ofBuffer buffer(is);
//	
//	// this will tell us all about the winding of the triangles of the triangles and
//	// how they connect to the vertices of the original icosahedron (the first 12 vertices in the mesh, that is).
//	// we need this to know which normals to recalculate.
//	while (!buffer.isLastLine()) {
//		line = buffer.getNextLine();
//		
//		ofIndexType i;
//		Triangle t;
//		
//		stringstream sline;
//		sline.str(line);
//		
//		sline >> i >> t.A >> t.B >> t.C;
//		
//		spikeTriangles[i].push_back(t);
//		
//	}
	
	ofLogNotice() << "normal count:" << mMshBucky.getNormals().size();
	ofLogNotice() << "vertex count:" << mMshBucky.getVertices().size();
	
	
	
}

//--------------------------------------------------------------
void testApp::update(){
	
	
	if (isShaderDirty){
		
		GLuint err = glGetError();	// we need this to clear out the error buffer.
		
		if (mShdPhong != NULL ) delete mShdPhong;
		mShdPhong = new ofShader();
		mShdPhong->load("shaders/phong");
		err = glGetError();	// we need this to clear out the error buffer.
		ofLogNotice() << "Loaded Shader: " << err;
		
		
		isShaderDirty = false;
	}

	float s = 0.5 + (mFltSpikeScaleFactor) * 0.5;
	
//	for(int i=0; i<12; ++i){
//		// update icosahedron vertices (these are the spikes)
//		mMshBucky.getVertices()[i] = mMshBucky.getVertices()[i].getNormalized() * (1.0 + (s) * 0.5 ) ;
//		
//		// recalculate normals based on new spike positions
//		
//		for(int j = 0; j<spikeTriangles[i].size(); j++){
//			Triangle& t_ = spikeTriangles[i][j];
//			mMshBucky.setNormal(t_.A,
//								((mMshBucky.getVertex(t_.A) - mMshBucky.getVertex(t_.C)).
//								 getCrossed(mMshBucky.getVertex(t_.B) - mMshBucky.getVertex(t_.C))).getNormalized());
//		};
//		
//	}

}

//--------------------------------------------------------------
void testApp::draw(){

	
	glShadeModel(GL_SMOOTH);
	glProvokingVertex(GL_LAST_VERTEX_CONVENTION);
	
	ofBackgroundGradient(ofColor::fromHsb(0, 0, 120), ofColor::fromHsb(0, 0, 0));
	
	mCamMainCam.begin();
	
	ofEnableLighting();
	
	mLigDirectional.setGlobalPosition(1000, 1000, 1000);
	mLigDirectional.lookAt(ofVec3f(0,0,0));
	
	ofEnableSeparateSpecularLight();
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
	
	mLigDirectional.enable();
	ofSetColor(mLigDirectional.getDiffuseColor());
	mMatMainMaterial.begin();
	
	mShdPhong->begin();
	
	if (shouldRenderNormals){
		mShdPhong->setUniform1f("shouldRenderNormals", 1.0);
	} else {
		mShdPhong->setUniform1f("shouldRenderNormals", 0.0);
	}
	
	
	if (shouldUseFlatShading){
		mShdPhong->setUniform1f("shouldUseFlatShading", 1.0);
		glShadeModel(GL_FLAT);
		glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);		// OpenGL default is GL_LAST_VERTEX_CONVENTION
	} else {
		mShdPhong->setUniform1f("shouldUseFlatShading", 0.0);
		glShadeModel(GL_SMOOTH);
		glProvokingVertex(GL_LAST_VERTEX_CONVENTION);
	}
	
	// draw geometry
	
	ofPushMatrix();
	ofScale(50, 50,50);
	mMshBucky.draw();
	ofPopMatrix();
	
	// restores shade model to standard GL_SMOOTH
	glShadeModel(GL_SMOOTH);
	// restores vertex convention defaults.
	glProvokingVertex(GL_LAST_VERTEX_CONVENTION);
	
	mShdPhong->end();
	
	// we revert to default openGL values, cleaning up.
	
	mMatMainMaterial.end();
	mLigDirectional.disable();
	
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	
	ofDisableLighting();
	
	ofSetColor(255);
	for (int i=0; i<mMshBucky.getVertices().size(); i++){
		// we do a manual scale by scaling the mesh vertex coord by 50.f to render the point at the vertex position
		ofDrawBitmapString(ofToString(i), mMshBucky.getVertex(i) * 50.0f);
	}
	
	mCamMainCam.end();
	
	// draw keystroke hints
	
	string normalsIndicator = ((shouldRenderNormals) ? "normals" : "faces");
	string normalsFlipIndicator = ((shouldRenderNormals) ? "faces" : "normals");
	ofDrawBitmapStringHighlight( "Showing " + normalsIndicator + ". Press 'n' to show " + normalsFlipIndicator, ofVec3f(10,40), ofColor(255), ofColor(0));
	
	string flatShadingIndicator = ((shouldUseFlatShading) ? "on" : "off");
	ofDrawBitmapStringHighlight( "Flat Shading is " + flatShadingIndicator + ". Press 'f' to toggle."  , ofVec3f(10,60), ofColor(255), ofColor(0));

//	string spikeScaleFactorIndicator = ofToString(0.5 + mFltSpikeScaleFactor * 0.5, 2);
//	ofDrawBitmapStringHighlight( "Spikes scaled by " + spikeScaleFactorIndicator + "; Press '+/-' to modify, 'r' to reset."  , ofVec3f(10,80), ofColor(255), ofColor(0));

	
	ofDrawBitmapStringHighlight( "Press SPACEBAR to reload shader."  , ofVec3f(10,100), ofColor(0), ofColor(255));

	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	switch (key) {
		case ' ':
			isShaderDirty = true;
			break;
		case 'n':
			shouldRenderNormals ^= true;
			break;
		case 'f':
			shouldUseFlatShading ^= true;
			break;
//		case '+':
//			mFltSpikeScaleFactor += 0.1;
//			break;
//		case '-':
//			mFltSpikeScaleFactor -= 0.1;
//		break;
//		case 'r':
//			mFltSpikeScaleFactor = 1.0;
//			break;
		
		default:
			break;
	}
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}