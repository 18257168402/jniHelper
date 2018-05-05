#include"jniutils.h"
#include"stdlib.h"
void transJavaClassToJniClass(JNI_IN const char* src, JNI_OUT char* to){
    char* p_s=const_cast<char*>(src);
    char* p_d=to;
    while((*p_s)!='\0'){
        if((*p_s)!='.'){
            (*p_d)=(*p_s);
        }else{
            (*p_d)='/';
        }
        p_s++;
        p_d++;
    }
    (*p_d)='\0';
}
void transJavaClassToSimpleClassname(const char* src,char* to){
    char* p_s=const_cast<char*>(src);
    char* p_d=to;
    int index=0;
    while((*p_s)!='\0'){
        index++;
        if(index==1){
            if((*p_s) == 'L'){
                p_s++;
                continue;
            }
        }

        if( ((*p_s)=='.') ||
            ((*p_s)=='/') ||
            ((*p_s)=='\\')||
            ((*p_s)==';')){
                p_s++;
                continue;

        }else{
           (*p_d)=(*p_s);
        }
        p_s++;
        p_d++;
    }
    (*p_d)='\0';
}
