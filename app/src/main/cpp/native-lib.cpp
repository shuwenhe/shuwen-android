#include <jni.h>
#include <string>
//#include <curl/curl.h>
#include "C:\Users\vcpkg\installed\x64-windows\include\curl\curl.h"
#include <android/log.h>

// 定义日志标签
#define LOG_TAG "NativeLib"
// 定义日志输出函数宏
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// 用于存储获取到的数据
struct MemoryStruct {
    char *memory;
    size_t size;
};

// curl写回调函数，用于将获取到的数据存储到MemoryStruct结构体中
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *) userp;

    char *ptr = (char *) realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr) {
        // 内存分配失败
        LOGE("Memory allocation failed");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_shuwen_MainActivity_stringFromJNI(JNIEnv* env,jobject /* this */) {
    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;
    chunk.memory = (char *) malloc(1);
    chunk.size = 0;

    curl = curl_easy_init();
    if(curl){
        std::string url = "http://www.xstiku.com/getUserData";
        curl_easy_setopt(curl,CURLOPT_URL,url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        res = curl_easy_perform(curl);
        if (res!= CURLE_OK) {
            LOGE("curl_easy_perform() failed: %s", curl_easy_strerror(res));
            free(chunk.memory);
            curl_easy_cleanup(curl);
            return env->NewStringUTF("");
        }

        curl_easy_cleanup(curl);

        std::string result(chunk.memory, chunk.size);
        free(chunk.memory);
        return env->NewStringUTF(result.c_str());
    }
    return env->NewStringUTF("");
}