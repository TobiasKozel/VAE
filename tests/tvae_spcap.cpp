#include <stdio.h>
#include <cmath>
// #include <new>
#define VAE_IMPL
// #define TKLB_NO_STDLIB
// #define TKLB_NO_LOG
#include "../src/core/algo/vae_spcap.hpp"


void test51() {
	printf("deg,FL,FR,RL,RR,C\n");
	for (int i = 0; i < 360; i += 10) {
		float t = i / float(180) * 3.14159265358979323846;
		vae::Sample p[5];
		const float x = float(sin(t));
		const float z = float(cos(t));
		vae::core::SPCAP::SuroundSPCAP.pan({ x, 0, z }, p, 1.0, 10.0);
		printf("%i,%f,%f,%f,%f,%f\n", i, p[0], p[1], p[2], p[3], p[4]);
	}
}

void testHeadPhones() {
	printf("deg,L,R\n");
	for (int i = 0; i < 360; i += 10) {
		float t = i / float(180) * 3.14159265358979323846;
		vae::Sample p[2];
		const float x = float(sin(t));
		const float z = float(cos(t));
		vae::core::SPCAP::HeadphoneSPCAP.pan({ x, 0, z }, p, 1.0, 1.0);
		printf("%i,%f,%f\n", i, p[0], p[1]);
	}
}

void testStereo() {
	printf("deg,FL,FR\n");
	for (int i = 0; i < 360; i += 10) {
		float t = i / float(180) * 3.14159265358979323846;
		vae::Sample p[2];
		const float x = float(sin(t));
		const float z = float(cos(t));
		vae::core::SPCAP::StereroSPCAP.pan({ x, 0, z }, p, 1.0, 1.0);
		printf("%i,%f,%f\n", i, p[0], p[1]);
	}
}

int main() {
	// testStereo();
	test51();
	// vae::Sample p[2];
	// vae::core::SPCAP::StereroSPCAP.pan({ (float)sin(0.3), 0, (float)cos(0.3) }, p, 1.0, 1.0);
	return 0;
}
