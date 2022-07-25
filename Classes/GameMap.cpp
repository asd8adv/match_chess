#include "iostream"
#include "GameMap.h"
#include "GameMain.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"

/*************
***ModelMap***
*************/
class GameMain;

ModelMap::ModelMap(int variableCount) {
	gemDistr_ = std::make_shared<GemDistributor>(1, variableCount);
}

void ModelMap::init(std::shared_ptr<GameMain> gm) {
	gm_ = gm;
	mapEqualer_.init(shared_from_this());
}

Gem::Type ModelMap::getType(int i) {
	if (i >= 0 && i < playArea_.size())
		return playArea_[i]->type;
	return Gem::Type::none;
}

int ModelMap::getAreaSize() {
	return playArea_.size();
}

void ModelMap::createArea() {
	auto GemQty = GAME_MANAGER::GetInstance()->getAreaGemsQty();
	playArea_.reserve(GemQty);
	for (int i =0 ; i < GemQty; i++) {
		auto it = std::make_shared<Gem>(i);
		it->type = gemDistr_->getRandomeGemType();
		playArea_.push_back(it);
	}
	checkMapMatñh();
};


bool ModelMap::changeForRandomGem(int i) {
	if (i < playArea_.size()) {
		playArea_[i]->type = gemDistr_->getRandomeGemType();
		return true;
	}
	return false;
}

void ModelMap::deleteValids() {
	for (auto i : deletable_) {
		playArea_[i]->type = Gem::Type::none;
	}
}

void ModelMap::waterfallGems() {
	auto width = GAME_MANAGER::GetInstance()->getAreaWidth();
	auto gemsqty = GAME_MANAGER::GetInstance()->getAreaGemsQty();
	std::ranges::reverse_view reverseArea{ deletable_ };

	for (auto it : deletable_) {
		int index = it;
		for (; index >= width; index -= width)
			std::swap(playArea_[index]->type, playArea_[index - width]->type);
		changeForRandomGem(index);
	}
	gm_->incrementStats(deletable_.size(), game_stats::score);
	deletable_.clear();
	checkMapMatñh();
}

void ModelMap::removeMatch() {
	deleteValids();
	waterfallGems();
}

bool ModelMap::trySwap(int first, int second) {
	std::swap(playArea_[first]->type, playArea_[second]->type);
	if(!isGemsBeside(first, second)) return false;
	chekMatch(first);
	chekMatch(second);
	return hasDeleteElements();
};

bool ModelMap::isGemsBeside(int first, int second) {
	auto width = GAME_MANAGER::GetInstance()->getAreaWidth();
	int value = abs(first - second);
	return value == 1 || value == width;
}

void ModelMap::swapUpdate(int first, int second) {
	if (hasDeleteElements()) {
		removeMatch();
	}
	else
		brokenSwap(first, second);
}

bool ModelMap::hasDeleteElements() {
	return deletable_.size();
};

void ModelMap::brokenSwap(int first, int second) {
	//show Effect
	std::swap(playArea_[first]->type, playArea_[second]->type);
};

void ModelMap::checkMapMatñh() {
	for (auto gem : playArea_) {
		if (hasDeleteElements()) {
			removeMatch();
			break;
		}
		chekMatch(gem->posInArea);
	}
	if (!mapEqualer_.isAvailableMatch())
		gm_->stopGame();
}

void ModelMap::checkMatchDirection(int x, int direction, std::set<int>& matches) {

	int width = GAME_MANAGER::GetInstance()->getAreaWidth();
	for (int y = x; y >= 0 && y < playArea_.size(); y += direction) {
		if (*playArea_[x] == *playArea_[y]) {
			matches.insert(y);
		}
		else
			break;

		if (std::abs(direction) == 1) {
			if (y / width != x / width)break;
		}
		else {
			if (y % width != x % width)break;

		}
	}
}

void ModelMap::chekMatch(int pos) {
	std::set<int> VerticaleMatch;
	std::set<int> HorizontalMatch;
	int width = GAME_MANAGER::GetInstance()->getAreaWidth();
	int leftDirection = -1;
	int rightDirection = 1;
	int botDirection = width;
	int topDirection = -1 * width;

	checkMatchDirection(pos, leftDirection, HorizontalMatch);
	checkMatchDirection(pos, rightDirection, HorizontalMatch);
	checkMatchDirection(pos, botDirection, VerticaleMatch);
	checkMatchDirection(pos, topDirection, VerticaleMatch);

	if (VerticaleMatch.size() >= 3) {
		deletable_.insert(VerticaleMatch.begin(), VerticaleMatch.end());
	}
	if (HorizontalMatch.size() >= 3) {
		deletable_.insert(HorizontalMatch.begin(), HorizontalMatch.end());
	}
}

void ModelMap::reloadMap() {
	for (auto gem : playArea_) {
		gem->type = gemDistr_->getRandomeGemType();
	}
	checkMapMatñh();
}

char ModelMap::symbolForGem(Gem::Type type)const {
	switch (type)
	{
	case Gem::Type::orange:	return 'o';
	case Gem::Type::green:	return 'g';
	case Gem::Type::blue:	return 'b';
	case Gem::Type::red:	return 'r';
	case Gem::Type::yellow:	return 'y';
	case Gem::Type::purple:	return 'p';
	case Gem::Type::none:	return 'n';
	default:				return 'n';
	}
}


