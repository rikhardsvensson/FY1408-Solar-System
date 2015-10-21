#pragma once
#include <irrlicht.h>
#include <string>
using namespace irr;
using namespace core;

class Body
{
public:
	Body(stringw name,
		vector3d<double> position,
		vector3d<double> velocity,
		double radius,
		double mass,
		io::path texturePath,
		double distanceScale,
		u32 fixedPlanetDrawSize,
		IrrlichtDevice *device
		);
	~Body();
	void prepareDraw();
	
	vector3d<double> position;
	vector3d<double> velocity;
	double mass;
	double radius;

	array<irr::scene::IMeshSceneNode*> orbitHistory;
	double distanceScale;
	stringw name;
	irr::scene::IMeshSceneNode* sphere;
};