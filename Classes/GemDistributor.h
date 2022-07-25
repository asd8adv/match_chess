#pragma once
#include <memory>
#include <random>
#include <iostream>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

class randomize {
	int32_t min_;
	int32_t max_;

	std::unique_ptr<std::mt19937> rand_;
public:
	randomize(int32_t min, int32_t max) :min_(min), max_(max) {
		if (min > max)std::cerr << "error min/max value\n";
		rand_ = std::make_unique<std::mt19937>(std::mt19937(time(0)));
	}

	randomize(int32_t max) :min_(0), max_(max) {
		if (0 > max)std::cerr << "error min/max value\n";
		rand_ = std::make_unique<std::mt19937>(std::mt19937(time(0)));
	};

	int32_t get() {
		auto res = (*rand_)() % (max_ - min_ + 1);
		return min_ + res;
	}


	void setMin(int32_t value) {
		min_ = value;
	}

	int32_t getMin() const {
		return min_;
	}

	void setMax(int32_t value) {
		min_ = value;
	}

	int32_t getMax() const {
		return min_;
	};
};


struct Gem {
	enum class Type :int {
		none,
		orange,
		green,
		blue,
		red,
		yellow,
		purple
	};

	cocos2d::Vec2 pos = { 0,0 };
	int posInArea = 0;
	Type type = Type::none;
	//metods:
	Gem(int pos) :
		posInArea(pos)
	{};


	friend bool operator==(const Gem& lhs, const Gem& rhs) {
		return lhs.type == rhs.type;
	};
};

struct GemDistributor {
	randomize randome;
	GemDistributor(int min, int max) :randome(min, max) {
	}

	Gem::Type getRandomeGemType() {
		return static_cast<Gem::Type>(randome.get());
	}

};
