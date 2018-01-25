#include "Engine/Math/Matrix4.hpp"


//---------------------------Consturctors-----------------------------------------------------------
Matrix4::Matrix4()
{
	m_iBasis = Vector4(1, 0, 0, 0);
	m_jBasis = Vector4(0, 1, 0, 0);
	m_kBasis = Vector4(0, 0, 1, 0);
	m_translation = Vector4(0, 0, 0, 1);
}

Matrix4::Matrix4(float initialX, float initialY, float initalZ)
{
	m_iBasis = Vector4(1, 0, 0, 0);
	m_jBasis = Vector4(0, 1, 0, 0);
	m_kBasis = Vector4(0, 0, 1, 0);
	m_translation = Vector4(0, 0, 0, 1);
	
	
	m_iBasis.x = initialX;
	m_jBasis.y = initialY;
	m_kBasis.z = initalZ;
}

Matrix4::Matrix4(const Matrix4& copy)
	:m_iBasis(copy.m_iBasis),
	m_jBasis(copy.m_jBasis),
	m_kBasis(copy.m_kBasis),
	m_translation(copy.m_translation)
{

}

Matrix4::Matrix4(const float* arrayOfFloats)
{
	m_iBasis.x = arrayOfFloats[0];
	m_iBasis.y = arrayOfFloats[4];
	m_iBasis.z = arrayOfFloats[8];
	m_iBasis.w = arrayOfFloats[12];

	m_jBasis.x = arrayOfFloats[1];
	m_jBasis.y = arrayOfFloats[5];
	m_jBasis.z = arrayOfFloats[9];
	m_jBasis.w = arrayOfFloats[13];

	m_kBasis.x = arrayOfFloats[2];
	m_kBasis.y = arrayOfFloats[6];
	m_kBasis.z = arrayOfFloats[10];
	m_kBasis.w = arrayOfFloats[14];

	m_translation.x = arrayOfFloats[3];
	m_translation.y = arrayOfFloats[7];
	m_translation.z = arrayOfFloats[11];
	m_translation.w = arrayOfFloats[15];

}

Matrix4::Matrix4(const Vector2& iBasis, const Vector2& jBasis, const Vector2& translation)
{
	m_iBasis.x = iBasis.x;
	m_iBasis.y = iBasis.y;
	m_iBasis.z = 0;
	m_iBasis.w = 0;

	m_jBasis.x = jBasis.x;
	m_jBasis.y = jBasis.y;
	m_jBasis.z = 0;
	m_jBasis.w = 0;

	m_kBasis = Vector4(0, 0, 0, 0);

	m_translation.x = 0.f;
	m_translation.y = 0.f;
	m_translation.z = 0;
	m_translation.w = 0;

	translation;
}

Matrix4::Matrix4(const Vector3& iBasis, const Vector3& jBasis, const Vector3& kBasis, const Vector3& translation)
{
	m_iBasis.x = iBasis.x;
	m_iBasis.y = iBasis.y;
	m_iBasis.z = iBasis.z;
	m_iBasis.w = 0;

	m_jBasis.x = jBasis.x;
	m_jBasis.y = jBasis.y;
	m_jBasis.z = jBasis.z;
	m_jBasis.w = 0;

	m_kBasis.x = kBasis.x;
	m_kBasis.y = kBasis.y;
	m_kBasis.z = kBasis.z;
	m_kBasis.w = 0;

	m_translation.x = 0.f;
	m_translation.y = 0.f;
	m_translation.z = 0.f;
	m_translation.w = 0;

	translation;
}

Matrix4::Matrix4(const Vector4& iBasis, const Vector4& jBasis, const Vector4& kBasis, const Vector4& translation)
{
	m_iBasis = iBasis;
	m_jBasis = jBasis;
	m_kBasis = kBasis;
	m_translation =  translation;
}

//---------------------------Create Matrix's Based off of Main Matrix-------------------------------------------
Matrix4 Matrix4::CreateTranslation(const Vector2& translation2D)
{
	Matrix4 matrixTranslation;
	matrixTranslation.m_iBasis.x += translation2D.x;
	matrixTranslation.m_jBasis.x += translation2D.x;

	matrixTranslation.m_iBasis.y += translation2D.y;
	matrixTranslation.m_jBasis.y += translation2D.y;

	return matrixTranslation;
}

Matrix4 Matrix4::CreateTranslation(const Vector3& translation3D)
{
	Matrix4 matrixTranslation;
	matrixTranslation.m_iBasis.x += translation3D.x;
	matrixTranslation.m_jBasis.x += translation3D.x;
	matrixTranslation.m_kBasis.x += translation3D.x;

	matrixTranslation.m_iBasis.y += translation3D.y;
	matrixTranslation.m_jBasis.y += translation3D.y;
	matrixTranslation.m_kBasis.y += translation3D.y;

	matrixTranslation.m_iBasis.z += translation3D.z;
	matrixTranslation.m_jBasis.z += translation3D.z;
	matrixTranslation.m_kBasis.z += translation3D.z;

	return matrixTranslation;
}

Matrix4 Matrix4::CreateScale(float uniformScale)
{
	Matrix4 matrixScale;
	matrixScale.m_iBasis *= uniformScale;
	matrixScale.m_jBasis *= uniformScale;
	matrixScale.m_kBasis *= uniformScale;
	matrixScale.m_translation *= uniformScale;
	return matrixScale;
}

Matrix4 Matrix4::CreateScale(const Vector2& nonUniformScale2D)
{
	Matrix4 matrixScale;
	matrixScale.m_iBasis.x += nonUniformScale2D.x;
	matrixScale.m_jBasis.x += nonUniformScale2D.x;

	matrixScale.m_iBasis.y += nonUniformScale2D.y;
	matrixScale.m_jBasis.y += nonUniformScale2D.y;
	return matrixScale;
}

Matrix4 Matrix4::CreateScale(const Vector3& nonUniformScale3D)
{
	Matrix4 matrixScale;
	matrixScale.m_iBasis.x += nonUniformScale3D.x;
	matrixScale.m_jBasis.x += nonUniformScale3D.x;
	matrixScale.m_kBasis.x += nonUniformScale3D.x;

	matrixScale.m_iBasis.y += nonUniformScale3D.y;
	matrixScale.m_jBasis.y += nonUniformScale3D.y;
	matrixScale.m_kBasis.y += nonUniformScale3D.y;

	matrixScale.m_iBasis.z += nonUniformScale3D.z;
	matrixScale.m_jBasis.z += nonUniformScale3D.z;
	matrixScale.m_kBasis.z += nonUniformScale3D.z;
	return matrixScale;
}

