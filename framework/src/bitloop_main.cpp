#include <bitloop/bitloop.h>
#include "bitloop_simulations.h"

int main(int argc, char* argv[])
{
    std::cout << "Hello from example!\n";
    return bitloop_main(argc, argv);
}

#ifdef _WIN32
int WINAPI CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    return main(__argc, __argv); // Redirect entry point for WIN32 build
}
#endif