//
//  GameScene.cpp
//  chenguang
//
//  Created by sun on 16/6/26.
//
//

#include <stdio.h>
#include "GameScene.h"
#include <SimpleAudioEngine.h>
#include "EndScene.h"

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    fileNames["enemy0.png"] = "enemy0_down1.png";
    fileNames["enemy0_down1.png"] = "enemy0_down2.png";
    fileNames["enemy0_down2.png"] = "enemy0.png";
//    fileNames["enemy0_down3.png"] = "enemy0_down4.png";
    fileNames["enemy1.png"] = "enemy1_down1.png";
    fileNames["enemy1_down1.png"] = "enemy1_down2.png";
    fileNames["enemy1_down2.png"] = "enemy1_down3.png";
    fileNames["enemy1_down3.png"] = "enemy1.png";
    fileNames["enemy2.png"] = "enemy2_down1.png";
    fileNames["enemy2_down1.png"] = "enemy2_down2.png";
    fileNames["enemy2_down2.png"] = "enemy2_down3.png";
    fileNames["enemy2_down3.png"] = "enemy2_down4.png";
    fileNames["enemy2_down4.png"] = "enemy2_down5.png";
    fileNames["enemy2_down5.png"] = "enemy2.png";
    
    //资源初始化
    SpriteFrameCache::getInstance() -> addSpriteFramesWithFile("images.plist");
    auto texture = Director::getInstance() -> getTextureCache() -> addImage("images.pvr.ccz");
    batchnode = SpriteBatchNode::createWithTexture(texture);
    
    //设置背景
    auto bg = Sprite::createWithSpriteFrameName("bg.png");
    bg -> setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this -> addChild(bg);
    
    //背景音乐
    CocosDenshion::SimpleAudioEngine::getInstance() -> preloadBackgroundMusic("bg.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance() -> playBackgroundMusic("bg.mp3", true);
 
    //绘制player并添加特技
    player = Sprite::create("player.png");
    player -> setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/3 + origin.y));
    player -> setScale(0.3f);
    this -> addChild(player, 2, 1001);
    auto fire = ParticleSystemQuad::create("fire.plist");
    fire->setScale(4);
    fire->setPosition(player->getContentSize().width/2, 0);
    
    player->addChild(fire, 0);
//    auto streak = MotionStreak::create(0.3f, 50, 30, Color3B::WHITE, "player.png");
//    streak->setPosition(player->getPosition());
//    this->addChild(streak,2,1002);
    
    //绘制飞机
    auto tmpPlane = Sprite::createWithSpriteFrameName("enemy1.png");
    tmpPlane -> setScale(0.6f);
    auto tmpPlane2 = Sprite::createWithSpriteFrameName("enemy1.png");
    tmpPlane2 -> setScale(0.3f);
    int count = visibleSize.width / tmpPlane2 -> getContentSize().width;
    for (int i=0; i<= count*2; i++) {
        auto enemy0 = Sprite::createWithSpriteFrameName("enemy0.png");
        enemy0 -> setScale(0.6f);
        auto location = Point(enemy0 -> getContentSize().width/4   + i * enemy0 -> getContentSize().width, visibleSize.height + enemy0 -> getContentSize().height);
        location = Director::getInstance() -> convertToUI(location);
        enemy0 -> setPosition(location);
        batchnode -> addChild(enemy0);
    }
    for (int i = 0; i <= count; i++)
    {
        auto enemy1 = Sprite::createWithSpriteFrameName("enemy1.png");
        enemy1 -> setScale(0.6f);
        auto location = Point(tmpPlane -> getContentSize().width/4   + i * tmpPlane -> getContentSize().width, visibleSize.height + tmpPlane -> getContentSize().height);
        location = Director::getInstance() -> convertToUI(location);
        enemy1 -> setPosition(location);
        batchnode -> addChild(enemy1);
    }
    for (int i = 0; i <= count/2; i++)
    {
        auto enemy2 = Sprite::createWithSpriteFrameName("enemy2.png");
        enemy2 -> setScale(0.3f);
        auto location = Point(tmpPlane2 -> getContentSize().width + i * tmpPlane2 -> getContentSize().width, visibleSize.height + tmpPlane2 -> getContentSize().height);
        location = Director::getInstance() -> convertToUI(location);
        enemy2 -> setPosition(location);
        batchnode -> addChild(enemy2);
    }
    this -> addChild(batchnode);

    //让飞机动起来
    this -> schedule(CC_SCHEDULE_SELECTOR(GameScene::planeUpdate),0.2);
    this -> scheduleUpdate();
    
    //绘制分数面板
    auto score = Label::createWithTTF("SCORE:0","fonts/Marker Felt.ttf", 24);
    score->setPosition(Vec2(score -> getContentSize().width - 20,
                            origin.y + visibleSize.height - score->getContentSize().height));
    this->addChild(score,1,1003);
    
    //加分
    this->schedule(CC_SCHEDULE_SELECTOR(GameScene::addScore), 1);
    
    //单键监听
    auto listener = EventListenerTouchOneByOne::create();
    listener -> onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    listener -> onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    
    //穿透监听
    listener -> setSwallowTouches(true);
    auto dispatcher = Director::getInstance() -> getEventDispatcher();
    dispatcher -> addEventListenerWithSceneGraphPriority(listener, this);
    
    
    
    return true;
}

