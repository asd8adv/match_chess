#pragma once
#include <memory>
#include "cocos2d.h"

class ModelMap;
class DrawableMap;
class GameMenu;

struct GameData {
	int score = 0;
	bool gameOver = true;

	void restart();

	void incrementScore(int value);
};

enum class game_stats : int {
	none = 0,
	score = 1
};

class GameMain :public std::enable_shared_from_this <GameMain> {
	std::shared_ptr<ModelMap> modelMap_;
	std::shared_ptr<DrawableMap> DrawableMap_;
	std::shared_ptr<GameMenu> gameMenu_;
	GameData data_;
public:
	cocos2d::Node* gameMainNode;

	bool isGameOn();

	void init();

	void incrementStats(int value, game_stats stat);

	int getStatsValue(game_stats stat);

	void stopGame();

	void startGame();


	GameMain() = default;
};