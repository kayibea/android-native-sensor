#include <stdio.h>
#include <stdlib.h>
#include <android/sensor.h>

int main(void) {
    ASensorManager *sensorManager = ASensorManager_getInstanceForPackage(NULL);
    if (!sensorManager) {
        fprintf(stderr, "Failed to get ASensorManager instance\n");
        return 1;
    }

    ASensorList list;
    int count = ASensorManager_getSensorList(sensorManager, &list);
    if (count < 0) {
        fprintf(stderr, "Failed to get sensor list\n");
        return 1;
    }

    printf("Total Sensors: %d\n", count);
    for (int i = 0; i < count; i++) {
        const ASensor *sensor = list[i];
        printf("name:   %s\n", ASensor_getName(sensor));
        printf("vendor: %s\n", ASensor_getVendor(sensor));
        printf("type:   %s\n\n", ASensor_getStringType(sensor));
    }

    return 0;
}
