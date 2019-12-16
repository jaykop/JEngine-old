#include "MathUtils.h"

jeBegin

namespace Math {

	Matrix4x4 Perspective(float fovy, float aspect, float zNear, float zFar)
	{
		float distance = 1.f / tanf(fovy * .5f);

		Matrix4x4 Result;

		Result.m[0][0] = distance / aspect;
		Result.m[1][1] = distance;
		Result.m[2][2] = (zFar + zNear) / (zNear - zFar);

		Result.m[2][3] = (2 * zFar * zNear) / (zNear - zFar);
		Result.m[3][2] = -1.f;

		return Result;
	}

	Matrix4x4 Orthogonal(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		Matrix4x4 Result;

		Result.m[0][0] = 2.f / (right - left);
		Result.m[1][1] = 2.f / (top - bottom);
		Result.m[2][2] = 2.f / (zNear - zFar);
		Result.m[0][3] = (right + left) / (left - right);
		Result.m[1][3] = (top + bottom) / (bottom - top);
		Result.m[2][3] = (zFar + zNear) / (zNear - zFar);
		Result.m[3][3] = 1.f;

		return Result;
	}

	Matrix4x4 Orthogonal(float left, float right, float bottom, float top)
	{
		mat4 Result;

		Result.m[0][0] = 2.f / (right - left);
		Result.m[1][1] = 2.f / (top - bottom);
		Result.m[2][2] = 1.f;
		Result.m[0][3] = (right + left) / (left - right);
		Result.m[1][3] = (top + bottom) / (bottom - top);
		Result.m[3][3] = 1.f;
		return Result;
	}

	Matrix4x4 LookAt(CR_Vec3 eye, CR_Vec3 target, CR_Vec3 up)
	{
		Vector3 _look, _up, _right;

		if (eye == target)
			jeDebugPrint("!Matrix4x4 - LookAt's position and target are identical.\n");

		_look = GetNormalize(eye - target);
		_up = up;
		_right = CrossProduct(up, _look);
		_up = CrossProduct(_look, _right);

		Matrix4x4 Result;

		Result.m[0][0] = _right.x;
		Result.m[0][1] = _right.y;
		Result.m[0][2] = _right.z;
		Result.m[0][3] = DotProduct(-_right, eye);

		Result.m[1][0] = _up.x;
		Result.m[1][1] = _up.y;
		Result.m[1][2] = _up.z;
		Result.m[1][3] = DotProduct(-_up, eye);

		Result.m[2][0] = _look.x;
		Result.m[2][1] = _look.y;
		Result.m[2][2] = _look.z;
		Result.m[2][3] = DotProduct(-_look, eye);

		Result.m[3][3] = 1.f;

		return Result;
	}

	Matrix4x4 Translate(CR_Vec3 vec)
	{
		Matrix4x4 Result(1.f);

		Result.m[0][3] = vec.x;
		Result.m[1][3] = vec.y;
		Result.m[2][3] = vec.z;

		return Result;
	}

	Matrix4x4 Scale(const vec3& vec)
	{
		Matrix4x4 Result;

		Result.m[0][0] = vec.x;
		Result.m[1][1] = vec.y;
		Result.m[2][2] = vec.z;
		Result.m[3][3] = 1.f;

		return Result;
	}

	Matrix4x4 RotateX(float radian)
	{
		Matrix4x4 rotation;
		float c = std::cosf(radian);
		float s = std::sinf(radian);

		rotation.m[0][0] = 1.f;
		rotation.m[1][1] = c;
		rotation.m[1][2] = -s;
		rotation.m[2][1] = s;
		rotation.m[2][2] = c;
		rotation.m[3][3] = 1.f;

		return rotation;
	}

	Matrix4x4 RotateY(float radian)
	{
		Matrix4x4 rotation;
		float c = std::cosf(radian);
		float s = std::sinf(radian);

		rotation.m[0][0] = c;
		rotation.m[1][1] = 1.f;
		rotation.m[0][2] = s;
		rotation.m[2][0] = -s;
		rotation.m[2][2] = c;
		rotation.m[3][3] = 1.f;

		return rotation;
	}

	Matrix4x4 RotateZ(float radian)
	{
		Matrix4x4 rotation;
		float c = std::cosf(radian);
		float s = std::sinf(radian);

		rotation.m[0][0] = c;
		rotation.m[0][1] = -s;
		rotation.m[1][0] = s;
		rotation.m[1][1] = c;
		rotation.m[2][2] = 1.f;
		rotation.m[3][3] = 1.f;

		return rotation;
	}

	Matrix4x4 Rotate(float radian, CR_Vec3 vec)
	{
		Matrix4x4 Result;

		float cosine = cos(radian);
		float sine = sin(radian);

		Vector3 norm = GetNormalize(vec);
		Vector3 offset = (1.f - cosine) * norm;

		Result.m[0][0] = offset.x * norm.x + cosine;
		Result.m[1][0] = offset.x * norm.y + norm.z * sine;
		Result.m[2][0] = offset.x * norm.z - norm.y * sine;

		Result.m[0][1] = offset.x * norm.y - norm.z * sine;
		Result.m[1][1] = offset.y * norm.y + cosine;
		Result.m[2][1] = offset.y * norm.z + norm.x * sine;

		Result.m[0][2] = offset.x * norm.z + norm.y * sine;
		Result.m[1][2] = offset.y * norm.z - norm.x * sine;
		Result.m[2][2] = offset.z * norm.z + cosine;

		Result.m[3][3] = 1.f;

		return Result;
	}

