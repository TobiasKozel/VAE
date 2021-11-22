#include <stdio.h>
#define TKLB_ASSERT_SEGFAULT
#include "core/vae_engine.hpp"

using namespace vae;

void eventTriggered(EventCallbackData data) {
	printf("callback event %i %s from bank %i\n", (int) data.event, data.name, (int) data.bank);
}

int main() {
	EngineConfig config;
	config.eventCallback = &eventTriggered;
	core::Engine engine(config);
	if (!engine.loadBank("../../dev/bank1")) {
		engine.fireEvent(0, 0);
	};
	return 0;
}
