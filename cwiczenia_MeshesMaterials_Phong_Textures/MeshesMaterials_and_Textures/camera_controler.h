#include "camera.h"

class CameraControler {
public:

    void rotate_camera(float dx, float dy) {
        camera_->rotate_around_center(-scale_ * dy, camera_->x());
        camera_->rotate_around_center(-scale_ * dx, camera_->y());
    }

    CameraControler():camera_(nullptr) {    //setting the scale to 0.01 will slow the rotation making the program way more usable
        scale_ = 0.01;
    }
    CameraControler(Camera* camera):camera_(camera) {
        scale_ = 0.01;
    }
    void set_camera(Camera *camera) { camera_ = camera; }

    void mouse_moved(float x, float y) {    //from CameraMovement README
        if (LMB_pressed_) {
            auto dx = x - x_;
            auto dy = y - y_;
            x_ = x;
            y_ = y;

            rotate_camera(dx, dy);
        }
    }
    void LMB_pressed(float x, float y) {    //from CameraMovement README
        LMB_pressed_ = true;
        x_ = x;
        y_ = y;
    }

    void LMB_released(float x, float y) {   //from CameraMovement README
        LMB_pressed_ = false;
        auto dx = x - x_;
        auto dy = y - y_;

        rotate_camera(dx, dy);
    }

private:
    float x_;
    float y_;

    Camera *camera_; 
    float scale_;
    bool LMB_pressed_ = false;  //setting mousepress to false by default

};