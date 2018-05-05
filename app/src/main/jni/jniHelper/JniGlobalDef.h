#ifndef JNIGLOBALDEF
#define JNIGLOBALDEF
#define JNI_IN
#define JNI_OUT
#include<jni.h>
#define MakeSig(classname) "L" classname ";"

#define SIG_Int       "I"
#define SIG_Boolean   "Z"
#define SIG_Void      "V"
#define SIG_Char      "C"
#define SIG_Byte      "B"
#define SIG_Float     "F"
#define SIG_Long      "J"
#define SIG_Short     "S"
#define SIG_Int       "I"
#define SIG_Double    "D"



#define CLASS_Str              "java/lang/String"
#define CLASS_Obj              "java/lang/Object"
#define CLASS_ObjBool          "java/lang/Boolean"
#define CLASS_ObjInt           "java/lang/Integer"
#define CLASS_ObjShort         "java/lang/Short"
#define CLASS_ObjChar          "java/lang/Char"
#define CLASS_ObjByte          "java/lang/Byte"
#define CLASS_ObjLong          "java/lang/Long"
#define CLASS_ObjFloat         "java/lang/Float"
#define CLASS_ObjDouble        "java/lang/Double"
#define CLASS_ObjArrList       "java/util/ArrayList"
#define CLASS_ObjList          "java/util/List"
#define CLASS_Surface          "android/view/Surface"
#define CLASS_CLS              "java/lang/Class"
#define CLASS_Context          "android/content/Context"

#define SIG_Str                MakeSig(CLASS_Str)
#define SIG_Surface            MakeSig(CLASS_Surface)
#define SIG_ObjBool            MakeSig(CLASS_ObjBool)
#define SIG_ObjInt             MakeSig(CLASS_ObjInt)
#define SIG_ObjShort           MakeSig(CLASS_ObjShort)
#define SIG_ObjChar            MakeSig(CLASS_ObjChar)
#define SIG_ObjByte            MakeSig(CLASS_ObjByte)
#define SIG_ObjLong            MakeSig(CLASS_ObjLong)
#define SIG_ObjFloat           MakeSig(CLASS_ObjFloat)
#define SIG_ObjDouble          MakeSig(CLASS_ObjDouble)
#define SIG_ObjList            MakeSig(CLASS_ObjList)
#define SIG_ObjArrList         MakeSig(CLASS_ObjArrList)
#define SIG_Obj                MakeSig(CLASS_Obj)
#define SIG_CLS                MakeSig(CLASS_CLS)
#define SIG_Context            MakeSig(CLASS_Context)

#define SIG_IntArr        "[" SIG_Int
#define SIG_BooleanArr    "[" SIG_Boolean
#define SIG_CharArr       "[" SIG_Char
#define SIG_ByteArr       "[" SIG_Byte
#define SIG_FloatArr      "[" SIG_Float
#define SIG_LongArr       "[" SIG_Long
#define SIG_ShortArr      "[" SIG_Short
#define SIG_IntArr        "[" SIG_Int
#define SIG_DoubleArr     "[" SIG_Double
#define SIG_StrArr        "[" SIG_Str
#define SIG_ObjectArr     "[" SIG_Obj

#define SIG_Method_0(RetParaSig)                              "("                             ")" RetParaSig
#define SIG_Method_1(RetParaSig,P1)                           "(" P1                          ")" RetParaSig
#define SIG_Method_2(RetParaSig,P1,P2)                        "(" P1 P2                       ")" RetParaSig
#define SIG_Method_3(RetParaSig,P1,P2,P3)                     "(" P1 P2 P3                    ")" RetParaSig
#define SIG_Method_4(RetParaSig,P1,P2,P3,P4)                  "(" P1 P2 P3 P4                 ")" RetParaSig
#define SIG_Method_5(RetParaSig,P1,P2,P3,P4,P5)               "(" P1 P2 P3 P4 P5              ")" RetParaSig
#define SIG_Method_6(RetParaSig,P1,P2,P3,P4,P5,P6)            "(" P1 P2 P3 P4 P5 P6           ")" RetParaSig
#define SIG_Method_7(RetParaSig,P1,P2,P3,P4,P5,P6,P7)         "(" P1 P2 P3 P4 P5 P6 P7        ")" RetParaSig
#define SIG_Method_8(RetParaSig,P1,P2,P3,P4,P5,P6,P7,P8)      "(" P1 P2 P3 P4 P5 P6 P7 P8     ")" RetParaSig
#define SIG_Method_9(RetParaSig,P1,P2,P3,P4,P5,P6,P7,P8,P9)   "(" P1 P2 P3 P4 P5 P6 P7 P8 P9  ")" RetParaSig

