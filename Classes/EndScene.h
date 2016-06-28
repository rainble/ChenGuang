//
//  EndScene.h
//  chenguang
//
//  Created by sun on 16/6/25.
//
//

#ifndef EndScene_h
#define EndScene_h
#include "cocos2d.h"
#include "GameScene.h"

USING_NS_CC;

class EndScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(EndScene);
    
    void setScoreNow(int score);
    std::string getScoreNow();
    void reStartCallBack(Ref* pSender);
    
private:
    int score_now;
    
    
};

#endif /* EndScene_h */
