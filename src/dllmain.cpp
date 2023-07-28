#include "includes.h"
#include <chrono>
#include <thread>
#include <random>
#include <fstream>

bool hasSai = false;

template<typename Base, typename T>
bool instanceof(const T ptr) {
    return dynamic_cast<const Base>(ptr) != nullptr;
}

template<typename O, typename T>
T getFromObjectIndex(CCObject* obj, int index) {
    CCObject* pObj = nullptr;

    int idx = 0;

    CCARRAY_FOREACH(dynamic_cast<O>(obj)->getChildren(), pObj) {
        CCNode* currentNode = (CCNode*)pObj;

        T obj = dynamic_cast<T>(pObj);

        if (instanceof<T>(obj)) {
            if (idx == index) return obj;
            idx++;
        }
    }
    return nullptr;
}

template<typename O, typename T>
T getFromObjectContentSizeWidthBetween(CCObject* obj, float widthS, float widthE, int index) {
    CCObject* pObj = nullptr;

    int idx = 0;

    CCARRAY_FOREACH(dynamic_cast<O>(obj)->getChildren(), pObj) {
        CCNode* currentNode = (CCNode*)pObj;
        float objWidth = currentNode->getContentSize().width;

        T obj = dynamic_cast<T>(pObj);

        if (instanceof<T>(obj)) {
            if (objWidth > widthS && objWidth < widthE) {
                if (idx == index) return obj;
                idx++;
            }
        }
    }
    return nullptr;
}

CCSprite* getOriginalSelector(CCLayer* obj) {
    CCObject* pObj = nullptr;

    CCARRAY_FOREACH(obj->getChildren(), pObj) {
        CCNode* currentNode = (CCNode*)pObj;

        if (instanceof<CCSprite*>(pObj)) {
            
            if (pObj->getTag() == 69543 || pObj->getTag() == 69542) continue;
            return reinterpret_cast<CCSprite*>(pObj);
        }
    }
    return nullptr;
}

bool is2ndPlayer = true;
int icon = 1;
int ship = 1;
int ball = 1;
int UFO = 1;
int wave = 1;
int robot = 1;
int spider = 1;
int trail = 1;
int deathEffect = 1;
int primaryColor = 0;
int secondaryColor = 3;
bool hasGlow = false;
bool usesDefault = false;

gd::IconType lastPlayer = gd::IconType::kIconTypeCube;

void saveIcons();
void setGlow(gd::GJGarageLayer* garage);
void setFrame(gd::PlayerObject* playerObject, gd::IconType iconType);
void setColor(gd::PlayerObject* playerObject);
void setIconColor(gd::GJGarageLayer* self, int ID, bool primary, bool isP2);
bool isSecondPlayerInPlay(gd::PlayerObject* playerObject);

class ButtonStuff {
public:
    void ButtonStuff::setGlowA(CCObject* target) {

        CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
        gd::GJGarageLayer* garage = reinterpret_cast<gd::GJGarageLayer*>(scene->getChildren()->objectAtIndex(0));
        CCMenu* menu = reinterpret_cast<CCMenu*>(garage->getChildByTag(23423));
        gd::CCMenuItemToggler* toggler = reinterpret_cast<gd::CCMenuItemToggler*>(menu->getChildByTag(67532));

        hasGlow = !toggler->isOn();
        setGlow(garage);
    }

    void ButtonStuff::setP2(CCObject* target) {

        CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
        gd::GJGarageLayer* garage = reinterpret_cast<gd::GJGarageLayer*>(scene->getChildren()->objectAtIndex(0));
        CCMenu* playerMenu = reinterpret_cast<CCMenu*>(garage->getChildByTag(74356));
        CCSprite* rightArrow = reinterpret_cast<CCSprite*>(playerMenu->getChildByTag(34730));
        CCSprite* leftArrow = reinterpret_cast<CCSprite*>(playerMenu->getChildByTag(34729));

        rightArrow->setVisible(true);
        leftArrow->setVisible(false);

        is2ndPlayer = true;
    }

    void ButtonStuff::setP1(CCObject* target) {

        CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
        gd::GJGarageLayer* garage = reinterpret_cast<gd::GJGarageLayer*>(scene->getChildren()->objectAtIndex(0));
        CCMenu* playerMenu = reinterpret_cast<CCMenu*>(garage->getChildByTag(74356));
        CCSprite* rightArrow = reinterpret_cast<CCSprite*>(playerMenu->getChildByTag(34730));
        CCSprite* leftArrow = reinterpret_cast<CCSprite*>(playerMenu->getChildByTag(34729));

        rightArrow->setVisible(false);
        leftArrow->setVisible(true);

        is2ndPlayer = false;
    }

    void ButtonStuff::setDefaultP2(CCObject* target) {

        CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
        gd::GJGarageLayer* garage = reinterpret_cast<gd::GJGarageLayer*>(scene->getChildren()->objectAtIndex(0));
        CCMenu* menu = reinterpret_cast<CCMenu*>(garage->getChildByTag(74356));
        gd::CCMenuItemToggler* toggler = reinterpret_cast<gd::CCMenuItemToggler*>(menu->getChildByTag(14736));

        usesDefault = !toggler->isOn();
    }
    void ButtonStuff::showSaiAlert(CCObject* target) {
        auto alertLayer = gd::FLAlertLayer::create(NULL, "SaiModPack Detected", "Okay", NULL, 300, "Remove SaiModPack to use SeparateDualIcons as they are incompatible with each other.");
        alertLayer->show();
    }
};

