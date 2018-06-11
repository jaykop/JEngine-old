#include "MathUtils.h"

jeBegin

namespace Math {

	Matrix4x4 Perspective(float _fovy, float _aspect, float _zNear, float _zFar)
	{
		float distance = 1.f / tanf(_fovy * .5f);

		Matrix4x4 Result;

		Result.m[0][0] = distance / _aspect;
		Result.m[1][1] = distance;
		Result.m[2][2] = (_zFar + _zNear) / (_zNear - _zFar);

		Result.m[2][3] = (2 * _zFar * _zNear) / (_zNear - _zFar);
		Result.m[3][2] = -1.f;

		return Result;
	}

	Matrix4x4 Orthogonal(float _left, float _right, float _bottom, float _top, float _zNear, float _zFar)
	{
		Matrix4x4 Result;

		Result.m[0][0] = 2.f / (_right - _left);
		Result.m[1][1] = 2.f / (_top - _bottom);
		Result.m[2][2] = 2.f / (_zNear - _zFar);
		Result.m[0][3] = (_right + _left) / (_left - _right);
		Result.m[1][3] = (_top + _bottom) / (_bottom - _top);
		Result.m[2][3] = (_zFar + _zNear) / (_zNear - _zFar);
		Result.m[3][3] = 1.f;

		return Result;
	}

	Matrix4x4 Orthogonal(float _left, float _right, float _bottom, float _top)
	{
		mat4 Result;

		Result.m[0][0] = 2.f / (_right - _left);
		Result.m[1][1] = 2.f / (_top - _bottom);
		Result.m[2][2] = 1.f;
		Result.m[0][3] = (_right + _left) / (_left - _right);
		Result.m[1][3] = (_top + _bottom) / (_bottom - _top);
		Result.m[3][3] = 1.f;
		return Result;
	}

	Matrix4x4 LookAt(CR_Vec3 _eye, CR_Vec3 _target, CR_Vec3 _up)
	{
		Vector3 look, up, right;

		if (_eye == _target)
			jeDebugPrint("!Matrix4x4 - LookAt's position and target are identical.\n");

		look = GetNormalize(_eye - _target);
		up = _up;
		right = CrossProduct(up, look);
		up = CrossProduct(look, right);

		Matrix4x4 Result;

		Result.m[0][0] = right.x;
		Result.m[0][1] = right.y;
		Result.m[0][2] = right.z;
		Result.m[0][3] = DotProduct(-right, _eye);

		Result.m[1][0] = up.x;
		Result.m[1][1] = up.y;
		Result.m[1][2] = up.z;
		Result.m[1][3] = DotProduct(-up, _eye);

		Result.m[2][0] = look.x;
		Result.m[2][1] = look.y;
		Result.m[2][2] = look.z;
		Result.m[2][3] = DotProduct(-look, _eye);

		Result.m[3][3] = 1.f;

		return Result;
	}

	Matrix4x4 Translate(CR_Vec3 _vec)
	{
		Matrix4x4 Result(1.f);

		Result.m[0][3] = _vec.x;
		Result.m[1][3] = _vec.y;
		Result.m[2][3] = _vec.z;

		return Result;
	}

	Matrix4x4 Scale(const vec3& _vec)
	{
		Matrix4x4 Result;

		Result.m[0][0] = _vec.x;
		Result.m[1][1] = _vec.y;
		Result.m[2][2] = _vec.z;
		Result.m[3][3] = 1.f;

		return Result;
	}

	Matrix4x4 RotateX(float _radian)
	{
		Matrix4x4 rotation;
		float c = std::cosf(_radian);
		float s = std::sinf(_radian);

		rotation.m[0][0] = 1.f;
		rotation.m[1][1] = c;
		rotation.m[1][2] = -s;
		rotation.m[2][1] = s;
		rotation.m[2][2] = c;
		rotation.m[3][3] = 1.f;

		return rotation;
	}

