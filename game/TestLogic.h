#pragma once
#include "CustomComponent.h"
#include "ComponentBuilder.h"
#include "ComponentManager.h"
#include "Timer.h"

jeBegin

class Object;
struct Telegram;

class TestLogic : public CustomComponent
{
    template <class T>
    friend class MemoryAllocator;

    friend class ComponentManager;
    friend class TestLogicBuilder;

public:

    Object * m_ortho, *m_pers;

private:

    std::vector <Object*> m_list;

    TestLogic(Object* pObject);
    ~TestLogic() {};

    static int a;

    void Register() override;
    void Load(CR_RJValue _data) override;
    void Init() override;
    void Update(const float _dt) override;
    void Close() override;
    void Unload() override;
    bool OnMessage(Telegram& msg) override { msg; return false; }

    void EditorUpdate(const float _dt) override;

    Timer t;
};

jeDeclareCustomComponentBuilder(TestLogic);
jeEnd