void setSelectorPosColors(gd::GJGarageLayer* page, bool isP2) {

    int ID = 0;
    bool hasSecondSelect = true;
    int tag = 0;

    gd::GameManager* manager = gd::GameManager::sharedState();

    CCSprite* selector1p1 = reinterpret_cast<CCSprite*>(page->getChildByTag(16547));
    CCSprite* selector2p1 = reinterpret_cast<CCSprite*>(page->getChildByTag(16548));

    if (!selector1p1 || !selector2p1) return;

    selector1p1->setZOrder(101);
    selector2p1->setZOrder(101);

    CCSprite* selector1p2 = reinterpret_cast<CCSprite*>(page->getChildByTag(16549));
    CCSprite* selector2p2 = reinterpret_cast<CCSprite*>(page->getChildByTag(16550));

    selector1p2->setZOrder(101);
    selector2p2->setZOrder(101);

    if (isP2) {
        selector1p2->setZOrder(102);
        selector2p2->setZOrder(102);
    }
    else {
        selector1p1->setZOrder(102);
        selector2p1->setZOrder(102);
    }


    if (isP2) {
        for (int c = 0; c < 2; c++) {

            if (c == 0) ID = primaryColor;
            if (c == 1) ID = secondaryColor;

            gd::ListButtonBar* listButtonBar = reinterpret_cast<gd::ListButtonBar*>(page->getChildByTag(1));
            gd::BoomScrollLayer* boomScrollLayer = reinterpret_cast<gd::BoomScrollLayer*>(listButtonBar->getChildren()->objectAtIndex(0));
            CCLayer* extendedLayer = getFromObjectIndex<CCLayer*, CCLayer*>(boomScrollLayer, 0);

            int posInPage = 0;
            int pageNum = 0;
            for (int i = 0; i < extendedLayer->getChildrenCount(); i++) {

                CCLayer* pageLayer = reinterpret_cast<CCLayer*>(extendedLayer->getChildren()->objectAtIndex(i));
                CCMenu* menu = reinterpret_cast<CCMenu*>(pageLayer->getChildren()->objectAtIndex(0));

                for (int j = 0; j < menu->getChildrenCount() / 2; j++) {
                    if (menu->getChildren()->objectAtIndex(j)->getTag() == ID) {
                        posInPage = j;
                        pageNum = i;
                        goto to_break;
                    }
                }
            }
            to_break:

            int xExtened = extendedLayer->getPositionX();
            CCLayer* pageLayer = reinterpret_cast<CCLayer*>(extendedLayer->getChildren()->objectAtIndex(pageNum));

            if (pageLayer->getPositionX() * -1 == xExtened) {

                auto winSize = CCDirector::sharedDirector()->getWinSize();

                float centerX = winSize.width / 2;

                if (c == 0) {
                    selector1p2->setVisible(true);
                    selector1p2->setPosition({ centerX - 159.8f + 29.035f * (posInPage), 55.4f });
                }
                else {
                    selector2p2->setVisible(true);
                    selector2p2->setPosition({ centerX - 159.8f + 29.035f * (posInPage), 26.4f });
                }
            }
            else {
                if (c == 0) {
                    selector1p2->setVisible(false);
                }
                else {
                    selector2p2->setVisible(false);
                }
            }
        }
    }
}


void setSelectorPosIcons(gd::GaragePage* page, bool isP2) {
    
    int ID = 0;
    bool hasSecondSelect = true;
    int tag = 0;

    CCSprite* originalSelector = getOriginalSelector(page);
    originalSelector->setOpacity(0);

    gd::GameManager* manager = gd::GameManager::sharedState();

    CCSprite* selector = nullptr;

    CCSprite* selector1 = reinterpret_cast<CCSprite*>(page->getChildByTag(69542));
    CCSprite* selector2 = reinterpret_cast<CCSprite*>(page->getChildByTag(69543));

    selector1->setZOrder(101);
    selector2->setZOrder(101);

    if (isP2) {
        selector = selector2;
    }
    else {
        selector = selector1;
    }

    selector->setZOrder(102);

    switch (page->m_nType)
    {
    case gd::kIconTypeCube:
        page->setTag(10);
        if (isP2) ID = icon;
        else ID = manager->getPlayerFrame();
        tag = 0;
        break;
    case gd::kIconTypeShip:
        page->setTag(11);
        if (isP2) ID = ship;
        else ID = manager->getPlayerShip();
        tag = 1;
        break;
    case gd::kIconTypeBall:
        page->setTag(12);
        if (isP2) ID = ball;
        else ID = manager->getPlayerBall();
        tag = 2;
        break;
    case gd::kIconTypeUfo:
        page->setTag(13);
        if (isP2) ID = UFO;
        else ID = manager->getPlayerBird();
        tag = 3;
        break;
    case gd::kIconTypeWave:
        page->setTag(14);
        if (isP2) ID = wave;
        else ID = manager->getPlayerDart();
        tag = 4;
        break;
    case gd::kIconTypeRobot:
        page->setTag(15);
        if (isP2) ID = robot;
        else ID = manager->getPlayerRobot();
        tag = 5;
        break;
    case gd::kIconTypeSpider:
        page->setTag(16);
        if (isP2) ID = spider;
        else ID = manager->getPlayerSpider();
        tag = 6;
        break;
    case gd::kIconTypeDeathEffect:
        page->setTag(17);
        hasSecondSelect = false;
        if (!isP2) ID = manager->getPlayerDeathEffect();
        tag = 98;
        break;
    case gd::kIconTypeSpecial:
        page->setTag(18);
        if (isP2) ID = trail;
        else ID = manager->getPlayerStreak();
        tag = 99;
        break;
    default:
        hasSecondSelect = false;
        break;
    }

    CCLabelBMFont* label = reinterpret_cast<CCLabelBMFont*>(selector->getChildren()->objectAtIndex(0));

    if (!hasSecondSelect) {
        label->setVisible(false);
        selector->setColor({ 255,255,255 });
    }
    else {
        label->setVisible(true);
        if (isP2) {
            selector->setColor({ 0, 255, 255 });
        }
        else {
            selector->setColor({ 255, 175, 0 });
        }
    }

    if (hasSecondSelect || !isP2) {

        gd::ListButtonBar* listButtonBar = reinterpret_cast<gd::ListButtonBar*>(page->getChildByTag(tag));
        gd::BoomScrollLayer* boomScrollLayer = reinterpret_cast<gd::BoomScrollLayer*>(listButtonBar->getChildren()->objectAtIndex(0));
        CCLayer* extendedLayer = reinterpret_cast<CCLayer*>(boomScrollLayer->getChildren()->objectAtIndex(0));

        int pageNum = 0;
        for (int i = 0; i < extendedLayer->getChildrenCount(); i++) {
            if (ID <= ((pageNum + 1) * 36)) {
                break;
            }
            pageNum++;
        }

        int xExtened = extendedLayer->getPositionX();

        CCLayer* pageLayer = reinterpret_cast<CCLayer*>(extendedLayer->getChildren()->objectAtIndex(pageNum));

        if(pageLayer->getPositionX() * -1 == xExtened){
            selector->setVisible(true);

            int IDtoPageValue = ID - pageNum * 36;

            int xPos = ((IDtoPageValue - 1) % 12) + 1;
            int yPos = ((IDtoPageValue - 1) / 12) + 1;

            auto winSize = CCDirector::sharedDirector()->getWinSize();

            float centerX = winSize.width / 2;
            float centerY = winSize.height / 2;

            selector->setPosition({ centerX - 165 + 30 * (xPos - 1), centerY - 6 - 30 * (yPos - 1) });
        }
        else {
            selector->setVisible(false);
        }
        
    }
    else {
        selector->setVisible(false);
    }
}


