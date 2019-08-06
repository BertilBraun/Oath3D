#include "Common/Common.h"

#include "Test.h"

//#include "Examples/PPExample.h"

int main() {

	LOG("STARTED");

	Oath::APP::Application("Oath3D Game Engine Project", Oath::Settings::WIDTH, Oath::Settings::HEIGHT).runLoop<Oath::TestState>(nullptr);
}