Matrix4 Matrix4::CreateRotationDegreesAboutX(float degrees)
{
	Matrix4 matrixRotation;

	matrixRotation.m_iBasis.x *= CosDegrees(degrees);
	matrixRotation.m_jBasis.x *= SinDegrees(degrees);

	matrixRotation.m_iBasis.y *= SinDegrees(degrees);
	matrixRotation.m_jBasis.y *= CosDegrees(degrees);

	matrixRotation.m_kBasis.z *= 1;
	matrixRotation.m_translation.z *= 1;

	return matrixRotation;
}

Matrix4 Matrix4::CreateRotationDegreesAboutY(float degrees)
{
	Matrix4 matrixRotation;
	matrixRotation.m_iBasis.x *= 1;

	matrixRotation.m_jBasis.y *= CosDegrees(degrees);
	matrixRotation.m_jBasis.y *= -SinDegrees(degrees);

	matrixRotation.m_iBasis.z *= SinDegrees(degrees);
	matrixRotation.m_kBasis.z *= CosDegrees(degrees);


	matrixRotation.m_translation.z *= 1;
	return matrixRotation;
}

Matrix4 Matrix4::CreateRotationDegreesAboutZ(float degrees)
{
	Matrix4 matrixRotation;

	matrixRotation.m_iBasis.x *= CosDegrees(degrees);
	matrixRotation.m_kBasis.x *= SinDegrees(degrees);

	matrixRotation.m_jBasis.y *= 1;

	matrixRotation.m_iBasis.z *= -SinDegrees(degrees);
	matrixRotation.m_kBasis.z *= CosDegrees(degrees);

	matrixRotation.m_translation.z *= 1;

	return matrixRotation;
}

Matrix4 Matrix4::CreateRotationRadiansAboutX(float radians)
{
	float degrees = ConvertRadiansToDegrees(radians);
	return CreateRotationDegreesAboutZ(degrees);
}

Matrix4 Matrix4::CreateRotationRadiansAboutY(float radians)
{
	float degrees = ConvertRadiansToDegrees(radians);
	return CreateRotationDegreesAboutZ(degrees);
}

Matrix4 Matrix4::CreateRotationRadiansAboutZ(float radians)
{
	float degrees = ConvertRadiansToDegrees(radians);
	return CreateRotationDegreesAboutZ(degrees);

}


//-----------------------------------Transforms---------------------------------------------------------------------
Vector2 Matrix4::TransformPosition(const Vector2& position2D) const
{
	Vector2 outputVector;

	outputVector.x = (position2D.x * m_iBasis.x) + (position2D.y * m_jBasis.x) + (m_translation.x);
	outputVector.y = (position2D.x * m_iBasis.y) + (position2D.y * m_jBasis.y) + (m_translation.y);

	return outputVector;
}

Vector3 Matrix4::TransformPosition(const Vector3& position3D) const
{
	Vector3 outputVector;

	outputVector.x = (position3D.x * m_iBasis.x) + (position3D.y * m_jBasis.x) + (position3D.z * m_kBasis.x) + (m_translation.x);
	outputVector.y = (position3D.x * m_iBasis.y) + (position3D.y * m_jBasis.y) + (position3D.z * m_kBasis.y) + (m_translation.y);
	outputVector.z = (position3D.x * m_iBasis.z) + (position3D.y * m_jBasis.z) + (position3D.z * m_kBasis.z) + (m_translation.z);

	return outputVector;
}
Vector2 Matrix4::TransformDirection(const Vector2& direction2D) const
{
	Vector2 transformedPosition = direction2D;
	transformedPosition.x += m_iBasis.x;
	transformedPosition.x += m_jBasis.x;

	transformedPosition.y += m_iBasis.y;
	transformedPosition.y += m_jBasis.y;

	return transformedPosition;
}

Vector3 Matrix4::TransformDirection(const Vector3& direction3D) const
{
	Vector3 transformedPosition = direction3D;
	transformedPosition.x += m_iBasis.x;
	transformedPosition.x += m_jBasis.x;
	transformedPosition.x += m_kBasis.x;

	transformedPosition.y +=m_iBasis.y;
	transformedPosition.y += m_jBasis.y;
	transformedPosition.y += m_kBasis.y;

	return transformedPosition;
}

Vector4 Matrix4::TransformVector(const Vector4& homogeneousVector) const
{
	Vector4 transformedPosition = homogeneousVector;
	transformedPosition.x += m_iBasis.x;
	transformedPosition.x += m_jBasis.x;
	transformedPosition.x += m_kBasis.x;
	transformedPosition.x += m_translation.x;

	transformedPosition.y += m_iBasis.y;
	transformedPosition.y += m_jBasis.y;
	transformedPosition.y += m_kBasis.y;
	transformedPosition.y += m_translation.y;

	transformedPosition.z += m_iBasis.z;
	transformedPosition.z += m_jBasis.z;
	transformedPosition.z += m_kBasis.z;
	transformedPosition.z += m_translation.z;

	transformedPosition.w += m_iBasis.w;
	transformedPosition.w += m_jBasis.w;
	transformedPosition.w += m_kBasis.w;
	transformedPosition.w += m_translation.w;

	return transformedPosition;
}



//-----------------------------------Do Stuff To Matrix---------------------------------------
void Matrix4::Translate(const Vector2& translation2D)
{
	m_iBasis.x += translation2D.x;
	m_jBasis.x += translation2D.x;

	m_iBasis.y += translation2D.y;
	m_jBasis.y += translation2D.y;
}

void Matrix4::Translate(const Vector3& translation3D)
{
	m_iBasis.x += translation3D.x;
	m_jBasis.x += translation3D.x;
	m_kBasis.x += translation3D.x;

	m_iBasis.y += translation3D.y;
	m_jBasis.y += translation3D.y;
	m_kBasis.y += translation3D.y;

	m_iBasis.z += translation3D.z;
	m_jBasis.z += translation3D.z;
	m_kBasis.z += translation3D.z;
}

void Matrix4::Scale(float uniformScale)
{
	m_iBasis *= uniformScale;
	m_jBasis *= uniformScale;
	m_kBasis *= uniformScale;
	m_translation *= uniformScale;
}

void Matrix4::Scale(const Vector2& nonUniformScale2D)
{
    m_iBasis.x += nonUniformScale2D.x;
	m_jBasis.x += nonUniformScale2D.x;

	m_iBasis.y += nonUniformScale2D.y;
	m_jBasis.y += nonUniformScale2D.y;
}

