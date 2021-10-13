#include "Application.h"


static const char* MAIN_WINDOW_TITLE = "COSC1226 - Assignment 2";

int main(int argc, char** argv)
{
    std::cout << argv[0] << std::endl;
    Application* app = new Application(MAIN_WINDOW_TITLE, true, 1024, 768);
    if (int err = app->Init() != 0) {
        std::cout << "RTR:ERROR: Failed to initialise application. Please see error messages for details..." << std::endl;
        return -1;
    };
    app->Run();
    app->Done();
    return 0;
}
