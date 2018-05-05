#ifndef __JNIDATADEF_IN__
#define __JNIDATADEF_IN__
enum FIELDTYPE{
    IntField,StringField,ObjectField,
    LongField,DoubleField,ShortField,
    FloatField,BooleanField,CharField,
    ByteField,
    StaticIntField,StaticObjectField,
    StaticLongField,StaticDoubleField,StaticShortField,
    StaticFloatField,StaticBooleanField,StaticCharField,
    StaticByteField,
};
enum METHODTYPE{
    IntMethod,ObjectMethod,LongMethod,
    ShortMethod,FloatMethod,DoubleMethod,
    BooleanMethod,CharMethod,ByteMethod,
    VoidMethod,
    StaticIntMethod,StaticObjectMethod,StaticLongMethod,
    StaticShortMethod,StaticFloatMethod,StaticDoubleMethod,
    StaticBooleanMethod,StaticCharMethod,StaticByteMethod,
    StaticVoidMethod,
};
#endif