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

#define DEBUG   true
#define DEBUG_R false
#define DEBUG_U false
#define DEBUG_C true
#ifdef DEBUG
    #define HEADER "[DEBUG] "
    #define WHERE __FILE__<<" | "<<__func__<<":"<<__LINE__

    #define Error(msg)std::cerr<<HEADER<<WHERE<<"\t\t"<<msg<<END_LINE;exit(1);
    #define ASSERT(exp)if(!(exp)){std::cerr<<HEADER>
    #define ASSERT2(exp,msg)if(!(exp)){std::cerr<<HEADER<<WHERE<<"\t\tAssertion Failed." <<msg<< "\n";exit(1);}
    #define SDL_ASSERT(exp)if(!(exp)){std::cerr<<HEADER<<WHERE<<"\t\tAssertion Failed:" << SDL_GetError()<<END_LINE;exit(1);}

    #define CHECK_SDL_ERROR std::cerr<<HEADER<<WHERE<<"\t\t"<<SDL_GetError()<<END_LINE

    #define REPORT_DEBUG(msg) if(DEBUG == 1){std::cout<<HEADER<<WHERE<<msg<<END_LINE;}
    #define REPORT_DEBUG2(cond, msg) if(cond||DEBUG){std::cout<<HEADER<<WHERE<<msg<<END_LINE;}

    #define DEBUG_PRINT(class, msg) if(DEBUG){std::cout << HEADER <<class<<"::"<<__func__<< "(): " << msg << END_LINE;}
    #define DEBUG_RENDER(class, msg) if(DEBUG_R){std::cout<<HEADER<<class<<"::"<<__func__<< "(): " << msg << END_LINE;}
    #define DEBUG_UPDATE(class, msg) if(DEBUG_U){std::cout<<HEADER<<class<<"::"<<__func__<< "(): " << msg << END_LINE;}
    #define DEBUG_CONSTRUCTOR(class, msg) if(DEBUG_C){std::cout<<HEADER<<class<<"::"<<__func__<< "(): " << msg<<END_LINE;}


    #define REPORT_I_WAS_HERE if(DEBUG){std::cout <<HEADER <<" I was here!\t"<<WHERE<<END_LINE;}

    #define TEMP_REPORT_I_WAS_HERE if(1){std::cout<< HEADER <<" I was here!\t"<<WHERE<<END_LINE;}

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

    #define DEBUG_UPDATE(class, msg)
    #define DEBUG_RENDER(class, msg)
    #define DEBUG_CONSTRUCTOR(class, arg2)

#endif // DEBUG
typedef unsigned int uint;

//void Error(char const * errMsg);

#endif // ERROR_H
