#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <functional>
#include <vector>
#include <string>

using namespace geode::prelude;

namespace EmirGui {

    // ========== TEMA RENKLERİ ==========
    struct Theme {
        static ccColor4F primary()    { return ccc4f(0.2f, 0.5f, 0.9f, 1.0f); }
        static ccColor4F primaryDark(){ return ccc4f(0.1f, 0.3f, 0.7f, 1.0f); }
        static ccColor4F success()    { return ccc4f(0.2f, 0.8f, 0.3f, 1.0f); }
        static ccColor4F danger()     { return ccc4f(0.9f, 0.2f, 0.2f, 1.0f); }
        static ccColor4F warning()    { return ccc4f(0.9f, 0.7f, 0.1f, 1.0f); }
        static ccColor4F bg()         { return ccc4f(0.1f, 0.1f, 0.15f, 0.95f); }
        static ccColor4F border()     { return ccc4f(0.3f, 0.3f, 0.4f, 1.0f); }
        static ccColor4F text()       { return ccc4f(1.0f, 1.0f, 1.0f, 1.0f); }
        static ccColor4F textDim()    { return ccc4f(0.7f, 0.7f, 0.8f, 1.0f); }
    };

    // ========== YARDIMCI ==========
    inline float clampVal(float val, float min, float max) {
        if (val < min) return min;
        if (val > max) return max;
        return val;
    }

    // ========== LABEL ==========
    class Label : public CCLabelBMFont {
    public:
        static Label* create(const std::string& text, float scale = 0.5f, const char* font = "bigFont.fnt") {
            auto ret = new Label();
            if (ret && ret->initWithString(text.c_str(), font)) {
                ret->setScale(scale);
                ret->autorelease();
                return ret;
            }
            CC_SAFE_DELETE(ret);
            return nullptr;
        }
        void setText(const std::string& text) { setString(text.c_str()); }
    };

    // ========== BUTON ==========
    inline CCMenuItemSpriteExtra* createButton(
        const std::string& text,
        CCObject* target,
        SEL_MenuHandler callback,
        float scale = 0.8f
    ) {
        auto spr = ButtonSprite::create(text.c_str(), "bigFont.fnt", "GJ_button_01.png");
        spr->setScale(scale);
        return CCMenuItemSpriteExtra::create(spr, target, callback);
    }

    // ========== DAİRE BUTON ==========
    inline CCMenuItemSpriteExtra* createCircleButton(
        const std::string& sprName,
        CCObject* target,
        SEL_MenuHandler callback
    ) {
        auto spr = CircleButtonSprite::createWithSpriteFrameName(sprName.c_str());
        return CCMenuItemSpriteExtra::create(spr, target, callback);
    }

    // ========== TOGGLE ==========
    inline CCMenuItemToggler* createToggle(
        const std::string& textOn,
        const std::string& textOff,
        CCObject* target,
        SEL_MenuHandler callback
    ) {
        auto sprOn  = ButtonSprite::create(textOn.c_str(),  "bigFont.fnt", "GJ_button_02.png");
        auto sprOff = ButtonSprite::create(textOff.c_str(), "bigFont.fnt", "GJ_button_04.png");
        return CCMenuItemToggler::create(sprOff, sprOn, target, callback);
    }

    // ========== CHECKBOX ==========
    class Checkbox : public CCMenuItemToggler {
    protected:
        std::function<void(bool)> m_callback;
        bool m_isChecked = false;

    public:
        static Checkbox* create(bool initial, std::function<void(bool)> callback) {
            auto on = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
            auto off = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
            auto ret = new Checkbox();
            if (ret && ret->initWithTarget(ret, menu_selector(Checkbox::onToggle), off, on)) {
                ret->m_callback = callback;
                ret->toggle(initial);
                ret->m_isChecked = initial;
                ret->autorelease();
                return ret;
            }
            CC_SAFE_DELETE(ret);
            return nullptr;
        }

        void onToggle(CCObject*) {
            toggle(!m_isChecked);
            m_isChecked = !m_isChecked;
            if (m_callback) m_callback(m_isChecked);
        }