	/******************************************************************************/
	/*!
	\brief - Transpose matrix
	\param mat
	\return result
	*/
	/******************************************************************************/
	Matrix4x4 GetTranspose(CR_Mat4 mat)
	{
		Matrix4x4 result;

		for (int i = 0; i < 4; ++i)
			for (int j = 0; j <= i; ++j) {
				result.m[j][i] = mat.m[i][j];
				result.m[i][j] = mat.m[j][i];
			}
		return result;
	}

	void Transpose(mat4& mat)
	{
		mat = GetTranspose(mat);
	}

	Matrix4x4 GetInverse(CR_Mat4 mat)
	{
		Matrix4x4 result;

		// Caculate all the elements in matrix
		result.m[0][0] = mat.m[1][1] * mat.m[2][2] * mat.m[3][3] -
			mat.m[1][1] * mat.m[2][3] * mat.m[3][2] -
			mat.m[2][1] * mat.m[1][2] * mat.m[3][3] +
			mat.m[2][1] * mat.m[1][3] * mat.m[3][2] +
			mat.m[3][1] * mat.m[1][2] * mat.m[2][3] -
			mat.m[3][1] * mat.m[1][3] * mat.m[2][2];

		result.m[1][0] = -mat.m[1][0] * mat.m[2][2] * mat.m[3][3] +
			mat.m[1][0] * mat.m[2][3] * mat.m[3][2] +
			mat.m[2][0] * mat.m[1][2] * mat.m[3][3] -
			mat.m[2][0] * mat.m[1][3] * mat.m[3][2] -
			mat.m[3][0] * mat.m[1][2] * mat.m[2][3] +
			mat.m[3][0] * mat.m[1][3] * mat.m[2][2];

		result.m[2][0] = mat.m[1][0] * mat.m[2][1] * mat.m[3][3] -
			mat.m[1][0] * mat.m[2][3] * mat.m[3][1] -
			mat.m[2][0] * mat.m[1][1] * mat.m[3][3] +
			mat.m[2][0] * mat.m[1][3] * mat.m[3][1] +
			mat.m[3][0] * mat.m[1][1] * mat.m[2][3] -
			mat.m[3][0] * mat.m[1][3] * mat.m[2][1];

		result.m[3][0] = -mat.m[1][0] * mat.m[2][1] * mat.m[3][2] +
			mat.m[1][0] * mat.m[2][2] * mat.m[3][1] +
			mat.m[2][0] * mat.m[1][1] * mat.m[3][2] -
			mat.m[2][0] * mat.m[1][2] * mat.m[3][1] -
			mat.m[3][0] * mat.m[1][1] * mat.m[2][2] +
			mat.m[3][0] * mat.m[1][2] * mat.m[2][1];

		result.m[0][1] = -mat.m[0][1] * mat.m[2][2] * mat.m[3][3] +
			mat.m[0][1] * mat.m[2][3] * mat.m[3][2] +
			mat.m[2][1] * mat.m[0][2] * mat.m[3][3] -
			mat.m[2][1] * mat.m[0][3] * mat.m[3][2] -
			mat.m[3][1] * mat.m[0][2] * mat.m[2][3] +
			mat.m[3][1] * mat.m[0][3] * mat.m[2][2];

		result.m[1][1] = mat.m[0][0] * mat.m[2][2] * mat.m[3][3] -
			mat.m[0][0] * mat.m[2][3] * mat.m[3][2] -
			mat.m[2][0] * mat.m[0][2] * mat.m[3][3] +
			mat.m[2][0] * mat.m[0][3] * mat.m[3][2] +
			mat.m[3][0] * mat.m[0][2] * mat.m[2][3] -
			mat.m[3][0] * mat.m[0][3] * mat.m[2][2];

		result.m[2][1] = -mat.m[0][0] * mat.m[2][1] * mat.m[3][3] +
			mat.m[0][0] * mat.m[2][3] * mat.m[3][1] +
			mat.m[2][0] * mat.m[0][1] * mat.m[3][3] -
			mat.m[2][0] * mat.m[0][3] * mat.m[3][1] -
			mat.m[3][0] * mat.m[0][1] * mat.m[2][3] +
			mat.m[3][0] * mat.m[0][3] * mat.m[2][1];

		result.m[3][1] = mat.m[0][0] * mat.m[2][1] * mat.m[3][2] -
			mat.m[0][0] * mat.m[2][2] * mat.m[3][1] -
			mat.m[2][0] * mat.m[0][1] * mat.m[3][2] +
			mat.m[2][0] * mat.m[0][2] * mat.m[3][1] +
			mat.m[3][0] * mat.m[0][1] * mat.m[2][2] -
			mat.m[3][0] * mat.m[0][2] * mat.m[2][1];

		result.m[0][2] = mat.m[0][1] * mat.m[1][2] * mat.m[3][3] -
			mat.m[0][1] * mat.m[1][3] * mat.m[3][2] -
			mat.m[1][1] * mat.m[0][2] * mat.m[3][3] +
			mat.m[1][1] * mat.m[0][3] * mat.m[3][2] +
			mat.m[3][1] * mat.m[0][2] * mat.m[1][3] -
			mat.m[3][1] * mat.m[0][3] * mat.m[1][2];

		result.m[1][2] = -mat.m[0][0] * mat.m[1][2] * mat.m[3][3] +
			mat.m[0][0] * mat.m[1][3] * mat.m[3][2] +
			mat.m[1][0] * mat.m[0][2] * mat.m[3][3] -
			mat.m[1][0] * mat.m[0][3] * mat.m[3][2] -
			mat.m[3][0] * mat.m[0][2] * mat.m[1][3] +
			mat.m[3][0] * mat.m[0][3] * mat.m[1][2];

		result.m[2][2] = mat.m[0][0] * mat.m[1][1] * mat.m[3][3] -
			mat.m[0][0] * mat.m[1][3] * mat.m[3][1] -
			mat.m[1][0] * mat.m[0][1] * mat.m[3][3] +
			mat.m[1][0] * mat.m[0][3] * mat.m[3][1] +
			mat.m[3][0] * mat.m[0][1] * mat.m[1][3] -
			mat.m[3][0] * mat.m[0][3] * mat.m[1][1];

		result.m[3][2] = -mat.m[0][0] * mat.m[1][1] * mat.m[3][2] +
			mat.m[0][0] * mat.m[1][2] * mat.m[3][1] +
			mat.m[1][0] * mat.m[0][1] * mat.m[3][2] -
			mat.m[1][0] * mat.m[0][2] * mat.m[3][1] -
			mat.m[3][0] * mat.m[0][1] * mat.m[1][2] +
			mat.m[3][0] * mat.m[0][2] * mat.m[1][1];

		result.m[0][3] = -mat.m[0][1] * mat.m[1][2] * mat.m[2][3] +
			mat.m[0][1] * mat.m[1][3] * mat.m[2][2] +
			mat.m[1][1] * mat.m[0][2] * mat.m[2][3] -
			mat.m[1][1] * mat.m[0][3] * mat.m[2][2] -
			mat.m[2][1] * mat.m[0][2] * mat.m[1][3] +
			mat.m[2][1] * mat.m[0][3] * mat.m[1][2];

		result.m[1][3] = mat.m[0][0] * mat.m[1][2] * mat.m[2][3] -
			mat.m[0][0] * mat.m[1][3] * mat.m[2][2] -
			mat.m[1][0] * mat.m[0][2] * mat.m[2][3] +
			mat.m[1][0] * mat.m[0][3] * mat.m[2][2] +
			mat.m[2][0] * mat.m[0][2] * mat.m[1][3] -
			mat.m[2][0] * mat.m[0][3] * mat.m[1][2];

		result.m[2][3] = -mat.m[0][0] * mat.m[1][1] * mat.m[2][3] +
			mat.m[0][0] * mat.m[1][3] * mat.m[2][1] +
			mat.m[1][0] * mat.m[0][1] * mat.m[2][3] -
			mat.m[1][0] * mat.m[0][3] * mat.m[2][1] -
			mat.m[2][0] * mat.m[0][1] * mat.m[1][3] +
			mat.m[2][0] * mat.m[0][3] * mat.m[1][1];

		result.m[3][3] = mat.m[0][0] * mat.m[1][1] * mat.m[2][2] -
			mat.m[0][0] * mat.m[1][2] * mat.m[2][1] -
			mat.m[1][0] * mat.m[0][1] * mat.m[2][2] +
			mat.m[1][0] * mat.m[0][2] * mat.m[2][1] +
			mat.m[2][0] * mat.m[0][1] * mat.m[1][2] -
			mat.m[2][0] * mat.m[0][2] * mat.m[1][1];

		// Get determat.mine
		float det = mat.m[0][0] * result.m[0][0] +
			mat.m[0][1] * result.m[1][0] +
			mat.m[0][2] * result.m[2][0] +
			mat.m[0][3] * result.m[3][0];

		// No inverse in this case
		if (det == 0) {
			jeDebugPrint("!Matrix4x4 - Cannot devide by 0.\n");
			return mat;
		}

		// Make denomitator
		det = 1.f / det;

		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				result.m[i][j] = result.m[i][j] * det;

		return result;
	}

	void Inverse(mat4& mat)
	{
		mat = GetInverse(mat);
	}

	/******************************************************************************/
	/*!
	* \brief - Set matrix identity
	* \param mat
	* \param diagonal
	*/
	/******************************************************************************/
	void SetIdentity(mat4& mat, float diagonal)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				if (i == j)
					mat.m[i][j] = diagonal;
				else
					mat.m[i][j] = 0.f;
			}
		}
	}

}

jeEnd