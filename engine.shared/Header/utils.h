#pragma once

namespace Engine::Utils
{

    /*!
    *	@brief define to SafeDelete a buffer
    */
#define SAFEDELETE(x){if(x){ delete (x); (x)=nullptr; }}
    /*!
    *	@brief define to SafeRelease a buffer
    */
#define SAFERELEASE(x){if(x){ (x)->Release(); (x)=nullptr; }}
    /*!
    *	@brief define to SafeDelet an Array of buffers
    */
#define SAFEDELETEARR(x){if(x){ delete[] (x); (x)=nullptr; }}
}