bool(__thiscall* GaragePage_init)(gd::GaragePage* self, gd::IconType type, gd::GJGarageLayer* pGarage, cocos2d::SEL_MenuHandler pSelectCallback);

bool __fastcall GaragePage_init_H(gd::GaragePage* self, void*, gd::IconType type, gd::GJGarageLayer* pGarage, cocos2d::SEL_MenuHandler pSelectCallback) {

    CCSprite* playerTwoSelect = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
    playerTwoSelect->setTag(69543);
    playerTwoSelect->setColor({ 0, 255, 255 });
    playerTwoSelect->setScale(0.85f);
    playerTwoSelect->setZOrder(101);

    CCLabelBMFont* p1Label = CCLabelBMFont::create("P1", "bigFont.fnt");
    CCLabelBMFont* p2Label = CCLabelBMFont::create("P2", "bigFont.fnt");

    p1Label->setScale(0.25);
    p1Label->setPosition({ 10, 31 });
    p1Label->setColor({ 255, 175, 0 });
    p2Label->setScale(0.25);
    p2Label->setPosition({ 28, 8 });
    p2Label->setColor({ 0, 255, 255 });

    CCSprite* playerOneSelect = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
    playerOneSelect->setTag(69542);
    playerOneSelect->setColor({ 255, 175, 0 });
    playerOneSelect->setScale(0.85f);
    playerOneSelect->setZOrder(101);

    playerOneSelect->addChild(p1Label);
    playerTwoSelect->addChild(p2Label);

    self->addChild(playerTwoSelect);
    self->addChild(playerOneSelect);

    if (!GaragePage_init(self, type, pGarage, pSelectCallback)) return false;

    setSelectorPosIcons(self, true);
    setSelectorPosIcons(self, false);

    return true;
}

void(__thiscall* GJGarageLayer_showBlackCircleWave)(gd::GJGarageLayer*);

void __fastcall GJGarageLayer_showBlackCircleWave_H(gd::GJGarageLayer* self, void*) {
    //do nothing
}

void(__thiscall* GJGarageLayer_showCircleWave)(gd::GJGarageLayer*);

void __fastcall GJGarageLayer_showCircleWave_H(gd::GJGarageLayer* self, void*) {
    //do nothing
}

void(__thiscall* ListButtonBar_switchedPage)(gd::ListButtonBar*, int);

void __fastcall ListButtonBar_switchedPage_H(gd::ListButtonBar* self, void*, int param_1) {

    CCLayer* parent = reinterpret_cast<CCLayer*>(self->getParent());

    ListButtonBar_switchedPage(self, param_1);

    if (parent->getChildByTag(69543)) {

        gd::GaragePage* garagePage = reinterpret_cast<gd::GaragePage*>(parent);

        setSelectorPosIcons(garagePage, true);
        setSelectorPosIcons(garagePage, false);
    }
    else if(parent->getChildByTag(74356)) {

        gd::GJGarageLayer* garageLayer = reinterpret_cast<gd::GJGarageLayer*>(parent);

        setSelectorPosColors(garageLayer, true);
        setSelectorPosColors(garageLayer, false);
    }
}

bool(__thiscall* MenuLayer_init)(gd::MenuLayer* self);

bool __fastcall MenuLayer_init_H(gd::MenuLayer* self, void*) {

    if (!MenuLayer_init(self)) return false;

    if(hasSai){
        float delay = 1.0f;
        self->scheduleOnce(schedule_selector(ButtonStuff::showSaiAlert), delay);
    }

    return true;
}

bool(__thiscall* GJGarageLayer_init)(gd::GJGarageLayer* self);

