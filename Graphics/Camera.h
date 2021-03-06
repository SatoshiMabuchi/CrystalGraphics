#ifndef __CRYSTAL_GRAPHICS_CAMERA_H__
#define __CRYSTAL_GRAPHICS_CAMERA_H__

#include "../Math/Matrix.h"
#include "../Math/Vector.h"

#include <memory>

namespace Crystal {
	namespace Graphics {

template< typename T >
class Camera{
public:
	Camera()
	{
		init();
	}

	Camera( const T near__, const T far__, const T left, const T right, const T bottom, const T top ) :
		near_( near__ ),
		far_( far__),
		left( left ),
		right( right ),
		bottom( bottom ),
		top( top )
	{}

	void init()
	{
		near_ = (1.0f);
		far_ = (10.0f);
		left = (-0.5f);
		right = (0.5f);
		bottom = (0.0f);
		top = (1.0f);
		angle = Math::Vector3d<T>::Zero();
		pos = Math::Vector3d<T>::Zero();
	}

	Math::Matrix3d<T> getRotationMatrix() const {
		Math::Matrix3d<T> matrix = Math::Matrix3d<T>::Identity();
		matrix *= Math::Matrix3d<T>::RotateX(angle.getX());
		matrix *= Math::Matrix3d<T>::RotateY(angle.getY());
		matrix *= Math::Matrix3d<T>::RotateZ(angle.getZ());
		return matrix;
	}


	Math::Matrix4d<T> getModelviewMatrix() const {
		Math::Matrix4d<T> matrix;
		matrix.translate( pos.getX(), pos.getY(), pos.getZ() );
		matrix *= Math::Matrix4d<T>::RotateX( angle.getX() );
		matrix *= Math::Matrix4d<T>::RotateY( angle.getY() );
		matrix *= Math::Matrix4d<T>::RotateZ( angle.getZ() );
		return matrix;
	}

	Math::Matrix4d<float> getPerspectiveMatrix() const {
		const float dx = right - left;
		const float dy = top - bottom;
		const float dz = far_ - near_;

		return Math::Matrix4d<T>(
			2.0f / dx, 0.0, 0.0, 0.0,
			0.0, 2.0f / dy, 0.0, 0.0,
			0.0, 0.0, -1.0f/ dz, -1.0f,
			-(right + left) / dx, -(bottom + top) / dy, -near_ / dz, 0.0f
			);
	}

	T getNear() const { return near_; }

	T getFar() const { return far_; }

	void move( const Math::Vector3d<float>& move ) { this->pos += move; }

	void setPos( const Math::Vector3d<float>& p ) { this->pos = p; }

	Math::Vector3d<T> getPos() const { return pos; }


	Math::Matrix4d<T> getOrthogonalMatrix() const {
		const T left = T{ -0.5 };
		const T right = T{ 0.5 };
		const T bottom = T{ 0 };
		const T top = T{ 1 };

		const T dx = right - left;
		const T dy = top - bottom;
		const T dz = far_ - near_;

		Math::Matrix4d<T> matrix;
		matrix.setX00(T{ 2 } / dx);
		matrix.setX11(T{ 2 } / dy);
		matrix.setX22(T{-2 } / dz);
		matrix.setX30( -(right + left) / dx );
		matrix.setX31( -(top + bottom) / dy );
		matrix.setX32( -(far_ + near_) / dz );

		return matrix;
	}

	T getLeft() const { return left; }

	T getRight() const { return right; }

	T getBottom() const { return bottom; }

	T getTop() const { return top; }

	Math::Vector3d<float> getAngle() const { return angle; }

	void addAngle(const Math::Vector3d<T>& a ) { this->angle += a; }

	void setAngle(const Math::Vector3d<T>& a) { this->angle = a; }

	void setNear( const T n ) { this->near_ = n; } 

	void setFar( const T f ) { this->far_ = f; }

private:
	T near_;
	T far_;
	T left;
	T right;
	T bottom;
	T top;

	Math::Vector3d<T> pos;
	Math::Vector3d<T> angle;
};

template<typename T>
using CameraSPtr = std::shared_ptr < Camera<T> > ;

	}
}

#endif