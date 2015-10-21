#include <irrlicht.h>
#include "Body.h"
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

#define G 6.674e-11

enum IntegrationMethod { EULER, LEAPFROG, RK4 };

array<Body*> createBodies(IrrlichtDevice *device)
{
	array<Body*> bodies;

	//Data from A.D. 2000-Jan-01 00:00:00.0000 CT

	bodies.push_back(
		new Body(
		"Sol",
		vector3d<double>(0),
		vector3d<double>(0),
		6.955e8,
		1.988544e30,
		"resources/planet_textures/texture_sun.jpg",
		device
		));

	bodies.push_back(
		new Body(
		"Mercury",
		vector3d<double>(-2.105262111032039E+10, -6.640663808353403E+10, -3.492446023382954E+09),
		vector3d<double>(3.665298706393840E+04, -1.228983810111077E+04, -4.368172898981951E+03),
		2440000,
		3.302e23,
		"resources/planet_textures/texture_mercury.jpg",
		device
		));

	bodies.push_back(
		new Body(
		"Venus",
		vector3d<double>(-1.075055502695123E+11, -3.366520720591562E+09, 6.159219802771119E+09),
		vector3d<double>(8.891598046362434E-04, -3.515920774124290E+04, -5.318594054684045E-04),
		6051800,
		48.685e23,
		"resources/planet_textures/texture_venus_atmosphere.jpg",
		device
		));

	bodies.push_back(
		new Body(
		"Earth",
		vector3d<double>(-2.521092863852298E+10, 1.449279195712076E+11, -6.164888475164771E+05),
		vector3d<double>(-2.983983333368269E+04, -5.207633918704476E+03, 6.169062303484907E-08),
		6371010,
		5.97219e24,
		"resources/planet_textures/texture_earth_surface.jpg",
		device
		));

	bodies.push_back(
		new Body(
		"Mars",
		vector3d<double>(2.079950549908331E+11, -3.143009561106971E+09, -5.178781160069674E+09),
		vector3d<double>(1.295003532851602E+03, 2.629442067068712E+04, 5.190097267545717E-04),
		3389900,
		6.4185e23,
		"resources/planet_textures/texture_mars.jpg",
		device
		));

	bodies.push_back(
		new Body(
		"Jupiter",
		vector3d<double>(5.989091594973032E+11, 4.391225931530510E+11, -1.523254614945272E+10),
		vector3d<double>(-7.901937610713569E+03, 1.116317695450082E+04, 1.306729070868444E-04),
		69911000,
		1898.13e24,
		"resources/planet_textures/texture_jupiter.jpg",
		device
		));

	bodies.push_back(
		new Body(
		"Saturn",
		vector3d<double>(9.587063368200246E+11, 9.825652109121954E+11, -5.522065682385063E+10),
		vector3d<double>(-7.428885683466339E+03, 6.738814237717373E+03, 1.776643613880609E-04),
		58232000,
		5.68319e26,
		"resources/planet_textures/texture_saturn.jpg",
		device
		));

	bodies.push_back(
		new Body(
		"Uranus",
		vector3d<double>(2.158774703477132E+12, -2.054825231595053E+12, -3.562348723541665E+10),
		vector3d<double>(4.637648411798584E+03, 4.627192877193528E+03, -4.285025663198061E-05),
		25362000,
		86.8103e24,
		"resources/planet_textures/texture_uranus.jpg",
		device
		));

	bodies.push_back(
		new Body(
		"Neptune",
		vector3d<double>(2.514853420151505E+12, -3.738847412364252E+12, 1.903947325211763E+10),
		vector3d<double>(4.465799984073191E+03, 3.075681163952201E+03, -1.665654118310400E-04),
		24624000,
		102.41e24,
		"resources/planet_textures/texture_neptune.jpg",
		device
		));

	return bodies;
}

