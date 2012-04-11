#include "jni_helper.hpp"
#include "logging.hpp"

#include "../../../../../base/assert.hpp"

static JavaVM * g_jvm = 0;

// @TODO remove after refactoring. Needed for NVidia code
void InitNVEvent(JavaVM * jvm);

extern "C"
{

  JNIEXPORT jint JNICALL
  JNI_OnLoad(JavaVM * jvm, void *)
  {
    g_jvm = jvm;
    jni::InitSystemLog();
    jni::InitAssertLog();
    // @TODO remove line below after refactoring
    InitNVEvent(jvm);
    return JNI_VERSION_1_6;
  }

  JNIEXPORT void JNICALL
  JNI_OnUnload(JavaVM *, void *)
  {
    g_jvm = 0;
  }

} // extern "C"


namespace jni
{
  jmethodID GetJavaMethodID(JNIEnv * env, jobject obj, char const * fn, char const * sig)
  {
    ASSERT(env, ("JNIEnv can't be 0"));
    ASSERT(obj, ("jobject can't be 0"));
    jclass cls = env->GetObjectClass(obj);
    ASSERT(cls, ("Can't get java class"));
    jmethodID mid = env->GetMethodID(cls, fn, sig);
    ASSERT(mid, ("Can't find java method", fn, sig));
    return mid;
  }

  string ToString(JNIEnv * env, jstring str)
  {
    string result;
    char const * utfBuffer = env->GetStringUTFChars(str, 0);
    if (utfBuffer)
    {
      result = utfBuffer;
      env->ReleaseStringUTFChars(str, utfBuffer);
    }
    return result;
  }

  JNIEnv * GetEnv()
  {
    JNIEnv * env;
    if (JNI_OK != g_jvm->GetEnv((void **)&env, JNI_VERSION_1_6))
    {
      ASSERT(false, ("Can't get JNIEnv. Was thread attached to JVM?"));
      return 0;
    }
    return env;
  }

  JavaVM * GetJVM()
  {
    ASSERT(g_jvm, ("JVM is not initialized"));
    return g_jvm;
  }

} // namespace jni
