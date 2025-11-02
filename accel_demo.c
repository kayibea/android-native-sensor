#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <android/sensor.h>
#include <android/looper.h>

#define LOOPER_ID 3

static volatile int running = 1;
static ASensorManager *sensorManager = NULL;
static const ASensor *accelerometer = NULL;
static ASensorEventQueue *sensorEventQueue = NULL;
static ALooper *looper = NULL;

static void handle_sigint(int sig) {
    (void)sig;
    running = 0;
    if (looper) ALooper_wake(looper);
    printf("\nExiting...\n");
    fflush(stdout);
}

static int get_sensor_events(int fd, int events, void *data) {
    (void)fd;
    (void)events;
    (void)data;

    ASensorEvent event;
    while (ASensorEventQueue_getEvents(sensorEventQueue, &event, 1) > 0) {
        if (event.type == ASENSOR_TYPE_ACCELEROMETER) {
            printf("ACCEL: x=%.3f y=%.3f z=%.3f\n",
                   event.acceleration.x,
                   event.acceleration.y,
                   event.acceleration.z);
            fflush(stdout);
        }
    }
    return 1;
}

int main(void) {
    signal(SIGINT, handle_sigint);

    sensorManager = ASensorManager_getInstanceForPackage(NULL);
    if (!sensorManager) {
        fprintf(stderr, "Failed to get ASensorManager instance\n");
        return 1;
    }

    accelerometer = ASensorManager_getDefaultSensor(sensorManager, ASENSOR_TYPE_ACCELEROMETER);
    if (!accelerometer) {
        fprintf(stderr, "Could not get ASENSOR_TYPE_ACCELEROMETER\n");
        return 1;
    }

    printf("Using sensor: %s\n", ASensor_getName(accelerometer));

    looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
    sensorEventQueue = ASensorManager_createEventQueue(
        sensorManager,
        looper,
        LOOPER_ID,
        get_sensor_events,
        NULL
    );
    if (!sensorEventQueue) {
        fprintf(stderr, "Failed to create event queue\n");
        return 1;
    }

    if (ASensorEventQueue_enableSensor(sensorEventQueue, accelerometer) < 0) {
        fprintf(stderr, "Failed to enable accelerometer\n");
        return 1;
    }

    ASensorEventQueue_setEventRate(sensorEventQueue, accelerometer, 20000); // ~50Hz

    printf("Reading accelerometer data... (Ctrl+C to stop)\n");
    while (running) {
        ALooper_pollOnce(-1, NULL, NULL, NULL);
    }

    ASensorEventQueue_disableSensor(sensorEventQueue, accelerometer);
    ASensorManager_destroyEventQueue(sensorManager, sensorEventQueue);
    return 0;
}
