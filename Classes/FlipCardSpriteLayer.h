/***************************************************************************** 
    *  @file     : FlipCardSpriteLayer.h 
    *  @version  : ver 1.0 
    *  @author   : guofeng 
    *  @date     : 2016/11/8 11:34 
    *  @brief    : 翻牌界面实现
*****************************************************************************/  
#ifndef _FLIP_CARD_LAYER_H_
#define _FLIP_CARD_LAYER_H_
#include "cocos2d.h"
#include "FlipCardLayerInterface.h"
using namespace cocos2d;
class FlipCardSpriteLayer : public GameTools::FlipCardLayerInterface
{
public:
	static FlipCardSpriteLayer* create(const char* cardBgImageName, CCRect cardFrameRect, CCSize size);
public:
	FlipCardSpriteLayer(){};
	~FlipCardSpriteLayer()
	{ 
		free(_itemProb); 
		CC_SAFE_DELETE(_pItemIdArray);
	};
	virtual bool init();
	virtual bool initData(const char* cardBgImageName, CCRect cardFrameRect, CCSize size);

	virtual void startActionEnd();
	virtual void touchCardShowCardFront(CCObject* pObject);
	virtual void touchCardActionEnd(CCObject* pObject);
private:
	int getRandomItemID();
private:
	float* _itemProb;
	CCArray* _pItemIdArray;
};
#endif //_FLIP_CARD_LAYER_H_