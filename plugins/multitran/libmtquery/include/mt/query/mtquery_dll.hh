#ifndef MTQUERY_DLL_HH__
#define MTQUERY_DLL_HH__

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LIBMTSUPPORT_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LIBMTSUPPORT_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef WIN32
#ifdef LIBMTQUERY_EXPORTS
#define LIBMTQUERY_API __declspec(dllexport)
#else
#define LIBMTQUERY_API __declspec(dllimport)
#endif
#else
#define LIBMTQUERY_API
#endif

#endif
