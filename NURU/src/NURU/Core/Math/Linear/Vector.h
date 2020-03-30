#pragma once

#include <initializer_list>
#include <assert.h>
#include <array>

namespace NURU
{
    /* NOTE(Joey):

      Generic Vector template version supporting Vectors of any type and any
      positive dimension. Constructors and templated Vector mathematics are 
      defined outside the templated class to support shared functionality in
      combination with template specialization of vec2,vec3 and vec4.

      The underlying implementation of all the Vector types are based on C++
      templates. The most common found Vector classes are however defined with
      preprocessor types for better readability hiding the underlying template
      implementation. For Vectors of a dimension > 4 it is required to directly
      specify the template variables.

    */
    template <std::size_t n, class T>
    struct Vector
    {
    public:
		std::array<T, n> data;

        // NOTE(Joey): constructor0: default empty constructor; default initialize all Vector elements
        Vector()
        {
            // NOTE(Joey): the empty constructor frequently gets called (when declaring large lists of
            // Vectors for instance) so default initializing them can be expensive as in almost all 
            // cases we'll be setting them to different values anyways.
            /*for (unsigned int i = 0; i < n; ++i)
                e[i] = {};*/
        }
        // NOTE(Joey): constructor1: one argument given: initialize all Vectors elements w/ same value
        Vector(const T& v)
        {
			for (auto& el : data) {
				el = v;
			}
        }
        // NOTE(Joey): constructor2: use std::initializer list for accepting any number of arguments
        Vector(const std::initializer_list<T> args)
        {
            assert(args.size() < n);
			data = args;
        }

        // NOTE(Joey): subscript operator
        T& operator[] (const std::size_t index)
        {
            assert(index >= 0 && index < n);
            return data.at(index);
        }

        // NOTE(Joey): math member operators (defined in operation.h)
        // ---------------------------------------------------
        // NOTE(Joey): negate operator
        Vector<n, T> operator-();
    };

    /* NOTE(Joey): 

      Specialized Vector version for 2-dimensional Vectors.

    */
    template <typename T>
    struct Vector<2, T>
    {
        union 
        {
			std::array<T, 2> data;
            // position
            struct 
            {
                T x;
                T y;
            };
            // texture coordinates
            // NOTE(Joey): only available in vec2, due to .w being reserved for position.
            struct
            {
                T u;
                T v;
            };
            // texture coordinates
            // NOTE(Joey): s,t,r is the general form of accessing texture coordinates in OpenGL.
            // Using u,v,w in 3 dimensions requires w which is reserved for the position's 
            // homogenous coordinate set.
            struct
            {
                T s;
                T t;
            };
        };

        // NOTE(Joey): constructor0: default empty constructor; default initialize all Vector elements
        Vector()
        {
			data = { };
        }
        // NOTE(Joey): constructor1: one argument given: initialize all Vectors elements w/ same value
        Vector(const T& v)
        {
			data = { v, v };
        }
        // NOTE(Joey): constructor2: use std::initializer list for accepting any number of arguments
        Vector(const std::initializer_list<T> args)
        {
            assert(args.size() <= 2);
            int index = 0;
			for (auto begin = args.begin(); begin != args.end(); ++begin) {
				data.at(index++) = *begin;
			}
        }
        // NOTE(Joey): constructor3: vec2 per-element initialization
        Vector(const T& x, const T& y)
        {
			data = { x, y };
        }

        // NOTE(Joey): subscript operator
        T& operator[] (const std::size_t index)
        {
            assert(index >= 0 && index < 2);
            return data.at(index);
        }

        // NOTE(Joey): math operators (defined in operation.h)
        // ---------------------------------------------------
        // NOTE(Joey): negate operator
        Vector<2, T> operator-();
    };

    /* NOTE(Joey):
        
       Specialized Vector version for 3-dimensional Vectors.

    */
    template<typename T>
    struct Vector<3, T>
    {
        union 
        {
			std::array<T, 3> data;
            // position
            struct
            {
                T x;
                T y;
                T z;
            };
            // color
            struct
            {
                T r;
                T g;
                T b;
            };
            // texture coordinates
            struct
            {
                T s;
                T t;
                T r;
            };
            struct
            {
                Vector<2, T> xy;
            };
            struct
            {
                Vector<2, T> yz;
            };
        };

        // NOTE(Joey): static frequently used Vectors
        static Vector<3, T> UP;
        static Vector<3, T> DOWN;
        static Vector<3, T> LEFT;
        static Vector<3, T> RIGHT;
        static Vector<3, T> FORWARD;
        static Vector<3, T> BACK;

