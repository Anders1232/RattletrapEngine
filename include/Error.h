#ifndef ERROR_H
#define ERROR_H

#include <iostream>
#include "stdlib.h"

#ifdef _WIN32
	#define END_LINE "\r\n"
#else
	#define END_LINE "\n"
#endif

//#define USE_AT

using std::endl;

#define CONVERSAO_GRAUS_RADIANOS 57.324840764

#ifdef DEBUG
    #define DEBUG_PRINT(x) do{std::cout << x << std::endl;}while(0)
    #define Error(msg)std::cerr<<"[ERROR] "<<WHERE<<"\t\t"<<msg<<END_LINE;exit(1);
    #define ASSERT(exp)if(!(exp)){std::cerr<<"[ERROR] "<<WHERE<<"\t\tAssertion Failed."<<END_LINE;exit(1);}
    #define ASSERT2(exp,msg)if(!(exp)){std::cerr<<"[ERROR] "<<WHERE<<"\t\tAssertion Failed." <<msg<< "\n";exit(1);}
    #define SDL_ASSERT(exp)if(!(exp)){std::cerr<<"[ERROR] "<<WHERE<<"\t\tAssertion Failed:" << SDL_GetError()<<END_LINE;exit(1);}

    #define WHERE __FILE__<<" | "<<__func__<<":"<<__LINE__

    #define CHECK_SDL_ERROR std::cerr<<"[ERROR] "<<WHERE<<"\t\t"<<SDL_GetError()<<END_LINE

    #define REPORT_DEBUG(msg) if(DEBUG){std::cout<<"[DEBUG]"<<WHERE<<msg<<END_LINE;}
    #define REPORT_DEBUG2(cond, msg) if(cond||DEBUG){std::cout<<"[DEBUG]"<<WHERE<<msg<<END_LINE;}

    #define REPORT_I_WAS_HERE if(DEBUG){std::cout <<"[DEBUG] I was here!\t"<<WHERE<<END_LINE;}

    #define TEMP_REPORT_I_WAS_HERE if(1){std::cout<<"[DEBUG] I was here!\t"<<WHERE<<END_LINE;}

    #ifdef USE_AT
        #define ELEMENT_ACESS(container, position) container.at(position)
    #else
        #define ELEMENT_ACESS(container, position) container[position]
    #endif
#else
    #define DEBUG_PRINT(x)
    #define REPORT_DEBUG2(cond, msg)
    #define Error(msg)
    #define REPORT_I_WAS_HERE
    #define WHERE ""
    #define REPORT_DEBUG(msg)
    #define ASSERT(exp)
    #define ASSERT2(exp,msg)
    #define SDL_ASSERT(exp)
    #define CHECK_SDL_ERROR
    #define TEMP_REPORT_I_WAS_HERE

#endif // DEBUG
typedef unsigned int uint;

//void Error(char const * errMsg);

#endif // ERROR_H
