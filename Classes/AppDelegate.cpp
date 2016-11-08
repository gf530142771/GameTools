#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "controller.h"
USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);
	
    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

	//重新设置搜索路径
	resetSearchPath();

    // create a scene. it's an autorelease object
    //CCScene *pScene = HelloWorld::scene();

    // run
    //pDirector->runWithScene(pScene);
	CCScene * pScene = CCScene::create();
	CCLayer * pLayer = new TestController();
	pLayer->autorelease();

	pScene->addChild(pLayer);
	pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

void AppDelegate::resetSearchPath()
{
	CCFileUtils* pFileUtils = CCFileUtils::sharedFileUtils();
	pFileUtils->addSearchPath("flipCard");
}
