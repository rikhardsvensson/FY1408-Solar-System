#pragma once
#include <irrlicht.h>
#include <string>
using namespace irr;
using namespace core;

#define DRAW_SCALE 1e-7

class Body
{
public:
	Body(stringw name,
		vector3d<double> position,
		vector3d<double> velocity,
		double radius,
		double mass,
		io::path texturePath,
		IrrlichtDevice *device
		);
	~Body();
	void update();
	void prepareDraw();

	vector3d<double> position;
	vector3d<double> velocity;
	double mass;
	double radius;

	array<irr::scene::IMeshSceneNode*> orbitHistory;

	stringw name;
	irr::scene::IMeshSceneNode* sphere;
};