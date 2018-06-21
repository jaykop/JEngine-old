//#include <thread>
#include "TestLogic.h"
#include "CustomLogicHeader.h"

jeBegin
jeDefineCustomComponentBuilder(TestLogic);

int TestLogic::a = 0;

TestLogic::TestLogic(Object* _pObject)
    :CustomComponent(_pObject)
{}

void TestLogic::Register()
{
    SYSTEM::GetBehaviorSystem()->AddBehavior(this);
}

void TestLogic::Load(CR_RJValue /*_data*/)
{}

void TestLogic::Init()
{
    //std::thread a(&threadTest, 10);

    //if (a.joinable())
    //	a.join();
    //float sum = 0.f;
    //for (int i = 0; i < 10; i++) {
    //    t.Start();

    //    for (int index = 0; index < 10000; index++)
    //    {
    //        FACTORY::CreateObject(std::to_string(index).c_str());
    //        FACTORY::AddCreatedObject();
    //        m_list.push_back(FACTORY::GetCreatedObject());
    //    }

    //    for (auto obj : m_list) 
    //        CONTAINER->RemoveObject(obj);
    //    
    //    sum += t.GetTime();
    //    m_list.clear();
    //}
    //jeDebugPrint("Total: %f\nAverage:%f\n", sum, sum/10.f);

    //unsigned a = sizeof(std::string);

	camera = CONTAINER->GetObject("Camera")->GetComponent<Camera>();
	pointer = CONTAINER->GetObject("MousePointer")->GetComponent<Transform>();
    /*m_ortho = CONTAINER->GetObject("OrthogonalModel");
    m_pers = CONTAINER->GetObject("PerspectiveModel");*/
    //CONTAINER->GET_COMPONENT("Model", Model);
    //CONTAINER->GET_COMPONENT(5, Model);

    //std::vector<A*> b;
    //b.push_back(&A);

    //int* c = b[0];
}

void TestLogic::Update(const float _dt)
{
	if (INPUT::KeyPressed(JE_DOWN))
		camera->zoom -= _dt;
	if (INPUT::KeyPressed(JE_UP))
		camera->zoom += _dt;

	//float aspect = SYSTEM::GetGraphicSystem()->aspect;
	//float zNear = SYSTEM::GetGraphicSystem()->zNear;
	//float zFar = SYSTEM::GetGraphicSystem()->zFar;

	//vec4 ray(INPUT::GetOrhtoPosition().x, INPUT::GetOrhtoPosition().y, INPUT::GetOrhtoPosition().z, 1.f);
	//Camera *pMain = SYSTEM::GetGraphicSystem()->GetMainCamera();
	//ray = ray * Math::GetInverse(Math::Perspective(pMain->zoom, aspect, zNear, zFar));
	//ray = ray * Math::GetInverse(Math::LookAt(pMain->position, pMain->target, pMain->up));

	//std::cout << ray << std::endl;
	//vec3 ray3(ray.x, ray.y, 1.f);
	//pointer->position.Set(ray3);
}

void TestLogic::Close()
{}

void TestLogic::Unload()
{}

void TestLogic::EditorUpdate(const float /*_dt*/)
{
    // TODO
}

jeEnd
