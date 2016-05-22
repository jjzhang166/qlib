#ifndef __QLIBDEF__H_
#define __QLIBDEF__H_

#if (defined(DLL_EXP_DEF))
#define DLL_EXP_QLIB __declspec(dllexport)
#elif (defined(STATIC_EXP_DEF))
#define DLL_EXP_QLIB
#else
#define DLL_EXP_QLIB __declspec(dllimport)
#endif

#endif
