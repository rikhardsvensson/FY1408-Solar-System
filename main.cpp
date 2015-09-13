#include <irrlicht.h>
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

int main()
{
	IrrlichtDevice *device =
		createDevice(video::EDT_OPENGL, dimension2d<u32>(640, 480), 16, false, false, true, 0);

	if (!device)
		return 1;

	device->setWindowCaption(L"Solar System");

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	IMeshSceneNode* sphere = smgr->addSphereSceneNode(25, 128, 0, 1, vector3df(0, 0, 0), vector3df(0, 0, 0), vector3df(1, 1, 1));

	if (sphere)
	{
		sphere->setMaterialFlag(EMF_LIGHTING, false);
		sphere->setMaterialTexture(0, driver->getTexture("resources/planet_textures/texture_sun.jpg"));
	}

	smgr->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,0,0));

	u32 frames = 0;

	while(device->run())
	{
		driver->beginScene(true, true, SColor(255,0,0,0));

		sphere->setRotation(sphere->getRotation() + vector3df(0,1,0));

		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();

		if (++frames == 100)
		{
			core::stringw str = L"Solar System [";
			str += driver->getName();
			str += L"] FPS: ";
			str += (s32)driver->getFPS();

			device->setWindowCaption(str.c_str());
			frames = 0;
		}
	}

	device->drop();

	return 0;
}