#include <obj_loader.hpp>
#include <fstream>
#include <sstream>
#include <mesh.hpp>

jeBegin

static const float normScale = 0.05f;
static const unsigned max_unsinged = (std::numeric_limits<unsigned>::max)();

std::string ObjLoader::key;
MeshList ObjLoader::meshList;
vec3 ObjLoader::maxPoint, ObjLoader::minPoint;

bool ObjLoader::load(const char* path)
{
	std::string parsed_key = parse_name(path);

	if (meshList.find(parsed_key) == meshList.end()) {

		std::ifstream obj(path, std::ios::in);
		std::stringstream buffer;
		buffer << obj.rdbuf();

		// Check if obj file is valid 
		if (!obj) return false;

		Mesh* newMesh = new Mesh;
		newMesh->setNormals_ = true;

		newMesh->key_ = parsed_key;

		// Initialize checkers
		minPoint.set(max_f), maxPoint.set(min_f);

		parse_vertex(buffer.str(), &newMesh);
		convert_mesh(&newMesh);
		newMesh->hEdgeMesh_ = new HalfEdgeMesh(newMesh->vertex_, newMesh->indices_);
		calculate_normals(&newMesh);
		Mesh::DescribeMeshAttrib(newMesh);

		meshList.insert({ parsed_key, newMesh });
	}

	key = parsed_key;
	return true;
}

Mesh* ObjLoader::get_mesh(const char* name)
{
	auto found = meshList.find(name);
	if (found != meshList.end())
		return found->second;

	return nullptr;
}

std::string ObjLoader::parse_name(const char* name)
{
	std::string key;

	for (int i = 0; name[i] != '\0'; i++) {

		key += name[i];

		if (name[i] == '/')
			key.clear();
	}

	key.erase(key.size() - 4);

	return key;
}

void ObjLoader::clear()
{
	for (auto& m : meshList) {
		delete m.second;
		m.second = nullptr;
	}
}

void ObjLoader::update_max_min(const vec3& v)
{
	if (v.x < minPoint.x)
		minPoint.x = v.x;
	else
		maxPoint.x = v.x;

	if (v.y < minPoint.y)
		minPoint.y = v.y;
	else
		maxPoint.y = v.y;

	if (v.z < minPoint.z)
		minPoint.z = v.z;
	else
		maxPoint.z = v.z;
}

void ObjLoader::convert_mesh(Mesh** pMesh)
{
	// Assign the min and max
	(*pMesh)->min_ = minPoint;
	(*pMesh)->max_ = maxPoint;

	vec3 sum = { (*pMesh)->max_.x - (*pMesh)->min_.x,
		(*pMesh)->max_.y - (*pMesh)->min_.y,
		(*pMesh)->max_.z - (*pMesh)->min_.z };

	sum *= .5f;

	if ((*pMesh)->absMax_ < sum.x)
		(*pMesh)->absMax_ = sum.x;
	if ((*pMesh)->absMax_ < sum.y)
		(*pMesh)->absMax_ = sum.y;
	if ((*pMesh)->absMax_ < sum.z)
		(*pMesh)->absMax_ = sum.z;

	(*pMesh)->max_ /= (*pMesh)->absMax_;
	(*pMesh)->min_ /= (*pMesh)->absMax_;
	(*pMesh)->centerOffset_ = (((*pMesh)->max_ + (*pMesh)->min_) * .5f);
	(*pMesh)->min_ -= (*pMesh)->centerOffset_;
	(*pMesh)->max_ -= (*pMesh)->centerOffset_;

	unsigned size = unsigned((*pMesh)->points_.size());
	vec3 centerOffset = (*pMesh)->centerOffset_;
	float absMax = (*pMesh)->absMax_;

	// Set vertex container
	for (unsigned i = 0; i < size; i++) {

		// convert normal
		vec3 convertedPos = get_converted_position((*pMesh)->points_[i],
			centerOffset, absMax);

		// Normal vertexes
		(*pMesh)->vertex_.push_back(Vertex{ convertedPos, vec3::zero , vec3::zero });
		(*pMesh)->vPoints_.push_back(convertedPos);
	}
}

void ObjLoader::parse_vertex(const std::string& data, Mesh** pMesh)
{
	// skip any leading white space
	unsigned it = unsigned(data.find_first_not_of("\n\r\0 "));
	unsigned size = unsigned((*pMesh)->points_.size());

	while (it != max_unsinged)
	{
		// extract vertex data
		if (data[it] == 'v')
			read_vertex(data, it + 1, (*pMesh)->points_);

		// extract face data
		if (data[it] == 'f')
			read_face(data, it + 1, (*pMesh)->indices_, size);

		// skip to next line
		it = unsigned(data.find_first_of("\n\r\0 ", it));
		it = unsigned(data.find_first_not_of("\n\r\0 ", it));
	}
}

