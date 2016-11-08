#ifndef _FLIPCARDLAYER_H_
#define _FLIPCARDLAYER_H_
#include "cocos2d.h"
using namespace cocos2d;
namespace GameTools
{
	/*****************************************************************************
	*  @file     : CardSprite
	*  @version  : ver 1.0
	*  @author   : guofeng
	*  @date     : 2016/11/8 10:43
	*  @brief    : 卡片精灵
	*****************************************************************************/
	class CardSprite : public cocos2d::CCSprite
	{
	public:
		static CardSprite* create(const char* imageName, int index);
	public:
		CardSprite(int index);
		~CardSprite();
		//翻到背面
		void actionFlipToBackGround(CCCallFunc* pFunction = NULL);
		//移动到中心位置
		void actionMoveToCenter(CCPoint centerPos, CCCallFunc* pFunction = NULL);
		//移动到原始位置
		void actionMoveToOriPos(CCCallFunc* pFunction = NULL);
		//移动到正面
		void actionFlipToFront(CCCallFuncO* pFuncShowFace , CCCallFuncO* pFuncActionEnd = NULL);
		void setItemImage(const char* ImageName, int itemID);
		int getItemId();
		int getIndex();
	private:
		void hideItemImag();
	private:
		CCSprite* _itemSprite;
		int _itemId;
		int _index;
		CCPoint _origionPos;
	};

	/*****************************************************************************
	*  @file     : FlipCardLayerInterface.h
	*  @version  : ver 1.0
	*  @author   : guofeng
	*  @date     : 2016/11/7 16:46
	*  @brief    : 翻牌界面接口，翻牌界面的父类
	*****************************************************************************/
	class FlipCardLayerInterface : public cocos2d::CCLayer
	{
	public:
		FlipCardLayerInterface(){};
		virtual ~FlipCardLayerInterface(){};
		virtual bool init();
		/*
		@纯虚函数 初始化所需数据
		@param cardBgImageName 卡片背景图片
		@param cardFrameRect   绘制卡片的范围(x,y位置，width,height宽高)
		@param size			   几行几列 x 行 y 列
		*/
		virtual bool initData(const char* cardBgImageName, CCRect cardFrameRect, CCSize size) = 0;
		/*开场动画结束*/
		virtual void startActionEnd();
		//显示卡片的正面
		virtual void touchCardShowCardFront(CCObject* pObject) = 0;
		//选择卡片翻转结束
		virtual void touchCardActionEnd(CCObject* pObject) = 0;

		void runActionBySequence();

		void showAllCard();

		/*触屏监听*/
		virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
		virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
		virtual void registerWithTouchDispatcher();
	protected:
		//图片精灵
		std::vector<CardSprite*> _vecCardSprite;
		//卡片背景图片
		const char* _cardBgImageName;
		//绘制卡片的范围(位置，宽高)
		CCRect _cardFrameRect;
		//几行几列
		CCSize _size;
		int _actionSequence;
		bool _isCanTouch;

		CCNode* _CardFrameNode;
		CardSprite* _touchCard;
	};
}
#endif


