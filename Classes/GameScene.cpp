
#include <thread>
#include "GameScene.h"
#include "ui/CocosGUI.h"
#include "Role.h"
#include "Animal.h"
#include "Diamond.h"
#include "Wall.h"
#include "Stone.h"
#include "TNT.h"
#include "AudioEngine.h"
#include "Gift.h"
#include <iostream>
#include <Winsock2.h>
  
#pragma comment(lib, "ws2_32.lib")

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

cocos2d::Scene *GameScene::createScene() {
    return GameScene::create();
}

bool GameScene::init() {
    if (!Scene::initWithPhysics()) {
        return false;
    }
    _physicsWorld->setGravity(Vec2(0, 0));
    // _physicsWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    setRunning(true);

    EventListenerKeyboard *elk = EventListenerKeyboard::create();
    elk->onKeyPressed = [=](EventKeyboard::KeyCode keycode, Event *event) {
        if (getRunning() && isSingle) {
            if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE) {
                setRunning(false);
                auto dialog = Sprite::create("dialog_background.png");
                auto pt = ProgressTimer::create(dialog);
                auto p = ProgressTo::create(0.5f, 100);
                //  pt->setType(ProgressTimer::Type::BAR);
                pt->runAction(Sequence::create(p, CallFunc::create([=] {
                    Director::getInstance()->pause();
                    //Director::getInstance()->stopAnimation();
                    Director::getInstance()->getScheduler()->setTimeScale(0);
                }), NULL));
                pt->setPosition(visibleSize / 2);
                addChild(pt);
                auto btn_pause = Button::create("btn_resume.png");
                btn_pause->setPosition(Vec2(pt->getContentSize().width / 2, pt->getContentSize().height / 2));
                btn_pause->addClickEventListener([=](Ref *pSend) {
                                                     Director::getInstance()->resume();
                                                     //Director::getInstance()->startAnimation();
                                                     Director::getInstance()->getScheduler()->setTimeScale(1);
                                                     auto moveOut = MoveBy::create(0.5, Vec2(-visibleSize.width, 0));
                                                     pt->runAction(Sequence::create(moveOut, CallFunc::create([=] {
                                                         setRunning(true);
                                                         pt->removeFromParent();
                                                     }), NULL));
                                                 }
                );
                pt->addChild(btn_pause);

                auto btn_quit = Button::create("btn_quit.png");
                btn_quit->setPosition(Vec2(pt->getContentSize().width / 2 + 140, pt->getContentSize().height / 2));
                btn_quit->addClickEventListener([=](Ref *pSend) {
                    Director::getInstance()->end();
                });
                pt->addChild(btn_quit);

                auto btn_music = CheckBox::create("music_on.png", "music_off.png");
                //need music_bg?
                btn_music->setSelected(music_playing);
                btn_music->addClickEventListener([=](Ref *pSend) {
                    !btn_music->isSelected() ? AudioEngine::pause(music_id) : AudioEngine::resume(music_id);
                    music_playing = !btn_music->isSelected();
                });
                btn_music->setPosition(Vec2(pt->getContentSize().width / 2 - 140, pt->getContentSize().height / 2));
                pt->addChild(btn_music);
            }
        }

    };
    ed->addEventListenerWithSceneGraphPriority(elk, this);

    auto listener_contact = EventListenerPhysicsContact::create();
    listener_contact->onContactBegin = CC_CALLBACK_1(GameScene::hit, this);
    ed->addEventListenerWithSceneGraphPriority(listener_contact, this);

    visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto bg = Sprite::create("bg2.jpg");
    bg->setPosition(visibleSize / 2);
    addChild(bg);

    auto bar_target = Sprite::create("bar_target.png");
    auto bar_score = Sprite::create("bar_score.png");
    bar_target->setPosition(bar_target->getContentSize().width / 2 + 10,
                            visibleSize.height - bar_target->getContentSize().height / 2 - 20);
    bar_score->setPosition(bar_score->getContentSize().width / 2 + 10,
                           visibleSize.height - bar_score->getContentSize().height * 3 / 2 - 30);
    addChild(bar_target);
    addChild(bar_score);
    setTarget(8000);
    auto target_score = Text::create(std::to_string(getTarget()), "楷体", 40);
    auto now_score = Text::create("0", "楷体", 40);
    now_score->setName("score");
    target_score->setPosition(Vec2(bar_target->getContentSize().width / 2 + 180,
                                   visibleSize.height - bar_target->getContentSize().height / 2 - 20));
    now_score->setPosition(Vec2(bar_score->getContentSize().width / 2 + 180,
                                visibleSize.height - bar_score->getContentSize().height * 3 / 2 - 30));
    addChild(target_score);
    addChild(now_score);

    auto bar_time = Text::create("剩余时间", "楷体", 40);
    bar_time->setPosition(Vec2(850, 580));
    addChild(bar_time);


    return true;
}

