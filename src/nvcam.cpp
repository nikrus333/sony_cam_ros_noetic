#include "nvcam.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <gphoto2/gphoto2-camera.h>

using namespace std;

static void
ctx_error_func (GPContext *context, const char *str, void *data)
{
        fprintf  (stderr, "\n*** Contexterror ***              \n%s\n",str);
        fflush   (stderr);
}

static void
ctx_status_func (GPContext *context, const char *str, void *data)
{
        fprintf  (stderr, "%s\n", str);
        fflush   (stderr);
}

void NVCamera::DetectCamera() {
    int count;
    context = gp_context_new();
    gp_context_set_error_func (context, ctx_error_func, NULL);
    gp_context_set_status_func (context, ctx_status_func, NULL);
    
    CameraList	*camlist;
    if (gp_list_new(&camlist) != GP_OK) {
        camera = 0;
        return;
    }
    gp_list_reset(camlist);
    count = gp_camera_autodetect(camlist, context);
    if (count < GP_OK) {
        camera = 0;
        return;    
    }
    
    gp_camera_new(&camera);
    if (gp_camera_init(camera, context) != GP_OK) {
        camera = 0;
        return;
    }
    
    if (gp_file_new(&camfile) != GP_OK) {
        camera = 0;
        return;
    }
    
    return;
}

uint32_t NVCamera::ShotToMemory(uint8_t **buff) {
    uint32_t ret = 0;
    const char* ret_buf;
    unsigned long int size;
    CameraFilePath camera_file_path;    
    
    if (!camera) return 0;
    
    strcpy(camera_file_path.folder, "/");
    strcpy(camera_file_path.name, "temp.jpg");
    
    if (gp_camera_capture(camera, GP_CAPTURE_IMAGE, &camera_file_path, context) != GP_OK) {
        camera = 0;
        return 0;
    }
           
    if (gp_camera_file_get(camera, camera_file_path.folder, camera_file_path.name,
		     GP_FILE_TYPE_NORMAL, camfile, context) != GP_OK) {
        camera = 0;
        return 0;
    }       
    
    gp_file_get_data_and_size(camfile, &ret_buf, &size);
    ret = size;
    *buff = (uint8_t*)ret_buf;
    
    if (gp_camera_file_delete(camera, camera_file_path.folder, camera_file_path.name,
			context) != GP_OK) {
        camera = 0;
        return 0;
    }        
    
    return ret;
}

uint32_t NVCamera::PreviewToMemory(uint8_t **buff) {
    uint32_t ret = 0;
    const char* ret_buf;
    unsigned long int size;    
     
    
    if (gp_camera_capture_preview(camera, camfile, context) != GP_OK) {
        camera = 0;
        return 0;
    }

    gp_file_get_data_and_size(camfile, &ret_buf, &size);
    ret = size;
    *buff = (uint8_t*)ret_buf;    
    
    return ret;    
}

uint32_t NVCamera::CameraConnected() {
    if (camera) return 1;
    DetectCamera();
    if (camera) return 1;
    return 0;
}

NVCamera::NVCamera() {
    DetectCamera();        
}

NVCamera::~NVCamera() {
    if (camera)
        gp_camera_exit(camera, context);
    if (camfile)
        gp_file_free(camfile);
    camera = 0;
    context = 0;
}

