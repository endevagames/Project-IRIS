#pragma once
#ifdef NURU_PLATFORM_WINDOWS
    #ifdef NURU_DYNAMIC_LINK
        #ifdef NURU_BUILD_DLL
            #define  __declspec(dllexport)
        #else
            #define  __declspec(dllimport)
        #endif
    #else #define 
#endif
#else
    #error Only Windows Supported at the moment
#endif

#ifdef NURU_ENABLE_ASSERTS
    #define NURU_ASSERT(x, ...) { if(!(x)) { NURU_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak; } }
    #define NURU_CORE_ASSERT(x, ...) { if(!(x)) { NURU_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak; } }
#else
    #define NURU_ASSERT(x, ...)
    #define NURU_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define NURU_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace NURU
{

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

    template<typename T>
    using List = std::vector<T>;
}