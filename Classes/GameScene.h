
#ifndef __GameScene_SCENE_H__
#define __GameScene_SCENE_H__

#define COCOS2D_DEBUG 1
#include "cocos2d.h"
#include "GameMain.h"
#include <memory>

class GameScene : public cocos2d::Scene
{
	std::shared_ptr<GameMain> GM;
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    CREATE_FUNC(GameScene);
};

#endif // __GameScene_SCENE_H__