bool __fastcall GJGarageLayer_init_H(gd::GJGarageLayer* self, void*) {

    if (!GJGarageLayer_init(self)) return false;

        is2ndPlayer = false;
        CCSprite* unlockTxt = getFromObjectContentSizeWidthBetween<CCLayer*, CCSprite*>(self, 90, 120, 0);
        unlockTxt->setVisible(false);

        CCSprite* selectSpriteP1primary = getFromObjectContentSizeWidthBetween<CCLayer*, CCSprite*>(self, 30, 40, 0);
        CCSprite* selectSpriteP1secondary = getFromObjectContentSizeWidthBetween<CCLayer*, CCSprite*>(self, 30, 40, 1);

        selectSpriteP1primary->setColor({ 255, 175, 0 });
        selectSpriteP1secondary->setColor({ 255, 175, 0 });

        selectSpriteP1primary->setTag(16547);
        selectSpriteP1secondary->setTag(16548);

        CCSprite* selectSpriteP2primary = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
        CCSprite* selectSpriteP2secondary = CCSprite::createWithSpriteFrameName("GJ_select_001.png");

        selectSpriteP2primary->setColor({ 0, 255, 255 });
        selectSpriteP2secondary->setColor({ 0, 255, 255 });

        selectSpriteP2primary->setScale(0.8f);
        selectSpriteP2secondary->setScale(0.8f);


        selectSpriteP2primary->setTag(16549);
        selectSpriteP2secondary->setTag(16550);

        self->addChild(selectSpriteP2primary);
        self->addChild(selectSpriteP2secondary);

        CCLabelBMFont* p1Label = CCLabelBMFont::create("P1", "bigFont.fnt");
        CCLabelBMFont* p1Label2 = CCLabelBMFont::create("P1", "bigFont.fnt");

        CCLabelBMFont* p2Label = CCLabelBMFont::create("P2", "bigFont.fnt");
        CCLabelBMFont* p2Label2 = CCLabelBMFont::create("P2", "bigFont.fnt");

        p1Label->setScale(0.25);
        p1Label->setPosition({ 10, 31 });
        p1Label->setColor({ 255, 175, 0 });

        p1Label2->setScale(0.25);
        p1Label2->setPosition({ 10, 31 });
        p1Label2->setColor({ 255, 175, 0 });

        p2Label->setScale(0.25);
        p2Label->setPosition({ 28, 8 });
        p2Label->setColor({ 0, 255, 255 });

        p2Label2->setScale(0.25);
        p2Label2->setPosition({ 28, 8 });
        p2Label2->setColor({ 0, 255, 255 });

        selectSpriteP1primary->addChild(p1Label);
        selectSpriteP1secondary->addChild(p1Label2);

        selectSpriteP2primary->addChild(p2Label);
        selectSpriteP2secondary->addChild(p2Label2);

        selectSpriteP1primary->setZOrder(101);
        selectSpriteP1secondary->setZOrder(101);

        selectSpriteP2primary->setZOrder(101);
        selectSpriteP2secondary->setZOrder(101);

        gd::CCMenuItemToggler* defaultToggler = gd::CCMenuItemToggler::createWithStandardSprites(self, menu_selector(ButtonStuff::setDefaultP2), 0.7f);
        defaultToggler->setTag(14736);
        defaultToggler->setPosition(106, -44);
        defaultToggler->setScale(0.675f);;

        CCLabelBMFont* defaultToggleLabel = CCLabelBMFont::create("Default P2", "bigFont.fnt");
        defaultToggleLabel->setPosition(176, -44);
        defaultToggleLabel->setScale(0.3);
        defaultToggleLabel->setAnchorPoint({ 1, .5 });

        gd::SimplePlayer* mainPlayer = self->m_pPlayerPreview;

        self->removeChild(mainPlayer);

        CCSprite* pointerLeft = CCSprite::createWithSpriteFrameName("navArrowBtn_001.png");
        CCSprite* pointerRight = CCSprite::createWithSpriteFrameName("navArrowBtn_001.png");

        pointerRight->setFlipX(true);
        pointerLeft->setScale(0.4f);
        pointerRight->setScale(0.4f);

        pointerLeft->setTag(34729);
        pointerRight->setTag(34730);

        pointerLeft->setPosition({ -90, 0 });
        pointerRight->setPosition({ 90, 0 });

        pointerRight->setVisible(false);

        CCAction* moveLeft = CCMoveBy::create(0.5, { 5, 0 });
        CCAction* moveRight = CCMoveBy::create(0.5, { -5, 0 });

        CCArray* actions = CCArray::create();
        actions->addObject(moveLeft);
        actions->addObject(moveRight);

        CCSequence* moveSeq = CCSequence::create(actions);

        CCAction* moveLeft2 = CCMoveBy::create(0.5, { -5, 0 });
        CCAction* moveRight2 = CCMoveBy::create(0.5, { 5, 0 });

        CCArray* actions2 = CCArray::create();
        actions2->addObject(moveLeft2);
        actions2->addObject(moveRight2);

        CCSequence* moveSeq2 = CCSequence::create(actions2);

        pointerLeft->runAction(CCRepeatForever::create(moveSeq));
        pointerRight->runAction(CCRepeatForever::create(moveSeq2));

        CCMenu* playerSelect = CCMenu::create();
        playerSelect->setPosition(mainPlayer->getPosition());

        gd::SimplePlayer* secondPlayer = gd::SimplePlayer::create(0);
        secondPlayer->setTag(43221);
        secondPlayer->setScale(1.6f);

        playerSelect->addChild(pointerLeft);
        playerSelect->addChild(pointerRight);

        mainPlayer->setContentSize({ 40,40 });
        secondPlayer->setContentSize({ 40,40 });

        gd::CCMenuItemSpriteExtra* mainPlayerBtn = gd::CCMenuItemSpriteExtra::create(mainPlayer, self, menu_selector(ButtonStuff::setP1));
        gd::CCMenuItemSpriteExtra* secondPlayerBtn = gd::CCMenuItemSpriteExtra::create(secondPlayer, self, menu_selector(ButtonStuff::setP2));
        secondPlayerBtn->setPosition({ 40, 0 });

        mainPlayerBtn->setPosition({ -40, 0 });

        mainPlayerBtn->setTag(1);
        secondPlayerBtn->setTag(2);

        playerSelect->setTag(74356);

        playerSelect->addChild(defaultToggler);
        playerSelect->addChild(defaultToggleLabel);

        playerSelect->addChild(mainPlayerBtn);
        playerSelect->addChild(secondPlayerBtn);

        mainPlayer->setAnchorPoint({ 0, 0 });
        secondPlayer->setAnchorPoint({ 0, 0 });

        CCMenu* menu = CCMenu::create();
        menu->setTag(23423);

        gd::CCMenuItemToggler* toggler = gd::CCMenuItemToggler::createWithStandardSprites(self, menu_selector(ButtonStuff::setGlowA), 0.7f);
        toggler->setTag(67532);
        toggler->setPosition(-165, -38);

        CCLabelBMFont* glowP2Label = CCLabelBMFont::create("Glow P2", "bigFont.fnt");
        glowP2Label->setPosition(-73.25, -37);
        glowP2Label->setScale(0.5);
        glowP2Label->setAnchorPoint({ 1, .5 });

        menu->addChild(glowP2Label);
        menu->addChild(toggler);
        self->addChild(menu);

        CCLabelBMFont* bothLabel = CCLabelBMFont::create("(Both Players)", "bigFont.fnt");
        bothLabel->setPosition(menu->getPosition().x - 50, menu->getPosition().y - 68);
        bothLabel->setScale(0.5);
        bothLabel->setAnchorPoint({ 1, .5 });
        bothLabel->setTag(63092);
        bothLabel->setVisible(false);

        self->addChild(bothLabel);

        int ID = 0;

        switch (lastPlayer)
        {
        case gd::kIconTypeCube:
            ID = icon;
            break;
        case gd::kIconTypeShip:
            ID = ship;
            break;
        case gd::kIconTypeBall:
            ID = ball;
            break;
        case gd::kIconTypeUfo:
            ID = UFO;
            break;
        case gd::kIconTypeWave:
            ID = wave;
            break;
        case gd::kIconTypeRobot:
            ID = robot;
            break;
        case gd::kIconTypeSpider:
            ID = spider;
            break;
        case gd::kIconTypeDeathEffect:
            ID = deathEffect;
            break;
        case gd::kIconTypeSpecial:
            ID = trail;
            break;
        default:
            break;
        }

        menu->setVisible(false);
        secondPlayer->updatePlayerFrame(ID, lastPlayer);
        self->addChild(playerSelect);

        setIconColor(self, primaryColor, true, true);
        setIconColor(self, secondaryColor, false, false);

        setSelectorPosColors(self, true);
        setSelectorPosColors(self, false);

        defaultToggler->toggle(usesDefault);
    
    return true;
}

