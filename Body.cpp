#include "Body.h"

Body::Body(stringw name,
	vector3d<double> position,
	vector3d<double> velocity,
	double radius,
	double mass,
	io::path texturePath,
	double distanceScale,
	u32 fixedPlanetDrawSize,
	IrrlichtDevice *device
	)
{
	this->name = name;
	this->position = position;
	this->velocity = velocity;
	this->radius = radius;
	this->mass = mass;
	this->distanceScale = distanceScale;
	double drawRadius;
	if (fixedPlanetDrawSize == 0)
	{
		drawRadius = radius;
	}
	else
	{
		drawRadius = fixedPlanetDrawSize;
	}

	sphere = device->getSceneManager()->addSphereSceneNode(drawRadius, 128, 0, 1, vector3df(position.X, position.Y, position.Z) * distanceScale, vector3df(0), vector3df(1));
	sphere->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	sphere->setMaterialTexture(0, device->getVideoDriver()->getTexture(texturePath));
}

Body::~Body()
{
}

void Body::prepareDraw()
{
	sphere->setPosition(vector3df(position.X * distanceScale, position.Y * distanceScale, position.Z * distanceScale));
}