        // NOTE(Joey): constructor0: default empty constructor; default initialize all Vector elements
        Vector()
        {
			data = {};
        }
        // NOTE(Joey): constructor1: one argument given: initialize all Vectors elements w/ same value
        Vector(const T& v)
        {
			data = { v, v, v };
        }
        // NOTE(Joey): constructor2: use std::initializer list for accepting any number of arguments
        Vector(const std::initializer_list<T> args)
        {
            assert(args.size() <= 3);
            int index = 0;
			for (auto begin = args.begin(); begin != args.end(); ++begin) {
				data.at(index++) = *begin;
			}
        }
        // NOTE(Joey): constructor3: vec3 per-element initialization
        Vector(const T& x, const T& y, const T& z)
        {
			data = { x, y, z };
        }
        // NOTE(Joey): constructor4: vec2 initialization
        Vector(const Vector<2, T>& vec, const T& z)
        {
			data = { vec.x, vec.y, z };
        }
        // NOTE(Joey): constructor5: vec2 initialization
        Vector(const T& x, const Vector<2, T>& vec)
        {
			data = { x, vec.x, vec.y };
        }

        // NOTE(Joey): subscript operator
        T& operator[] (const std::size_t index)
        {
            assert(index >= 0 && index < 3);
            return data.at(index);
        }

        // NOTE(Joey): math operators (defined in operation.h)
        // ---------------------------------------------------
        // NOTE(Joey): negate operator
        Vector<3, T> operator-();
    };

    // NOTE(Joey): initialize static variables of vec3
    template<typename T> Vector<3, T> Vector<3, T>::UP      = Vector<3, T>( 0.0,  1.0,  0.0);
    template<typename T> Vector<3, T> Vector<3, T>::DOWN    = Vector<3, T>( 0.0, -1.0,  0.0);
    template<typename T> Vector<3, T> Vector<3, T>::LEFT    = Vector<3, T>(-1.0,  0.0,  0.0);
    template<typename T> Vector<3, T> Vector<3, T>::RIGHT   = Vector<3, T>( 1.0,  0.0,  0.0);
    template<typename T> Vector<3, T> Vector<3, T>::FORWARD = Vector<3, T>( 0.0,  0.0, -1.0);
    template<typename T> Vector<3, T> Vector<3, T>::BACK    = Vector<3, T>( 0.0,  0.0,  1.0);

    /* NOTE(Joey):

    Specialized Vector version for 4-dimensional Vectors.

    */
    template<typename T>
    struct Vector<4, T>
    {
        union
        {
			std::array<T, 4> data;
            // position
            struct
            {
                T x;
                T y;
                T z;
                T w;
            };
            // color
            struct
            {
                T r;
                T g;
                T b;
                T a;
            };
            // texture coordinates
            struct
            {
                T s;
                T t;
                T r;
            };
            struct
            {
                Vector<2, T> xy;
                T _ignored1;
                T _ignored2;
            };
            struct
            {
                T _ignored1;
                T _ignored2;
                Vector<2, T> yz;
            };
            struct
            {
                Vector<3, T> xyz;
                T _ignored1;
            };
            struct
            {
                Vector<3, T> rgb;
                T _ignored1;
            };
            struct
            {
                Vector<3, T> srt;
                T _ignored1;
            };
        };

        // NOTE(Joey): constructor0: default empty constructor; default initialize all Vector elements
        Vector()
        {
			data = { };
        }
        // NOTE(Joey): constructor1: one argument given: initialize all Vectors elements w/ same value
        Vector(const T& v)
        {
			data = { v, v, v, v };
        }
        // NOTE(Joey): constructor2: use std::initializer list for accepting any number of arguments
        Vector(const std::initializer_list<T> args)
        {
            assert(args.size() <= 4);
            int index = 0;
			for (auto begin = args.begin(); begin != args.end(); ++begin) {
				data.at(index++) = *begin;
			}
        }
        // NOTE(Joey): constructor3: vec3 per-element initialization
        Vector(const T& x, const T& y, const T& z, const T& w)
        {
			data = { x, y, z, w };
        }
        // NOTE(Joey): constructor4: vec2 initialization
        Vector(const Vector<2, T>& xy, const Vector<2, T>& zw)
        {
			data = { xy.x, xy.y, zw.z, zw.w };
        }
        // NOTE(Joey): constructor5: vec3 initialization
        Vector(const Vector<3, T>& xyz, const T& w)
        {
			data = { xyz.x, xyz.y, xyz.z, w };
        }

        // NOTE(Joey): subscript operator
        T& operator[] (const std::size_t index)
        {
            assert(index >= 0 && index < 4);
            return data.at(index);
        }     

        // NOTE(Joey): math operators (defined in operation.h)
        // ---------------------------------------------------
        // NOTE(Joey): negate operator
        Vector<4, T> operator-();
    };

