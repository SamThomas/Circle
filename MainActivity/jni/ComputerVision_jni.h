#ifndef ComputerVision_jni
#define ComputerVision_jni

#include <jni.h>
#include <string>
#include <sstream>
#include <vector>
#include <math.h>
#include <android/log.h>
#include <opencv2/core/core.hpp>
#include <opencv2/contrib/detection_based_tracker.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

#define LOG_TAG "ComputerVision_jni"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__))

//Debug
#define DEBUG 0

using namespace std;
using namespace cv;

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT int JNICALL Java_com_example_circles_MainActivity_process(JNIEnv * jenv, jobject obj, jlong, jlong);

#ifdef __cplusplus
}
#endif

void find_circles(jlong addrGray, jlong addrRgba, vector<Vec3f>* circles);

void drawCircles(jlong addrImage, vector<Vec3f>* circles);

void computeOrders(jlong addImage, vector<Vec3f>* circles);

#endif