void setIcon(gd::GJGarageLayer* self, gd::IconType type, int ID, bool changeIcon, bool isP2) {

    if (self->getChildByTag(74356)) {
        CCMenu* playerMenu = reinterpret_cast<CCMenu*>(self->getChildByTag(74356));
        if (playerMenu->getChildByTag(2)) {

            if (isP2) {
                gd::CCMenuItemSpriteExtra* button = reinterpret_cast<gd::CCMenuItemSpriteExtra*>(playerMenu->getChildByTag(2));
                gd::SimplePlayer* simplePlayer = reinterpret_cast<gd::SimplePlayer*>(button->getChildren()->objectAtIndex(0));

                if (changeIcon) {
                    lastPlayer = type;
                    simplePlayer->updatePlayerFrame(ID, type);
                }
            }
            int pos = 0;

            switch (type)
            {
            case gd::kIconTypeCube:
                pos = 10;
                break;
            case gd::kIconTypeShip:
                pos = 11;
                break;
            case gd::kIconTypeBall:
                pos = 12;
                break;
            case gd::kIconTypeUfo:
                pos = 13;
                break;
            case gd::kIconTypeWave:
                pos = 14;
                break;
            case gd::kIconTypeRobot:
                pos = 15;
                break;
            case gd::kIconTypeSpider:
                pos = 16;
                break;
            case gd::kIconTypeDeathEffect:
                pos = 17;
                break;
            case gd::kIconTypeSpecial:
                pos = 18;
                break;
            default:
                break;
            }

            gd::GaragePage* garagePage = reinterpret_cast<gd::GaragePage*>(self->getChildByTag(pos));

            setSelectorPosIcons(garagePage, isP2);

            if (isP2) {
                saveIcons();
            }
        }
    }
}

void setIconColor(gd::GJGarageLayer* self, int ID, bool primary, bool isP2) {

    if (self->getChildByTag(74356)) {
        CCMenu* playerMenu = reinterpret_cast<CCMenu*>(self->getChildByTag(74356));
        if (playerMenu->getChildByTag(2)) {
            gd::CCMenuItemSpriteExtra* button = reinterpret_cast<gd::CCMenuItemSpriteExtra*>(playerMenu->getChildByTag(2));
            gd::SimplePlayer* simplePlayer = reinterpret_cast<gd::SimplePlayer*>(button->getChildren()->objectAtIndex(0));

            ccColor3B color = gd::GameManager::sharedState()->colorForIdx(ID);

            if (primary) simplePlayer->setColor(color);
            else simplePlayer->setSecondColor(color);

            simplePlayer->setGlowOutline(hasGlow);
            simplePlayer->updateColors();

            setSelectorPosColors(self, isP2);

            saveIcons();
        }
    }
}

void setGlow(gd::GJGarageLayer* garage) {

    CCMenu* playerMenu = reinterpret_cast<CCMenu*>(garage->getChildByTag(74356));
    gd::CCMenuItemSpriteExtra* button = reinterpret_cast<gd::CCMenuItemSpriteExtra*>(playerMenu->getChildByTag(2));
    gd::SimplePlayer* simplePlayer = reinterpret_cast<gd::SimplePlayer*>(button->getChildren()->objectAtIndex(0));
    
    simplePlayer->setGlowOutline(hasGlow);
    simplePlayer->updateColors();
    saveIcons();
}


void(__thiscall* GJGarageLayer_onPlayerIcon)(gd::GJGarageLayer* self, CCObject* param_1);

void __fastcall GJGarageLayer_onPlayerIcon_H(gd::GJGarageLayer* self, void*, CCObject* param_1) {

    if (is2ndPlayer) {
        icon = param_1->getTag();
        setIcon(self, gd::IconType::kIconTypeCube, icon, true, true);
    }
    else {
        GJGarageLayer_onPlayerIcon(self, param_1);
        setIcon(self, gd::IconType::kIconTypeCube, icon, true, false);
    }
}

void(__thiscall* GJGarageLayer_onPlayerShip)(gd::GJGarageLayer* self, CCObject* param_1);

void __fastcall GJGarageLayer_onPlayerShip_H(gd::GJGarageLayer* self, void*, CCObject* param_1) {

    if (is2ndPlayer) {
        ship = param_1->getTag();
        setIcon(self, gd::IconType::kIconTypeShip, ship, true, true);
    }
    else {
        GJGarageLayer_onPlayerShip(self, param_1);
        setIcon(self, gd::IconType::kIconTypeShip, ship, true, false);
    }
}

void(__thiscall* GJGarageLayer_onPlayerBall)(gd::GJGarageLayer* self, CCObject* param_1);

void __fastcall GJGarageLayer_onPlayerBall_H(gd::GJGarageLayer* self, void*, CCObject* param_1) {

    if (is2ndPlayer) {
        ball = param_1->getTag();
        setIcon(self, gd::IconType::kIconTypeBall, ball, true, true);

    }
    else {
        GJGarageLayer_onPlayerBall(self, param_1);
        setIcon(self, gd::IconType::kIconTypeBall, ball, true, false);
    }
}

void(__thiscall* GJGarageLayer_onPlayerUFO)(gd::GJGarageLayer* self, CCObject* param_1);

void __fastcall GJGarageLayer_onPlayerUFO_H(gd::GJGarageLayer* self, void*, CCObject* param_1) {

    if (is2ndPlayer) {
        UFO = param_1->getTag();
        setIcon(self, gd::IconType::kIconTypeUfo, UFO, true, true);

    }
    else {
        GJGarageLayer_onPlayerUFO(self, param_1);
        setIcon(self, gd::IconType::kIconTypeUfo, UFO, true, false);
    }
}

void(__thiscall* GJGarageLayer_onPlayerWave)(gd::GJGarageLayer* self, CCObject* param_1);

