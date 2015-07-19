#include "HelloWorldScene.h"
#include "CardSprite.h"
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init firstd
    if ( !Layer::init() )
    {
        return false;
    }
    score = 0;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    //Vec2 origin = Director::getInstance()->getVisibleOrigin();
    //加入游戏背景
    auto layerColorBG = cocos2d::LayerColor::create(cocos2d::Color4B(180, 170, 160, 255));
    this->addChild(layerColorBG);
    
    //在上方加入游戏的分数
    auto labTTFCardNumberName = LabelTTF::create("分数", "HirakakuProN-W6", 80);
    labTTFCardNumberName->setPosition(Point(visibleSize.width / 5, visibleSize.height - 150));
    addChild(labTTFCardNumberName);
    
    
    labTTFCardNumber = LabelTTF::create("0", "HirakakuProN-W6", 80);
    labTTFCardNumber->setPosition(Point(visibleSize.width / 2 + 100, visibleSize.height - 150));
    addChild(labTTFCardNumber);
    
    //加入手势识别监听事件进行绑定
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    createdCardSprite(visibleSize);
    autoCreateCardNumber();
    autoCreateCardNumber();
    return true;
}
void HelloWorld::autoCreateCardNumber(){
    int i = CCRANDOM_0_1() * 4;
    int j = CCRANDOM_0_1() * 4;
    //判断是否已经存在的位置
    if(cardArr[i][j]->getNumber() > 0){
        autoCreateCardNumber();
    }else{
        cardArr[i][j]->setNumber(CCRANDOM_0_1()*10<1?4:2);
    }
}
void HelloWorld::doCheckGameOver(){
    bool isGameOver = true;
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if(cardArr[x][y]->getNumber() == 0||
               (x > 0 && (cardArr[x][y]->getNumber() == cardArr[x - 1][y]->getNumber()))||
               (x < 3 && (cardArr[x][y]->getNumber() == cardArr[x + 1][y]->getNumber()))||
               (y > 0 && (cardArr[x][y]->getNumber() == cardArr[x][y - 1]->getNumber()))||
               (y < 3 && (cardArr[x][y]->getNumber() == cardArr[x][y + 1]->getNumber()))){
                isGameOver = false;
            }
        }
    }
    if(isGameOver){
        Director::getInstance()->replaceScene(TransitionFade::create(1, HelloWorld::createScene()));
    }
}
void HelloWorld::createdCardSprite(cocos2d::Size size){
    //求出单元格的宽和高
    int lon = (size.width - 28) / 4;
    //4*4的单元格
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
            CardSprite *card = CardSprite::createCardSprite(0, lon, lon, lon*j+20 , lon*i+20+size.height/6);
            addChild(card);
            cardArr[j][i] = card;
        }
    }
}
//加入手势识别
bool HelloWorld::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event){
    Point touchPO = touch->getLocation();
    firstX = touchPO.x;
    firstY = touchPO.y;
    return true;
}
void HelloWorld::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event){
    //获取X轴跟Y轴的移动范围
    Point touchPO = touch->getLocation();
    endX = firstX - touchPO.x;
    endY = firstY - touchPO.y;
    
    //判断 X轴跟Y轴的移动距离： X大就是左右 否则是上下
    if(abs(endX) > abs(endY)){
        //左右
        if(endX + 5 > 0){
            //左边
            if(doLeft()){
                autoCreateCardNumber();
                doCheckGameOver();
            }
        }else{
            if(doRight()){
                autoCreateCardNumber();
                doCheckGameOver();
            }
        }
    }else{
        if(endY + 5 > 0){
            if(doDown()){
                autoCreateCardNumber();
                doCheckGameOver();
            }
        }else{
            if(doUp()){
                autoCreateCardNumber();
                doCheckGameOver();
            }
        }
    }
}
//上下左右的方法
bool HelloWorld::doLeft(){
    bool isDo = false;
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            for(int x1 = x + 1; x1 < 4; x1++){
                if(cardArr[x1][y]->getNumber() > 0){
                    if(cardArr[x][y]->getNumber() <= 0){
                        cardArr[x][y]->setNumber(cardArr[x1][y]->getNumber());
                        cardArr[x1][y]->setNumber(0);
                        x--;
                        isDo = true;
                    }else if(cardArr[x1][y]->getNumber() == cardArr[x][y]->getNumber()){
                        cardArr[x][y]->setNumber(cardArr[x][y]->getNumber()*2);
                        cardArr[x1][y]->setNumber(0);
                        
                        //设置分数
                        score += cardArr[x][y]->getNumber();
                        labTTFCardNumber->setString(__String::createWithFormat("%i",score)->getCString());
                        isDo = true;
                    }
                    break;
                }
            }
        }
    }
    return isDo;
}
bool HelloWorld::doRight(){
    bool isDo = false;
    for (int y = 0; y < 4; y++) {
        for (int x = 3; x >= 0; x--) {
            for(int x1 = x - 1; x1 >= 0; x1--){
                if(cardArr[x1][y]->getNumber() > 0){
                    if(cardArr[x][y]->getNumber() <= 0){
                        cardArr[x][y]->setNumber(cardArr[x1][y]->getNumber());
                        cardArr[x1][y]->setNumber(0);
                        x++;
                        isDo = true;
                    }else if(cardArr[x1][y]->getNumber() == cardArr[x][y]->getNumber()){
                        cardArr[x][y]->setNumber(cardArr[x][y]->getNumber()*2);
                        cardArr[x1][y]->setNumber(0);
                        score += cardArr[x][y]->getNumber();
                        labTTFCardNumber->setString(__String::createWithFormat("%i",score)->getCString());
                        isDo = true;
                    }
                    break;
                }
            }
        }
    }
    return true;
}
bool HelloWorld::doUp(){
    bool isDo = false;
    for (int x = 0; x < 4; x++) {
        for (int y = 3; y >= 0; y--) {
            for(int y1 = y - 1; y1 >= 0; y1--){
                if(cardArr[x][y1]->getNumber() > 0){
                    if(cardArr[x][y]->getNumber() <= 0){
                        cardArr[x][y]->setNumber(cardArr[x][y1]->getNumber());
                        cardArr[x][y1]->setNumber(0);
                        y++;
                        isDo = true;
                    }else if(cardArr[x][y]->getNumber() == cardArr[x][y1]->getNumber()){
                        cardArr[x][y]->setNumber(cardArr[x][y]->getNumber()*2);
                        cardArr[x][y1]->setNumber(0);
                        score += cardArr[x][y]->getNumber();
                        labTTFCardNumber->setString(__String::createWithFormat("%i",score)->getCString());
                        isDo = true;
                    }
                    break;
                }
            }
        }
    }
    return isDo;
}
bool HelloWorld::doDown(){
    bool isDo = false;
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            for(int y1 = y + 1; y1 < 4; y1++){
                if(cardArr[x][y1]->getNumber() > 0){
                    if(cardArr[x][y]->getNumber() <= 0){
                        cardArr[x][y]->setNumber(cardArr[x][y1]->getNumber());
                        cardArr[x][y1]->setNumber(0);
                        y--;
                        isDo = true;
                    }else if(cardArr[x][y]->getNumber() == cardArr[x][y1]->getNumber()){
                        cardArr[x][y]->setNumber(cardArr[x][y]->getNumber()*2);
                        cardArr[x][y1]->setNumber(0);
                        score += cardArr[x][y]->getNumber();
                        labTTFCardNumber->setString(__String::createWithFormat("%i",score)->getCString());
                        isDo = true;
                    }
                    break;
                }
            }
        }
    }
    return isDo;
}
void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
