#include "GameMain.h"
#include "GameMap.h"
#include "DrawableMap.h"
#include "Menu.h"

/*************
***GameData***
*************/
void GameData::restart() {
	score = 0;
	gameOver = false;
}

void GameData::incrementScore(int value) {
	score += value; 
};

/*************
***GameMain***
*************/


bool GameMain::isGameOn() {
	return !data_.gameOver;
}

void GameMain::init() {
	gameMainNode = cocos2d::Node::create();
	modelMap_ = std::make_shared<ModelMap>();
	modelMap_->init(shared_from_this());
	DrawableMap_ = std::make_shared<DrawableMap>(modelMap_);
	gameMenu_ = std::make_shared<GameMenu>(shared_from_this());
	gameMenu_->menuNode->setPosition({ 1300, 1000 });
	DrawableMap_->init();
	gameMainNode->addChild(DrawableMap_->mapNode, 1);
	gameMainNode->addChild(gameMenu_->menuNode, 2);
	modelMap_->createArea();
	DrawableMap_->updateMap();
	gameMenu_->setEnabledNewGame(false);
}

void GameMain::incrementStats(int value, game_stats stat) {
	switch (stat) {
	case(game_stats::score):
		data_.incrementScore(value);
		break;
	default:
		break;
	}
}

int GameMain::getStatsValue(game_stats stat) {
	switch (stat) {
	case(game_stats::score):
		return data_.score;
		break;
	default:
		break;
	}
	return 0;
}

void GameMain::stopGame() {
	data_.gameOver = true;
	DrawableMap_->mapNode;
	gameMenu_->onChangeGameInfo();
}

void GameMain::startGame() {
	if (!isGameOn()) {
		modelMap_->reloadMap();
		DrawableMap_->updateMap();
		data_.restart();
		DrawableMap_->mapNode->setVisible(true);
	}
	gameMenu_->onChangeGameInfo();
}

