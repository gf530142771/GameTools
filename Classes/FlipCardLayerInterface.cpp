#include "FlipCardLayerInterface.h"
#include "AppDelegate.h"
GameTools::CardSprite* GameTools::CardSprite::create(const char* imageName, int index)
{
	GameTools::CardSprite* pRet = new GameTools::CardSprite(index);
	if (pRet && pRet->initWithFile(imageName))
	{
		pRet->autorelease();
		return pRet;
	}
	delete pRet;
	pRet = NULL;
	return pRet;
}

GameTools::CardSprite::CardSprite(int index)
:_itemSprite(NULL)
, _index(index)
{

}

GameTools::CardSprite::~CardSprite()
{

}
void GameTools::CardSprite::setItemImage(const char* ImageName, int itemID)
{
	if (_itemSprite == NULL)
	{
		_itemSprite = CCSprite::create(ImageName);
		_itemSprite->setAnchorPoint(ccp(0,0));
		this->addChild(_itemSprite);
	}
	_itemId = itemID;

	CCTextureCache::sharedTextureCache()->addImage(ImageName);
	_itemSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(ImageName));
	_itemSprite->setVisible(true);

}
int GameTools::CardSprite::getItemId()
{
	return _itemId;
}

int GameTools::CardSprite::getIndex()
{
	return _index;
}


void GameTools::CardSprite::actionFlipToBackGround(CCCallFunc* pFunction /* = NULL */)
{
	if (pFunction != NULL)
	{
		this->runAction(CCSequence::create(
			CCDelayTime::create(1.0f),
			CCScaleTo::create(0.2f, 0.1f, 1.0f),
			CCCallFunc::create(this, callfunc_selector(CardSprite::hideItemImag)),
			CCScaleTo::create(0.2f, 1.0f, 1.0f),
			pFunction
			, NULL));

	}
	else{
		this->runAction(CCSequence::create(
			CCDelayTime::create(1.0f),
			CCScaleTo::create(0.2f, 0.1f, 1.0f),
			CCCallFunc::create(this, callfunc_selector(CardSprite::hideItemImag)),
			CCScaleTo::create(0.2f, 1.0f, 1.0f)
			, NULL));
	}
}
void GameTools::CardSprite::actionMoveToCenter(CCPoint centerPos, CCCallFunc* pFunction)
{
	_origionPos = this->getPosition();
	if (pFunction != NULL)
	{
		this->runAction(
			CCSequence::create(
			CCDelayTime::create(_index*0.1f),
			CCMoveTo::create(0.2f, centerPos),
			CCDelayTime::create(0.5f),
			pFunction,
			NULL));
	}
	else{
		this->runAction(
			CCSequence::create(
			CCDelayTime::create(_index*0.1f),
			CCMoveTo::create(0.2f, centerPos),
			NULL));
	}
}
void GameTools::CardSprite::actionMoveToOriPos(CCCallFunc* pFunction /* = NULL */)
{
	if (pFunction != NULL)
	{
		this->runAction(
			CCSequence::create(
			CCDelayTime::create(_index*0.1f),
			CCMoveTo::create(0.2f, _origionPos),
			pFunction,
			NULL));
	}
	else{
		this->runAction(
			CCSequence::create(
			CCDelayTime::create(_index*0.1f),
			CCMoveTo::create(0.2f, _origionPos),
			NULL));
	}
}

//移动到正面
void GameTools::CardSprite::actionFlipToFront(CCCallFuncO* pFuncShowFace, CCCallFuncO* pFuncActionEnd)
{
	if (pFuncActionEnd == NULL)
	{
		this->runAction(CCSequence::create(
			CCScaleTo::create(0.2f, 0.1f, 1.0f),
			pFuncShowFace,
			CCScaleTo::create(0.2f, 1.0f, 1.0f),
			NULL));
	}
	else{
		this->runAction(CCSequence::create(
			CCScaleTo::create(0.2f, 0.1f, 1.0f),
			pFuncShowFace,
			CCScaleTo::create(0.2f, 1.0f, 1.0f),
			pFuncActionEnd,
			NULL));
	}
	
}

void GameTools::CardSprite::hideItemImag()
{
	if (_itemSprite)
	{
		_itemSprite->setVisible(false);
	}
}



/************************************************************************/