void GameScene::planeUpdate(float delta)
{
    auto randomPlane = batchnode -> getChildren().getRandomObject();
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    if (randomPlane && randomPlane -> getNumberOfRunningActions() == 0)
    {
        auto move = MoveTo::create
        (5, Point(randomPlane -> getPosition().x, -randomPlane -> getContentSize().height));
        auto place = Place::create(Point(randomPlane -> getPosition().x, visibleSize.height + randomPlane -> getContentSize().height));
        auto sequence = Sequence::create(move, place, NULL);
        randomPlane -> runAction(sequence);
    }
    
}

void GameScene::update(float delta)
{
    auto vec = batchnode -> getDescendants();
    for (std::vector<Sprite*>::iterator iter = vec.begin(); iter != vec.end(); iter++)
    {
        auto rect1 = player -> getBoundingBox();
        auto rect2 = (*iter) -> getBoundingBox();
        if (rect1.intersectsRect(rect2))
        {
            for (std::vector<Sprite*>::iterator iter = vec.begin(); iter != vec.end(); iter++)
            {
                (*iter) -> stopAllActions();
            }
            //终止分数
            this->unschedule(schedule_selector(GameScene::addScore));
            this -> unschedule(schedule_selector(GameScene::planeUpdate));
            
            //烟火特效
            this->schedule(CC_SCHEDULE_SELECTOR(GameScene::explotion), 1, 3, 1);
            
            //打出结束字幕
            Size visibleSize = Director::getInstance()->getVisibleSize();
            Vec2 origin = Director::getInstance()->getVisibleOrigin();
            auto center = Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y);
            auto jump = JumpTo::create(1, center, 4, 3);
            auto label = Label::createWithTTF("GAME OVER","fonts/Marker Felt.ttf", 50);
            this->addChild(label,2);
            label->setPosition(Vec2(origin.x,origin.y));
            label->runAction(jump);
            
            //播放结束音乐
            CocosDenshion::SimpleAudioEngine::getInstance() -> stopBackgroundMusic("bg.mp3");
            CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("end.mp3");
            
            //判断分数是不是最高，是最高则写入
//            path = "/users/sun/Downloads/data.txt";
            std::string scorenow_string;
            std::string scorehis = UserDefault::getInstance() -> getStringForKey("scorehis");
            int scorehis_int = atoi(scorehis.c_str());
            
            std::stringstream ss;
            ss << scorenow;
            ss >> scorenow_string;
            
            if (scorenow > scorehis_int)
            {
                UserDefault::getInstance() -> setStringForKey("scorehis", scorenow_string);
            }
            
            UserDefault::getInstance()->setStringForKey("ScoreNow", scorenow_string);
            //setScoreNow(scorenow);
            
            //前往结束页面
            auto contiune = Label::createWithBMFont("my_font.fnt", "contiune");
            contiune -> setScale(0.6);
            auto item = MenuItemLabel::create(contiune, CC_CALLBACK_1(GameScene::contiuneCallBack, this));
            auto menu = Menu::create(item, NULL);
            menu -> setPosition(Vec2(visibleSize.width/2 + origin.x + 60, visibleSize.height - menu -> getContentSize().height + 75));
            this -> addChild(menu);
            this->unscheduleUpdate();
            break;
        }
    }
    removeBullet();
    removeEnemy();
}

void GameScene::contiuneCallBack(Ref* pSender)
{
    auto endScene = EndScene::createScene();
    
    auto trans = TransitionPageTurn::create(1, endScene, true);
    Director::getInstance() -> pushScene(trans);
}

