#pragma once

class MyRand {
public:
	static float _0to1();
	static float _m1to1();

private:
	static std::random_device trueRandomGenerator;
	static std::default_random_engine generator;
	static std::uniform_real_distribution<float> distribution_0to1;
	static std::uniform_real_distribution<float> distribution_minus1to1;

	MyRand() = delete;
	~MyRand() = delete;
};