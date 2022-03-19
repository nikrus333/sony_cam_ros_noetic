#ifndef NVCAM_H
#define NVCAM_H
#include <stdint.h>
#include <gphoto2/gphoto2-camera.h>

class NVCamera {
    
private:
    Camera *camera;
    GPContext *context;
    CameraFile *camfile;
    void DetectCamera();
public:
    uint32_t ShotToMemory(uint8_t **buff);
    uint32_t PreviewToMemory(uint8_t **buff);
    uint32_t CameraConnected();
    NVCamera();
    ~NVCamera();
    
};


#endif /* NVCAM_H */

