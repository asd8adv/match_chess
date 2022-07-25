#pragma once
#include <iostream>
#include <set>
#include <vector>
#include <ranges>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GemDistributor.h"
#include "GameManager.h"

class GameMain;
class ModelMap;

class ModelMapEqualer {
	std::shared_ptr<ModelMap> modelMap_;
public:
	ModelMapEqualer() = default;

	void init(std::shared_ptr<ModelMap> modelMap);

	bool includedInRange(int value, int range);

	bool isEqualType(int first, int second);

	bool isHorizontMatch(int first, int second, int width);

	bool isVerticalMatch(int first, int second, int width);

	bool checkEqualHorizont(int target, int second, int width, int size, int diff);

	bool checkEqualHorizontWithSpace(int target, int second, int width, int size);

	bool isEqualHorizont(int first, int second, int width, int size);

	bool checkEqualVertical(int target, int second, int width, int size, int diff);

	bool checkEqualVerticalWithSpace(int target, int second, int width, int size);

	bool isEqualVertical(int first, int second, int width, int size);

	bool isEqualHorizontWithSpace(int first, int second, int width, int size);

	bool isEqualVerticalWithSpace(int first, int second, int width, int size);

	bool isAvailableMatch();
};

class ModelMap : public std::enable_shared_from_this <ModelMap> {
	using GemPtr = std::shared_ptr<Gem>;
	std::shared_ptr <GemDistributor> gemDistr_;
	std::vector<GemPtr> playArea_;
	std::set<int> deletable_;
	std::shared_ptr<GameMain> gm_;
	ModelMapEqualer mapEqualer_;
public:
	ModelMap(int variableCount = 5);

	void init(std::shared_ptr<GameMain> gm);

	Gem::Type getType(int i);

	int getAreaSize();

	void createArea();

	bool changeForRandomGem(int i);

	void deleteValids();

	void waterfallGems();

	void removeMatch();

	bool trySwap(int first, int second);

	void swapUpdate(int first, int second);

	bool isGemsBeside(int first, int second);

	bool hasDeleteElements();

	void brokenSwap(int first, int second);

	void checkMapMatñh();

	void checkMatchDirection(int x, int direction, std::set<int>& matches);

	void chekMatch(int pos);

	char symbolForGem(Gem::Type type)const;

	void reloadMap();

	friend std::ostream& operator<<(std::ostream& os, const ModelMap& gm) {
		int width = GAME_MANAGER::GetInstance()->getAreaWidth();
		for (size_t i = 0; i < gm.playArea_.size(); i++) {
			if (i != 0 && i % width == 0)os << "\n";
			os << gm.symbolForGem(gm.playArea_[i]->type);
			os << " ";
		}
		return os;
	}
};
//////////////////////