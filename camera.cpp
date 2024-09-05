#include "camera.h"
#include "options.h"
#include "ui_camera.h"
#include <QCameraInfo>  // Ensure this is included

camera::camera(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::camera),
    Camera(nullptr),  // Initialize pointers to nullptr
    imageCapture(nullptr),
    isCapturingImage(false)
{
    ui->setupUi(this);

    // Initialize the camera with the default camera device
    setCamera(QCameraInfo::defaultCamera());
}

camera::~camera()
{
    // Clean up
    delete Camera;
    delete imageCapture;
    delete ui;
}

void camera::setCamera(const QCameraInfo &cameraInfo)
{
    // Clean up any previous camera instance
    if (Camera) {
        Camera->stop();
        delete Camera;
    }
    if (imageCapture) {
        delete imageCapture;
    }

    // Create and set up the new camera
    Camera = new QCamera(cameraInfo);
    imageCapture = new QCameraImageCapture(Camera);

    Camera->setViewfinder(ui->viewfinder);

    // Connect signals and slots
    connect(imageCapture, &QCameraImageCapture::readyForCaptureChanged,
            this, &camera::readyForCapture);

    Camera->start();
}

void camera::readyForCapture(bool ready)
{
    ui->takeImageButton->setEnabled(ready);
}

void camera::on_takeImageButton_clicked()
{
    isCapturingImage = true;
    imageCapture->capture();
}
void camera::on_pb_back2_clicked()
{
    options *m = new options;
    m ->show();
    hide();

}