void GameScene::createOfflineScene() {
    auto pos_role = Vec2(visibleSize.width / 2, 560);
    auto role = Role::create("role1.png");
    role->setPosition(pos_role);
    addChild(role);
    addChild(role->getNowMagic());

    auto text_time = Text::create("30", "楷体", 40);
    text_time->setPosition(Vec2(850, 540));
    addChild(text_time);
    schedule([=](float dt) {
                 if (getTime() > 0) {
                     setTime();
                     text_time->setString(std::to_string(getTime()));
                 } else {
                     if (!getEnd()) {
                         this->runAction(Sequence::create(CallFunc::create([=] {
                             text_time->setString(std::to_string(0));
                         }), CallFunc::create([=] {
                             Label *l = Label::createWithSystemFont("", "楷体", 72);
                             l->setPosition(visibleSize.width / 2, visibleSize.height / 2);
                             l->setColor(Color3B::RED);
                             l->enableBold();
                             addChild(l);
                             l->setVisible(false);
                             if (role->getScore() >= getTarget()) {
                                 l->setString("通关");
                                 ParticleSystemQuad *eh = ParticleSystemQuad::create("end_happy.plist");
                                 eh->setPosition(visibleSize / 2);
                                 // psd->setScale(0.4f);
                                 setEnd(true);
                                 addChild(eh);

                             } else {
                                 l->setString("失败");
                                 ParticleSystemQuad *es = ParticleSystemQuad::create("end_sad.plist");
                                 es->setPosition(visibleSize / 2);
                                 //psd->setScale(0.4f);
                                 setEnd(true);
                                 addChild(es);

                             }
                             role->getRing()->setColor(Color3B::BLUE);
                             role->getRing()->getPhysicsBody()->setVelocity(Vec2(0, 0));
                             role->getRing()->getPhysicsBody()->setCollisionBitmask(0);
                             role->getRing()->setTag(0);
                             role->getRing()->move(1);
                             // Director::getInstance()->stopAnimation();
                         }), NULL));
                     }
                 }
             },
             1, "time over");

    EventListenerMouse *elm = EventListenerMouse::create();
    elm->onMouseUp = [=](EventMouse *event) {
        if (getRunning() && !getEnd()) {
            if (event->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT) {
                auto pos_mouse = Vec2(event->getCursorX(), event->getCursorY());
                role->heave(pos_mouse);
            } else if (event->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT) {
                if (!role->getMagic().empty())
                    role->conjure(role->getMagic().back());
            }
        }
    };
    ed->addEventListenerWithSceneGraphPriority(elm, this);


    auto edge = Wall::create();
    edge->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 75);
    addChild(edge);

    auto wall = Wall::create("wall.png");
    wall->setRotation(20);
    wall->setPosition(visibleSize.width / 2, 40);
    addChild(wall);

    auto wall2 = Wall::create("wall.png");
    wall2->setRotation(-45);
    wall2->setPosition(500, 300);
    addChild(wall2);

    auto wall3 = Wall::create("wall.png");
    wall3->setPosition(600, 400);
    addChild(wall3);

    auto animal = Animal::create("mouse_r1.png");
    animal->setPosition(0, visibleSize.height / 2 - 40);
    addChild(animal);
    animal->act("mouse");

    auto animal2 = Animal::create("cow_r1.png");
    animal2->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 45);
    addChild(animal2);
    animal2->act("cow");
    animal2->setWeight(2);
    animal2->setValue(500);

    auto stone = Stone::create("stone1.png");
    stone->setPosition(visibleSize.width / 4, visibleSize.height / 6);
    stone->setValue(50);
    stone->setWeight(1.5);
    addChild(stone);

    auto stone2 = Stone::create("stone2.png");
    stone2->setPosition(visibleSize.width * 2 / 5, visibleSize.height / 9);
    stone2->setValue(75);
    stone2->setWeight(2.5);
    addChild(stone2);

    auto stone3 = Stone::create("stone3.png");
    stone3->setPosition(visibleSize.width * 3 / 4, visibleSize.height / 5);
    stone3->setValue(150);
    stone3->setWeight(5);
    addChild(stone3);

    auto diamond = Diamond::create("diamond1.png");
    diamond->setPosition(visibleSize.width / 4, visibleSize.height / 2);
    addChild(diamond);

    auto diamond2 = Diamond::create("diamond2.png");
    diamond2->setPosition(visibleSize.width / 4 - 120, visibleSize.height / 2 + 80);
    addChild(diamond2);

    auto diamond3 = Diamond::create("diamond3.png");
    diamond3->setPosition(visibleSize.width / 8, visibleSize.height / 2 - 200);
    addChild(diamond3);

    auto diamond4 = Diamond::create("diamond4.png");
    diamond4->setPosition(visibleSize.width / 4, visibleSize.height / 2 - 300);
    addChild(diamond4);

    auto tnt = TNT::create("TNT.png");
    tnt->setPosition(visibleSize.width / 4 + 40, visibleSize.height / 2 + 80);
    addChild(tnt);

    auto tnt2 = TNT::create("TNT.png");
    tnt2->setPosition(visibleSize.width / 4 + 40, visibleSize.height / 2 - 120);
    addChild(tnt2);

    auto tnt3 = TNT::create("TNT.png");
    tnt3->setPosition(visibleSize.width / 4 - 60, visibleSize.height / 2 + 80);
    addChild(tnt3);

    auto gift = Gift::create("gift.png");
    gift->setValue(-1);
    gift->setWeight(2);
    gift->setPosition(450, 200);
    addChild(gift);

    auto gift2 = Gift::create("gift.png");
    gift2->setValue(-2);
    gift2->setWeight(2);
    gift2->setPosition(700, 300);
    addChild(gift2);

    auto gift3 = Gift::create("gift.png");
    gift3->setValue(-3);
    gift3->setWeight(2);
    gift3->setPosition(680, 220);
    addChild(gift3);

    auto gift4 = Gift::create("gift.png");
    gift4->setValue(-4);
    gift4->setWeight(1);
    gift4->setPosition(100, 200);
    addChild(gift4);

}

