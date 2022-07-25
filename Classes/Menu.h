#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <memory>

class GameMain;

struct MenuButton {
	cocos2d::ui::Button* btn;
	MenuButton() {
		btn = cocos2d::ui::Button::create("btn1_normal.png", "btn1_select.png", "btn1_disabled.png");
	};

};

class GameMenu {
	MenuButton newGameBtn_;
	cocos2d::Label* myLabel_;
	std::shared_ptr<GameMain> game_;
public:
	cocos2d::Node* menuNode;
public:
	GameMenu(std::shared_ptr<GameMain> game);

	void setEnabledNewGame(bool enabled) {
		newGameBtn_.btn->setEnabled(enabled);
	}

	void onChangeGameInfo();
};