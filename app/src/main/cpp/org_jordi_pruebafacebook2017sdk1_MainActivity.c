//
// Created by marzzelo on 2/4/2017.
//
#include "org_jordi_pruebafacebook2017sdk1_MainActivity.h"
#include <android/log.h>
#include <android/bitmap.h>

#define LOG_TAG "libimgprocesadondk"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
} rgba;

/*Conversion a grises por pixel*/
JNIEXPORT void JNICALL
Java_org_jordi_pruebafacebook2017sdk1_MainActivity_convertirGrises(JNIEnv *env, jobject obj,
                                                                   jobject bitmapcolor,
                                                                   jobject bitmapgris) {
    AndroidBitmapInfo infocolor;
    void *pixelscolor;
    AndroidBitmapInfo infogris;
    void *pixelsgris;
    int ret;
    int y;
    int x;

    LOGI("convertirGrises");

    if ((ret = AndroidBitmap_getInfo(env, bitmapcolor, &infocolor)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }

    if ((ret = AndroidBitmap_getInfo(env, bitmapgris, &infogris)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }

    LOGI("imagen color :: ancho %d;alto %d;avance %d;formato %d;flags %d", infocolor.width,
         infocolor.height, infocolor.stride, infocolor.format, infocolor.flags);

    if (infocolor.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Bitmap no es formato RGBA_8888 !");
        return;
    }

    LOGI("imagen color :: ancho %d;alto %d;avance %d;formato %d;flags %d", infogris.width,
         infogris.height, infogris.stride, infogris.format, infogris.flags);

    if (infogris.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Bitmap no es formato RGBA_8888 !");
        return;
    }

    if ((ret = AndroidBitmap_lockPixels(env, bitmapcolor, &pixelscolor)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }

    if ((ret = AndroidBitmap_lockPixels(env, bitmapgris, &pixelsgris)) < 0) {
        LOGE("AndroidBitmap_lockPixels() fallo ! error=%d", ret);
    }

    // modificacion pixeles en el algoritmo de escala grises
    for (y = 0; y < infocolor.height; y++) {
        rgba *line = (rgba *) pixelscolor;
        rgba *grisline = (rgba *) pixelsgris;

        for (x = 0; x < infocolor.width; x++) {
            float output = (line[x].red + line[x].green + line[x].blue) / 3;
            if (output > 255) output = 255;
            grisline[x].red = grisline[x].green = grisline[x].blue = (uint8_t) output;
            grisline[x].alpha = line[x].alpha;
        }

        pixelscolor = (char *) pixelscolor + infocolor.stride;
        pixelsgris = (char *) pixelsgris + infogris.stride;
    }

    LOGI("unlocking pixels");
    AndroidBitmap_unlockPixels(env, bitmapcolor);
    AndroidBitmap_unlockPixels(env, bitmapgris);
}

/*Conversion a sepia por pixel*/
JNIEXPORT void JNICALL
Java_org_jordi_pruebafacebook2017sdk1_MainActivity_convertirSepia(JNIEnv *env, jobject obj,
                                                                  jobject bitmapcolor,
                                                                  jobject bitmapsepia) {
    AndroidBitmapInfo infocolor;
    void *pixelscolor;
    AndroidBitmapInfo infosepia;
    void *pixelssepia;
    int ret;
    int y;
    int x;

    LOGI("convertirGrises");

    if ((ret = AndroidBitmap_getInfo(env, bitmapcolor, &infocolor)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }

    if ((ret = AndroidBitmap_getInfo(env, bitmapsepia, &infosepia)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }

    LOGI("imagen color :: ancho %d;alto %d;avance %d;formato %d;flags %d", infocolor.width,
         infocolor.height, infocolor.stride, infocolor.format, infocolor.flags);

    if (infocolor.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Bitmap no es formato RGBA_8888 !");
        return;
    }

    LOGI("imagen color :: ancho %d;alto %d;avance %d;formato %d;flags %d", infosepia.width,
         infosepia.height, infosepia.stride, infosepia.format, infosepia.flags);

    if (infosepia.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Bitmap no es formato RGBA_8888 !");
        return;
    }

    if ((ret = AndroidBitmap_lockPixels(env, bitmapcolor, &pixelscolor)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }

    if ((ret = AndroidBitmap_lockPixels(env, bitmapsepia, &pixelssepia)) < 0) {
        LOGE("AndroidBitmap_lockPixels() fallo ! error=%d", ret);
    }

    // modificacion pixeles en el algoritmo de escala grises
    for (y = 0; y < infocolor.height; y++) {
        rgba *line = (rgba *) pixelscolor;
        rgba *sepialine = (rgba *) pixelssepia;

        for (x = 0; x < infocolor.width; x++) {
            double outputRed =
                    (line[x].red * .393) + (line[x].green * .769) + (line[x].blue * .189);
            double outputGreen =
                    (line[x].red * .349) + (line[x].green * .686) + (line[x].blue * .168);
            double outputBlue =
                    (line[x].red * .272) + (line[x].green * .534) + (line[x].blue * .131);

            if (outputRed > 255) {
                sepialine[x].red = (uint8_t) 255;
            } else {
                sepialine[x].red = (uint8_t) outputRed;
            }
            if (outputGreen > 255) {
                sepialine[x].green = (uint8_t) 255;
            } else {
                sepialine[x].green = (uint8_t) outputGreen;
            }
            if (outputBlue > 255) {
                sepialine[x].blue = (uint8_t) 255;
            } else {
                sepialine[x].blue = (uint8_t) outputBlue;
            }

            sepialine[x].alpha = line[x].alpha;
        }

        pixelscolor = (char *) pixelscolor + infocolor.stride;
        pixelssepia = (char *) pixelssepia + infosepia.stride;
    }

    LOGI("unlocking pixels");
    AndroidBitmap_unlockPixels(env, bitmapcolor);
    AndroidBitmap_unlockPixels(env, bitmapsepia);
}

JNIEXPORT void JNICALL Java_org_jordi_pruebafacebook2017sdk1_MainActivity_crearMarco1
        (JNIEnv *env, jobject obj, jobject bitmapcolor, jobject bitmapmarco) {
    AndroidBitmapInfo infocolor;
    void *pixelscolor;
    AndroidBitmapInfo infoborde;
    void *pixelsborde;
    int ret;
    int y;
    int x;
    int altofinal;
    int anchofinal;

    LOGI("convertirMarco");
    if ((ret = AndroidBitmap_getInfo(env, bitmapcolor, &infocolor)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }
    if ((ret = AndroidBitmap_getInfo(env, bitmapmarco, &infoborde)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }
    LOGI("imagen color :: ancho %d;alto %d;avance %d;formato %d;flags %d", infocolor.width,
         infocolor.height, infocolor.stride, infocolor.format, infocolor.flags);
    if (infocolor.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Bitmap no es formato RGBA_8888 !");
        return;
    }

    LOGI("imagen color :: ancho %d;alto %d;avance %d;formato %d;flags %d", infoborde.width,
         infoborde.height, infoborde.stride, infoborde.format, infoborde.flags);
    if (infoborde.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Bitmap no es formato RGBA_8888 !");
        return;
    }
    if ((ret = AndroidBitmap_lockPixels(env, bitmapcolor, &pixelscolor)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }
    if ((ret = AndroidBitmap_lockPixels(env, bitmapmarco, &pixelsborde)) < 0) {
        LOGE("AndroidBitmap_lockPixels() fallo ! error=%d", ret);
    }

    anchofinal = infocolor.width - 10;
    altofinal = infocolor.height - 10;

    for (y = 0; y < infocolor.height; y++) {
        rgba *line = (rgba *) pixelscolor;
        rgba *bordeline = (rgba *) pixelsborde;

        for (x = 0; x < infocolor.width; x++) {
            if (y <= 9 || y >= altofinal) {
                bordeline[x].red = bordeline[x].green = bordeline[x].blue = (uint8_t) 0;
            } else if (x <= 9 || x >= anchofinal) {
                bordeline[x].red = bordeline[x].green = bordeline[x].blue = (uint8_t) 0;
            } else {
                bordeline[x].red = line[x].red;
                bordeline[x].green = line[x].green;
                bordeline[x].blue = line[x].blue;
            }
            bordeline[x].alpha = line[x].alpha;
        }
        pixelscolor = (char *) pixelscolor + infocolor.stride;
        pixelsborde = (char *) pixelsborde + infoborde.stride;
    }
    LOGI("unlocking pixels");
    AndroidBitmap_unlockPixels(env, bitmapcolor);
    AndroidBitmap_unlockPixels(env, bitmapmarco);
}

JNIEXPORT void JNICALL Java_org_jordi_pruebafacebook2017sdk1_MainActivity_crearMarco2
        (JNIEnv *env, jobject obj, jobject bitmapcolor, jobject bitmapmarco) {

    jboolean prueba = JNI_FALSE;
    LOGI("haypixel llamada!");

    jclass clazz = (*env)->GetObjectClass(env, obj);
    if (!clazz)
    {
        LOGE("callback_handler: FALLO object Class");
        goto failure;
    }

    jmethodID method = (*env)->GetStaticMethodID(env, clazz, "hayPixel", "(II)Z");
    if (!method)
    {
        LOGE("callback_hand ler: FALLO metodo ID");
        goto failure;
    }

    AndroidBitmapInfo infocolor;
    void *pixelscolor;
    AndroidBitmapInfo infoborde;
    void *pixelsborde;
    int ret;
    int y;
    int x;
    int altofinal;
    int anchofinal;
    uint8_t color;

    LOGI("convertir Borde 2");
    if ((ret = AndroidBitmap_getInfo(env, bitmapcolor, &infocolor)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }
    if ((ret = AndroidBitmap_getInfo(env, bitmapmarco, &infoborde)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }
    LOGI("imagen color :: ancho %d;alto %d;avance %d;formato %d;flags %d", infocolor.width,
         infocolor.height, infocolor.stride, infocolor.format, infocolor.flags);
    if (infocolor.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Bitmap no es formato RGBA_8888 !");
        return;
    }

    LOGI("imagen color :: ancho %d;alto %d;avance %d;formato %d;flags %d", infoborde.width,
         infoborde.height, infoborde.stride, infoborde.format, infoborde.flags);
    if (infoborde.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Bitmap no es formato RGBA_8888 !");
        return;
    }
    if ((ret = AndroidBitmap_lockPixels(env, bitmapcolor, &pixelscolor)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }
    if ((ret = AndroidBitmap_lockPixels(env, bitmapmarco, &pixelsborde)) < 0) {
        LOGE("AndroidBitmap_lockPixels() fallo ! error=%d", ret);
    }

    anchofinal = infocolor.width - 10;
    altofinal = infocolor.height - 10;

    for (y = 0; y < infocolor.height; y++) {
        rgba *line = (rgba *) pixelscolor;
        rgba *marcoline = (rgba *) pixelsborde;
        for (x = 0; x < infocolor.width; x++) {
            prueba = (*env)->CallStaticBooleanMethod(env, clazz, method);
            if (prueba == JNI_TRUE) { color = 0; }
            else { color = 255; }
            if (y <= 9 || y >= altofinal) {
                marcoline[x].red = marcoline[x].green = marcoline[x].blue = color;
            } else if (x <= 9 || x >= anchofinal) {
                marcoline[x].red = marcoline[x].green = marcoline[x].blue = color;
            } else {
                marcoline[x].red = line[x].red;
                marcoline[x].green = line[x].green;
                marcoline[x].blue = line[x].blue;
            }

            marcoline[x].alpha = line[x].alpha;
        }
        pixelscolor = (char *) pixelscolor + infocolor.stride;
        pixelsborde = (char *) pixelsborde + infoborde.stride;
    }
    LOGI("unlocking pixels");
    AndroidBitmap_unlockPixels(env, bitmapcolor);
    AndroidBitmap_unlockPixels(env, bitmapmarco);
    failure:
    return;
}