void Matrix4::Scale(const Vector3& nonUniformScale3D)
{
	m_iBasis.x += nonUniformScale3D.x;
	m_jBasis.x += nonUniformScale3D.x;
	m_kBasis.x += nonUniformScale3D.x;

	m_iBasis.y += nonUniformScale3D.y;
	m_jBasis.y += nonUniformScale3D.y;
	m_kBasis.y += nonUniformScale3D.y;

	m_iBasis.z += nonUniformScale3D.z;
	m_jBasis.z += nonUniformScale3D.z;
	m_kBasis.z += nonUniformScale3D.z;
}

void Matrix4::RotateDegreesAboutX(float degrees)
{
	m_iBasis.x = CosDegrees(degrees);
	m_jBasis.x = SinDegrees(degrees);
	m_iBasis.z = 0;

	m_iBasis.y = -SinDegrees(degrees);
	m_jBasis.y = CosDegrees(degrees);
	m_jBasis.z = 0;
	
	m_kBasis = Vector4(0,0,1,0);
	m_translation.z *= 1;
}

void Matrix4::RotateDegreesAboutY(float degrees)
{
	float cosDegrees = CosDegrees(degrees);
	float sinDegrees = SinDegrees(degrees);

	Vector4 rotatedI = m_iBasis * cosDegrees + m_kBasis * sinDegrees;
	Vector4 rotatedK = m_iBasis * -sinDegrees + m_kBasis * cosDegrees;
	
	m_iBasis = rotatedI;
	m_kBasis = rotatedK;
}

void Matrix4::RotateDegreesAboutZ(float degrees)
{
	m_iBasis.x *= CosDegrees(degrees);
	m_kBasis.x *= SinDegrees(degrees);

	m_jBasis.y *= 1;

	m_iBasis.z *= -SinDegrees(degrees);
	m_kBasis.z *= CosDegrees(degrees);


	m_translation.z *= 1;
}

void Matrix4::RotateRadiansAboutX(float radians)
{
	float degrees = ConvertRadiansToDegrees(radians);
	RotateDegreesAboutX(degrees);
}

void Matrix4::RotateRadiansAboutY(float radians)
{
	float degrees = ConvertRadiansToDegrees(radians);
	RotateDegreesAboutY(degrees);
}

void Matrix4::RotateRadiansAboutZ(float radians)
{
	float degrees = ConvertRadiansToDegrees(radians);
	RotateDegreesAboutZ(degrees);
}

//--------------------------------------Others-----------------------------------------------------------

void Matrix4::MakeIdentity()
{
	m_iBasis = Vector4(1.f, 0.f, 0.f, 0.f);
	m_jBasis = Vector4(0.f, 1.f, 0.f, 0.f);
	m_kBasis = Vector4(0.f, 0.f, 1.f, 0.f);
	m_translation = Vector4(0.f, 0.f, 0.f, 1.f);
}

void Matrix4::ConcatenateTransform(Matrix4 otherMatrix)
{
	Matrix4 concatenated;
	concatenated.m_iBasis.x = (this->m_iBasis.x * otherMatrix.m_iBasis.x) + (this->m_iBasis.y * otherMatrix.m_jBasis.x) + (this->m_iBasis.z * otherMatrix.m_kBasis.x) + (this->m_iBasis.w * otherMatrix.m_translation.x);
	concatenated.m_iBasis.y = (this->m_jBasis.x * otherMatrix.m_iBasis.x) + (this->m_jBasis.y * otherMatrix.m_jBasis.x) + (this->m_jBasis.z * otherMatrix.m_kBasis.x) + (this->m_jBasis.w * otherMatrix.m_translation.x);
	concatenated.m_iBasis.z = (this->m_kBasis.x * otherMatrix.m_iBasis.x) + (this->m_kBasis.y * otherMatrix.m_jBasis.x) + (this->m_kBasis.z * otherMatrix.m_kBasis.x) + (this->m_kBasis.w * otherMatrix.m_translation.x);
	concatenated.m_iBasis.w = (this->m_translation.x * otherMatrix.m_iBasis.x) + (this->m_translation.y * otherMatrix.m_jBasis.x) + (this->m_translation.z * otherMatrix.m_kBasis.x) + (this->m_translation.w * otherMatrix.m_translation.x);

	concatenated.m_jBasis.x = (this->m_iBasis.x * otherMatrix.m_iBasis.y) + (this->m_iBasis.y * otherMatrix.m_jBasis.y) + (this->m_iBasis.z * otherMatrix.m_kBasis.y) + (this->m_iBasis.w * otherMatrix.m_translation.y);
	concatenated.m_jBasis.y = (this->m_jBasis.x * otherMatrix.m_iBasis.y) + (this->m_jBasis.y * otherMatrix.m_jBasis.y) + (this->m_jBasis.z * otherMatrix.m_kBasis.y) + (this->m_jBasis.w * otherMatrix.m_translation.y);
	concatenated.m_jBasis.z = (this->m_kBasis.x * otherMatrix.m_iBasis.y) + (this->m_kBasis.y * otherMatrix.m_jBasis.y) + (this->m_kBasis.z * otherMatrix.m_kBasis.y) + (this->m_kBasis.w * otherMatrix.m_translation.y);
	concatenated.m_jBasis.w = (this->m_translation.x * otherMatrix.m_iBasis.y) + (this->m_translation.y * otherMatrix.m_jBasis.y) + (this->m_translation.z * otherMatrix.m_kBasis.y) + (this->m_translation.w * otherMatrix.m_translation.y);

	concatenated.m_kBasis.x = (this->m_iBasis.x * otherMatrix.m_iBasis.z) + (this->m_iBasis.y * otherMatrix.m_jBasis.z) + (this->m_iBasis.z * otherMatrix.m_kBasis.z) + (this->m_iBasis.w * otherMatrix.m_translation.z);
	concatenated.m_kBasis.y = (this->m_jBasis.x * otherMatrix.m_iBasis.z) + (this->m_jBasis.y * otherMatrix.m_jBasis.z) + (this->m_jBasis.z * otherMatrix.m_kBasis.z) + (this->m_jBasis.w * otherMatrix.m_translation.z);
	concatenated.m_kBasis.z = (this->m_kBasis.x * otherMatrix.m_iBasis.z) + (this->m_kBasis.y * otherMatrix.m_jBasis.z) + (this->m_kBasis.z * otherMatrix.m_kBasis.z) + (this->m_kBasis.w * otherMatrix.m_translation.z);
	concatenated.m_kBasis.w = (this->m_translation.x * otherMatrix.m_iBasis.z) + (this->m_translation.y * otherMatrix.m_jBasis.z) + (this->m_translation.z * otherMatrix.m_kBasis.z) + (this->m_translation.w * otherMatrix.m_translation.z);

	concatenated.m_translation.x = (this->m_iBasis.x * otherMatrix.m_iBasis.w) + (this->m_iBasis.y * otherMatrix.m_jBasis.w) + (this->m_iBasis.z * otherMatrix.m_kBasis.w) + (this->m_iBasis.w * otherMatrix.m_translation.w);
	concatenated.m_translation.y = (this->m_jBasis.x * otherMatrix.m_iBasis.w) + (this->m_jBasis.y * otherMatrix.m_jBasis.w) + (this->m_jBasis.z * otherMatrix.m_kBasis.w) + (this->m_jBasis.w * otherMatrix.m_translation.w);
	concatenated.m_translation.z = (this->m_kBasis.x * otherMatrix.m_iBasis.w) + (this->m_kBasis.y * otherMatrix.m_jBasis.w) + (this->m_kBasis.z * otherMatrix.m_kBasis.w) + (this->m_kBasis.w * otherMatrix.m_translation.w);
	concatenated.m_translation.w = (this->m_translation.x * otherMatrix.m_iBasis.w) + (this->m_translation.y * otherMatrix.m_jBasis.w) + (this->m_translation.z * otherMatrix.m_kBasis.w) + (this->m_translation.w * otherMatrix.m_translation.w);

	*this = concatenated;
}

