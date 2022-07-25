#include "DrawableMap.h"


/***************
***GemFactory***
***************/

void GemFactory::loadTexture(const std::string& filename) {
	image = new cocos2d::Image;
	texture = new cocos2d::Texture2D;
	image->initWithImageFile(filename);
	texture->initWithImage(image);
}

std::shared_ptr<DrawableGem> GemFactory::create(Gem::Type gemType, int number) {
	if (!texture) return nullptr;
	std::shared_ptr<DrawableGem> gem = std::make_shared<DrawableGem>(number, gemType, cocos2d::Sprite::createWithTexture(texture), cocos2d::Sprite::createWithTexture(texture));
	setGem(gem, gemType);
	return gem;
}

void GemFactory::setGem(std::shared_ptr<DrawableGem> Drawgem, Gem::Type gem) {
	if (!texture) return;
	float gemSize = 100;
	int typeNum = static_cast<int>(gem);
	cocos2d::Rect rect;
	rect.size = cocos2d::Size(gemSize, gemSize);
	rect.origin = cocos2d::Vec2(gemSize * typeNum, 0.0f);

	Drawgem->setType(gem);
	Drawgem->spriteNormal->setTextureRect(rect);
	rect.origin = cocos2d::Vec2(gemSize * typeNum, 0.0f + gemSize);
	Drawgem->spriteSelect->setTextureRect(rect);
}

/****************
***DrawableMap***
****************/

DrawableMap::DrawableMap(std::shared_ptr<ModelMap> map) :map_(map) {
	auto GemQty = GAME_MANAGER::GetInstance()->getAreaGemsQty();
	int width = GAME_MANAGER::GetInstance()->getAreaWidth();
	factory_ = std::make_shared<GemFactory>();
	mapNode = cocos2d::Node::create();
	factory_->loadTexture("gems.png");
	for (int i = 0; i < GemQty; i++) {
		std::shared_ptr<DrawableGem> gem = factory_->create(map_->getType(i), i);
		gem->setAnchorPoint({ 0.0f, 0.0f });
		gem->setPosition({ 100.0f * (i % width), 100.0f * ((GemQty / width - 1) - i / width) });
		mapNode->addChild(gem->gemNode);
		GemMap_.emplace(i, gem);

	}
	mapNode->setPosition(140, 140);
}

void DrawableMap::updateMap() {
	auto GemQty = GAME_MANAGER::GetInstance()->getAreaGemsQty();
	for (int i = 0; i < GemQty; i++) {
		auto drawableGem = GemMap_[i];
		auto modelType = map_->getType(i);
		if (modelType != drawableGem->getType())
			factory_->setGem(drawableGem, modelType);
	}
}

void DrawableMap::init() {
	std::shared_ptr<DrawableMap> drawMap = sharedDrawableMap();
	for (auto& [key, value] : GemMap_) {
		value->initGem(drawMap);
	}
};


void DrawableMap::setSelect(int num, bool select) {
	if (select) {
		selectable_.insert(num);
		if (selectable_.size() == 2) {
			int first = *(selectable_.begin());
			int second = *(std::next(selectable_.begin()));

			bool succes = map_->trySwap(first, second);
			resetSelect();
			map_->swapUpdate(first, second);
		}
	}
	else {
		selectable_.erase(num);
		GemMap_[num]->setSelectGem(select);
	}
	updateMap();
}

void DrawableMap::resetSelect() {
	while (selectable_.size() > 0) {
		setSelect(*(selectable_.begin()), false);
	}
}

/******************
***DrawableGem***
******************/
DrawableGem::DrawableGem(int number, Gem::Type type, cocos2d::Sprite* Normal, cocos2d::Sprite* Select)
	:pos(number), type_(type), spriteNormal(Normal), spriteSelect(Select) {
	btn = cocos2d::ui::Button::create("empty.png", "empty.png", "empty.png");
	btn->addTouchEventListener([&](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
		if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
			selectedChangeTo(!selected_);
		});
	gemNode = cocos2d::Node::create();
	gemNode->addChild(spriteSelect, 1);
	gemNode->addChild(spriteNormal, 1);
	gemNode->addChild(btn, 2);
}

void DrawableGem::selectedChangeTo(bool select) {
	setSelectGem(select);
	if (drawMap_)drawMap_->setSelect(pos, select);
}

void DrawableGem::setSelectGem(bool select) {
	selected_ = select;
	spriteSelect->setVisible(selected_);
	spriteNormal->setVisible(!selected_);

}

void DrawableGem::setAnchorPoint(cocos2d::Vec2 point) {
	spriteNormal->setAnchorPoint(point);
	spriteSelect->setAnchorPoint(point);
	btn->setAnchorPoint(point);
}

void DrawableGem::setPosition(cocos2d::Vec2 pos) {
	spriteNormal->setPosition(pos);
	spriteSelect->setPosition(pos);
	btn->setPosition(pos);
}