vector<string> split(const string &str, const string &delim) {
    vector<string> res;
    if (str.empty()) return res;
    //先将要切割的字符串从string类型转换为char*类型
    char *strs = new char[str.length() + 1]; //不要忘了
    strcpy(strs, str.c_str());

    char *d = new char[delim.length() + 1];
    strcpy(d, delim.c_str());

    char *p = strtok(strs, d);
    while (p) {
        string s = p; //分割得到的字符串转换为string类型
        res.push_back(s); //存入结果数组
        p = strtok(nullptr, d);
    }
    delete[] strs;
    delete[] d;
    return res;
}

void GameScene::createOnlineScene() {
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD(2, 2);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0)
        cout << "error" << endl;
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
        WSACleanup();
        cout << "error" << endl;
    }

    SOCKET socketClient = socket(AF_INET, SOCK_DGRAM, 0);
    SOCKADDR_IN addrSrv;
    addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(6800);

    int len = sizeof(SOCKADDR);
    char sendBuf[100] = {0};
    char recvBuf[100] = {0};
    sendBuf[0] = 'n';//new role
    sendto(socketClient, sendBuf, sizeof(sendBuf), 0, (SOCKADDR *) &addrSrv, len);//new scene
    recvfrom(socketClient, recvBuf, sizeof(recvBuf), 0, (SOCKADDR *) &addrSrv, &len);//receive my name
    char name = recvBuf[0];//my game id
    auto pos_role = Vec2(visibleSize.width / 3 * (float) (name - '0'), 560);
    auto role = Role::create("role1.png");
    role->setTag(name - '0');
    role->setPosition(pos_role);
    role->setMagicPos(Vec2(visibleSize.width / 3 * (float) (name - '0') + 70, 560));
    addChild(role);
    addChild(role->getNowMagic());

    auto pos_role2 = Vec2(visibleSize.width / 3 * (3 - (float) (name - '0')), 560);
    auto role2 = Role::create("role1.png");
    role2->setTag(3 - name + '0');
    role2->setPosition(pos_role2);
    role2->setMagicPos(Vec2(visibleSize.width / 3 * (3 - (float) (name - '0')) + 70, 560));
    addChild(role2);
    addChild(role2->getNowMagic());

    auto text_time = Text::create("30", "楷体", 40);
    text_time->setPosition(Vec2(850, 540));
    addChild(text_time);

    thread t1([=] {
        char s[100];
        char r[100];
        int l = sizeof(SOCKADDR);
        SOCKET skt = socket(AF_INET, SOCK_DGRAM, 0);
        SOCKADDR_IN addr;
        addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
        addr.sin_family = AF_INET;
        addr.sin_port = htons(6800);
        s[0] = 't';
        sendto(skt, s, sizeof(s), 0, (SOCKADDR *) &addr, l);
        recvfrom(skt, r, sizeof(r), 0, (SOCKADDR *) &addr, &l);
        while (true) {
            recvfrom(skt, r, sizeof(r), 0, (SOCKADDR *) &addr, &l);
            // printf("%s\n",r);
            auto command = split(r, " ");
            if ("p" == command[0]) {
                int who = atoi(command[1].c_str());
                int x = atoi(command[3].c_str());
                int y = atoi(command[4].c_str());
                if (who == role->getTag()) {
                    role->heave(Vec2((float) x, (float) y));
                } else if (who == role2->getTag()) {
                    role2->heave(Vec2((float) x, (float) y));
                }
            } else if ("m" == command[0]) {
                int who = atoi(command[1].c_str());
                int m = atoi(command[3].c_str());
                if (who == role->getTag()) {
                    if (!role->getMagic().empty())
                        role->conjure(role->getMagic().back());
                } else if (who == role2->getTag()) {
                    if (!role2->getMagic().empty())
                        role2->conjure(role2->getMagic().back());
                }
            }
        }
    });
    t1.detach();

    recvfrom(socketClient, recvBuf, sizeof(recvBuf), 0, (SOCKADDR *) &addrSrv, &len);//start
    schedule([=](float dt) {
                 if (getTime() > 0) {
                     setTime();
                     text_time->setString(std::to_string(getTime()));
                 } else {
                     if (!getEnd()) {
                         this->runAction(Sequence::create(CallFunc::create([=] {
                             text_time->setString(std::to_string(0));
                         }), CallFunc::create([=] {
                             Label *l = Label::createWithSystemFont("", "楷体", 72);
                             l->setPosition(visibleSize.width / 2, visibleSize.height / 2);
                             l->setColor(Color3B::RED);
                             l->enableBold();
                             addChild(l);
                             l->setVisible(false);
                             if (role->getScore() + role2->getScore() >= getTarget()) {
                                 l->setString("通关");
                                 ParticleSystemQuad *eh = ParticleSystemQuad::create("end_happy.plist");
                                 eh->setPosition(visibleSize / 2);
                                 // psd->setScale(0.4f);
                                 setEnd(true);
                                 addChild(eh);

                             } else {
                                 l->setString("失败");
                                 ParticleSystemQuad *es = ParticleSystemQuad::create("end_sad.plist");
                                 es->setPosition(visibleSize / 2);
                                 //psd->setScale(0.4f);
                                 setEnd(true);
                                 addChild(es);

                             }
                             role->getRing()->setColor(Color3B::BLUE);
                             role->getRing()->getPhysicsBody()->setVelocity(Vec2(0, 0));
                             role->getRing()->getPhysicsBody()->setCollisionBitmask(0);
                             role->getRing()->setTag(0);
                             role->getRing()->move(1);
                             role2->getRing()->setColor(Color3B::BLUE);
                             role2->getRing()->getPhysicsBody()->setVelocity(Vec2(0, 0));
                             role2->getRing()->getPhysicsBody()->setCollisionBitmask(0);
                             role2->getRing()->setTag(0);
                             role2->getRing()->move(1);
                             // Director::getInstance()->stopAnimation();
                         }), NULL));
                     }
                 }
             },
             1, "time over");

    EventListenerMouse *elm = EventListenerMouse::create();
    elm->onMouseUp = [=](EventMouse *event) {
        char send[100] = {0};
        if (getRunning() && !getEnd()) {
            if (event->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT) {
                auto pos_mouse = Vec2(event->getCursorX(), event->getCursorY());
                sprintf_s(send, "p %c 1 %d %d", name, (int) pos_mouse.x, (int) pos_mouse.y);
                sendto(socketClient, send, sizeof(send), 0, (SOCKADDR *) &addrSrv, len);
            } else if (event->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT) {
                sprintf_s(send, "m %c 2 %d", name, -1);
                sendto(socketClient, send, sizeof(send), 0, (SOCKADDR *) &addrSrv, len);
            }
        }
    };
    ed->addEventListenerWithSceneGraphPriority(elm, this);

    auto edge = Wall::create();
    edge->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 75);
    addChild(edge);

    auto wall = Wall::create("wall.png");
    wall->setRotation(20);
    wall->setPosition(visibleSize.width / 2, 40);
    addChild(wall);

    auto wall2 = Wall::create("wall.png");
    wall2->setRotation(-45);
    wall2->setPosition(500, 300);
    addChild(wall2);

    auto wall3 = Wall::create("wall.png");
    wall3->setPosition(600, 400);
    addChild(wall3);

    auto animal = Animal::create("mouse_r1.png");
    animal->setPosition(0, visibleSize.height / 2 - 40);
    addChild(animal);
    animal->act("mouse");

    auto animal2 = Animal::create("cow_r1.png");
    animal2->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 45);
    addChild(animal2);
    animal2->act("cow");
    animal2->setWeight(2);
    animal2->setValue(500);

    auto stone = Stone::create("stone1.png");
    stone->setPosition(visibleSize.width / 4, visibleSize.height / 6);
    stone->setValue(50);
    stone->setWeight(1.5);
    addChild(stone);

    auto stone2 = Stone::create("stone2.png");
    stone2->setPosition(visibleSize.width * 2 / 5, visibleSize.height / 9);
    stone2->setValue(75);
    stone2->setWeight(2.5);
    addChild(stone2);

    auto stone3 = Stone::create("stone3.png");
    stone3->setPosition(visibleSize.width * 3 / 4, visibleSize.height / 5);
    stone3->setValue(150);
    stone3->setWeight(5);
    addChild(stone3);

    auto diamond = Diamond::create("diamond1.png");
    diamond->setPosition(visibleSize.width / 4, visibleSize.height / 2);
    addChild(diamond);

    auto diamond2 = Diamond::create("diamond2.png");
    diamond2->setPosition(visibleSize.width / 4 - 120, visibleSize.height / 2 + 80);
    addChild(diamond2);

    auto diamond3 = Diamond::create("diamond3.png");
    diamond3->setPosition(visibleSize.width / 8, visibleSize.height / 2 - 200);
    addChild(diamond3);

    auto diamond4 = Diamond::create("diamond4.png");
    diamond4->setPosition(visibleSize.width / 4, visibleSize.height / 2 - 300);
    addChild(diamond4);

    auto tnt = TNT::create("TNT.png");
    tnt->setPosition(visibleSize.width / 4 + 40, visibleSize.height / 2 + 80);
    addChild(tnt);

    auto tnt2 = TNT::create("TNT.png");
    tnt2->setPosition(visibleSize.width / 4 + 40, visibleSize.height / 2 - 120);
    addChild(tnt2);

    auto tnt3 = TNT::create("TNT.png");
    tnt3->setPosition(visibleSize.width / 4 - 60, visibleSize.height / 2 + 80);
    addChild(tnt3);

    auto gift = Gift::create("gift.png");
    gift->setValue(-1);
    gift->setWeight(2);
    gift->setPosition(450, 200);
    addChild(gift);

    auto gift2 = Gift::create("gift.png");
    gift2->setValue(-2);
    gift2->setWeight(2);
    gift2->setPosition(700, 300);
    addChild(gift2);

    auto gift3 = Gift::create("gift.png");
    gift3->setValue(-3);
    gift3->setWeight(2);
    gift3->setPosition(680, 220);
    addChild(gift3);

    auto gift4 = Gift::create("gift.png");
    gift4->setValue(-4);
    gift4->setWeight(1);
    gift4->setPosition(100, 200);
    addChild(gift4);
}


