#include "GameScene.h"

USING_NS_CC;
	
Scene* GameScene::createScene()
{
    return GameScene::create();
}

bool GameScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	GM = std::make_shared<GameMain>();
	GM->init();
	this->addChild(GM->gameMainNode, 1);
	
	auto backGroundSprite = Sprite::create("main_back_1.png");
	backGroundSprite->setPosition(0,0);
	backGroundSprite->setAnchorPoint({ 0, 0 });
	this->addChild(backGroundSprite, 0);
	
    return true;
}


void GameScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}
