#include "HelloWorldScene.h"
#include "ui/CocosGUI.h"
#include "GameScene.h"
#include "AudioEngine.h"

USING_NS_CC;
using namespace cocos2d::ui;

Scene *HelloWorld::createScene() {
    return HelloWorld::create();
}

bool HelloWorld::init() {
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    int id_music = AudioEngine::play2d("music_bg.mp3", true);
    EventDispatcher *ed = Director::getInstance()->getEventDispatcher();

    EventListenerKeyboard *elk = EventListenerKeyboard::create();//quit game
    elk->onKeyReleased = [=](EventKeyboard::KeyCode keycode, Event *event) {
        if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE)
            Director::getInstance()->end();
    };
    ed->addEventListenerWithSceneGraphPriority(elk, this);

    auto bg = Sprite::create("bg1.jpg");
    bg->setPosition(visibleSize / 2);
    addChild(bg);

    auto btn_music = CheckBox::create("music_on.png", "music_off.png");
    //need music_bg?
    btn_music->addClickEventListener([=](Ref *pSend) {
        !btn_music->isSelected() ? AudioEngine::pause(id_music) : AudioEngine::resume(id_music);
    });
    btn_music->setPosition(Vec2(900, 50));
    addChild(btn_music);

    auto start = Button::create("btn_start1.png", "btn_start2.png");
    start->setPosition(visibleSize / 2);
    addChild(start);
    //start game and choose mode
    start->addClickEventListener([=](Ref *pSend) {
        MenuItemFont::setFontName("黑体");
        MenuItemFont::setFontSize(60);
        auto btn_offline = MenuItemFont::create("单人游戏", [=](Ref *pSend) {
            auto gameScene = GameScene::createScene();
            ((GameScene *) gameScene)->setMode(true);
            ((GameScene *) gameScene)->setMusic(id_music, btn_music->isSelected());
            Director::getInstance()->purgeCachedData();
            Director::getInstance()->replaceScene(gameScene);
        });
        auto btn_online = MenuItemFont::create("联机游戏", [=](Ref *pSend) {
            auto gameScene = GameScene::createScene();
            ((GameScene *) gameScene)->setMode(false);
            Director::getInstance()->purgeCachedData();
            Director::getInstance()->replaceScene(gameScene);
        });
        btn_offline->setColor(Color3B::BLACK);
        btn_online->setColor(Color3B::BLACK);
        auto menu = Menu::create(btn_offline, btn_online, NULL);
        menu->alignItemsVertically();
        menu->setPosition(visibleSize / 2);

        auto dialog = Sprite::create("dialog_background.png");
        dialog->setPosition(visibleSize / 2);
        auto pt = ProgressTimer::create(dialog);
        auto p = ProgressTo::create(0.1f, 100);
        pt->setType(ProgressTimer::Type::BAR);
        pt->runAction(p);
        pt->setPosition(visibleSize / 2);

        addChild(pt);
        addChild(menu);
    });


    return true;
}