bool GameTools::FlipCardLayerInterface::init()
{
	CCLayer::init();
	//绘制卡片绘制区域
	_CardFrameNode = CCNode::create();
	_CardFrameNode->setAnchorPoint(ccp(0.5f, 0.5f));
	_CardFrameNode->setContentSize(_cardFrameRect.size);
	_CardFrameNode->setPosition(_cardFrameRect.origin);
	_CardFrameNode->addChild(CCLayerColor::create(
		ccc4(255, 0, 0, 100), _cardFrameRect.size.width, _cardFrameRect.size.height));
	this->addChild(_CardFrameNode, 10);

	//计算图片间隔
	int row = _size.width;
	int col = _size.height;
	CCTextureCache::sharedTextureCache()->addImage(_cardBgImageName);
	CCSize cardBgSize = CCTextureCache::sharedTextureCache()->textureForKey(_cardBgImageName)->getContentSize();
	float marginX = (_cardFrameRect.size.width - (cardBgSize.width * col)) / (col + 1);
	float marginY = (_cardFrameRect.size.height - (cardBgSize.height * row)) / (row + 1);
	float leftPosX = 0;
	float topPosY = _cardFrameRect.size.height;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			int index = row * i + j + 1;
			float posX = leftPosX + marginX + cardBgSize.width/2 + (marginX + cardBgSize.width) * (j%col);
			float posY = topPosY - marginY - cardBgSize.height /2 - (marginY + cardBgSize.height) * i;
			GameTools::CardSprite* pCardSprite = CardSprite::create(_cardBgImageName,index - 1);
			pCardSprite->setPosition(ccp(posX, posY));
			pCardSprite->setItemImage(CCString::createWithFormat("FlipCardItem%d.png", index)->getCString(), index - 1);
			_CardFrameNode->addChild(pCardSprite);
			_vecCardSprite.push_back(pCardSprite);
		}
	}

	_actionSequence = 1;
	runActionBySequence();

	//设置触屏
	setTouchEnabled(true);
	_isCanTouch = false;
	_touchCard = NULL;
	return true;
}

void GameTools::FlipCardLayerInterface::startActionEnd()
{
	_isCanTouch = true;
}
void GameTools::FlipCardLayerInterface::runActionBySequence()
{
	switch (_actionSequence)
	{
	//翻到背面
	case 1:
	{
			  for (unsigned int i = 0; i < _vecCardSprite.size(); i++)
			  {
				  if (i == _vecCardSprite.size() - 1)
				  {
					  _vecCardSprite[i]->actionFlipToBackGround(CCCallFunc::create(this, callfunc_selector(GameTools::FlipCardLayerInterface::runActionBySequence)));
				  }
				  else{
					  _vecCardSprite[i]->actionFlipToBackGround();
				  }
			  }
	}
		break;
	//移动到中心
	case 2:
	{
			  CCPoint centerPos = ccp(_cardFrameRect.size.width / 2, _cardFrameRect.size.height/2);
			   for (unsigned int i = 0; i < _vecCardSprite.size(); i++)
			   {
				   if (i == _vecCardSprite.size() - 1)
				   {
					   _vecCardSprite[i]->actionMoveToCenter(centerPos, CCCallFunc::create(this, callfunc_selector(GameTools::FlipCardLayerInterface::runActionBySequence)));
				   }
				   else{
					   _vecCardSprite[i]->actionMoveToCenter(centerPos);
				   }
			   }
	}
		break;
	case 3://分开
	{
			   for (unsigned int i = 0; i < _vecCardSprite.size(); i++)
			   {
				   if (i == _vecCardSprite.size() - 1)
				   {
					   _vecCardSprite[i]->actionMoveToOriPos( CCCallFunc::create(this, callfunc_selector(GameTools::FlipCardLayerInterface::startActionEnd)));
				   }
				   else{
					   _vecCardSprite[i]->actionMoveToOriPos();
				   }
			   }
	}
	break;
	}
	_actionSequence++;
}

