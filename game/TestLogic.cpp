//#include <thread>
#include "TestLogic.h"
#include "CustomLogicHeader.h"
#include "MemoryAllocator.h"

jeBegin
jeDefineCustomComponentBuilder(TestLogic);

int TestLogic::a = 0;

TestLogic::TestLogic(Object* _pObject)
	:CustomComponent(_pObject)
{}

void threadTest(int a)
{
	for (int index = 0; index < a; ++index)
		printf("%d ", index);
}

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

	m_ortho = CONTAINER->GetObject("OrthogonalSprite");
	m_pers = CONTAINER->GetObject("PerspectiveModel");
	//CONTAINER->GET_COMPONENT("Model", Sprite);
	//CONTAINER->GET_COMPONENT(5, Sprite);

	//std::vector<A*> b;
	//b.push_back(&A);

	//int* c = b[0];
}

void TestLogic::Update(const float /*_dt*/)
{
	//if (INPUT::KeyPressed(JE_MOUSE_LEFT)) 
	//	SYSTEM::GetGraphicSystem()->Ray(m_pers->GetComponent<Model>(), m_pers->GetComponent<Transform>());

	//std::cout << INPUT::GetOrhtoPosition() << std::endl;
	//std::cout << INPUT::GetPerspPosition() << std::endl;

	//mat4 viewProjectionMatrix 
	//	= mat4::LookAt(SYSTEM::GetGraphicSystem()->GetMainCamera()->m_position,
	//		SYSTEM::GetGraphicSystem()->GetMainCamera()->m_target,
	//		SYSTEM::GetGraphicSystem()->GetMainCamera()->m_up).Transpose()
	//	* mat4::Perspective(SYSTEM::GetGraphicSystem()->m_fovy,
	//		SYSTEM::GetGraphicSystem()->m_aspect,
	//		SYSTEM::GetGraphicSystem()->m_zNear,
	//		SYSTEM::GetGraphicSystem()->m_zFar).Transpose();
	//vec4 temp(10, 10, 0, 1);
	//
	//temp = temp * viewProjectionMatrix;
	//
	////transform world to clipping coordinates
	////point3D = viewProjectionMatrix.multiply(point3D);
	//int winX = int(((temp.x + 1) * .5) * SYSTEM::GetGraphicSystem()->GetWidth()); 
	////(int)Math.round(((point3D.getX() + 1) / 2.0) *	width);
	////we calculate -point3D.getY() because the screen Y axis is
	////oriented top->down 
	//int winY = int(((temp.y + 1) * .5) * SYSTEM::GetGraphicSystem()->GetHeight());
	////int winY = (int)Math.round(((1 - point3D.getY()) / 2.0) *height);
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