bool GameScene::onTouchBegan(cocos2d::Touch *t, cocos2d::Event *e)
{
    auto location = t -> getLocation();
    auto playRect = player->getBoundingBox();
    if(player->getBoundingBox().containsPoint(location)) {
        auto bullet = Sprite::create("bullet.jpg");
        bullet->setPosition(location);
        bullet->setScale(0.2f);
        bullet->setAnchorPoint(Vec2(0.5, 0));
        auto move = MoveTo::create(2, Vec2(location.x, visibleSize.height));
        bullet->runAction(move);
        this->addChild(bullet);
        bullets.push_back(bullet);
    }
    
    
//    auto location = t -> getLocationInView();
//    auto fire = this->getChildByTag(1004);
//    location = Director::getInstance() -> convertToUI(location);
//    if(location.x < 0 || location.y < 0 || location.x >  visibleSize.width || location.y > visibleSize.height) {
//        return true;
//    }else{
//        player -> setPosition(location);
//        fire -> setPosition(location);
//    }
    return true;
}

void GameScene::onTouchMoved(cocos2d::Touch *t, cocos2d::Event *e)
{
    auto location = t -> getLocationInView();
//    auto streak = this->getChildByTag(1002);
//    auto fire = this->getChildByTag(1004);
    location = Director::getInstance() -> convertToUI(location);
    if(location.x < 0 || location.y < 0 || location.x >  visibleSize.width || location.y > visibleSize.height) {
        return;
    }else{
        player -> setPosition(location);
//        streak -> setPosition(location);
//        fire -> setPosition(location);
    }
}

void GameScene::removeBullet()
{
    if(bullets.size()>0 && bullets[bullets.size()-1]->getPosition().y >= visibleSize.height)
    {
        bullets.pop_back();
    }
}

void GameScene::removeEnemy()
{
    auto vec = batchnode -> getDescendants();
    bool isDead = false;
    for (std::vector<Sprite*>::iterator iterEnemy = vec.begin(); iterEnemy!=vec.end(); iterEnemy++) {
        for (std::vector<Sprite*>::iterator iterBullet = bullets.begin(); iterBullet != bullets.end() ; iterBullet++) {
            if((*iterEnemy)->getBoundingBox().intersectsRect((*iterBullet)->getBoundingBox()))
            {
                (*iterBullet)->stopAllActions();
                (*iterBullet)->setPosition(0, visibleSize.height);
                auto eFrame = (*iterEnemy)->getSpriteFrame();
//                (*iterEnemy)->setSpriteFrame("enemy1_down1.png");
//                auto n = (*iterEnemy)->getSpriteFrame()->getTexture()->getStringForFormat();
                isDead = false;
                for (std::map<std::string, std::string>::iterator iterFile = fileNames.begin(); iterFile!=fileNames.end(); iterFile++) {
                    if(eFrame == SpriteFrameCache::getInstance()->getSpriteFrameByName((*iterFile).first))
                    {
                        std::string nextFile =(*iterFile).second;
                        (*iterEnemy)->setSpriteFrame(nextFile);
                        
                        if(nextFile.compare("enemy0.png")==0 || nextFile.compare("enemy1.png")==0  || nextFile.compare("enemy2.png")==0 )
                        {
                            isDead = true;
                        }
                        break;
                    }
                }
                if(isDead)
                {
                    (*iterEnemy)->stopAllActions();
                    (*iterEnemy)->setPosition((*iterEnemy)->getPosition().x, visibleSize.height + (*iterEnemy)->getContentSize().height);
                    continue;
                }
            }
        }
        if(isDead) {
            continue;
        }
    }
}

void GameScene::addScore(float delta)
{
    Label* score=(Label*)this->getChildByTag(1003);
    scorenow++;
    std::stringstream ss;
    ss << "SCORE:";
    ss << scorenow;
    score->setString(ss.str());
}

void GameScene::explotion(float delta)
{
    auto particle = ParticleExplosion::create();
    particle->setAutoRemoveOnFinish(true);
    auto place = randomPlace(1.0);
    particle->stopAllActions();
    particle->runAction(place);
    this->addChild(particle, 1, 104);
    CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("use_bomb.mp3");
}

Place* GameScene::randomPlace(float delta)
{
    Size visibleSize = Director::getInstance()->getWinSize();
    Point p = ccp(CCRANDOM_0_1() * visibleSize.width, CCRANDOM_0_1()*visibleSize.height);
    auto ap = Place::create(p);
    return ap;
}


