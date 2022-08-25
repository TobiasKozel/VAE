#include <stdio.h>
#include <cmath>
#include "../src/core/algo/vae_spcap.hpp"


/**
 * @brief Test 5.1 panning setup and print volumes for each degree
 */
int main() {
	printf("deg,s1,s2,s3,s4,s5\n");
	for (int i = 0; i < 360; i++) {
		float t = i / float(180) * 3.14159265358979323846;
		auto test = vae::core::SPCAP::MonoSPCAP.speakers;
		// vae::core::Sample p[5];
		// vae::core::SPCAP::SuroundSPCAP.pan({
		// 	sin(t), cos(t), 0
		// }, p, 1, 0.5);
		// printf("%i,%f,%f,%f,%f,%f\n", i, p[0], p[1], p[2], p[3], p[4]);
	}
	return 0;
}