    typedef Vector<2, float>  Vec2;
    typedef Vector<3, float>  Vec3;
    typedef Vector<4, float>  Vec4;
    typedef Vector<2, int>    IVec2;
    typedef Vector<3, int>    IVec3;
    typedef Vector<4, int>    IVec4;
    typedef Vector<2, double> DVec2;
    typedef Vector<3, double> DVec3;
    typedef Vector<4, double> DVec4;

    // NOTE(Joey): per-Vector operations
    // ---------------------------------
    // NOTE(Joey): negate operator
    // NOTE(Joey): because this is a member operator, we have to
    // define this for each specialization.
    template <std::size_t n, typename T>
    inline Vector<n, T> Vector<n, T>::operator-()
    {
        Vector<n, T> result;
		for (std::size_t i = 0; i < n; ++i) {
			result[i] = -data[i];
		}
        return result;
    }
    template <typename T>
    inline Vector<2, T> Vector<2, T>::operator-()
    {
        return{ -x, -y };
    }
    template <typename T>
    inline Vector<3, T> Vector<3, T>::operator-()
    {
        return{ -x, -y, -z };
    }
    template <typename T>
    inline Vector<4, T> Vector<4, T>::operator-()
    {
        return{ -x, -y, -z, -w };
    }

    // NOTE(Joey): addition
    template <std::size_t n, typename T>
    inline Vector<n, T> operator+(Vector<n, T> lhs, T scalar)
    {
        Vector<n, T> result;
		for (std::size_t i = 0; i < n; ++i) {
			result[i] = lhs[i] + scalar;
		}
        return result;
    }
    template <std::size_t n, typename T>
    inline Vector<n, T> operator+(T scalar, Vector<n, T> rhs)
    {
        Vector<n, T> result;
        for (std::size_t i = 0; i < n; ++i)
            result[i] = lhs[i] + scalar;
        return result;
    }
    template <std::size_t n, typename T>
    inline Vector<n, T> operator+(Vector<n, T> lhs, Vector<n, T> rhs)
    {
        Vector<n, T> result;
        for (std::size_t i = 0; i < n; ++i)
            result[i] = lhs[i] + rhs[i];
        return result;
    }

    // NOTE(Joey): subtraction
    template <std::size_t n, typename T>
    inline Vector<n, T> operator-(Vector<n, T> lhs, T scalar)
    {
        Vector<n, T> result;
		for (std::size_t i = 0; i < n; ++i) {
			result[i] = lhs[i] - scalar;
		}
        return result;
    }
    template <std::size_t n, typename T>
    inline Vector<n, T> operator-(Vector<n, T> lhs, Vector<n, T> rhs)
    {
        Vector<n, T> result;
		for (std::size_t i = 0; i < n; ++i) {
			result[i] = lhs[i] - rhs[i];
		}
        return result;
    }

    // NOTE(Joey): multiplication
    template <std::size_t n, typename T>
    inline Vector<n, T> operator*(Vector<n, T> lhs, T scalar)
    {
        Vector<n, T> result;
		for (std::size_t i = 0; i < n; ++i) {
			result[i] = lhs[i] * scalar;
		}
        return result;
    }
    template <std::size_t n, typename T>
    Vector<n, T>& operator*=(Vector<n, T>& lhs, T scalar)
    {
        for (std::size_t i = 0; i < n; ++i) {
            lhs[i] *= scalar;
        }
        return lhs;
    }
    template <std::size_t n, typename T>
    inline Vector<n, T> operator*(T scalar, Vector<n, T> lhs)
    {
        Vector<n, T> result;
		for (std::size_t i = 0; i < n; ++i) {
			result[i] = lhs[i] * scalar;
		}
        return result;
    }
    template <std::size_t n, typename T>
    inline Vector<n, T> operator*(Vector<n, T> lhs, Vector<n, T> rhs) // NOTE(Joey): hadamard product
    {
        Vector<n, T> result;
		for (std::size_t i = 0; i < n; ++i) {
			result[i] = lhs[i] * rhs[i];
		}
        return result;
    }

    // NOTE(Joey): division
    template <std::size_t n, typename T>
    inline Vector<n, T> operator/(Vector<n, T> lhs, T scalar)
    {
        Vector<n, T> result;
		for (unsigned int i = 0; i < n; ++i) {
			result[i] = lhs[i] / scalar;
		}
        return result;
    }
    template <std::size_t n, typename T>
    inline Vector<n, T> operator/(T scalar, Vector<n, T> lhs)
    {
        Vector<n, T> result;
		for (std::size_t i = 0; i < n; ++i) {
			result[i] = lhs[i] / scalar;
		}
        return result;
    }
    template <std::size_t n, typename T>
    inline Vector<n, T> operator/(Vector<n, T> lhs, Vector<n, T> rhs) // NOTE(Joey): hadamard product
    {
        Vector<n, T> result;
		for (std::size_t i = 0; i < n; ++i) {
			result[i] = lhs[i] / rhs[i];
		}
        return result;
    }
}