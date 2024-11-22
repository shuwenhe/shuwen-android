#include <jni.h>
#include <string>
#include <json/json.h>

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_shuwen_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "shuwen android";
    return env->NewStringUTF(hello.c_str());
}