void __fastcall GJGarageLayer_onPlayerWave_H(gd::GJGarageLayer* self, void*, CCObject* param_1) {
    
    if (is2ndPlayer) {
        wave = param_1->getTag();
        setIcon(self, gd::IconType::kIconTypeWave, wave, true, true);
    }
    else {
        GJGarageLayer_onPlayerWave(self, param_1);
        setIcon(self, gd::IconType::kIconTypeWave, wave, true, false);
    }
}

void(__thiscall* GJGarageLayer_onPlayerRobot)(gd::GJGarageLayer* self, CCObject* param_1);

void __fastcall GJGarageLayer_onPlayerRobot_H(gd::GJGarageLayer* self, void*, CCObject* param_1) {

    if (is2ndPlayer) {
        robot = param_1->getTag();
        setIcon(self, gd::IconType::kIconTypeRobot, robot, true, true);

    }
    else {
        GJGarageLayer_onPlayerRobot(self, param_1);
        setIcon(self, gd::IconType::kIconTypeRobot, robot, true, false);
    }
}

void(__thiscall* GJGarageLayer_onPlayerSpider)(gd::GJGarageLayer* self, CCObject* param_1);

void __fastcall GJGarageLayer_onPlayerSpider_H(gd::GJGarageLayer* self, void*, CCObject* param_1) {
    
    if (is2ndPlayer) {
        spider = param_1->getTag();
        setIcon(self, gd::IconType::kIconTypeSpider, spider, true, true);

    }
    else {
        GJGarageLayer_onPlayerSpider(self, param_1);
        setIcon(self, gd::IconType::kIconTypeSpider, spider, true, false);
    }
}

void(__thiscall* GJGarageLayer_onPlayerTrail)(gd::GJGarageLayer* self, CCObject* param_1);

void __fastcall GJGarageLayer_onPlayerTrail_H(gd::GJGarageLayer* self, void*, CCObject* param_1) {

    if (is2ndPlayer) {
        trail = param_1->getTag();
        setIcon(self, gd::IconType::kIconTypeSpecial, trail, false, true);

    }
    else {
        GJGarageLayer_onPlayerTrail(self, param_1);
        setIcon(self, gd::IconType::kIconTypeSpecial, trail, false, false);
    }
}

void(__thiscall* GJGarageLayer_onPlayerDeathEffect)(gd::GJGarageLayer* self, CCObject* param_1);

void __fastcall GJGarageLayer_onPlayerDeathEffect_H(gd::GJGarageLayer* self, void*, CCObject* param_1) {

    GJGarageLayer_onPlayerDeathEffect(self, param_1);
    setIcon(self, gd::IconType::kIconTypeDeathEffect, deathEffect, false, false);
}

void(__thiscall* GJGarageLayer_onPrimaryColor)(gd::GJGarageLayer* self, CCObject* param_1);

void __fastcall GJGarageLayer_onPrimaryColor_H(gd::GJGarageLayer* self, void*, CCObject* param_1) {

    if (is2ndPlayer) {
        primaryColor = param_1->getTag();
        setIconColor(self, primaryColor, true, true);
    }
    else {
        GJGarageLayer_onPrimaryColor(self, param_1);
        setIconColor(self, primaryColor, true, false);
    }
}

void(__thiscall* GJGarageLayer_onSecondaryColor)(gd::GJGarageLayer* self, CCObject* param_1);

void __fastcall GJGarageLayer_onSecondaryColor_H(gd::GJGarageLayer* self, void*, CCObject* param_1) {

    if (is2ndPlayer) {
        secondaryColor = param_1->getTag();
        setIconColor(self, secondaryColor, false, true);
    }
    else {
        GJGarageLayer_onSecondaryColor(self, param_1);
        setIconColor(self, secondaryColor, false, false);
    }
}

void(__thiscall* PlayerObject_updatePlayerBirdFrame)(gd::PlayerObject* self, int param_1);

void __fastcall PlayerObject_updatePlayerBirdFrame_H(gd::PlayerObject* self, void*, int param_1) {

    PlayerObject_updatePlayerBirdFrame(self, param_1);
    if (!usesDefault) {
        setColor(self);
        setFrame(self, gd::IconType::kIconTypeUfo);
    }
}

void(__thiscall* PlayerObject_updatePlayerDartFrame)(gd::PlayerObject* self, int param_1);

void __fastcall PlayerObject_updatePlayerDartFrame_H(gd::PlayerObject* self, void*, int param_1) {

    PlayerObject_updatePlayerDartFrame(self, param_1);
    if (!usesDefault) {
        setColor(self);
        setFrame(self, gd::IconType::kIconTypeWave);
    }
}

void(__thiscall* PlayerObject_updatePlayerFrame)(gd::PlayerObject* self, int param_1);

void __fastcall PlayerObject_updatePlayerFrame_H(gd::PlayerObject* self, void*, int param_1) {

    PlayerObject_updatePlayerFrame(self, param_1);
    if (!usesDefault) {
        setColor(self);
        setFrame(self, gd::IconType::kIconTypeCube);
    }
}

void(__thiscall* PlayerObject_updatePlayerRollFrame)(gd::PlayerObject* self, int param_1);

void __fastcall PlayerObject_updatePlayerRollFrame_H(gd::PlayerObject* self, void*, int param_1) {

    PlayerObject_updatePlayerRollFrame(self, param_1);
    if (!usesDefault) {
        setColor(self);
        setFrame(self, gd::IconType::kIconTypeBall);
    }
}

void(__thiscall* PlayerObject_updatePlayerShipFrame)(gd::PlayerObject* self, int param_1);

void __fastcall PlayerObject_updatePlayerShipFrame_H(gd::PlayerObject* self, void*, int param_1) {

    PlayerObject_updatePlayerShipFrame(self, param_1);
    if (!usesDefault) {
        setColor(self);
        setFrame(self, gd::IconType::kIconTypeShip);
    }
}

int callPosStreak = 0;

void(__thiscall* PlayerObject_setupStreak)(gd::PlayerObject* self);

void __fastcall PlayerObject_setupStreak_H(gd::PlayerObject* self, void*) {

    if (!usesDefault) {
        int origStreak = gd::GameManager::sharedState()->getPlayerStreak();

        if (gd::PlayLayer::get()) {
            if (callPosStreak == 1) {
                gd::GameManager::sharedState()->setPlayerStreak(trail);
            }

            callPosStreak++;
            if (callPosStreak == 2) callPosStreak = 0;
        }

        PlayerObject_setupStreak(self);

        gd::GameManager::sharedState()->setPlayerStreak(origStreak);
    }
    else {
        PlayerObject_setupStreak(self);
    }
}