        void setChecked(bool checked) {
            if (m_isChecked != checked) {
                toggle(checked);
                m_isChecked = checked;
                if (m_callback) m_callback(m_isChecked);
            }
        }

        bool isChecked() const { return m_isChecked; }
    };

    // ========== TEXTBOX ==========
    class TextBox : public TextInput {
    protected:
        CCLayerColor* m_bg;
        std::function<void(const std::string&)> m_callback;

    public:
        static TextBox* create(const CCSize& size, const std::string& placeholder = "") {
            auto ret = new TextBox();
            if (ret && ret->init(size.width, size.height, placeholder.c_str())) {
                ret->setCommonAnchors(Anchor::Left, Anchor::Top);
                
                ret->m_bg = CCLayerColor::create(ccc4(30, 30, 40, 200));
                ret->m_bg->setContentSize(size);
                ret->m_bg->setPosition(0, 0);
                ret->addChild(ret->m_bg, -1);
                
                ret->setScale(0.8f);
                ret->autorelease();
                return ret;
            }
            CC_SAFE_DELETE(ret);
            return nullptr;
        }

        void setCallback(std::function<void(const std::string&)> cb) { m_callback = cb; }
        
        virtual void onConfirm(CCObject*) override {
            TextInput::onConfirm(nullptr);
            if (m_callback) m_callback(getString());
        }
    };

    // ========== SLIDER (CCTouchDelegate kullanmadan) ==========
    class Slider : public CCNode {
    protected:
        float m_value = 0.5f;
        float m_min = 0.0f, m_max = 1.0f;
        CCSprite* m_thumb;
        CCLayerColor* m_track;
        CCLayerColor* m_fill;
        std::function<void(float)> m_callback;
        bool m_dragging = false;
        float m_trackWidth = 0;
        CCTouch* m_activeTouch = nullptr;

    public:
        static Slider* create(float min, float max, float initial, const CCSize& size, std::function<void(float)> callback) {
            auto ret = new Slider();
            if (ret && ret->init()) {
                ret->m_min = min;
                ret->m_max = max;
                ret->m_value = initial;
                ret->m_callback = callback;
                ret->setContentSize(size);
                ret->m_trackWidth = size.width - 20;
                
                ret->m_track = CCLayerColor::create(ccc4(80, 80, 100, 255));
                ret->m_track->setContentSize(CCSize(size.width - 20, 4));
                ret->m_track->setPosition(10, size.height / 2 - 2);
                ret->addChild(ret->m_track);
                
                ret->m_fill = CCLayerColor::create(ccc4(80, 180, 255, 255));
                ret->m_fill->setContentSize(CCSize(0, 4));
                ret->m_fill->setPosition(10, size.height / 2 - 2);
                ret->addChild(ret->m_fill);
                
                ret->m_thumb = CCSprite::createWithSpriteFrameName("GJ_sliderKnob_001.png");
                ret->m_thumb->setScale(0.7f);
                ret->m_thumb->setPosition(10, size.height / 2);
                ret->addChild(ret->m_thumb);
                
                ret->setValue(initial);
                ret->setTouchEnabled(true);
                ret->autorelease();
                return ret;
            }
            CC_SAFE_DELETE(ret);
            return nullptr;
        }
        
        void setValue(float val) {
            m_value = clampVal(val, m_min, m_max);
            float percent = (m_value - m_min) / (m_max - m_min);
            float thumbX = 10 + percent * m_trackWidth;
            m_thumb->setPositionX(thumbX);
            m_fill->setContentSize(CCSize(percent * m_trackWidth, 4));
            if (m_callback) m_callback(m_value);
        }
        
        float getValue() const { return m_value; }
        
        virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event) override {
            CCPoint pos = convertToNodeSpace(touch->getLocation());
            if (pos.x >= 0 && pos.x <= m_trackWidth + 20) {
                m_dragging = true;
                m_activeTouch = touch;
                setValueFromPos(pos.x);
                return true;
            }
            return false;
        }
        
        virtual void ccTouchMoved(CCTouch* touch, CCEvent* event) override {
            if (m_dragging && touch == m_activeTouch) {
                CCPoint pos = convertToNodeSpace(touch->getLocation());
                setValueFromPos(pos.x);
            }
        }
        
