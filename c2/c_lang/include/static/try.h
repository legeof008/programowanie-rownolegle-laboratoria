//
// Created by maciek on 3/14/23.
//

#ifndef C_LANG_TRY_H
#define C_LANG_TRY_H

#include <stdbool.h>

#define try bool __HadError=false;
#define catch(x) ExitJmp:if(__HadError)
#define throw_if_null(x) if(x==NULL){__HadError=true;goto ExitJmp;}
#define throw_if_fail(x) if(x==-1){__HadError=true;goto ExitJmp;}

#endif //C_LANG_TRY_H
