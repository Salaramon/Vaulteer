#include "vpch.h"

#define STB_IMAGE_IMPLEMENTATION 

#include "Vaulteer.h"


int main() {
	//Vaulteer application("resourceFolder?");
	ApplicationSpecification spec = { "Vaulteer window" };
	Vaulteer application = Vaulteer(spec);
	size_t gameFlags = 1;

	while (gameFlags) {
		application.init();
		gameFlags = application.run();
	}

	return 0;
}