void Matrix4::Transpose()
{
	Swap(m_iBasis.y, m_jBasis.x);
	Swap(m_iBasis.z, m_kBasis.x);
	Swap(m_iBasis.w, m_translation.x);
	Swap(m_jBasis.z, m_kBasis.y);
	Swap(m_jBasis.w, m_translation.y);
	Swap(m_kBasis.w, m_translation.z);
}

Matrix4 Matrix4::MatrixMakeLookAt(Vector3 const &position, Vector3 const &target, Vector3 const &world_up /*= Vector3(0.0f, 1.0f, 0.0f)*/)
{
	Vector3 forward;
	Vector3 dir = target - position;
	dir.Normalize();
	forward = dir;
	Vector3 right = world_up.Cross(forward);
	Vector3 up = forward.Cross(right);

	Matrix4 mat;
	mat.m_iBasis = Vector4(right, 0.0f);
	mat.m_jBasis = Vector4(up, 0.0f);
	mat.m_kBasis = Vector4(forward, 0.0f);
	mat.m_translation = Vector4(position, 1.0f);

	return mat;
}


/*
Vector3 Matrix4::transform(Vector3 const &v)
{
	Vector3 r;
	//r.x = v.DotProduct(v, get_column(0).GetXYZ());
	//r.y = v.DotProduct(v, get_column(1).xyz());
	//r.z = v.DotProduct(v, get_column(2).xyz());

	return r;
}
*/
void Matrix4::invert_orthonormal()
{
	Vector3 translation = getTranslation();
	set_translation(Vector3(0.0f, 0.0f, 0.0f));

	// invert the rotation part
	Transpose();

	//translation = transform(translation);
	translation.x = -translation.x;
	translation.y = -translation.y;
	translation.z = -translation.z;
	set_translation(translation);
}

Matrix4  Matrix4::MatrixMakeOrthoProjection(float nx, float fx, float ny, float fy, float nz /*= 0.0f*/, float fz /*= 1.0f*/)
{
	Matrix4 mat;

	float sx = 1.0f / (fx - nx);
	float sy = 1.0f / (fy - ny);
	float sz = 1.0f / (fz - nz);

	mat.m_iBasis = Vector4(2.0f * sx, 0.0f, 0.0f, 0.0f);
	mat.m_jBasis = Vector4(0.0f, 2.0f * sy, 0.0f, 0.0f);
	mat.m_kBasis = Vector4(0.0f, 0.0f, sz, 0.0f);
	mat.m_translation = Vector4(-(fx + nx) * sx, -(fy + ny) * sy, -nz * sz, 1.0f);

	return mat;
}

Matrix4  Matrix4::MatrixMakePerspectiveProjection(float const fov_radians, float const aspect_ratio, float const nz, float const fz)
{
	float size = 1.0f / tanf(fov_radians / 2.0f);

	// scale X or Y depending which dimension is bigger
	float w = size;
	float h = size;
	if (aspect_ratio > 1.0f) {
		w /= aspect_ratio;
	}
	else {
		h *= aspect_ratio;
	}

	float q = 1.0f / (fz - nz);

	Matrix4 mat;
	mat.m_iBasis = Vector4(w, 0.0f, 0.0f, 0.0f);
	mat.m_jBasis = Vector4(0.0f, h, 0.0f, 0.0f);
	mat.m_kBasis = Vector4(0.0f, 0.0f, fz * q, 1.0f);
	mat.m_translation = Vector4(0.0f, 0.0f, -nz * fz * q, 0.0f);

	return mat;
}



//---------------------------------Getters and Setters----------------------------------------------------
const float* Matrix4::GetAsFloatArray() const
{
	return &m_iBasis.x;
}

float* Matrix4::GetAsFloatArray()
{
	return &m_iBasis.x;
}

Matrix4 Matrix4::getInverseOrthonormal() const
{
	Matrix4 ret = *this;
	ret.invert_orthonormal();
	return ret;
}

Matrix4  Matrix4::getTranspose() const
{
	Matrix4 ret = *this;
	ret.Transpose();
	return ret;
}

