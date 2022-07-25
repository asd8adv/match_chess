#include "Menu.h"
#include "GameMain.h"

/*************
***GameMenu***
*************/
GameMenu::GameMenu(std::shared_ptr<GameMain> game) :game_(game) {
	newGameBtn_.btn->setTitleText("New Game");
	newGameBtn_.btn->setTitleFontSize(24);
	newGameBtn_.btn->addTouchEventListener([this](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
		if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
			game_->startGame();
		});

	myLabel_ = cocos2d::Label::createWithTTF("GAME OVER", "Comic_Mono/ComicMono.ttf", 72);
	myLabel_->setTextColor(cocos2d::Color4B::RED);
	myLabel_->setPosition(-600, 0);
	myLabel_->setVisible(0);

	menuNode = cocos2d::Node::create();
	menuNode->addChild(newGameBtn_.btn);
	menuNode->addChild(myLabel_);
}

void GameMenu::onChangeGameInfo() {
	bool isGameOn = game_->isGameOn();
	newGameBtn_.btn->setEnabled(!isGameOn);
	myLabel_->setVisible(!isGameOn);

}