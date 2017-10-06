// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dOlibbdc_dict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "TBeamEnergy.h"
#include "TBDCProjection.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *TBeamEnergy_Dictionary();
   static void TBeamEnergy_TClassManip(TClass*);
   static void *new_TBeamEnergy(void *p = 0);
   static void *newArray_TBeamEnergy(Long_t size, void *p);
   static void delete_TBeamEnergy(void *p);
   static void deleteArray_TBeamEnergy(void *p);
   static void destruct_TBeamEnergy(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TBeamEnergy*)
   {
      ::TBeamEnergy *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TBeamEnergy));
      static ::ROOT::TGenericClassInfo 
         instance("TBeamEnergy", "TBeamEnergy.h", 4,
                  typeid(::TBeamEnergy), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TBeamEnergy_Dictionary, isa_proxy, 0,
                  sizeof(::TBeamEnergy) );
      instance.SetNew(&new_TBeamEnergy);
      instance.SetNewArray(&newArray_TBeamEnergy);
      instance.SetDelete(&delete_TBeamEnergy);
      instance.SetDeleteArray(&deleteArray_TBeamEnergy);
      instance.SetDestructor(&destruct_TBeamEnergy);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TBeamEnergy*)
   {
      return GenerateInitInstanceLocal((::TBeamEnergy*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TBeamEnergy*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TBeamEnergy_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TBeamEnergy*)0x0)->GetClass();
      TBeamEnergy_TClassManip(theClass);
   return theClass;
   }

   static void TBeamEnergy_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TBDCProjection_Dictionary();
   static void TBDCProjection_TClassManip(TClass*);
   static void *new_TBDCProjection(void *p = 0);
   static void *newArray_TBDCProjection(Long_t size, void *p);
   static void delete_TBDCProjection(void *p);
   static void deleteArray_TBDCProjection(void *p);
   static void destruct_TBDCProjection(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TBDCProjection*)
   {
      ::TBDCProjection *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TBDCProjection));
      static ::ROOT::TGenericClassInfo 
         instance("TBDCProjection", "TBDCProjection.h", 4,
                  typeid(::TBDCProjection), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TBDCProjection_Dictionary, isa_proxy, 0,
                  sizeof(::TBDCProjection) );
      instance.SetNew(&new_TBDCProjection);
      instance.SetNewArray(&newArray_TBDCProjection);
      instance.SetDelete(&delete_TBDCProjection);
      instance.SetDeleteArray(&deleteArray_TBDCProjection);
      instance.SetDestructor(&destruct_TBDCProjection);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TBDCProjection*)
   {
      return GenerateInitInstanceLocal((::TBDCProjection*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TBDCProjection*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TBDCProjection_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TBDCProjection*)0x0)->GetClass();
      TBDCProjection_TClassManip(theClass);
   return theClass;
   }

   static void TBDCProjection_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_TBeamEnergy(void *p) {
      return  p ? new(p) ::TBeamEnergy : new ::TBeamEnergy;
   }
   static void *newArray_TBeamEnergy(Long_t nElements, void *p) {
      return p ? new(p) ::TBeamEnergy[nElements] : new ::TBeamEnergy[nElements];
   }
   // Wrapper around operator delete
   static void delete_TBeamEnergy(void *p) {
      delete ((::TBeamEnergy*)p);
   }
   static void deleteArray_TBeamEnergy(void *p) {
      delete [] ((::TBeamEnergy*)p);
   }
   static void destruct_TBeamEnergy(void *p) {
      typedef ::TBeamEnergy current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TBeamEnergy

namespace ROOT {
   // Wrappers around operator new
   static void *new_TBDCProjection(void *p) {
      return  p ? new(p) ::TBDCProjection : new ::TBDCProjection;
   }
   static void *newArray_TBDCProjection(Long_t nElements, void *p) {
      return p ? new(p) ::TBDCProjection[nElements] : new ::TBDCProjection[nElements];
   }
   // Wrapper around operator delete
   static void delete_TBDCProjection(void *p) {
      delete ((::TBDCProjection*)p);
   }
   static void deleteArray_TBDCProjection(void *p) {
      delete [] ((::TBDCProjection*)p);
   }
   static void destruct_TBDCProjection(void *p) {
      typedef ::TBDCProjection current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TBDCProjection

namespace {
  void TriggerDictionaryInitialization_dOlibbdc_dict_Impl() {
    static const char* headers[] = {
"TBeamEnergy.h",
"TBDCProjection.h",
0
    };
    static const char* includePaths[] = {
"/raid/01/home/s015/v6.08.06/include",
"/raid/01/home/s015/exp/exp1605_s015/anaroot/users/S22_analysis/class/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "dOlibbdc_dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$TBeamEnergy.h")))  TBeamEnergy;
class __attribute__((annotate("$clingAutoload$TBDCProjection.h")))  TBDCProjection;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "dOlibbdc_dict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "TBeamEnergy.h"
#include "TBDCProjection.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"TBDCProjection", payloadCode, "@",
"TBeamEnergy", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule(".libbdc_dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_dOlibbdc_dict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_dOlibbdc_dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_dOlibbdc_dict() {
  TriggerDictionaryInitialization_dOlibbdc_dict_Impl();
}