Matrix4 Matrix4::getTransform(Matrix4 otherMatrix)
{
	Matrix4 newMatrix;
	newMatrix.m_iBasis.x = (this->m_iBasis.x * otherMatrix.m_iBasis.x) + (this->m_iBasis.y * otherMatrix.m_jBasis.x) + (this->m_iBasis.z * otherMatrix.m_kBasis.x) + (this->m_iBasis.w * otherMatrix.m_translation.x);
	newMatrix.m_iBasis.y = (this->m_jBasis.x * otherMatrix.m_iBasis.x) + (this->m_jBasis.y * otherMatrix.m_jBasis.x) + (this->m_jBasis.z * otherMatrix.m_kBasis.x) + (this->m_jBasis.w * otherMatrix.m_translation.x);
	newMatrix.m_iBasis.z = (this->m_kBasis.x * otherMatrix.m_iBasis.x) + (this->m_kBasis.y * otherMatrix.m_jBasis.x) + (this->m_kBasis.z * otherMatrix.m_kBasis.x) + (this->m_kBasis.w * otherMatrix.m_translation.x);
	newMatrix.m_iBasis.w = (this->m_translation.x * otherMatrix.m_iBasis.x) + (this->m_translation.y * otherMatrix.m_jBasis.x) + (this->m_translation.z * otherMatrix.m_kBasis.x) + (this->m_translation.w * otherMatrix.m_translation.x);

	newMatrix.m_jBasis.x = (this->m_iBasis.x * otherMatrix.m_iBasis.y) + (this->m_iBasis.y * otherMatrix.m_jBasis.y) + (this->m_iBasis.z * otherMatrix.m_kBasis.y) + (this->m_iBasis.w * otherMatrix.m_translation.y);
	newMatrix.m_jBasis.y = (this->m_jBasis.x * otherMatrix.m_iBasis.y) + (this->m_jBasis.y * otherMatrix.m_jBasis.y) + (this->m_jBasis.z * otherMatrix.m_kBasis.y) + (this->m_jBasis.w * otherMatrix.m_translation.y);
	newMatrix.m_jBasis.z = (this->m_kBasis.x * otherMatrix.m_iBasis.y) + (this->m_kBasis.y * otherMatrix.m_jBasis.y) + (this->m_kBasis.z * otherMatrix.m_kBasis.y) + (this->m_kBasis.w * otherMatrix.m_translation.y);
	newMatrix.m_jBasis.w = (this->m_translation.x * otherMatrix.m_iBasis.y) + (this->m_translation.y * otherMatrix.m_jBasis.y) + (this->m_translation.z * otherMatrix.m_kBasis.y) + (this->m_translation.w * otherMatrix.m_translation.y);

	newMatrix.m_kBasis.x = (this->m_iBasis.x * otherMatrix.m_iBasis.z) + (this->m_iBasis.y * otherMatrix.m_jBasis.z) + (this->m_iBasis.z * otherMatrix.m_kBasis.z) + (this->m_iBasis.w * otherMatrix.m_translation.z);
	newMatrix.m_kBasis.y = (this->m_jBasis.x * otherMatrix.m_iBasis.z) + (this->m_jBasis.y * otherMatrix.m_jBasis.z) + (this->m_jBasis.z * otherMatrix.m_kBasis.z) + (this->m_jBasis.w * otherMatrix.m_translation.z);
	newMatrix.m_kBasis.z = (this->m_kBasis.x * otherMatrix.m_iBasis.z) + (this->m_kBasis.y * otherMatrix.m_jBasis.z) + (this->m_kBasis.z * otherMatrix.m_kBasis.z) + (this->m_kBasis.w * otherMatrix.m_translation.z);
	newMatrix.m_kBasis.w = (this->m_translation.x * otherMatrix.m_iBasis.z) + (this->m_translation.y * otherMatrix.m_jBasis.z) + (this->m_translation.z * otherMatrix.m_kBasis.z) + (this->m_translation.w * otherMatrix.m_translation.z);

	newMatrix.m_translation.x = (this->m_iBasis.x * otherMatrix.m_iBasis.w) + (this->m_iBasis.y * otherMatrix.m_jBasis.w) + (this->m_iBasis.z * otherMatrix.m_kBasis.w) + (this->m_iBasis.w * otherMatrix.m_translation.w);
	newMatrix.m_translation.y = (this->m_jBasis.x * otherMatrix.m_iBasis.w) + (this->m_jBasis.y * otherMatrix.m_jBasis.w) + (this->m_jBasis.z * otherMatrix.m_kBasis.w) + (this->m_jBasis.w * otherMatrix.m_translation.w);
	newMatrix.m_translation.z = (this->m_kBasis.x * otherMatrix.m_iBasis.w) + (this->m_kBasis.y * otherMatrix.m_jBasis.w) + (this->m_kBasis.z * otherMatrix.m_kBasis.w) + (this->m_kBasis.w * otherMatrix.m_translation.w);
	newMatrix.m_translation.w = (this->m_translation.x * otherMatrix.m_iBasis.w) + (this->m_translation.y * otherMatrix.m_jBasis.w) + (this->m_translation.z * otherMatrix.m_kBasis.w) + (this->m_translation.w * otherMatrix.m_translation.w);
	return newMatrix;
}

/*
Matrix4 Matrix4::getInverse(Matrix4 baseMatrix)
{
	Matrix4 inverse;
	float detA = baseMatrix.getDeterminate();
	inverse = baseMatrix * (1 / detA);
	return inverse;
}*/

