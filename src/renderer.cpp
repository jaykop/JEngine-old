#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <renderer.hpp>
#include <shader.hpp>
#include <graphic_system.hpp>
#include <math_util.hpp>
#include <obj_loader.hpp>
#include <mesh.hpp>
#include <transform.hpp>

jeBegin

jeDefineComponentBuilder(Renderer);

using namespace Math;

bool Renderer::renderObj_ = true;
Renderer::RenderType Renderer::renderType_ = Renderer::RenderType::R_NONE;

void Renderer::add_to_system() {
	GraphicSystem::add_model(this);
}

void Renderer::remove_from_system() {
	GraphicSystem::remove_model(this);
}

void Renderer::load(const rapidjson::Value& data) {

}

Renderer::Renderer(Object* owner)
: Component(owner), h_(false), renderBoundary_(false),
	renderFaceNormals_(false), renderVertexNormals_(false) {}

void Renderer::set_mesh(const std::string& name)
{
	// get new mesh
	Mesh* newMesh = ObjLoader::get_mesh(name.c_str());

	// add to the mesh
	meshes_.emplace_back(newMesh);

	//if (pMesh_->key_ == std::string("bunny"))
	//	h_ = true;
	//else
	//	h_ = false;
}

const Renderer::Meshes& Renderer::get_meshes(void) const
{
	return meshes_;
}

void Renderer::draw(Shader* pShader)
{
	for (const auto& m : meshes_) {

		// Set uniform location 
		pShader->set_matrix("model", pTrans_->model_to_world());

		glBindVertexArray(m->vao_);
		glDrawElements(GL_TRIANGLES, m->get_indice_count(), GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Renderer::draw_lighting_effect(Light* pLight, Shader* pShader)
{
	//pShader->set_int("renderType", renderType_);

	///*Calculate the light max and set the radius for light volume optimization*/
	//// Calculate the light max
	//float aLightMax = std::fmaxf(
	//	std::fmaxf(pLight->ambientColor_.x, pLight->ambientColor_.y),
	//	pLight->ambientColor_.z);
	//float dLightMax = std::fmaxf(
	//	std::fmaxf(pLight->diffuseColor_.x, pLight->diffuseColor_.y),
	//	pLight->diffuseColor_.z);
	//float sLightMax = std::fmaxf(
	//	std::fmaxf(pLight->specularColor_.x, pLight->specularColor_.y),
	//	pLight->specularColor_.z);
	//float lightMax = std::fmaxf(
	//	std::fmaxf(aLightMax, dLightMax), sLightMax);

	//// Get radius
	//pLight->radius_ = (-Light::linear_ + std::sqrtf(Light::linear_ * Light::linear_
	//	- 4 * Light::quadratic_ * (Light::constant_ - (256.f / 5.f) * lightMax))) / 2 * Light::quadratic_;

	//// Update shader uniform info
	//pShader->set_int(pLight->modeStr_, int(pLight->mode_));
	//pShader->set_vec3(pLight->positionStr_, pLight->pTransform_->position);
	//pShader->set_vec3(pLight->directionStr_, pLight->direction_);
	//pShader->set_vec3(pLight->aColorStr_, pLight->ambientColor_);
	//pShader->set_vec3(pLight->sColorStr_, pLight->specularColor_);
	//pShader->set_vec3(pLight->dColorStr_, pLight->diffuseColor_);
	//pShader->set_float(pLight->fallOffStr_, pLight->fallOff_);
	//pShader->set_float(pLight->radiusStr_, pLight->radius_);
	//pShader->set_float(pLight->innerAngleStr_, pLight->innerAngle_ * deg_to_rad);
	//pShader->set_float(pLight->outerAngleStr_, pLight->outerAngle_ * deg_to_rad);

}

void Renderer::draw_normals(Shader* pShader)
{
	// Rendering normals
	if (renderVertexNormals_
		|| renderFaceNormals_) {

		for (const auto& m : meshes_) {

			// Set uniform location 
			pShader->set_matrix("model", pTrans_->model_to_world());
			pShader->set_bool("uniformColor", false);

			if (renderVertexNormals_) {
				glBindVertexArray(m->vnVao_);
				glDrawArrays(GL_LINES, 0, GLsizei(m->vertexNormalsDraw.size()));
			}

			if (renderFaceNormals_) {
				glBindVertexArray(m->fnVao_);
				glDrawArrays(GL_LINES, 0, GLsizei(m->faceNormalsDraw.size()));
			}

			// Unbind the vao
			glBindVertexArray(0);
		}
	}
}

void Renderer::draw_quad(unsigned& vao)
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::draw_debug_info(Shader* pShader)
{
	//pDDrawer_->render_lines(pShader, pTrans_->model_to_world());
	//pDDrawer_->render_meshes(pShader);
}

//
//void Renderer::GenerateBV(void)
//{
//	auto points = pMesh_->GetPoints();
//	pDDrawer_->clear();
//
//	switch (Mesh::bvType_)
//	{
//	case Mesh::BVType::BV_AABB:
//		aabb_ = AABB::Generate(points);
//		pDDrawer_->AddCube(aabb_, yellow);
//		break;
//	case Mesh::BVType::BV_SPHERE_CENTEROID:
//		sphere_ = BoundingSphere::GenerateCentroid(points);
//		pDDrawer_->AddSphere(sphere_.Transformed(*pTrans_), yellow);
//		break;
//	case Mesh::BVType::BV_SPHERE_RITTER:
//		sphere_ = BoundingSphere::GenerateRitter(points);
//		pDDrawer_->AddSphere(sphere_.Transformed(*pTrans_), yellow);
//		break;
//	case Mesh::BVType::BV_SPHERE_LARSSON:
//		sphere_ = BoundingSphere::GenerateLarsson(points);
//		pDDrawer_->AddSphere(sphere_.Transformed(*pTrans_), yellow);
//		break;
//	case Mesh::BVType::BV_SPHERE_PCA:
//		sphere_ = BoundingSphere::GeneratePCA(points);
//		pDDrawer_->AddSphere(sphere_.Transformed(*pTrans_), yellow);
//		break;
//	case Mesh::BVType::BV_ELLIPSOID_PCA:
//		ellipse_ = BoundingEllipsoid::Generate(points);
//		pDDrawer_->AddEllipse(ellipse_.Transformed(*pTrans_), h_, yellow);
//		break;
//	case Mesh::BVType::BV_OBB:
//		obb_ = OBB::Generate(points);
//		pDDrawer_->AddCube(obb_, yellow);
//		break;
//
//	case Mesh::BVType::BV_NONE:
//	default:
//		break;
//	}
//}


jeEnd