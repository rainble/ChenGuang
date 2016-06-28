//
//  GameScene.h
//  chenguang
//
//  Created by sun on 16/6/26.
//
//

#ifndef GameScene_h
#define GameScene_h
#include "cocos2d.h"
USING_NS_CC;

class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
    
    void planeUpdate(float delta);
    void addScore(float delta);
    void update(float delta);
    bool onTouchBegan(Touch* t, Event* e);
    void onTouchMoved(Touch* t, Event* e);
    void explotion(float delta);
    Place* randomPlace(float delta);
    void removeBullet();
    void removeEnemy();
    void contiuneCallBack(Ref* pSender);
    int scorenow;
    Sprite* player;
    SpriteBatchNode* batchnode;
    std::string path;
    Size visibleSize;
private:
    std::vector<Sprite*> bullets;
    std::map<std::string, std::string> fileNames;
};



#endif /* GameScene_h */
