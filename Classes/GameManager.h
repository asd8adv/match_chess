#pragma once
#include <memory>
#include <random>
#include <iostream>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

class GAME_MANAGER {
private:
	cocos2d::Vec2 areaSize_ = { 8, 8 };
	int areaGemQty_ = 0;
public:

	GAME_MANAGER() {
		areaGemQty_ = areaSize_.x * areaSize_.y;
	};
	//singleton metods
	GAME_MANAGER(GAME_MANAGER& other) = delete;
	void operator=(const GAME_MANAGER&) = delete;
	static GAME_MANAGER* GetInstance() {
		static GAME_MANAGER inst;
		return &inst;
	}
	//end singleton metods

	void setAreaSize(cocos2d::Vec2 value) {
		areaSize_ = value;
		areaGemQty_ = areaSize_.x * areaSize_.y;
	};

	cocos2d::Vec2 getAreaSize()const {
		return areaSize_;
	};

	int getAreaWidth() const {
		return areaSize_.x;
	}

	int getAreaGemsQty() const {
		return areaGemQty_;
	}

};
