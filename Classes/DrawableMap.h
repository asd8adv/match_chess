#pragma once
#include <memory>
#include <string>
#include "cocos2d.h"
#include "GemDistributor.h"
#include "GameMap.h"

class DrawableGem;
struct GemFactory {
	cocos2d::Image* image;
	cocos2d::Texture2D* texture;

	void loadTexture(const std::string& filename);

	std::shared_ptr<DrawableGem> create(Gem::Type gemType, int number);

	void setGem(std::shared_ptr<DrawableGem> Drawgem, Gem::Type gem);
};

class DrawableMap :public std::enable_shared_from_this <DrawableMap> {
	using gemPtr = std::shared_ptr<DrawableGem>;
	std::map<int, gemPtr> GemMap_;
	std::set<int> selectable_;
	std::shared_ptr<GemFactory> factory_;
	std::shared_ptr<ModelMap> map_;
public:
	cocos2d::Node* mapNode;

	DrawableMap(std::shared_ptr<ModelMap> map);

	std::shared_ptr<DrawableMap> sharedDrawableMap() {
		return shared_from_this();
	}

	void init();

	void updateMap();

	void setSelect(int num, bool select);

	void resetSelect();
};


class DrawableGem {
	bool selected_ = false;
	int pos = -1;
	std::shared_ptr<DrawableMap> drawMap_;
	Gem::Type type_ = Gem::Type::none;
public:
	cocos2d::ui::Button* btn;
	cocos2d::Sprite* spriteSelect;
	cocos2d::Sprite* spriteNormal;
	cocos2d::Node* gemNode;

	DrawableGem(int number, Gem::Type type, cocos2d::Sprite* Normal, cocos2d::Sprite* Select);

	Gem::Type getType() {
		return type_;
	}

	void setType(Gem::Type type) {
		type_ = type;
	}

	void initGem(std::shared_ptr<DrawableMap> drawMap) {
		drawMap_ = drawMap;
	}

	void setSelectGem(bool select);

	void selectedChangeTo(bool select);

	void setAnchorPoint(cocos2d::Vec2 point);

	void setPosition(cocos2d::Vec2 pos);
};