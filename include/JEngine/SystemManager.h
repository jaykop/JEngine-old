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

    struct SystemBlock {
        SoundSystem		*m_pSoundSystem;
        GraphicSystem	*m_pGraphicSystem;
        PhysicsSystem	*m_pPhysicsSystem;
        BehaviorSystem	*m_pBehaviorSystem;
    };

    friend class State;
    friend class StateManager;

    using SystemStack = std::stack<SystemBlock*>;

public:

    static SystemBlock*		pBlock_;

	static SoundSystem*	    pSound_;
    static GraphicSystem*   pGraphic_;
    static PhysicsSystem*   pPhysics_;
    static BehaviorSystem*  pBehavior_;

private:

    static void Load(CR_RJDoc _data);
    static void Init();
    static void Update(float dt);
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
