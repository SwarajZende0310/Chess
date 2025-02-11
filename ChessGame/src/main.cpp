#include"gameFramework/GameApplication.h"

int main()
{
    chess::Application* app = new chess::GameApplication();
    app->Run();
    delete app;
}