void GameScene::setMode(bool i) {
    this->isSingle = i;
    if (isSingle)//offline mode
    {
        createOfflineScene();
    } else//online mode
    {
        createOnlineScene();
    }
}

bool GameScene::hit(PhysicsContact &contact) {

    auto tempa = (Sprite *) contact.getShapeA()->getBody()->getNode();
    auto tempb = (Sprite *) contact.getShapeB()->getBody()->getNode();
    tempa->retain();
    tempb->retain();
    if (tempa->getTag() == 1) {
        AudioEngine::play2d("music_duang.mp3");
        if (((Objects *) tempb)->isRemoveable()) {
            ((Ring *) tempa)->transformValue(((Objects *) tempb)->getValue());
            ((Objects *) tempb)->leave();
            ((Ring *) tempa)->setColor(Color3B::BLUE);
            ((Ring *) tempa)->getPhysicsBody()->setVelocity(Vec2(0, 0));
            ((Ring *) tempa)->getPhysicsBody()->setCollisionBitmask(0);
            ((Ring *) tempa)->setTag(0);
            ((Ring *) tempa)->move(((Objects *) tempb)->getWeight());
        }

    } else if (tempb->getTag() == 1) {
        AudioEngine::play2d("music_duang.mp3");
        if (((Objects *) tempa)->isRemoveable()) {
            ((Ring *) tempb)->transformValue(((Objects *) tempa)->getValue());
            ((Objects *) tempa)->leave();
            ((Ring *) tempa)->setColor(Color3B::BLUE);
            ((Ring *) tempb)->getPhysicsBody()->setVelocity(Vec2(0, 0));
            ((Ring *) tempb)->getPhysicsBody()->setCollisionBitmask(0);
            ((Ring *) tempb)->setTag(0);
            ((Ring *) tempb)->move(((Objects *) tempb)->getWeight());
        }
    } else if (tempa->getTag() == 2) {
        if (tempb->getTag() != 1 && tempb->getTag() != 0 && ((Objects *) tempb)->isRemoveable()) {
            ((Objects *) tempb)->leave();
        }

    } else if (tempb->getTag() == 2) {
        if (tempa->getTag() != 1 && tempa->getTag() != 0 && ((Objects *) tempa)->isRemoveable()) {
            ((Objects *) tempa)->leave();
        }

    }
    return true;
}

void GameScene::setMusic(int id, bool playing) {
    music_id = id;
    music_playing = playing;
}

int GameScene::getTime() const {
    return time;
}

void GameScene::setTime() {
    time--;
}

void GameScene::setEnd(bool i) {
    end = i;
}

bool GameScene::getEnd() const {
    return end;
}

void GameScene::setTarget(int i) {
    target = i;
}

int GameScene::getTarget() const {
    return target;
}

bool GameScene::getRunning() {
    return running;
}

void GameScene::setRunning(bool i) {
    running = i;
}