bool(__thiscall* PlayLayer_init)(gd::PlayLayer* self, gd::GJGameLevel* level);

bool __fastcall PlayLayer_init_H(gd::PlayLayer* self, void*, gd::GJGameLevel* level) {

    callPosStreak = 0;
    if (!PlayLayer_init(self, level)) return false;

    if (!usesDefault) {

        gd::PlayerObject* player2 = self->m_pPlayer2;

        if (player2->m_isShip) {
            setFrame(player2, gd::IconType::kIconTypeShip);
        }
        else if (player2->m_isBall) {
            setFrame(player2, gd::IconType::kIconTypeBall);
        }
        else if (player2->m_isBird) {
            setFrame(player2, gd::IconType::kIconTypeUfo);
        }
        else if (player2->m_isDart) {
            setFrame(player2, gd::IconType::kIconTypeWave);
        }
        else if (player2->m_isRobot) {
            player2->m_pRobot->updateFrame(robot);
        }
        else if (player2->m_isSpider) {
            player2->m_pSpider->updateFrame(spider);
        }
        else {
            setFrame(player2, gd::IconType::kIconTypeCube);
        }

        setColor(player2);
    }

    return true;
}

void(__thiscall* GJRobotSprite_updateFrame)(gd::GJRobotSprite*, int);

void __fastcall GJRobotSprite_updateFrame_H(gd::GJRobotSprite* self, void*, int iconID) {

    if (!usesDefault && gd::PlayLayer::get() && gd::PlayLayer::get()->m_pPlayer2) {
        setColor(gd::PlayLayer::get()->m_pPlayer2);
    }
    
    GJRobotSprite_updateFrame(self, iconID);
}

void(__thiscall* PlayerObject_switchedToMode)(gd::PlayerObject*, int);

void __fastcall PlayerObject_switchedToMode_H(gd::PlayerObject* self, void*, int param_1) {

    PlayerObject_switchedToMode(self, param_1);
    if (!usesDefault && isSecondPlayerInPlay(self)) {
        if (param_1 == 0x1b) {
            gd::PlayLayer::get()->m_pPlayer2->m_pRobot->updateFrame(robot);
            setColor(self);
        }
        if (param_1 == 0x21) {
            gd::PlayLayer::get()->m_pPlayer2->m_pSpider->updateFrame(spider);
            setColor(self);
        }
    }
}

bool isSecondPlayerInPlay(gd::PlayerObject* playerObject) {

    if (playerObject && gd::PlayLayer::get() && gd::PlayLayer::get()->m_pPlayer2) {
       return playerObject == gd::PlayLayer::get()->m_pPlayer2; 
    }

    return false;
}

void setColor(gd::PlayerObject* playerObject) {
    
    if (isSecondPlayerInPlay(playerObject)) {
        ccColor3B colorPrimary = gd::GameManager::sharedState()->colorForIdx(primaryColor);
        ccColor3B colorSecondary = gd::GameManager::sharedState()->colorForIdx(secondaryColor);

        playerObject->setColor(colorPrimary);
        playerObject->setSecondColor(colorSecondary);
        playerObject->m_bHasGlow = hasGlow;

        playerObject->updatePlayerGlow();
    }
}

void setFrame(gd::PlayerObject* playerObject, gd::IconType iconType) {

    if (isSecondPlayerInPlay(playerObject)) {
        switch (iconType)
        {
        case gd::kIconTypeCube:
            PlayerObject_updatePlayerFrame(playerObject, icon);
            break;
        case gd::kIconTypeShip:
            PlayerObject_updatePlayerShipFrame(playerObject, ship);
            break;
        case gd::kIconTypeBall:
            PlayerObject_updatePlayerRollFrame(playerObject, ball);
            break;
        case gd::kIconTypeUfo:
            PlayerObject_updatePlayerBirdFrame(playerObject, UFO);
            break;
        case gd::kIconTypeWave:
            PlayerObject_updatePlayerDartFrame(playerObject, wave);
            break;
        case gd::kIconTypeRobot:
            break;
        case gd::kIconTypeSpider:
            break;
        case gd::kIconTypeDeathEffect:
            break;
        case gd::kIconTypeSpecial:
            break;
        default:
            break;
        }
    }
}

void setGlowToggler(bool enabled, gd::GJGarageLayer* garage) {

    if (garage->getChildByTag(23423)) {
        CCMenu* menu = reinterpret_cast<CCMenu*>(garage->getChildByTag(23423));
        if (menu->getChildByTag(67532)) {

            gd::CCMenuItemToggler* toggler = reinterpret_cast<gd::CCMenuItemToggler*>(menu->getChildByTag(67532));

            if (enabled) {
                menu->setVisible(true);
                if (hasGlow && !toggler->isOn()) {
                    toggler->activate();
                }
            }
            else {
                menu->setVisible(false);
            }
        }
    }
}

std::vector<std::string> split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }
    res.push_back(s.substr(pos_start));
    return res;
}


void loadIcons() {
    
    if (!std::filesystem::exists("p2IconData")) {
        return;
    }

    std::ifstream configFile("p2IconData");

    try {
        std::stringstream buffer;
        buffer << configFile.rdbuf();

        std::string value = buffer.str();
        std::vector<std::string> values = split(value, ",");
        icon = std::stoi(values.at(0));
        ship = std::stoi(values.at(1));
        ball = std::stoi(values.at(2));
        UFO = std::stoi(values.at(3));
        wave = std::stoi(values.at(4));
        robot = std::stoi(values.at(5));
        spider = std::stoi(values.at(6));
        trail = std::stoi(values.at(7));
        deathEffect = std::stoi(values.at(8));
        primaryColor = std::stoi(values.at(9));
        secondaryColor = std::stoi(values.at(10));
        hasGlow = std::stoi(values.at(11));
        lastPlayer = (gd::IconType)std::stoi(values.at(12));
        usesDefault = std::stoi(values.at(13));
    }
    catch (std::exception& E) {
        std::cout << E.what() << std::endl;
    }

    configFile.close();
}
void saveIcons() {

    std::ofstream configFile("p2IconData", std::ios::out | std::ios::trunc);
    configFile << std::to_string(icon) << ",";
    configFile << std::to_string(ship) << ",";
    configFile << std::to_string(ball) << ",";
    configFile << std::to_string(UFO) << ",";
    configFile << std::to_string(wave) << ",";
    configFile << std::to_string(robot) << ",";
    configFile << std::to_string(spider) << ",";
    configFile << std::to_string(trail) << ",";
    configFile << std::to_string(deathEffect) << ",";
    configFile << std::to_string(primaryColor) << ",";
    configFile << std::to_string(secondaryColor) << ",";
    configFile << std::to_string((int)hasGlow) << ",";
    configFile << std::to_string((int)lastPlayer) << ",";
    configFile << std::to_string((int)usesDefault) << ",";

    configFile.close();
}

