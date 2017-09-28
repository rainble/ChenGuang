#include "StartScene.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"



USING_NS_CC;

Scene* StartScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = StartScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool StartScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    SpriteFrameCache::getInstance() -> addSpriteFramesWithFile("images.plist");
    auto texture = Director::getInstance() -> getTextureCache() -> addImage("images.pvr.ccz");
    auto batchNode = SpriteBatchNode::createWithTexture(texture);
    this -> addChild(batchNode);
    
    //设置背景
    auto bg = Sprite::createWithSpriteFrameName("background.png");
    bg -> setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this -> addChild(bg);
    
    //背景音乐
    CocosDenshion::SimpleAudioEngine::getInstance() -> preloadBackgroundMusic("game_music.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance() -> playBackgroundMusic("game_music.mp3", true);
    
    //飞机大战的logo
    auto title = Sprite::createWithSpriteFrameName("name.png");
    title -> setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height - title -> getContentSize().height - 50));
    title -> setScale(0.5f);
    this -> addChild(title);

    //晨光的label
    auto label = Label::createWithBMFont("my_font.fnt", "Chen Guang \n series");
    label -> setScale(0.4f);
    label -> setPosition(Vec2(title->getPosition().x,
                              visibleSize.height - title -> getContentSize().height + 30));
    this -> addChild(label);
    label -> enableShadow();
    auto fadein = FadeIn::create(4);
    auto rotate = RotateTo::create(2, -25);
    auto spawn = Spawn::create(fadein, rotate, NULL);
    label -> runAction(spawn);
    
    
    //两个点击item，开始游戏和退出
    auto startNor = Sprite::createWithSpriteFrameName("restart_nor.png");
    auto startSel = Sprite::createWithSpriteFrameName("restart_sel.png");
    auto quitNor = Sprite::createWithSpriteFrameName("quit_nor.png");
    auto quitSel = Sprite::createWithSpriteFrameName("quit_sel.png");
    auto startItem = MenuItemSprite::create(startNor, startSel, CC_CALLBACK_1(StartScene::startMenuCallBack, this));
    auto quitItem = MenuItemSprite::create(quitNor, quitSel, CC_CALLBACK_1(StartScene::menuCloseCallback, this));
//    startItem -> setPosition(Vec2(visibleSize.width / 4 + origin.x, startItem -> getContentSize().height + quitItem -> getContentSize().height));
//    quitItem -> setPosition(Vec2(visibleSize.width / 4 + origin.x, quitItem -> getContentSize().height));
    auto menu = Menu::create(startItem, quitItem,  NULL);
    menu -> setPosition(Vec2(visibleSize.width / 2 + origin.x, 100));
    menu -> alignItemsVertically();
    this -> addChild(menu);
    
    

    
    return true;
}








void StartScene::startMenuCallBack(Ref* pSender)
{
    CocosDenshion::SimpleAudioEngine::getInstance() -> stopBackgroundMusic("game_music.mp3");
    Scene* gamescene = GameScene::createScene();
    auto trans = TransitionCrossFade::create(1, gamescene);
    Director::getInstance() -> pushScene(trans);
}

void StartScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
