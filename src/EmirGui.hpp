#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace EmirGui {
    class DraggablePanel : public CCNode, public CCTouchDelegate {
    protected:
        bool m_isDragging = false;
        CCPoint m_dragOffset;
        CCScale9Sprite* m_bg;

    public:
        static DraggablePanel* create(float w, float h) {
            auto ret = new DraggablePanel();
            if (ret && ret->init(w, h)) {
                ret->autorelease();
                return ret;
            }
            CC_SAFE_DELETE(ret);
            return nullptr;
        }

        bool init(float w, float h) {
            this->setContentSize({w, h});
            m_bg = CCScale9Sprite::create("GJ_square01.png");
            m_bg->setContentSize({w, h});
            m_bg->setAnchorPoint({0, 0});
            this->addChild(m_bg);
            return true;
        }

        void onEnter() override {
            CCNode::onEnter();
            CCDirector::get()->getTouchDispatcher()->addTargetedDelegate(this, -500, true);
        }

        void onExit() override {
            CCDirector::get()->getTouchDispatcher()->removeDelegate(this);
            CCNode::onExit();
        }

        bool ccTouchBegan(CCTouch* touch, CCEvent* event) override {
            CCPoint pos = this->convertTouchToNodeSpace(touch);
            CCRect rect = {0, 0, this->getContentSize().width, this->getContentSize().height};
            if (rect.containsPoint(pos)) {
                m_isDragging = true;
                m_dragOffset = this->getPosition() - touch->getLocation();
                return true;
            }
            return false;
        }

        void ccTouchMoved(CCTouch* touch, CCEvent* event) override {
            if (m_isDragging) this->setPosition(touch->getLocation() + m_dragOffset);
        }

        void ccTouchEnded(CCTouch* touch, CCEvent* event) override {
            m_isDragging = false;
        }
    };
}
