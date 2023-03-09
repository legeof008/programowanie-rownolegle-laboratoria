//
// Created by maciek on 3/8/23.
//

#ifndef PROGRAMOWANIE_ROWNOLEGLE_LABORATORIA_TRY_H
#define PROGRAMOWANIE_ROWNOLEGLE_LABORATORIA_TRY_H

#define try bool __HadError=false;
#define catch(x) ExitJmp:if(__HadError)
#define throw_if_null(x) if(x==NULL){__HadError=true;goto ExitJmp;}
#define throw_if_fail(x) if(x==-1){__HadError=true;goto ExitJmp;}

#endif //PROGRAMOWANIE_ROWNOLEGLE_LABORATORIA_TRY_H