	Matrix4x4 RotateY(float _radian)
	{
		Matrix4x4 rotation;
		float c = std::cosf(_radian);
		float s = std::sinf(_radian);

		rotation.m[0][0] = c;
		rotation.m[1][1] = 1.f;
		rotation.m[0][2] = s;
		rotation.m[2][0] = -s;
		rotation.m[2][2] = c;
		rotation.m[3][3] = 1.f;

		return rotation;
	}

	Matrix4x4 RotateZ(float _radian)
	{
		Matrix4x4 rotation;
		float c = std::cosf(_radian);
		float s = std::sinf(_radian);

		rotation.m[0][0] = c;
		rotation.m[0][1] = -s;
		rotation.m[1][0] = s;
		rotation.m[1][1] = c;
		rotation.m[2][2] = 1.f;
		rotation.m[3][3] = 1.f;

		return rotation;
	}

	Matrix4x4 Rotate(float _radian, CR_Vec3 _vec)
	{
		Matrix4x4 Result;

		float cosine = cos(_radian);
		float sine = sin(_radian);

		Vector3 norm = GetNormalize(_vec);
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
	\param _mat
	\return result
	*/
	/******************************************************************************/
	Matrix4x4 GetTranspose(CR_Mat4 _mat)
	{
		Matrix4x4 result;

		for (int i = 0; i < 4; ++i)
			for (int j = 0; j <= i; ++j) {
				result.m[j][i] = _mat.m[i][j];
				result.m[i][j] = _mat.m[j][i];
			}
		return result;
	}

	Matrix4x4& Transpose(mat4& _mat)
	{
		_mat = GetTranspose(_mat);
		return _mat;
	}

	Matrix4x4 GetInverse(CR_Mat4 _mat)
	{
		Matrix4x4 result;

		// Caculate all the elements in matrix
		result.m[0][0] = _mat.m[1][1] * _mat.m[2][2] * _mat.m[3][3] -
			_mat.m[1][1] * _mat.m[2][3] * _mat.m[3][2] -
			_mat.m[2][1] * _mat.m[1][2] * _mat.m[3][3] +
			_mat.m[2][1] * _mat.m[1][3] * _mat.m[3][2] +
			_mat.m[3][1] * _mat.m[1][2] * _mat.m[2][3] -
			_mat.m[3][1] * _mat.m[1][3] * _mat.m[2][2];

		result.m[1][0] = -_mat.m[1][0] * _mat.m[2][2] * _mat.m[3][3] +
			_mat.m[1][0] * _mat.m[2][3] * _mat.m[3][2] +
			_mat.m[2][0] * _mat.m[1][2] * _mat.m[3][3] -
			_mat.m[2][0] * _mat.m[1][3] * _mat.m[3][2] -
			_mat.m[3][0] * _mat.m[1][2] * _mat.m[2][3] +
			_mat.m[3][0] * _mat.m[1][3] * _mat.m[2][2];

		result.m[2][0] = _mat.m[1][0] * _mat.m[2][1] * _mat.m[3][3] -
			_mat.m[1][0] * _mat.m[2][3] * _mat.m[3][1] -
			_mat.m[2][0] * _mat.m[1][1] * _mat.m[3][3] +
			_mat.m[2][0] * _mat.m[1][3] * _mat.m[3][1] +
			_mat.m[3][0] * _mat.m[1][1] * _mat.m[2][3] -
			_mat.m[3][0] * _mat.m[1][3] * _mat.m[2][1];

		result.m[3][0] = -_mat.m[1][0] * _mat.m[2][1] * _mat.m[3][2] +
			_mat.m[1][0] * _mat.m[2][2] * _mat.m[3][1] +
			_mat.m[2][0] * _mat.m[1][1] * _mat.m[3][2] -
			_mat.m[2][0] * _mat.m[1][2] * _mat.m[3][1] -
			_mat.m[3][0] * _mat.m[1][1] * _mat.m[2][2] +
			_mat.m[3][0] * _mat.m[1][2] * _mat.m[2][1];

		result.m[0][1] = -_mat.m[0][1] * _mat.m[2][2] * _mat.m[3][3] +
			_mat.m[0][1] * _mat.m[2][3] * _mat.m[3][2] +
			_mat.m[2][1] * _mat.m[0][2] * _mat.m[3][3] -
			_mat.m[2][1] * _mat.m[0][3] * _mat.m[3][2] -
			_mat.m[3][1] * _mat.m[0][2] * _mat.m[2][3] +
			_mat.m[3][1] * _mat.m[0][3] * _mat.m[2][2];

		result.m[1][1] = _mat.m[0][0] * _mat.m[2][2] * _mat.m[3][3] -
			_mat.m[0][0] * _mat.m[2][3] * _mat.m[3][2] -
			_mat.m[2][0] * _mat.m[0][2] * _mat.m[3][3] +
			_mat.m[2][0] * _mat.m[0][3] * _mat.m[3][2] +
			_mat.m[3][0] * _mat.m[0][2] * _mat.m[2][3] -
			_mat.m[3][0] * _mat.m[0][3] * _mat.m[2][2];

		result.m[2][1] = -_mat.m[0][0] * _mat.m[2][1] * _mat.m[3][3] +
			_mat.m[0][0] * _mat.m[2][3] * _mat.m[3][1] +
			_mat.m[2][0] * _mat.m[0][1] * _mat.m[3][3] -
			_mat.m[2][0] * _mat.m[0][3] * _mat.m[3][1] -
			_mat.m[3][0] * _mat.m[0][1] * _mat.m[2][3] +
			_mat.m[3][0] * _mat.m[0][3] * _mat.m[2][1];

		result.m[3][1] = _mat.m[0][0] * _mat.m[2][1] * _mat.m[3][2] -
			_mat.m[0][0] * _mat.m[2][2] * _mat.m[3][1] -
			_mat.m[2][0] * _mat.m[0][1] * _mat.m[3][2] +
			_mat.m[2][0] * _mat.m[0][2] * _mat.m[3][1] +
			_mat.m[3][0] * _mat.m[0][1] * _mat.m[2][2] -
			_mat.m[3][0] * _mat.m[0][2] * _mat.m[2][1];

		result.m[0][2] = _mat.m[0][1] * _mat.m[1][2] * _mat.m[3][3] -
			_mat.m[0][1] * _mat.m[1][3] * _mat.m[3][2] -
			_mat.m[1][1] * _mat.m[0][2] * _mat.m[3][3] +
			_mat.m[1][1] * _mat.m[0][3] * _mat.m[3][2] +
			_mat.m[3][1] * _mat.m[0][2] * _mat.m[1][3] -
			_mat.m[3][1] * _mat.m[0][3] * _mat.m[1][2];

		result.m[1][2] = -_mat.m[0][0] * _mat.m[1][2] * _mat.m[3][3] +
			_mat.m[0][0] * _mat.m[1][3] * _mat.m[3][2] +
			_mat.m[1][0] * _mat.m[0][2] * _mat.m[3][3] -
			_mat.m[1][0] * _mat.m[0][3] * _mat.m[3][2] -
			_mat.m[3][0] * _mat.m[0][2] * _mat.m[1][3] +
			_mat.m[3][0] * _mat.m[0][3] * _mat.m[1][2];

		result.m[2][2] = _mat.m[0][0] * _mat.m[1][1] * _mat.m[3][3] -
			_mat.m[0][0] * _mat.m[1][3] * _mat.m[3][1] -
			_mat.m[1][0] * _mat.m[0][1] * _mat.m[3][3] +
			_mat.m[1][0] * _mat.m[0][3] * _mat.m[3][1] +
			_mat.m[3][0] * _mat.m[0][1] * _mat.m[1][3] -
			_mat.m[3][0] * _mat.m[0][3] * _mat.m[1][1];

		result.m[3][2] = -_mat.m[0][0] * _mat.m[1][1] * _mat.m[3][2] +
			_mat.m[0][0] * _mat.m[1][2] * _mat.m[3][1] +
			_mat.m[1][0] * _mat.m[0][1] * _mat.m[3][2] -
			_mat.m[1][0] * _mat.m[0][2] * _mat.m[3][1] -
			_mat.m[3][0] * _mat.m[0][1] * _mat.m[1][2] +
			_mat.m[3][0] * _mat.m[0][2] * _mat.m[1][1];

		result.m[0][3] = -_mat.m[0][1] * _mat.m[1][2] * _mat.m[2][3] +
			_mat.m[0][1] * _mat.m[1][3] * _mat.m[2][2] +
			_mat.m[1][1] * _mat.m[0][2] * _mat.m[2][3] -
			_mat.m[1][1] * _mat.m[0][3] * _mat.m[2][2] -
			_mat.m[2][1] * _mat.m[0][2] * _mat.m[1][3] +
			_mat.m[2][1] * _mat.m[0][3] * _mat.m[1][2];

		result.m[1][3] = _mat.m[0][0] * _mat.m[1][2] * _mat.m[2][3] -
			_mat.m[0][0] * _mat.m[1][3] * _mat.m[2][2] -
			_mat.m[1][0] * _mat.m[0][2] * _mat.m[2][3] +
			_mat.m[1][0] * _mat.m[0][3] * _mat.m[2][2] +
			_mat.m[2][0] * _mat.m[0][2] * _mat.m[1][3] -
			_mat.m[2][0] * _mat.m[0][3] * _mat.m[1][2];

		result.m[2][3] = -_mat.m[0][0] * _mat.m[1][1] * _mat.m[2][3] +
			_mat.m[0][0] * _mat.m[1][3] * _mat.m[2][1] +
			_mat.m[1][0] * _mat.m[0][1] * _mat.m[2][3] -
			_mat.m[1][0] * _mat.m[0][3] * _mat.m[2][1] -
			_mat.m[2][0] * _mat.m[0][1] * _mat.m[1][3] +
			_mat.m[2][0] * _mat.m[0][3] * _mat.m[1][1];

		result.m[3][3] = _mat.m[0][0] * _mat.m[1][1] * _mat.m[2][2] -
			_mat.m[0][0] * _mat.m[1][2] * _mat.m[2][1] -
			_mat.m[1][0] * _mat.m[0][1] * _mat.m[2][2] +
			_mat.m[1][0] * _mat.m[0][2] * _mat.m[2][1] +
			_mat.m[2][0] * _mat.m[0][1] * _mat.m[1][2] -
			_mat.m[2][0] * _mat.m[0][2] * _mat.m[1][1];

		// Get deter_mat.mine
		float det = _mat.m[0][0] * result.m[0][0] +
			_mat.m[0][1] * result.m[1][0] +
			_mat.m[0][2] * result.m[2][0] +
			_mat.m[0][3] * result.m[3][0];

		// No inverse in this case
		if (det == 0) {
			jeDebugPrint("!Matrix4x4 - Cannot devide by 0.\n");
			return _mat;
		}

		// Make denomitator
		det = 1.f / det;

		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				result.m[i][j] = result.m[i][j] * det;

		return result;
	}

	Matrix4x4& Inverse(mat4& _mat)
	{
		_mat = GetInverse(_mat);
		return _mat;
	}

	/******************************************************************************/
	/*!
	* \brief - Set matrix identity
	* \param _mat
	* \param _diagonal
	*/
	/******************************************************************************/
	void SetIdentity(mat4& _mat, float _diagonal)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				if (i == j)
					_mat.m[i][j] = _diagonal;
				else
					_mat.m[i][j] = 0.f;
			}
		}
	}

}

jeEnd