#define SIG_VoidMethod_0()                             "("                             ")" SIG_Void
#define SIG_VoidMethod_1(P1)                           "(" P1                          ")" SIG_Void
#define SIG_VoidMethod_2(P1,P2)                        "(" P1 P2                       ")" SIG_Void
#define SIG_VoidMethod_3(P1,P2,P3)                     "(" P1 P2 P3                    ")" SIG_Void
#define SIG_VoidMethod_4(P1,P2,P3,P4)                  "(" P1 P2 P3 P4                 ")" SIG_Void
#define SIG_VoidMethod_5(P1,P2,P3,P4,P5)               "(" P1 P2 P3 P4 P5              ")" SIG_Void
#define SIG_VoidMethod_6(P1,P2,P3,P4,P5,P6)            "(" P1 P2 P3 P4 P5 P6           ")" SIG_Void
#define SIG_VoidMethod_7(P1,P2,P3,P4,P5,P6,P7)         "(" P1 P2 P3 P4 P5 P6 P7        ")" SIG_Void
#define SIG_VoidMethod_8(P1,P2,P3,P4,P5,P6,P7,P8)      "(" P1 P2 P3 P4 P5 P6 P7 P8     ")" SIG_Void
#define SIG_VoidMethod_9(P1,P2,P3,P4,P5,P6,P7,P8,P9)   "(" P1 P2 P3 P4 P5 P6 P7 P8 P9  ")" SIG_Void

#define SIG_BooleanMethod_0()                             "("                             ")" SIG_Boolean
#define SIG_BooleanMethod_1(P1)                           "(" P1                          ")" SIG_Boolean
#define SIG_BooleanMethod_2(P1,P2)                        "(" P1 P2                       ")" SIG_Boolean
#define SIG_BooleanMethod_3(P1,P2,P3)                     "(" P1 P2 P3                    ")" SIG_Boolean
#define SIG_BooleanMethod_4(P1,P2,P3,P4)                  "(" P1 P2 P3 P4                 ")" SIG_Boolean
#define SIG_BooleanMethod_5(P1,P2,P3,P4,P5)               "(" P1 P2 P3 P4 P5              ")" SIG_Boolean
#define SIG_BooleanMethod_6(P1,P2,P3,P4,P5,P6)            "(" P1 P2 P3 P4 P5 P6           ")" SIG_Boolean
#define SIG_BooleanMethod_7(P1,P2,P3,P4,P5,P6,P7)         "(" P1 P2 P3 P4 P5 P6 P7        ")" SIG_Boolean
#define SIG_BooleanMethod_8(P1,P2,P3,P4,P5,P6,P7,P8)      "(" P1 P2 P3 P4 P5 P6 P7 P8     ")" SIG_Boolean
#define SIG_BooleanMethod_9(P1,P2,P3,P4,P5,P6,P7,P8,P9)   "(" P1 P2 P3 P4 P5 P6 P7 P8 P9  ")" SIG_Boolean