Matrix4 Matrix4::getInverse(Matrix4 baseMatrix)
{
	Matrix4 inverse;
	double determinant;


	inverse.m_iBasis.x = baseMatrix.m_jBasis.y * baseMatrix.m_kBasis.z * baseMatrix.m_translation.w -
		baseMatrix.m_jBasis.y * baseMatrix.m_translation.z* baseMatrix.m_kBasis.w -
		baseMatrix.m_jBasis.z * baseMatrix.m_kBasis.y * baseMatrix.m_translation.w +
		baseMatrix.m_jBasis.z * baseMatrix.m_translation.y * baseMatrix.m_kBasis.w +
		baseMatrix.m_jBasis.w * baseMatrix.m_kBasis.y * baseMatrix.m_translation.z-
		baseMatrix.m_jBasis.w * baseMatrix.m_translation.y * baseMatrix.m_kBasis.z;

	inverse.m_iBasis.y = -baseMatrix.m_iBasis.y * baseMatrix.m_kBasis.z * baseMatrix.m_translation.w +
		baseMatrix.m_iBasis.y * baseMatrix.m_translation.z* baseMatrix.m_kBasis.w +
		baseMatrix.m_iBasis.z * baseMatrix.m_kBasis.y * baseMatrix.m_translation.w -
		baseMatrix.m_iBasis.z * baseMatrix.m_translation.y * baseMatrix.m_kBasis.w -
		baseMatrix.m_iBasis.w * baseMatrix.m_kBasis.y * baseMatrix.m_translation.z+
		baseMatrix.m_iBasis.w * baseMatrix.m_translation.y * baseMatrix.m_kBasis.z;

	inverse.m_iBasis.z = baseMatrix.m_iBasis.y * baseMatrix.m_jBasis.z * baseMatrix.m_translation.w -
		baseMatrix.m_iBasis.y * baseMatrix.m_translation.z* baseMatrix.m_jBasis.w -
		baseMatrix.m_iBasis.z * baseMatrix.m_jBasis.y * baseMatrix.m_translation.w +
		baseMatrix.m_iBasis.z * baseMatrix.m_translation.y * baseMatrix.m_jBasis.w +
		baseMatrix.m_iBasis.w * baseMatrix.m_jBasis.y * baseMatrix.m_translation.z-
		baseMatrix.m_iBasis.w * baseMatrix.m_translation.y * baseMatrix.m_jBasis.z;

	inverse.m_iBasis.w = -baseMatrix.m_iBasis.y * baseMatrix.m_jBasis.z * baseMatrix.m_kBasis.w +
		baseMatrix.m_iBasis.y * baseMatrix.m_kBasis.z * baseMatrix.m_jBasis.w +
		baseMatrix.m_iBasis.z * baseMatrix.m_jBasis.y * baseMatrix.m_kBasis.w -
		baseMatrix.m_iBasis.z * baseMatrix.m_kBasis.y * baseMatrix.m_jBasis.w -
		baseMatrix.m_iBasis.w * baseMatrix.m_jBasis.y * baseMatrix.m_kBasis.z +
		baseMatrix.m_iBasis.w * baseMatrix.m_kBasis.y * baseMatrix.m_jBasis.z;

	inverse.m_jBasis.x = -baseMatrix.m_jBasis.x * baseMatrix.m_kBasis.z * baseMatrix.m_translation.w +
		baseMatrix.m_jBasis.x * baseMatrix.m_translation.z* baseMatrix.m_kBasis.w +
		baseMatrix.m_jBasis.z * baseMatrix.m_kBasis.x  * baseMatrix.m_translation.w -
		baseMatrix.m_jBasis.z * baseMatrix.m_translation.x * baseMatrix.m_kBasis.w -
		baseMatrix.m_jBasis.w * baseMatrix.m_kBasis.x * baseMatrix.m_translation.z+
		baseMatrix.m_jBasis.w * baseMatrix.m_translation.x * baseMatrix.m_kBasis.z;

	inverse.m_jBasis.y = baseMatrix.m_iBasis.x * baseMatrix.m_kBasis.z * baseMatrix.m_translation.w -
		baseMatrix.m_iBasis.x * baseMatrix.m_translation.z* baseMatrix.m_kBasis.w -
		baseMatrix.m_iBasis.z * baseMatrix.m_kBasis.x  * baseMatrix.m_translation.w +
		baseMatrix.m_iBasis.z * baseMatrix.m_translation.x * baseMatrix.m_kBasis.w +
		baseMatrix.m_iBasis.w * baseMatrix.m_kBasis.x  * baseMatrix.m_translation.z-
		baseMatrix.m_iBasis.w * baseMatrix.m_translation.x * baseMatrix.m_kBasis.z;

	inverse.m_jBasis.z = -baseMatrix.m_iBasis.x * baseMatrix.m_jBasis.z * baseMatrix.m_translation.w +
		baseMatrix.m_iBasis.x * baseMatrix.m_translation.z* baseMatrix.m_jBasis.w +
		baseMatrix.m_iBasis.z * baseMatrix.m_jBasis.x * baseMatrix.m_translation.w -
		baseMatrix.m_iBasis.z * baseMatrix.m_translation.x * baseMatrix.m_jBasis.w -
		baseMatrix.m_iBasis.w * baseMatrix.m_jBasis.x * baseMatrix.m_translation.z+
		baseMatrix.m_iBasis.w *  baseMatrix.m_translation.x * baseMatrix.m_jBasis.z;

	inverse.m_jBasis.w = baseMatrix.m_iBasis.x * baseMatrix.m_jBasis.z * baseMatrix.m_kBasis.w -
		baseMatrix.m_iBasis.x * baseMatrix.m_kBasis.z * baseMatrix.m_jBasis.w -
		baseMatrix.m_iBasis.z * baseMatrix.m_jBasis.x * baseMatrix.m_kBasis.w +
		baseMatrix.m_iBasis.z * baseMatrix.m_kBasis.x  * baseMatrix.m_jBasis.w +
		baseMatrix.m_iBasis.w * baseMatrix.m_jBasis.x * baseMatrix.m_kBasis.z -
		baseMatrix.m_iBasis.w * baseMatrix.m_kBasis.x  * baseMatrix.m_jBasis.z;

	inverse.m_kBasis.x = baseMatrix.m_jBasis.x * baseMatrix.m_kBasis.y * baseMatrix.m_translation.w -
		baseMatrix.m_jBasis.x * baseMatrix.m_translation.y * baseMatrix.m_kBasis.w -
		baseMatrix.m_jBasis.y * baseMatrix.m_kBasis.x  * baseMatrix.m_translation.w +
		baseMatrix.m_jBasis.y * baseMatrix.m_translation.x * baseMatrix.m_kBasis.w +
		baseMatrix.m_jBasis.w * baseMatrix.m_kBasis.x  * baseMatrix.m_translation.y -
		baseMatrix.m_jBasis.w * baseMatrix.m_translation.x * baseMatrix.m_kBasis.y;

	inverse.m_kBasis.y = -baseMatrix.m_iBasis.x * baseMatrix.m_kBasis.y * baseMatrix.m_translation.w +
		baseMatrix.m_iBasis.x * baseMatrix.m_translation.y * baseMatrix.m_kBasis.w +
		baseMatrix.m_iBasis.y * baseMatrix.m_kBasis.x  * baseMatrix.m_translation.w -
		baseMatrix.m_iBasis.y * baseMatrix.m_translation.x * baseMatrix.m_kBasis.w -
		baseMatrix.m_iBasis.w * baseMatrix.m_kBasis.x  * baseMatrix.m_translation.y +
		baseMatrix.m_iBasis.w * baseMatrix.m_translation.x * baseMatrix.m_kBasis.y;

	inverse.m_kBasis.z = baseMatrix.m_iBasis.x * baseMatrix.m_jBasis.y * baseMatrix.m_translation.w -
		baseMatrix.m_iBasis.x * baseMatrix.m_translation.y * baseMatrix.m_jBasis.w -
		baseMatrix.m_iBasis.y * baseMatrix.m_jBasis.x * baseMatrix.m_translation.w +
		baseMatrix.m_iBasis.y * baseMatrix.m_translation.x * baseMatrix.m_jBasis.w +
		baseMatrix.m_iBasis.w * baseMatrix.m_jBasis.x * baseMatrix.m_translation.y -
		baseMatrix.m_iBasis.w * baseMatrix.m_translation.x * baseMatrix.m_jBasis.y;

	inverse.m_kBasis.w = -baseMatrix.m_iBasis.x * baseMatrix.m_jBasis.y * baseMatrix.m_kBasis.w +
		baseMatrix.m_iBasis.x * baseMatrix.m_kBasis.y * baseMatrix.m_jBasis.w +
		baseMatrix.m_iBasis.y * baseMatrix.m_jBasis.x * baseMatrix.m_kBasis.w -
		baseMatrix.m_iBasis.y * baseMatrix.m_kBasis.x  * baseMatrix.m_jBasis.w -
		baseMatrix.m_iBasis.w * baseMatrix.m_jBasis.x * baseMatrix.m_kBasis.y +
		baseMatrix.m_iBasis.w * baseMatrix.m_kBasis.x  * baseMatrix.m_jBasis.y;

	inverse.m_translation.x = -baseMatrix.m_jBasis.x * baseMatrix.m_kBasis.y * baseMatrix.m_translation.z+
		baseMatrix.m_jBasis.x * baseMatrix.m_translation.y * baseMatrix.m_kBasis.z +
		baseMatrix.m_jBasis.y * baseMatrix.m_kBasis.x  * baseMatrix.m_translation.z-
		baseMatrix.m_jBasis.y * baseMatrix.m_translation.x * baseMatrix.m_kBasis.z -
		baseMatrix.m_jBasis.z * baseMatrix.m_kBasis.x  * baseMatrix.m_translation.y +
		baseMatrix.m_jBasis.z * baseMatrix.m_translation.x * baseMatrix.m_kBasis.y;

	inverse.m_translation.y = baseMatrix.m_iBasis.x * baseMatrix.m_kBasis.y * baseMatrix.m_translation.z-
		baseMatrix.m_iBasis.x * baseMatrix.m_translation.y * baseMatrix.m_kBasis.z -
		baseMatrix.m_iBasis.y * baseMatrix.m_kBasis.x  * baseMatrix.m_translation.z+
		baseMatrix.m_iBasis.y * baseMatrix.m_translation.x * baseMatrix.m_kBasis.z +
		baseMatrix.m_iBasis.z * baseMatrix.m_kBasis.x  * baseMatrix.m_translation.y -
		baseMatrix.m_iBasis.z * baseMatrix.m_translation.x * baseMatrix.m_kBasis.y;

	inverse.m_translation.z = -baseMatrix.m_iBasis.x * baseMatrix.m_jBasis.y * baseMatrix.m_translation.z+
		baseMatrix.m_iBasis.x * baseMatrix.m_translation.y * baseMatrix.m_jBasis.z +
		baseMatrix.m_iBasis.y * baseMatrix.m_jBasis.x * baseMatrix.m_translation.z-
		baseMatrix.m_iBasis.y * baseMatrix.m_translation.x * baseMatrix.m_jBasis.z -
		baseMatrix.m_iBasis.z * baseMatrix.m_jBasis.x * baseMatrix.m_translation.y +
		baseMatrix.m_iBasis.z * baseMatrix.m_translation.x * baseMatrix.m_jBasis.y;

	inverse.m_translation.w = baseMatrix.m_iBasis.x * baseMatrix.m_jBasis.y * baseMatrix.m_kBasis.z -
		baseMatrix.m_iBasis.x * baseMatrix.m_kBasis.y * baseMatrix.m_jBasis.z -
		baseMatrix.m_iBasis.y * baseMatrix.m_jBasis.x * baseMatrix.m_kBasis.z +
		baseMatrix.m_iBasis.y * baseMatrix.m_kBasis.x  * baseMatrix.m_jBasis.z +
		baseMatrix.m_iBasis.z * baseMatrix.m_jBasis.x * baseMatrix.m_kBasis.y -
		baseMatrix.m_iBasis.z * baseMatrix.m_kBasis.x  * baseMatrix.m_jBasis.y;


	determinant = baseMatrix.m_iBasis.x * inverse.m_iBasis.x + baseMatrix.m_jBasis.x * inverse.m_iBasis.y + baseMatrix.m_kBasis.x  * inverse.m_iBasis.z + baseMatrix.m_translation.x * inverse.m_iBasis.w;

	double determinantInverse = 1 / determinant;
	return inverse * (float)determinantInverse;
}

