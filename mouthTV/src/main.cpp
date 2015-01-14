#include "mouthTV.h"
#include "ofAppGlutWindow.h"

int main()
{
    // create a window
	ofAppGlutWindow window;
    
    // screen setting
	ofSetupOpenGL(&window, 640, 480, OF_WINDOW);
    
    // start application
	ofRunApp(new mouthTV());
}
