#include <RoraymaEngine/RoraymaEngine.hh>

#include"EditorLayer.hh"

namespace rym {
	class Game : public rym::App {
	public:
		Game()
		{
			PushLayer(new EditorLayer());
		}
		~Game(){}
	};
	App* CreateApp() { return new Game(); }
}

int main() {
	rym::Log::init();
	auto app = rym::CreateApp();
	app->Run();
	delete app;
}
