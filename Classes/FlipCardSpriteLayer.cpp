#include "FlipCardSpriteLayer.h"
FlipCardSpriteLayer* FlipCardSpriteLayer::create(const char* cardBgImageName, CCRect cardFrameRect, CCSize size)
{
	FlipCardSpriteLayer* pRet = new FlipCardSpriteLayer();
	if (pRet && pRet->initData(cardBgImageName,cardFrameRect,size) && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}

	delete pRet;
	pRet = NULL;
	return pRet;
}

bool FlipCardSpriteLayer::init()
{
	FlipCardLayerInterface::init();
	CCSize size = this->getContentSize();
	//绘制翻牌背景界面
	CCSprite* pBgSprite = CCSprite::create("FlipCardBg.png");
	pBgSprite->setPosition(ccp(size.width/2,size.height/2));
	this->addChild(pBgSprite);
	return true;
}

bool FlipCardSpriteLayer::initData(const char* cardBgImageName, CCRect cardFrameRect, CCSize size)
{
	_cardBgImageName = cardBgImageName;
	_cardFrameRect = cardFrameRect;
	_size = size;

	
	//初始化道具的概率 重新配置
	int arraySize = size.width * size.height;
	_itemProb = (float*)malloc(sizeof(float)* arraySize);
	memset(_itemProb, 0, sizeof(float)* arraySize);
	_itemProb[0] = 5.0f;
	_itemProb[1] = 5.0f;
	_itemProb[2] = 5.0f;
	_itemProb[3] = 5.0f;
	_itemProb[4] = 5.0f;
	_itemProb[5] = 5.0f;
	_itemProb[6] = 30.0f;
	_itemProb[7] = 20.0f;
	_itemProb[8] = 20.0f;
	_pItemIdArray = new CCArray();
	for (int i = 1; i <= arraySize; i++)
	{
		_pItemIdArray->addObject(CCInteger::create(i));
	}
	return true;
}

void FlipCardSpriteLayer::startActionEnd()
{
	FlipCardLayerInterface::startActionEnd();
}

void FlipCardSpriteLayer::touchCardShowCardFront(CCObject* pObject)
{
	GameTools::CardSprite* pCardSprite = dynamic_cast<GameTools::CardSprite*>( pObject);
	_isCanTouch = false;
	if (pCardSprite)
	{
		//主动翻牌显示牌面
		if (pCardSprite == _touchCard)
		{
			int itemId = getRandomItemID();
			pCardSprite->setItemImage(CCString::createWithFormat("FlipCardItem%d.png",itemId)->getCString(), itemId);
		}
		//自动翻牌显示牌面
		else{
			CCInteger* pInteger = (CCInteger*)_pItemIdArray->randomObject();
			int itemId = pInteger->getValue();
			_pItemIdArray->removeObject(pInteger);
			pCardSprite->setItemImage(CCString::createWithFormat("FlipCardItem%d.png", itemId)->getCString(), itemId);
		}
	}
}

void FlipCardSpriteLayer::touchCardActionEnd(CCObject* pObject)
{
	GameTools::CardSprite* pCardSprite = dynamic_cast<GameTools::CardSprite*>(pObject);
	if (pCardSprite)
	{
		//主动翻牌结束
		if (pCardSprite == _touchCard)
		{
			showAllCard();
		}
		//自动翻牌结束
		else{

		}
	}
}

int FlipCardSpriteLayer::getRandomItemID()
{
	int maxSize = _size.width * _size.height;
	cc_timeval psv;
    CCTime::gettimeofdayCocos2d(&psv, NULL);
	unsigned long int seed = psv.tv_sec * 1000 + psv.tv_usec / 1000;
	srand(seed);

	float random = CCRANDOM_0_1() * 100.0f;
	int leftTotoal = 0;
	int index = 0;
	for (int i = 0; i < maxSize; i++)
	{
		if (random >= leftTotoal && random < leftTotoal + _itemProb[i])
		{
			index = i;
			break;
		}
		leftTotoal += _itemProb[i];
	}
	int itemId = ((CCInteger*)(_pItemIdArray->objectAtIndex(index)))->getValue();
	_pItemIdArray->removeObjectAtIndex(index);
	return itemId;
}