void(__thiscall* GJGarageLayer_onTab)(gd::GJGarageLayer* self, int);

void __fastcall GJGarageLayer_onTab_H(gd::GJGarageLayer* self, void*, int tabNumber) {

    GJGarageLayer_onTab(self, tabNumber);
    if (self->getChildByTag(63092)) {
        self->getChildByTag(63092)->setVisible(false);

        setGlowToggler(tabNumber == 99, self);
        if (tabNumber == 98) {
            self->getChildByTag(63092)->setVisible(true);
        }
    }
}

DWORD WINAPI thread_func(void* hModule) {
    MH_Initialize();

    /*AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);*/

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(100, 2000);

    int random = distr(gen);

    std::this_thread::sleep_for(std::chrono::milliseconds(random));

    auto base = reinterpret_cast<uintptr_t>(GetModuleHandle(0));

    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x1907b0),
        MenuLayer_init_H,
        reinterpret_cast<void**>(&MenuLayer_init)
    );
    
    MH_EnableHook(&MenuLayer_init);

    if (GetModuleHandle("SaiModPack.dll")) {
        hasSai = true;
        return 0;
    }

    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x1255d0),
        GJGarageLayer_init_H,
        reinterpret_cast<void**>(&GJGarageLayer_init)
    );
    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x127f30),
        GJGarageLayer_onPlayerIcon_H,
        reinterpret_cast<void**>(&GJGarageLayer_onPlayerIcon)
    );
    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x1281e0),
        GJGarageLayer_onPlayerShip_H,
        reinterpret_cast<void**>(&GJGarageLayer_onPlayerShip)
    );
    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x1282a0),
        GJGarageLayer_onPlayerBall_H,
        reinterpret_cast<void**>(&GJGarageLayer_onPlayerBall)
    );
    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x128360),
        GJGarageLayer_onPlayerUFO_H,
        reinterpret_cast<void**>(&GJGarageLayer_onPlayerUFO)
    );
    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x128420),
        GJGarageLayer_onPlayerWave_H,
        reinterpret_cast<void**>(&GJGarageLayer_onPlayerWave)
    );
    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x1286d0),
        GJGarageLayer_onPlayerRobot_H,
        reinterpret_cast<void**>(&GJGarageLayer_onPlayerRobot)
    );
    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x128890),
        GJGarageLayer_onPlayerSpider_H,
        reinterpret_cast<void**>(&GJGarageLayer_onPlayerSpider)
    );
    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x128af0),
        GJGarageLayer_onPlayerTrail_H,
        reinterpret_cast<void**>(&GJGarageLayer_onPlayerTrail)
    );
    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x128a50),
        GJGarageLayer_onPlayerDeathEffect_H,
        reinterpret_cast<void**>(&GJGarageLayer_onPlayerDeathEffect)
    );
    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x129470),
        GJGarageLayer_onPrimaryColor_H,
        reinterpret_cast<void**>(&GJGarageLayer_onPrimaryColor)
    );
    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x129590),
        GJGarageLayer_onSecondaryColor_H,
        reinterpret_cast<void**>(&GJGarageLayer_onSecondaryColor)
    );
    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x127c50),
        GJGarageLayer_onTab_H,
        reinterpret_cast<void**>(&GJGarageLayer_onTab)
    );
    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x1f81a0),
        PlayerObject_updatePlayerBirdFrame_H,
        reinterpret_cast<void**>(&PlayerObject_updatePlayerBirdFrame)
    );
    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x1f8580),
        PlayerObject_updatePlayerDartFrame_H,
        reinterpret_cast<void**>(&PlayerObject_updatePlayerDartFrame)
    );
    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x1f7e40),
        PlayerObject_updatePlayerFrame_H,
        reinterpret_cast<void**>(&PlayerObject_updatePlayerFrame)
    );
    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x1f83a0),
        PlayerObject_updatePlayerRollFrame_H,
        reinterpret_cast<void**>(&PlayerObject_updatePlayerRollFrame)
    );
    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x1f7ff0),
        PlayerObject_updatePlayerShipFrame_H,
        reinterpret_cast<void**>(&PlayerObject_updatePlayerShipFrame)
    );
    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x1f6f70),
        PlayerObject_switchedToMode_H,
        reinterpret_cast<void**>(&PlayerObject_switchedToMode)
    );
    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x146700),
        GJRobotSprite_updateFrame_H,
        reinterpret_cast<void**>(&GJRobotSprite_updateFrame)
    );
    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x1e7e90),
        PlayerObject_setupStreak_H,
        reinterpret_cast<void**>(&PlayerObject_setupStreak)
    );
    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x1fb780),
        PlayLayer_init_H,
        reinterpret_cast<void**>(&PlayLayer_init)
    );
    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x12af70),
        GaragePage_init_H,
        reinterpret_cast<void**>(&GaragePage_init)
    );
    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x29c50),
        ListButtonBar_switchedPage_H,
        reinterpret_cast<void**>(&ListButtonBar_switchedPage)
    );
    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x12aad0),
        GJGarageLayer_showCircleWave_H,
        reinterpret_cast<void**>(&GJGarageLayer_showCircleWave)
    );
    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x12a9d0),
        GJGarageLayer_showBlackCircleWave_H,
        reinterpret_cast<void**>(&GJGarageLayer_showBlackCircleWave)
    );

    MH_EnableHook(MH_ALL_HOOKS);

    loadIcons();

    return 0;
}

BOOL APIENTRY DllMain(HMODULE handle, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        CreateThread(0, 0x100, thread_func, handle, 0, 0);
    }

    return TRUE;
}