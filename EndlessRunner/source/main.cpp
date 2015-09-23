
#include <vix_game.h>
#include <vix_filemanager.h>
#include <vix_pathmanager.h>
#include <vix_resourcemanager.h>
#include <vix_font.h>
#include <vix_gameobject.h>
#include <vix_scenemanager.h>
#include <vix_modelmanager.h>
#include <vix_luaengine.h>
#include <vix_luascriptmanager.h>
#include <vix_objectmanager.h>

using namespace Vixen;

class EndlessRunner : public Game
{
public:
	EndlessRunner();

	void VOnStartup();
	void VOnShutdown();
	void VOnUpdate(float dt);
	void VOnRender(float dt);

private:
	ICamera3D*  m_camera3D;
	IModel*     m_floor;
	IModel*     m_player;
	IModel*     m_cube;
	IFont*      m_font;
	GameObject* m_go;
	GameObject* m_po;
	GameObject* m_co;
	Transform   fontTransform;
	bool paused;
};

EndlessRunner::EndlessRunner()
	: Game()
{

}

void EndlessRunner::VOnStartup()
{
	ObjectManager::Initialize();
    LuaEngine::Initialize();
	LuaScriptManager::Initialize();
    ModelManager::Initialize();
    SceneManager::Initialize();
    SceneManager::OpenScene(VTEXT("scene1"));

	paused = false;

	m_renderer->VSetClearColor(Vixen::Colors::Black);

	m_camera3D = m_renderer->Camera3D();
	m_camera3D->VSetView(Vector3(0.0f, 5.0f,  -5.0f), Vector3(0.0f, 5.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
	m_camera3D->VSetSpeed(50.0f);

	m_font = ResourceManager::OpenFont(VTEXT("Consolas_24.fnt"));

	fontTransform = Transform(20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	

	m_window->VToggleCursor();
}

void EndlessRunner::VOnUpdate(float dt)
{
	if (Input::SingleKeyPress(IKEY::F2))
		m_window->VClose();

	if (Input::SingleKeyPress(IKEY::P))
	{
		m_window->VToggleCursor();
		paused = !paused;
		if (paused)
			SceneManager::PauseScene(VTEXT("scene1"));
		else
			SceneManager::UnpauseScene(VTEXT("scene1"));

	}

	if (!paused)
	{
		if (Input::KeyPress(IKEY::S))
			m_camera3D->VWalk(-dt);

		if (Input::KeyPress(IKEY::W))
			m_camera3D->VWalk(dt);

		if (Input::KeyPress(IKEY::A))
			m_camera3D->VStrafe(-dt);

		if (Input::KeyPress(IKEY::D))
			m_camera3D->VStrafe(dt);

		int deltaX = Input::DeltaX(m_window->VGetClientBounds().w / 2);
		int deltaY = Input::DeltaY(m_window->VGetClientBounds().h / 2);
		m_camera3D->VRotateX(deltaY * 0.25f);
		m_camera3D->VRotateY(deltaX * 0.25f);


		m_camera3D->VUpdate(dt);

		SceneManager::UpdateScene(dt);

		
	    m_window->VTrapCursorCenter();
	}


	
}

void EndlessRunner::VOnRender(float dt)
{
    SceneManager::RenderScene(m_camera3D);


	//ALL 2D UI IS DRAW AFTER SCENE IS DRAWN
	USStream ss;
	ss << "FPS: " << m_window->VFPS();
	m_renderer->VRenderText2D(m_font, ss.str(), Vector2(20, 20));
}

void EndlessRunner::VOnShutdown()
{
    ModelManager::DeInitialize();
	ObjectManager::DeInitialize();
    SceneManager::DeInitialize();
    LuaEngine::DeInitialize();
    delete m_font;
}



int main(int argc, char* argv[])
{
	EndlessRunner _game;

	return _game.Run();
}