int main()
{
	IrrlichtDevice *device =
		createDevice(video::EDT_OPENGL, dimension2d<u32>(1600, 900), 16, false, false, false, 0);

	if (!device)
		return 1;

	device->setWindowCaption(L"Solar System");

	ITimer* timer = device->getTimer();

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	int integrationMethod = LEAPFROG;
	int timeStep = 86400;
	bool plotOrbits = true;
	u32 plotRadius = 50;
	u32 plotInterval = 10;

	array<Body*> bodies = createBodies(device);

	ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS(0, 100, 200, -1, 0, 0, false, 0, false, true);
	camera->setFOV(1);
	camera->setPosition(vector3df(0, 0, 5e5));
	camera->setTarget(bodies[0]->sphere->getPosition());
	camera->setFarValue(1e7);
	u32 frames = 0;

	u32 lastDrawTime = timer->getTime();
	u32 lastUpdateTime = lastDrawTime;

	while (device->run())
	{
		u32 currentTime = timer->getTime();

		if (currentTime - lastUpdateTime >= 16)
		{
			vector3d<double> gravity;
			for (int i = 1; i < bodies.size(); i++)
			{
				if (plotOrbits)
				{
					bool addPlotPoint = false;

					if (bodies[i]->orbitHistory.empty())
					{
						addPlotPoint = true;
					}
					else
					{
						vector3df lastPlotPos = bodies[i]->orbitHistory.getLast()->getPosition();
						vector3d<double> lastToCurrent = vector3d<double>(
							bodies[i]->position.X * DRAW_SCALE - lastPlotPos.X,
							bodies[i]->position.Y * DRAW_SCALE - lastPlotPos.Y,
							bodies[i]->position.Z * DRAW_SCALE - lastPlotPos.Z
							);

						double distanceToCurrent = lastToCurrent.getLength();

						if (distanceToCurrent >= plotInterval * plotRadius)
						{
							addPlotPoint = true;
						}
					}

					if (addPlotPoint)
					{
						irr::scene::IMeshSceneNode* sphere = smgr->addSphereSceneNode(plotRadius, 8, 0, 1, vector3df(bodies[i]->position.X, bodies[i]->position.Y, bodies[i]->position.Z) * DRAW_SCALE, vector3df(0), vector3df(1));
						sphere->setMaterialFlag(irr::video::EMF_LIGHTING, false);
						sphere->getMaterial(0).AmbientColor = video::SColor(255, 255, 255, 255);
						bodies[i]->orbitHistory.push_back(sphere);
					}
				}

				if (EULER) {
					gravity = ((-G * bodies[0]->mass * bodies[i]->mass * bodies[i]->position) / pow(bodies[i]->position.getLength(), 3)) / bodies[i]->mass;
					bodies[i]->position += bodies[i]->velocity * timeStep;
					bodies[i]->velocity += (gravity / bodies[i]->mass) * timeStep;
				}

				else if (LEAPFROG) {
					gravity = ((-G * bodies[0]->mass * bodies[i]->mass * bodies[i]->position) / pow(bodies[i]->position.getLength(), 3)) / bodies[i]->mass;
					bodies[i]->position += (bodies[i]->velocity * timeStep) + 0.5 * gravity * pow(timeStep, 2);
					vector3d<double> nextGravity = ((-G * bodies[0]->mass * bodies[i]->mass * bodies[i]->position) / pow(bodies[i]->position.getLength(), 3)) / bodies[i]->mass;
					bodies[i]->velocity += 0.5 * (gravity + nextGravity) * timeStep;
				}

				else if (RK4) {
					vector3d<double> pos1 = bodies[i]->position;
					vector3d<double> gravity1 = ((-G * bodies[0]->mass * bodies[i]->mass * pos1) / pow(pos1.getLength(), 3)) / bodies[i]->mass;
					vector3d<double> velocity1 = bodies[i]->velocity;

					vector3d<double> pos2 = pos1 + timeStep * velocity1 * 0.5;
					vector3d<double> gravity2 = ((-G * bodies[0]->mass * bodies[i]->mass * pos2) / pow(pos2.getLength(), 3)) / bodies[i]->mass;
					vector3d<double> velocity2 = bodies[i]->velocity + timeStep * gravity1 * 0.5;

					vector3d<double> pos3 = pos2 + timeStep * velocity2 * 0.5;
					vector3d<double> gravity3 = ((-G * bodies[0]->mass * bodies[i]->mass * pos3) / pow(pos3.getLength(), 3)) / bodies[i]->mass;
					vector3d<double> velocity3 = bodies[i]->velocity + timeStep * gravity2 * 0.5;

					vector3d<double> pos4 = pos3 + timeStep * velocity3;
					vector3d<double> gravity4 = ((-G * bodies[0]->mass * bodies[i]->mass * pos4) / pow(pos4.getLength(), 3)) / bodies[i]->mass;
					vector3d<double> velocity4 = bodies[i]->velocity + timeStep * gravity3;

					bodies[i]->position += timeStep * (velocity1 + 2 * velocity2 + 2 * velocity3 + velocity4) / 6;
					bodies[i]->velocity += timeStep * (gravity1 + 2 * gravity2 + 2 * gravity3 + gravity4) / 6;
				}
			}
			lastUpdateTime = currentTime;
		}

		if (currentTime - lastDrawTime >= 16)
		{
			driver->beginScene(true, true, SColor(255, 0, 0, 0));

			for (int i = 0; i < bodies.size(); i++)
			{
				bodies[i]->prepareDraw();
			}

			smgr->drawAll();
			guienv->drawAll();

			driver->endScene();

			core::stringw str = L"Solar System [";
			str += driver->getName();
			str += L"] FPS: ";
			str += (s32)driver->getFPS();
			device->setWindowCaption(str.c_str());

			lastDrawTime = currentTime;
		}
	}

	device->drop();

	return 0;
}