float Matrix4::getDeterminate()
{
	float detA;

	detA = m_iBasis.x * m_jBasis.y * m_kBasis.z * m_translation.w;
	detA += m_iBasis.x * m_jBasis.z * m_kBasis.w * m_translation.y;
	detA += m_iBasis.x * m_jBasis.w * m_kBasis.y * m_translation.z;
	detA += m_iBasis.y * m_jBasis.x * m_kBasis.w * m_translation.z;
	detA += m_iBasis.y * m_jBasis.z * m_kBasis.x * m_translation.w;
	detA += m_iBasis.y * m_jBasis.w * m_kBasis.z * m_translation.x;
	detA += m_iBasis.z * m_jBasis.x * m_kBasis.y * m_translation.w;
	detA += m_iBasis.z * m_jBasis.y * m_kBasis.w * m_translation.x;
	detA += m_iBasis.z * m_jBasis.w * m_kBasis.x * m_translation.y;
	detA += m_iBasis.w * m_jBasis.x * m_kBasis.z * m_translation.y;
	detA += m_iBasis.w * m_jBasis.y * m_kBasis.x * m_translation.z;
	detA += m_iBasis.w * m_jBasis.z * m_kBasis.y * m_translation.x;

	detA -= m_iBasis.x * m_jBasis.y * m_kBasis.w * m_translation.z;
	detA -= m_iBasis.x * m_jBasis.z * m_kBasis.y * m_translation.w;
	detA -= m_iBasis.x * m_jBasis.w * m_kBasis.z * m_translation.y;
	detA -= m_iBasis.y * m_jBasis.x * m_kBasis.z * m_translation.w;
	detA -= m_iBasis.y * m_jBasis.z * m_kBasis.w * m_translation.x;
	detA -= m_iBasis.y * m_jBasis.w * m_kBasis.x * m_translation.z;
	detA -= m_iBasis.z * m_jBasis.x * m_kBasis.w * m_translation.y;
	detA -= m_iBasis.z * m_jBasis.y * m_kBasis.x * m_translation.w;
	detA -= m_iBasis.z * m_jBasis.w * m_kBasis.y * m_translation.x;
	detA -= m_iBasis.w * m_jBasis.x * m_kBasis.y * m_translation.z;
	detA -= m_iBasis.w * m_jBasis.y * m_kBasis.z * m_translation.x;
	detA -= m_iBasis.w * m_jBasis.z * m_kBasis.x * m_translation.w;

	return detA;
}

