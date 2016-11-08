#include "FlipCardScene.h"
#include "FlipCardSpriteLayer.h"
void FlipCardScene::runThisTest()
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	float posX = size.width / 2;
	float posY = size.height / 2;
	float rectWidth = 350.0f;
	float rectHeight = 380.0f;
	FlipCardSpriteLayer* pLayer = FlipCardSpriteLayer::create("FlipCardItemBg.png", CCRectMake(posX, posY, rectWidth, rectHeight), CCSize(3, 3));
	this->addChild(pLayer);
	CCDirector::sharedDirector()->replaceScene(this);
}