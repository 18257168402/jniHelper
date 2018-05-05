#ifndef _NATIVE_DEFS_H
#define _NATIVE_DEFS_H

#define PkgCore               "ble/lss/com/jnihelper/"
#define ClassCore(stru)       PkgCore #stru
#define SigCore(stru)         "L" ClassCore(stru) ";"

#endif // _NATIVE_DEFS_H
