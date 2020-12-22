#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
	ofEnableDepthTest();
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
	ofNoFill();

	this->u_seed = ofRandom(1000);
	this->v_seed = ofRandom(1000);

	ofColor color;
	vector<int> hex_list = { 0xef476f, 0xffd166, 0x06d6a0, 0x118ab2, 0x073b4c };
	for (auto hex : hex_list) {

		color.setHex(hex);
		this->base_color_list.push_back(color);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	while (this->deg_list.size() < 800) {

		auto deg_pair = make_pair(ofRandom(360), ofRandom(360));
		this->deg_list.push_back(deg_pair);
		vector<glm::vec3> log;
		this->log_list.push_back(log);
		this->color_list.push_back(this->base_color_list[(int)ofRandom(this->base_color_list.size())]);
		this->life_list.push_back(ofRandom(60, 120));
	}

	for (int i = this->deg_list.size() - 1; i >= 0; i--) {

		this->life_list[i] -= 1;
		if (this->life_list[i] < 0) {

			this->deg_list.erase(this->deg_list.begin() + i);
			this->log_list.erase(this->log_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);
			this->life_list.erase(this->life_list.begin() + i);

			continue;
		}

		auto location = this->make_point(300, 50, this->deg_list[i].first, this->deg_list[i].second);
		auto u_param = ofMap(ofNoise(glm::vec4(location * 0.003, this->u_seed + ofGetFrameNum() * 0.005)), 0, 1, -30, 30);
		auto v_param = ofMap(ofNoise(glm::vec4(location * 0.003, this->v_seed + ofGetFrameNum() * 0.005)), 0, 1, -5, 5);

		this->deg_list[i].first += u_param;
		this->deg_list[i].second += v_param;

		this->log_list[i].push_back(this->make_point(300, 50, this->deg_list[i].first, this->deg_list[i].second));
		while (this->log_list[i].size() > 150) {

			this->log_list[i].erase(this->log_list[i].begin());
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	for (int i = 0; i < this->deg_list.size(); i++) {

		ofSetColor(this->color_list[i]);

		if (this->life_list[i] > 60) {

			ofSetLineWidth(2);
		}
		else {

			ofSetLineWidth(ofMap(this->life_list[i], 0, 60, 0, 2));
		}

		ofBeginShape();
		ofVertices(this->log_list[i]);
		ofEndShape();
	}

	this->cam.end();
}

//--------------------------------------------------------------
glm::vec3 ofApp::make_point(float R, float r, float u, float v) {

	// 数学デッサン教室 描いて楽しむ数学たち　P.31

	u *= DEG_TO_RAD;
	v *= DEG_TO_RAD;

	auto x = (R + r * cos(u)) * cos(v);
	auto y = (R + r * cos(u)) * sin(v);
	auto z = r * sin(u);

	return glm::vec3(x, y, z);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}