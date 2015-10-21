#include "Body.h"

Body::Body(stringw name,
	vector3d<double> position,
	vector3d<double> velocity,
	double radius,
	double mass,
	io::path texturePath,
	IrrlichtDevice *device
	)
{
	this->name = name;
	this->position = position;
	this->velocity = velocity;
	this->radius = radius;
	this->mass = mass;
	sphere = device->getSceneManager()->addSphereSceneNode(1e3, 128, 0, 1, vector3df(position.X, position.Y, position.Z) * DRAW_SCALE, vector3df(0), vector3df(1));
	sphere->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	sphere->setMaterialTexture(0, device->getVideoDriver()->getTexture(texturePath));
}

Body::~Body()
{
}

void Body::update()
{
	
}

void Body::prepareDraw()
{
	sphere->setPosition(vector3df(position.X * DRAW_SCALE, position.Y * DRAW_SCALE, position.Z * DRAW_SCALE));
}