void ObjLoader::read_vertex(const std::string& file_data, unsigned pos,
	std::vector<vec3>& points)
{
	vec3 p;
	const char* c_data = file_data.c_str();

	// Read vertex positions
	pos = get_next_elements(file_data, pos);
	p.x = static_cast<float>(atof(c_data + pos));

	pos = get_next_elements(file_data, pos);
	p.y = static_cast<float>(atof(c_data + pos));

	pos = get_next_elements(file_data, pos);
	p.z = static_cast<float>(atof(c_data + pos));

	// Check absolute max value from vertex
	update_max_min(p);

	points.push_back(p);
}

void ObjLoader::read_face(const std::string& file_data, unsigned pos,
	std::vector<unsigned>& indice, unsigned vertice_size)
{
	const char* c_data = file_data.c_str();

	// read face indices
	pos = get_next_elements(file_data, pos);
	indice.push_back(read_index(c_data + pos, vertice_size));

	pos = get_next_elements(file_data, pos);
	indice.push_back(read_index(c_data + pos, vertice_size));

	pos = get_next_elements(file_data, pos);
	indice.push_back(read_index(c_data + pos, vertice_size));
}

unsigned ObjLoader::read_index(const char* data, unsigned vertice_size)
{
	int index = atoi(data);

	// wrap index if needed
	if (index < 0)
		return unsigned(index + vertice_size);

	return unsigned(index - 1);
}

unsigned ObjLoader::get_next_elements(const std::string& file_data, unsigned pos)
{
	// skip past current element
	pos = unsigned(file_data.find_first_of(" ", pos));

	// skip past white space
	return unsigned(file_data.find_first_not_of(" ", pos));
}

void ObjLoader::calculate_normals(Mesh** pMesh)
{
	std::vector<Vertex>& vertices = (*pMesh)->vertex_;
	std::vector<Vertex>& fNormals = (*pMesh)->faceNormalsDraw_;
	std::vector<Vertex>& vNormals = (*pMesh)->vertexNormalsDraw_;
	HalfEdgeMesh* h_mesh = (*pMesh)->hEdgeMesh_;

	const unsigned num_vertices = unsigned(vertices.size());
	for (unsigned i = 0; i < num_vertices; ++i)
	{
		Vertex& vert = vertices[i];

		// find surrounding faces
		auto neighbors = h_mesh->GetNeighborFaces(i);

		for (const Face* f : neighbors)
		{
			// get vertex positions of face
			vec3 p1 = f->edge->vertex->pos;
			vec3 p2 = f->edge->next->vertex->pos;
			vec3 p3 = f->edge->Prev()->vertex->pos;

			// calculate face normal
			vec3 face_normal = vec3::cross(p2 - p1, p3 - p2);

			float len = face_normal.length();
			if (len > 0.0f)
			{
				// add to vertex normal
				vert.normal += face_normal.normalized();
			}
		}

		// re-normalize
		float len = vert.normal.length();
		if (len > 0.0f) {
			vert.normal /= len;

			vNormals.push_back(Vertex{ vertices[i].position, vec3::zero, blue });
			vNormals.push_back(Vertex{ vertices[i].position + vert.normal * normScale,
				vec3::zero, blue });
		}
	}

	vec3 centerOffset = (*pMesh)->centerOffset_;
	float absMax = (*pMesh)->absMax_;

	auto faces = h_mesh->GetFaces();

	for (auto& f : faces)
	{
		// get vertex positions of face
		vec3 p1 = f.edge->vertex->pos;
		vec3 p2 = f.edge->next->vertex->pos;
		vec3 p3 = f.edge->Prev()->vertex->pos;

		// calculate face normal
		vec3 face_normal = vec3::cross(p2 - p1, p3 - p2);
		vec3 center((p1 + p2 + p3) / 3.f);

		vec3 c_center = get_converted_position(center,
			centerOffset, absMax);

		vec3 c_fnorm = get_converted_position(face_normal,
			centerOffset, absMax);

		fNormals.push_back(Vertex{ center, vec3::zero, red });
		fNormals.push_back(Vertex{ center + face_normal.normalized() * normScale , vec3::zero, red });
	}
}

vec3 ObjLoader::get_converted_position(
	const vec3& position, const vec3& centerOffset, float absMax)
{
	return position / absMax - centerOffset;
}

jeEnd