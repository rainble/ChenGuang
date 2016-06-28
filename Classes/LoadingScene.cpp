#include "LoadingScene.h"
#include "StartScene.h"
#include <strstream>
USING_NS_CC;

Scene* LoadingScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LoadingScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LoadingScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    center = Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y);
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("images.plist");
    auto texture = Director::getInstance()->getTextureCache()->addImage("images.pvr.ccz");
    auto batchNode = SpriteBatchNode::createWithTexture(texture);
    this->addChild(batchNode);
    
    auto bg = Sprite::createWithSpriteFrameName("background.png");
    bg->setPosition(center);
    this->addChild(bg);
    
    auto loadingSp = Sprite::createWithSpriteFrameName("loading.png");
    loadingSp->setPosition(center);
    loadingSp->setScale(0.5f);
    auto rotateAction = RotateBy::create(0.3, 90);
    auto repeat = RepeatForever::create(rotateAction);
    loadingSp->runAction(repeat);
    this->addChild(loadingSp);
    
    this->schedule(CC_SCHEDULE_SELECTOR(LoadingScene::update), 1);
    
    this->setScale(0.8f);
    
    return true;
}


void LoadingScene::update(float delta)
{
    count++;
    std::stringstream imgName;
    imgName << "game_loading" << count << ".png";
    if(count<=3) {
        auto loadingImg = Sprite::createWithSpriteFrameName(imgName.str());
        loadingImg->setPosition(center);
        this->removeChildByTag(101);
        this->addChild(loadingImg, 1, 101);
    }else{
        auto startScene = StartScene::createScene();
        auto trans = TransitionFlipAngular::create(1, startScene);
        Director::getInstance()->pushScene(trans);
    }
}