#define SIG_IntMethod_0()                             "("                             ")" SIG_Int
#define SIG_IntMethod_1(P1)                           "(" P1                          ")" SIG_Int
#define SIG_IntMethod_2(P1,P2)                        "(" P1 P2                       ")" SIG_Int
#define SIG_IntMethod_3(P1,P2,P3)                     "(" P1 P2 P3                    ")" SIG_Int
#define SIG_IntMethod_4(P1,P2,P3,P4)                  "(" P1 P2 P3 P4                 ")" SIG_Int
#define SIG_IntMethod_5(P1,P2,P3,P4,P5)               "(" P1 P2 P3 P4 P5              ")" SIG_Int
#define SIG_IntMethod_6(P1,P2,P3,P4,P5,P6)            "(" P1 P2 P3 P4 P5 P6           ")" SIG_Int
#define SIG_IntMethod_7(P1,P2,P3,P4,P5,P6,P7)         "(" P1 P2 P3 P4 P5 P6 P7        ")" SIG_Int
#define SIG_IntMethod_8(P1,P2,P3,P4,P5,P6,P7,P8)      "(" P1 P2 P3 P4 P5 P6 P7 P8     ")" SIG_Int
#define SIG_IntMethod_9(P1,P2,P3,P4,P5,P6,P7,P8,P9)   "(" P1 P2 P3 P4 P5 P6 P7 P8 P9  ")" SIG_Int

#define SIG_FloatMethod_0()                             "("                             ")" SIG_Float
#define SIG_FloatMethod_1(P1)                           "(" P1                          ")" SIG_Float
#define SIG_FloatMethod_2(P1,P2)                        "(" P1 P2                       ")" SIG_Float
#define SIG_FloatMethod_3(P1,P2,P3)                     "(" P1 P2 P3                    ")" SIG_Float
#define SIG_FloatMethod_4(P1,P2,P3,P4)                  "(" P1 P2 P3 P4                 ")" SIG_Float
#define SIG_FloatMethod_5(P1,P2,P3,P4,P5)               "(" P1 P2 P3 P4 P5              ")" SIG_Float
#define SIG_FloatMethod_6(P1,P2,P3,P4,P5,P6)            "(" P1 P2 P3 P4 P5 P6           ")" SIG_Float
#define SIG_FloatMethod_7(P1,P2,P3,P4,P5,P6,P7)         "(" P1 P2 P3 P4 P5 P6 P7        ")" SIG_Float
#define SIG_FloatMethod_8(P1,P2,P3,P4,P5,P6,P7,P8)      "(" P1 P2 P3 P4 P5 P6 P7 P8     ")" SIG_Float
#define SIG_FloatMethod_9(P1,P2,P3,P4,P5,P6,P7,P8,P9)   "(" P1 P2 P3 P4 P5 P6 P7 P8 P9  ")" SIG_Float

#define SIG_LongMethod_0()                             "("                             ")" SIG_Long
#define SIG_LongMethod_1(P1)                           "(" P1                          ")" SIG_Long
#define SIG_LongMethod_2(P1,P2)                        "(" P1 P2                       ")" SIG_Long
#define SIG_LongMethod_3(P1,P2,P3)                     "(" P1 P2 P3                    ")" SIG_Long
#define SIG_LongMethod_4(P1,P2,P3,P4)                  "(" P1 P2 P3 P4                 ")" SIG_Long
#define SIG_LongMethod_5(P1,P2,P3,P4,P5)               "(" P1 P2 P3 P4 P5              ")" SIG_Long
#define SIG_LongMethod_6(P1,P2,P3,P4,P5,P6)            "(" P1 P2 P3 P4 P5 P6           ")" SIG_Long
#define SIG_LongMethod_7(P1,P2,P3,P4,P5,P6,P7)         "(" P1 P2 P3 P4 P5 P6 P7        ")" SIG_Long
#define SIG_LongMethod_8(P1,P2,P3,P4,P5,P6,P7,P8)      "(" P1 P2 P3 P4 P5 P6 P7 P8     ")" SIG_Long
#define SIG_LongMethod_9(P1,P2,P3,P4,P5,P6,P7,P8,P9)   "(" P1 P2 P3 P4 P5 P6 P7 P8 P9  ")" SIG_Long

void localRefCountAdd();
jobject LocalRefAdd(JNIEnv* env,jobject obj);
void LocalRefRemove(JNIEnv* env,jobject obj);
jobject GlobalRefAdd(JNIEnv* env,jobject obj);
void GlobalRefRemove(JNIEnv* env,jobject obj);
jobject GlobalWeakRefAdd(JNIEnv* env,jobject obj);
void GlobalWeakRefRemove(JNIEnv* env,jobject obj);
bool JNICheckIfFreed(JNIEnv* env,jobject weakref);
#endif // JNIGLOBALDEF

