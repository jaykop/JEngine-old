#pragma once
#include <stack>
#include "JsonParser.h"

jeBegin

class SoundSystem;
class GraphicSystem;
class PhysicsSystem;
class BehaviorSystem;

class SystemManager {

    // Locked constuctor, destructor, assign operator
    jeStaticClassDeclaration(SystemManager)

    class SystemBlock {

    friend class SystemManager;

    private:

        SystemBlock();
        ~SystemBlock();

        void Bind();
        void Unbind();

        void Load(CR_RJDoc _data);
        void Init();
        void Update(const float _dt);
        void Close();
        void Unload();

        SoundSystem		*m_pSoundSystem;
        GraphicSystem	*m_pGraphicSystem;
        PhysicsSystem	*m_pPhysicsSystem;
        BehaviorSystem	*m_pBehaviorSystem;

    private:

        SystemBlock(SystemBlock&&) = delete;
        SystemBlock(const SystemBlock&) = delete;
        SystemBlock& operator=(SystemBlock&&) = delete;
        SystemBlock& operator=(const SystemBlock&) = delete;

    };

    friend class State;
    friend class StateManager;

    using SystemStack = std::stack<SystemBlock*>;

public:

    static SystemBlock	    *m_systemBlock;

    static SoundSystem*	    GetSoundSystem();
    static GraphicSystem*   GetGraphicSystem();
    static PhysicsSystem*   GetPhysicsSystem();
    static BehaviorSystem*  GetBehaviorSystem();

private:

    static void Load(CR_RJDoc _data);
    static void Init();
    static void Update(const float _dt);
    static void Close();
    static void Unload();

    static void Pause();
    static void Resume();

    static void Bind();
    static void Unbind();

    static SystemStack	m_pauseStack;
};

using SYSTEM = SystemManager;

jeEnd