// 		// 	case 4://选牌
// 		// 	{
// 		// 			   CCMenu* menu = (CCMenu*)this->getChildByTag(TAG_GETBTN);
// 		// 			   CCMenuItemLabel* btn = (CCMenuItemLabel*)menu->getChildByTag(2);
// 		// 			   btn->runAction(CCFadeIn::create(0.2));
// 		// 
// 		// 			   for (int i = 0; i < itemLen; i++)
// 		// 			   {
// 		// 				   itemBtn[i]->setEnabled(true);
// 		// 			   }
// 		// 	}
// 		// 		break;
// 		// 	case 5://禁止选牌
// 		// 		for (int i = 0; i < itemLen; i++)
// 		// 		{
// 		// 			itemBtn[i]->setEnabled(false);
// 		// 		}
// 		// 		break;
// 		// 	case 6://显示全部领取
// 		// 	{
// 		// 			   CCMenu* menu = (CCMenu*)this->getChildByTag(TAG_GETBTN);
// 		// 
// 		// 			   CCMenuItemSprite* closBtn = (CCMenuItemSprite*)menu->getChildByTag(1);
// 		// 			   closBtn->runAction(CCFadeIn::create(0.2));
// 		// 
// 		// 			   CCMenuItemSprite* buyBtn = (CCMenuItemSprite*)menu->getChildByTag(2);
// 		// 
// 		// 			   this->setColorLayer();
// 		// 			   // 			if(CGlobalData::getSingleton()->getNoviceNum() == NOVICE_104)
// 		// 			   // 			{
// 		// 			   // 				buyBtn->setNormalImage(CCSprite::create("FlipCardGetBtn3.png"));
// 		// 			   // 				buyBtn->setSelectedImage(CCSprite::create("FlipCardGetBtn3.png"));
// 		// 			   // 			}
// 		// 			   // 			else
// 		// 			   // 			{
// 		// 			   // 
// 		// 			   // 				buyBtn->setNormalImage( CCSprite::create(LiuYangTool::getPackageName("FlipCardGetBtn1.png",true,1).c_str()));
// 		// 			   // 				buyBtn->setSelectedImage( LiuYangTool::getSprite(LiuYangTool::getPackageName("FlipCardGetBtn1.png",true,1).c_str()));
// 		// 			   // 			}
// 		// 			   buyBtn->setTag(3);
// 		// 			   buyBtn->setZOrder(14);
// 		// 			   buyBtn->setEnabled(true);
// 		// 			   buyBtn->runAction(CCSequence::create(
// 		// 				   CCShow::create(),
// 		// 				   CCCallFunc::create(this, callfunc_selector(FlipCardLayerInterface::actionSequence)),
// 		// 				   NULL));
// 		// 	}
// 		// 		break;
// 		// 	case 7://其他奖励翻开
// 		// 	{
// 		// 			   showItemId = 2;
// 		// 			   for (int i = 0; i < itemLen; i++)
// 		// 			   {
// 		// 				   if (itemData[i] != 0)
// 		// 				   {
// 		// 					   itemBtn[i]->runAction(
// 		// 						   CCSequence::create(
// 		// 						   //CCDelayTime::create(i*0.1f),
// 		// 						   CCScaleTo::create(0.2f, 0.1, 1),
// 		// 						   CCCallFuncN::create(this, callfuncN_selector(FlipCardLayerInterface::showFace)),
// 		// 						   CCScaleTo::create(0.2f, 1, 1)
// 		// 						   , NULL));
// 		// 				   }
// 		// 			   }
// 		// 			   // 			if(CGlobalData::getSingleton()->getNoviceNum()== NOVICE_104)
// 		// 			   // 			{
// 		// 			   // 				NoviceGuideLayer::show(this);
// 		// 			   // 			}
// 		// 	}
// 		// 		break;
// 		// 	case 8://领取其他奖励
// 		// 	{
// 		// 			   for (int i = 0; i < itemLen; i++)
// 		// 			   {
// 		// 				   if (itemData[i] != 0)
// 		// 				   {
// 		// 
// 		// 					   this->addItem(itemData[i]);
// 		// 					   itemData[i] = 0;
// 		// 				   }
// 		// 			   }
// 		// 	}
// 		// 		break;

void GameTools::FlipCardLayerInterface::showAllCard()
{
	for (unsigned int i = 0; i < _vecCardSprite.size(); i++)
	{
		if (i == _vecCardSprite.size() - 1)
		{
			_vecCardSprite[i]->actionFlipToFront(
				CCCallFuncO::create(this, callfuncO_selector(GameTools::FlipCardLayerInterface::touchCardShowCardFront), _vecCardSprite[i]),
				CCCallFuncO::create(this, callfuncO_selector(GameTools::FlipCardLayerInterface::touchCardActionEnd), _vecCardSprite[i])
				);
		}
		else{
			_vecCardSprite[i]->actionFlipToFront(
				CCCallFuncO::create(this, callfuncO_selector(GameTools::FlipCardLayerInterface::touchCardShowCardFront), _vecCardSprite[i]));
		}
	}
}
/*触屏监听*/
bool GameTools::FlipCardLayerInterface::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	if (!_isCanTouch)
		return true;

	CCPoint touchPos = _CardFrameNode->convertToNodeSpace(pTouch->getLocation());
	for (unsigned int i = 0; i < _vecCardSprite.size(); i++)
	{
		if (_vecCardSprite[i]->boundingBox().containsPoint(touchPos))
		{
			_touchCard = _vecCardSprite[i];
			return true;
		}
	}
	return true;
}
void GameTools::FlipCardLayerInterface::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint touchPos = _CardFrameNode->convertToNodeSpace(pTouch->getLocation());
	std::vector<CardSprite*>::iterator it = _vecCardSprite.begin();
	for (unsigned int i = 0; i < _vecCardSprite.size(); i++)
	{
		if (_vecCardSprite[i]->boundingBox().containsPoint(touchPos) && _touchCard == _vecCardSprite[i])
		{
			_touchCard = _vecCardSprite[i];
			_vecCardSprite[i]->actionFlipToFront(
				CCCallFuncO::create(this, callfuncO_selector(GameTools::FlipCardLayerInterface::touchCardShowCardFront), _touchCard),
				CCCallFuncO::create(this, callfuncO_selector(GameTools::FlipCardLayerInterface::touchCardActionEnd), _touchCard)
				);

			_vecCardSprite.erase(it);
			break;
		}
		it++;
	}
}
void GameTools::FlipCardLayerInterface::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
}