#ifndef __LOADINGSCENE_SCENE_H__
#define __LOADINGSCENE_SCENE_H__

#include "cocos2d.h"

class LoadingScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    void update(float delta);
    
    // implement the "static create()" method manually
    CREATE_FUNC(LoadingScene);
private:
    int count;
    cocos2d::Vec2 center;
};

#endif // __HELLOWORLD_SCENE_H__
