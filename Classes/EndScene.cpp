//
//  EndScene.cpp
//  chenguang
//
//  Created by sun on 16/6/25.
//
//

#include <stdio.h>
#include "EndScene.h"
#include "StartScene.h"
#include <SimpleAudioEngine.h>


Scene* EndScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = EndScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool EndScene::init()
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
    auto gameover = Sprite::createWithSpriteFrameName("gameover.png");
    gameover -> setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    gameover -> setScale(visibleSize.width / gameover -> getContentSize().width, visibleSize.height / gameover -> getContentSize().height);
    this -> addChild(gameover);
    
    //设置历史最高得分
    std::string str_his = UserDefault::getInstance() -> getStringForKey("scorehis");
    auto label_his = Label::createWithBMFont("my_font.fnt", str_his);
    label_his -> setPosition(
                        Vec2(visibleSize.width/2 + origin.x, visibleSize.height / 1.5));
    this -> addChild(label_his);
    
    //设置本次得分
    auto label_now = Label::createWithBMFont("my_font.fnt", getScoreNow());
    label_now -> setPosition(
                            Vec2(visibleSize.width/2 + origin.x, visibleSize.height / 3.4));
    this -> addChild(label_now);
    
    
    auto reStart = Sprite::createWithSpriteFrameName("btn_finish.png");
    auto reStartItem = MenuItemSprite::create(reStart, NULL, CC_CALLBACK_1(EndScene::reStartCallBack, this));
//    reStartItem -> setPosition(
//                            Vec2(visibleSize.width/2 + origin.x, gameover -> getContentSize().height / 6));
//    this -> addChild(reStartItem);
    auto menu = Menu::create(reStartItem, NULL);
    menu -> setPosition(Vec2(visibleSize.width/2 + origin.x,visibleSize.height / 7));
    this -> addChild(menu);
    
    return true;
}


void EndScene::reStartCallBack(Ref* pSender)
{
    Scene* startscene = StartScene::createScene();
    auto trans = TransitionJumpZoom::create(1, startscene);
    Director::getInstance() -> pushScene(trans);
}


void EndScene::setScoreNow(int score)
{
    score = this -> score_now;
}

std::string EndScene::getScoreNow()
{
    std::string score = UserDefault::getInstance()->getStringForKey("ScoreNow").c_str();
//    std::stringstream ss;
//    ss << score_now;
//    return ss.str();
    return score;
}





void EndScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