/********************
***ModelMapEqualer***
********************/


void ModelMapEqualer::init(std::shared_ptr<ModelMap> modelMap) {
	modelMap_ = modelMap;
}

bool ModelMapEqualer::includedInRange(int value, int range) {
	return value >= 0 && value < range;
}

bool ModelMapEqualer::isEqualType(int first, int second) {
	return modelMap_->getType(first) == modelMap_->getType(second);
}

bool ModelMapEqualer::isHorizontMatch(int first, int second, int width) {
	return first / width == second / width;
}
bool ModelMapEqualer::isVerticalMatch(int first, int second, int width) {
	return first % width == second % width;
}

bool ModelMapEqualer::checkEqualHorizont(int target, int second, int width, int size, int diff) {
	return(isHorizontMatch(target, second, width) &&
		((includedInRange(target + diff, size) && isHorizontMatch(target + diff, target, width) && isEqualType(target + diff, second)) ||
			checkEqualHorizontWithSpace(target, second, width, size)));
}

bool ModelMapEqualer::checkEqualHorizontWithSpace(int target, int second, int width, int size) {
	return(isHorizontMatch(target, second, width) &&
		((includedInRange(target + width, size) && isVerticalMatch(target + width, target, width) && isEqualType(target + width, second)) ||
			(includedInRange(target - width, size) && isVerticalMatch(target - width, target, width) && isEqualType(target - width, second))));
}

bool ModelMapEqualer::isEqualHorizont(int first, int second, int width, int size) {
	if (isHorizontMatch(first, second, width)) {
		int min = std::min(first, second);
		int max = std::max(first, second);
		if (checkEqualHorizont(min - 1, min, width, size, -1) ||
			checkEqualHorizont(max + 1, max, width, size, 1))
			return true;
	}
	return false;
}

bool ModelMapEqualer::checkEqualVertical(int target, int second, int width, int size, int diff) {
	return(isVerticalMatch(target, second, width) &&
		((includedInRange(target + diff, size) && isVerticalMatch(target + diff, target, width) && isEqualType(target + diff, second)) ||
			checkEqualVerticalWithSpace(target, second, width, size)));
}

bool ModelMapEqualer::checkEqualVerticalWithSpace(int target, int second, int width, int size) {
	return(isVerticalMatch(target, second, width) &&
		((includedInRange(target + 1, size) && isHorizontMatch(target + 1, target, width) && isEqualType(target + 1, second)) ||
			(includedInRange(target - 1, size) && isHorizontMatch(target - 1, target, width) && isEqualType(target - 1, second))));
}
bool ModelMapEqualer::isEqualVertical(int first, int second, int width, int size) {
	if (isVerticalMatch(first, second, width)) {
		int min = std::min(first, second);
		int max = std::max(first, second);
		if (checkEqualVertical(min - width, min, width, size, -1 * width) ||
			checkEqualVertical(max + width, max, width, size, width))
			return true;
	}
}

bool ModelMapEqualer::isEqualHorizontWithSpace(int first, int second, int width, int size) {
	if (isHorizontMatch(first, second, width)) {
		int min = std::min(first, second);
		int upper = min + width + 1;
		int lower = min - width + 1;
		if ((includedInRange(lower, size) && isEqualType(lower, first)) ||
			(includedInRange(upper, size) && isEqualType(upper, first)))
			return true;
	}
	return false;
}

bool ModelMapEqualer::isEqualVerticalWithSpace(int first, int second, int width, int size) {
	if (first % width == second % width) {
		int min = std::min(first, second);
		int upper = min + width + 1;
		int lower = min + width - 1;
		if ((includedInRange(lower, size) && isEqualType(lower, first)) ||
			(includedInRange(upper, size) && isEqualType(upper, first)))
			return true;
	}
	return false;
}

bool ModelMapEqualer::isAvailableMatch() {
	int width = GAME_MANAGER::GetInstance()->getAreaWidth();
	std::vector<int> horizontalDir = { 1,-1 };
	std::vector<int> vecrticalDir = { width,-1 * width };
	std::vector<int> horizontalDirWithSpace = { 2,-2 };
	std::vector<int> vecrticalDirWithSpace = { 2 * width,-2 * width };
	int size = modelMap_->getAreaSize();
	for (int i = 0; i < size; i++) {
		for (auto it : horizontalDir) {
			if (includedInRange(i + it, size) && isEqualType(i + it, i) && (isEqualHorizont(i + it, i, width, size))) {
				return true;
			}
		}
		for (auto it : vecrticalDir) {
			if (includedInRange(i + it, size) && isEqualType(i + it, i) && (isEqualVertical(i + it, i, width, size))) {
				return true;
			}
		}
		for (auto it : horizontalDirWithSpace) {
			if (includedInRange(i + it, size) && isEqualType(i + it, i) && (isEqualHorizontWithSpace(i + it, i, width, size))) {
				return true;
			}
		}
		for (auto it : vecrticalDirWithSpace) {
			if (includedInRange(i + it, size) && isEqualType(i + it, i) && (isEqualVerticalWithSpace(i + it, i, width, size))) {
				return true;
			}
		}
	}
	return false;
}