/*
Vector4 Matrix4::get_column(unsigned int idx)
{
return Vector4(m_iBasis[idx], m_jBasis[idx], m_kBasis[idx], m_translation[idx]);
}
*/
Vector3 Matrix4::getTranslation() const
{
	return Vector3(m_translation.x, m_translation.y, m_translation.z);
}

void Matrix4::set_translation(Vector3 const &pos)
{
	m_translation = Vector4(pos, 1.0f);
}

//------------------------------------------Operators-----------------------------------------------
Matrix4 Matrix4::operator*(const Matrix4& mulB)
{
	Matrix4 matrixAnswer;

	matrixAnswer.m_iBasis.x = m_iBasis.x * mulB.m_iBasis.x + m_iBasis.y * mulB.m_jBasis.x + m_iBasis.z * mulB.m_kBasis.x + m_iBasis.w * mulB.m_translation.x;
	matrixAnswer.m_iBasis.y = m_iBasis.x * mulB.m_iBasis.y + m_iBasis.y * mulB.m_jBasis.y + m_iBasis.z * mulB.m_kBasis.y + m_iBasis.w * mulB.m_translation.y;
	matrixAnswer.m_iBasis.z = m_iBasis.x * mulB.m_iBasis.z + m_iBasis.y * mulB.m_jBasis.z + m_iBasis.z * mulB.m_kBasis.z + m_iBasis.w * mulB.m_translation.z;
	matrixAnswer.m_iBasis.w = m_iBasis.x * mulB.m_iBasis.w + m_iBasis.y * mulB.m_jBasis.w + m_iBasis.z * mulB.m_kBasis.w + m_iBasis.w * mulB.m_translation.w;

	matrixAnswer.m_jBasis.x = m_jBasis.x * mulB.m_iBasis.x + m_jBasis.y * mulB.m_jBasis.x + m_jBasis.z * mulB.m_kBasis.x + m_jBasis.w * mulB.m_translation.x;
	matrixAnswer.m_jBasis.y = m_jBasis.x * mulB.m_iBasis.y + m_jBasis.y * mulB.m_jBasis.y + m_jBasis.z * mulB.m_kBasis.y + m_jBasis.w * mulB.m_translation.y;
	matrixAnswer.m_jBasis.z = m_jBasis.x * mulB.m_iBasis.z + m_jBasis.y * mulB.m_jBasis.z + m_jBasis.z * mulB.m_kBasis.z + m_jBasis.w * mulB.m_translation.z;
	matrixAnswer.m_jBasis.w = m_jBasis.x * mulB.m_iBasis.w + m_jBasis.y * mulB.m_jBasis.w + m_jBasis.z * mulB.m_kBasis.w + m_jBasis.w * mulB.m_translation.w;

	matrixAnswer.m_kBasis.x = m_kBasis.x * mulB.m_iBasis.x + m_kBasis.y * mulB.m_jBasis.x + m_kBasis.z * mulB.m_kBasis.x + m_kBasis.w * mulB.m_translation.x;
	matrixAnswer.m_kBasis.y = m_kBasis.x * mulB.m_iBasis.y + m_kBasis.y * mulB.m_jBasis.y + m_kBasis.z * mulB.m_kBasis.y + m_kBasis.w * mulB.m_translation.y;
	matrixAnswer.m_kBasis.z = m_kBasis.x * mulB.m_iBasis.z + m_kBasis.y * mulB.m_jBasis.z + m_kBasis.z * mulB.m_kBasis.z + m_kBasis.w * mulB.m_translation.z;
	matrixAnswer.m_kBasis.w = m_kBasis.x * mulB.m_iBasis.w + m_kBasis.y * mulB.m_jBasis.w + m_kBasis.z * mulB.m_kBasis.w + m_kBasis.w * mulB.m_translation.w;

	matrixAnswer.m_translation.x = m_translation.x * mulB.m_iBasis.x + m_translation.y * mulB.m_jBasis.x + m_translation.z * mulB.m_kBasis.x + m_translation.w * mulB.m_translation.x;
	matrixAnswer.m_translation.y = m_translation.x * mulB.m_iBasis.y + m_translation.y * mulB.m_jBasis.y + m_translation.z * mulB.m_kBasis.y + m_translation.w * mulB.m_translation.y;
	matrixAnswer.m_translation.z = m_translation.x * mulB.m_iBasis.z + m_translation.y * mulB.m_jBasis.z + m_translation.z * mulB.m_kBasis.z + m_translation.w * mulB.m_translation.z;
	matrixAnswer.m_translation.w = m_translation.x * mulB.m_iBasis.w + m_translation.y * mulB.m_jBasis.w + m_translation.z * mulB.m_kBasis.w + m_translation.w * mulB.m_translation.w;

	return matrixAnswer;
}

Matrix4 Matrix4::operator*(const float& number)
{
	Matrix4 matrixAnswer;
	matrixAnswer.m_iBasis.x = this->m_iBasis.x * number;
	matrixAnswer.m_iBasis.y = this->m_iBasis.y * number;
	matrixAnswer.m_iBasis.z = this->m_iBasis.z * number;
	matrixAnswer.m_iBasis.w = this->m_iBasis.w * number;

	matrixAnswer.m_jBasis.x = this->m_jBasis.x * number;
	matrixAnswer.m_jBasis.y = this->m_jBasis.y * number;
	matrixAnswer.m_jBasis.z = this->m_jBasis.z * number;
	matrixAnswer.m_jBasis.w = this->m_jBasis.w * number;

	matrixAnswer.m_kBasis.x = this->m_kBasis.x * number;
	matrixAnswer.m_kBasis.y = this->m_kBasis.y * number;
	matrixAnswer.m_kBasis.z = this->m_kBasis.z * number;
	matrixAnswer.m_kBasis.w = this->m_kBasis.w * number;

	matrixAnswer.m_translation.x = this->m_translation.x * number;
	matrixAnswer.m_translation.y = this->m_translation.y * number;
	matrixAnswer.m_translation.z = this->m_translation.z * number;
	matrixAnswer.m_translation.w = this->m_translation.w * number;

	return matrixAnswer;
}

bool Matrix4::operator==(const Matrix4& vectorToEqual) const
{
	if (m_iBasis == vectorToEqual.m_iBasis && m_jBasis == vectorToEqual.m_jBasis && m_kBasis == vectorToEqual.m_kBasis && m_translation == vectorToEqual.m_translation)
		return true;
	return false;
}