        virtual void ccTouchEnded(CCTouch* touch, CCEvent* event) override {
            if (touch == m_activeTouch) {
                m_dragging = false;
                m_activeTouch = nullptr;
            }
        }
        
        virtual void ccTouchCancelled(CCTouch* touch, CCEvent* event) override {
            if (touch == m_activeTouch) {
                m_dragging = false;
                m_activeTouch = nullptr;
            }
        }
        
        void setValueFromPos(float x) {
            x = clampVal(x, 10.0f, m_trackWidth + 10);
            float percent = (x - 10) / m_trackWidth;
            setValue(m_min + percent * (m_max - m_min));
        }
        
        virtual void onEnter() override {
            CCNode::onEnter();
            setTouchEnabled(true);
        }
        
        virtual void onExit() override {
            CCNode::onExit();
            setTouchEnabled(false);
        }
    };

    // ========== PROGRESS BAR ==========
    class ProgressBar : public CCNode {
    protected:
        CCLayerColor* m_bg;
        CCLayerColor* m_fill;
        float m_progress = 0.0f;
        
    public:
        static ProgressBar* create(const CCSize& size, ccColor4F fillColor = Theme::success()) {
            auto ret = new ProgressBar();
            if (ret && ret->init()) {
                ret->setContentSize(size);
                ret->m_bg = CCLayerColor::create(ccc4(40, 40, 50, 200));
                ret->m_bg->setContentSize(size);
                ret->addChild(ret->m_bg);
                
                ret->m_fill = CCLayerColor::create(ccc4(fillColor.r * 255, fillColor.g * 255, fillColor.b * 255, 255));
                ret->m_fill->setContentSize(CCSize(0, size.height));
                ret->addChild(ret->m_fill);
                ret->autorelease();
                return ret;
            }
            CC_SAFE_DELETE(ret);
            return nullptr;
        }
        
        void setProgress(float p) {
            m_progress = clampVal(p, 0.0f, 1.0f);
            m_fill->setContentSize(CCSize(m_progress * getContentSize().width, getContentSize().height));
        }
        
        float getProgress() const { return m_progress; }
    };

    // ========== DROPDOWN ==========
    class Dropdown : public CCMenu {
    protected:
        std::vector<std::string> m_items;
        int m_selected = 0;
        CCMenuItemSpriteExtra* m_selectedBtn;
        CCMenu* m_dropdownMenu;
        bool m_expanded = false;
        std::function<void(int, const std::string&)> m_callback;
        
    public:
        static Dropdown* create(const std::vector<std::string>& items, int defaultIdx, std::function<void(int, const std::string&)> callback) {
            auto ret = new Dropdown();
            if (ret && ret->init()) {
                ret->m_items = items;
                ret->m_selected = defaultIdx;
                ret->m_callback = callback;
                ret->setupUI();
                ret->autorelease();
                return ret;
            }
            CC_SAFE_DELETE(ret);
            return nullptr;
        }
        
        void setupUI() {
            auto mainSpr = ButtonSprite::create(m_items[m_selected].c_str(), "bigFont.fnt", "GJ_button_01.png");
            m_selectedBtn = CCMenuItemSpriteExtra::create(mainSpr, this, menu_selector(Dropdown::toggle));
            m_selectedBtn->setPosition(0, 0);
            this->addChild(m_selectedBtn);
            setContentSize(mainSpr->getContentSize());
            
            m_dropdownMenu = CCMenu::create();
            m_dropdownMenu->setPosition(0, -getContentSize().height);
            m_dropdownMenu->setVisible(false);
            this->addChild(m_dropdownMenu);
            
            for (int i = 0; i < m_items.size(); i++) {
                auto spr = ButtonSprite::create(m_items[i].c_str(), "bigFont.fnt", "GJ_button_04.png");
                auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(Dropdown::select));
                btn->setTag(i);
                btn->setPosition(0, -i * 35);
                m_dropdownMenu->addChild(btn);
            }
        }
        
        void toggle(CCObject*) {
            m_expanded = !m_expanded;
            m_dropdownMenu->setVisible(m_expanded);
        }
        
        void select(CCObject* sender) {
            int idx = sender->getTag();
            m_selected = idx;
            auto newSpr = ButtonSprite::create(m_items[idx].c_str(), "bigFont.fnt", "GJ_button_01.png");
            m_selectedBtn->setNormalImage(newSpr);
            toggle(nullptr);
            if (m_callback) m_callback(idx, m_items[idx]);
        }
        
        int getSelected() const { return m_selected; }
    };

    // ========== WINDOW ==========
    class Window : public CCNode {
    protected:
        CCLayerColor* m_background;
        CCNode* m_content;
        CCPoint m_dragStart;
        bool m_dragging = false;
        std::string m_title;
        CCSize m_size;
        CCTouch* m_activeTouch = nullptr;
        
    public:
        static Window* create(const CCSize& size, const std::string& title) {
            auto ret = new Window();
            if (ret && ret->init()) {
                ret->m_size = size;
                ret->m_title = title;
                ret->setupUI();
                ret->setTouchEnabled(true);
                ret->autorelease();
                return ret;
            }
            CC_SAFE_DELETE(ret);
            return nullptr;
        }
        
        void setupUI() {
            setContentSize(m_size);
            
            m_background = CCLayerColor::create(ccc4(Theme::bg().r * 255, Theme::bg().g * 255, Theme::bg().b * 255, 230));
            m_background->setContentSize(m_size);
            m_background->setPosition(0, 0);
            addChild(m_background);
            
            auto border = CCDrawNode::create();
            border->drawRect(CCPoint(0, 0), CCPoint(m_size.width, m_size.height), Theme::border(), 1.5f, Theme::border());
            addChild(border);
            
            auto titleBar = CCLayerColor::create(ccc4(Theme::primaryDark().r * 255, Theme::primaryDark().g * 255, Theme::primaryDark().b * 255, 255));
            titleBar->setContentSize(CCSize(m_size.width, 30));
            titleBar->setPosition(0, m_size.height - 30);
            addChild(titleBar);
            
            auto titleLbl = Label::create(m_title, 0.5f);
            titleLbl->setPosition(m_size.width / 2, m_size.height - 15);
            titleLbl->setAnchorPoint(ccp(0.5f, 0.5f));
            addChild(titleLbl);
            
            auto closeSpr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
            closeSpr->setScale(0.7f);
            auto closeBtn = CCMenuItemSpriteExtra::create(closeSpr, this, menu_selector(Window::close));
            closeBtn->setPosition(m_size.width - 20, m_size.height - 15);
            auto menu = CCMenu::create();
            menu->addChild(closeBtn);
            menu->setPosition(0, 0);
            addChild(menu);
        }
        
        void setContent(CCNode* content) {
            if (m_content) m_content->removeFromParent();
            m_content = content;
            content->setPosition(10, m_size.height - 40);
            addChild(content);
        }
        
        virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event) override {
            CCPoint pos = convertToNodeSpace(touch->getLocation());
            if (pos.y >= m_size.height - 30 && pos.y <= m_size.height) {
                m_dragging = true;
                m_activeTouch = touch;
                m_dragStart = touch->getLocation();
                return true;
            }
            return false;
        }
        
        virtual void ccTouchMoved(CCTouch* touch, CCEvent* event) override {
            if (m_dragging && touch == m_activeTouch) {
                CCPoint delta = ccpSub(touch->getLocation(), m_dragStart);
                setPosition(ccpAdd(getPosition(), delta));
                m_dragStart = touch->getLocation();
            }
        }
        
        virtual void ccTouchEnded(CCTouch* touch, CCEvent* event) override {
            if (touch == m_activeTouch) {
                m_dragging = false;
                m_activeTouch = nullptr;
            }
        }
        
        virtual void ccTouchCancelled(CCTouch* touch, CCEvent* event) override {
            if (touch == m_activeTouch) {
                m_dragging = false;
                m_activeTouch = nullptr;
            }
        }
        
        void close(CCObject*) {
            removeFromParent();
        }
    };

} // namespace EmirGui
