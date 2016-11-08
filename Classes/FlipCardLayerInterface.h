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
	*  @brief    : ��Ƭ����
	*****************************************************************************/
	class CardSprite : public cocos2d::CCSprite
	{
	public:
		static CardSprite* create(const char* imageName, int index);
	public:
		CardSprite(int index);
		~CardSprite();
		//��������
		void actionFlipToBackGround(CCCallFunc* pFunction = NULL);
		//�ƶ�������λ��
		void actionMoveToCenter(CCPoint centerPos, CCCallFunc* pFunction = NULL);
		//�ƶ���ԭʼλ��
		void actionMoveToOriPos(CCCallFunc* pFunction = NULL);
		//�ƶ�������
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
	*  @brief    : ���ƽ���ӿڣ����ƽ���ĸ���
	*****************************************************************************/
	class FlipCardLayerInterface : public cocos2d::CCLayer
	{
	public:
		FlipCardLayerInterface(){};
		virtual ~FlipCardLayerInterface(){};
		virtual bool init();
		/*
		@���麯�� ��ʼ����������
		@param cardBgImageName ��Ƭ����ͼƬ
		@param cardFrameRect   ���ƿ�Ƭ�ķ�Χ(x,yλ�ã�width,height���)
		@param size			   ���м��� x �� y ��
		*/
		virtual bool initData(const char* cardBgImageName, CCRect cardFrameRect, CCSize size) = 0;
		/*������������*/
		virtual void startActionEnd();
		//��ʾ��Ƭ������
		virtual void touchCardShowCardFront(CCObject* pObject) = 0;
		//ѡ��Ƭ��ת����
		virtual void touchCardActionEnd(CCObject* pObject) = 0;

		void runActionBySequence();

		void showAllCard();

		/*��������*/
		virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
		virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
		virtual void registerWithTouchDispatcher();
	protected:
		//ͼƬ����
		std::vector<CardSprite*> _vecCardSprite;
		//��Ƭ����ͼƬ
		const char* _cardBgImageName;
		//���ƿ�Ƭ�ķ�Χ(λ�ã����)
		CCRect _cardFrameRect;
		//���м���
		CCSize _size;
		int _actionSequence;
		bool _isCanTouch;

		CCNode* _CardFrameNode;
		CardSprite* _touchCard;
	};
}
#endif


