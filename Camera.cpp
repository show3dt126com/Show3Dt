#include "Camera.h"
#include "Defines.h"


Camera::Camera():
    movementSpeed(SPEED),
    mouseSensitivity(SENSITIVITY),
    zoom(ZOOM)
{
    worldUp = QVector3D(0.0, 1.0, 0.0);
    timeForScrollProc.start();

    setViewPort(1000, 600);
    setOrthoOptions(500, 500, 10, 30000);
    setCameraPos(QVector3D(0.0, 0.0f, 10000.0));
    setCameraAttitude(0.0, -90.0, 0.0);
}

void Camera::setViewPort(double w, double h)
{
    viewportW = w;
    viewportH = h;
}

void Camera::updateCameraVectors()
{
    //转换为弧度制Radians
    double yawR = qDegreesToRadians(yaw);
    double pitchR = qDegreesToRadians(pitch);
    //GLfloat rollR = qDegreesToRadians(roll);

    QVector3D front3(float(cos(yawR)*cos(pitchR)), float(sin(pitchR)), float(sin(yawR)*cos(pitchR)));
    front = front3.normalized();
    QVector3D wUp = worldUp;
    if (abs(pitch+90) < 0.5 || abs(pitch-90) < 0.5)
        wUp = QVector3D(0.0, 0.0, -1.0);
    right = QVector3D::crossProduct(front, wUp).normalized();
    up = QVector3D::crossProduct(right, front).normalized();
}

// 设置摄像机的姿态角: 俯仰 偏转 滚动
void Camera::setCameraAttitude(double pitch, double yaw, double roll)
{
    this->yaw = yaw;
    this->pitch = pitch;
    this->roll = roll;
    updateCameraVectors();
}

// 设置摄像机在世界坐标中的位置
void Camera::setCameraPos(double x, double y, double z)
{
    position = QVector3D(float(x), float(y), float(z));
}

void Camera::setCameraPos(QVector3D xyz)
{
    position = xyz;
}

void Camera::setWorldUp(QVector3D worldUp)
{
    //QVector3D(0.0f, 1.0f, 0.0f);
    this->worldUp = worldUp;
}

QMatrix4x4 Camera::getViewMatrix()
{
    QMatrix4x4 view;
    view.lookAt(position, position + front, up);
    return view;
}

void Camera::setPerspectiveOptions(double nearPlane, double farPlane, double _zoom)
{
    zoom = _zoom;
    frustumNear = nearPlane;
    frustumFar = farPlane;
}

// get Project Matrix
QMatrix4x4 Camera::getPerspectiveMatrix()
{
    QMatrix4x4 projection;
    projection.perspective(float(zoom), float(viewportW/viewportH),
                           float(frustumNear), float(frustumFar));
    return projection;
}

void Camera::setOrthoOptions(double w, double h, double orthoNear, double orthoFar)
{
    orthoW = w;
    orthoH = h;
    this->orthoNear = orthoNear;
    this->orthoFar = orthoFar;
}

void Camera::setOrthoOptions(double w, double orthoNear, double orthoFar)
{
    orthoW = w;
    if (viewportW > 1)
        orthoH = w*viewportH/viewportW;
    else
        orthoH = w;
    this->orthoNear = orthoNear;
    this->orthoFar = orthoFar;
}

QMatrix4x4 Camera::getOrthoMatrix()
{
    QMatrix4x4 projection;
    projection.ortho(float(-orthoW/2.0), float(orthoW/2.0), float(-orthoH/2.0),
                     float(orthoH/2.0), float(orthoNear), float(orthoFar));
    return projection;
}

QMatrix4x4 Camera::getOrthoMatrix(double size)
{
    QMatrix4x4 projection;
    projection.ortho(float(-size/2.0), float(size/2.0), float(-size/2.0),
                     float(size/2.0), float(orthoNear), float(orthoFar));
    return projection;
}

double Camera::getZoom()
{
    return orthoW/(viewportW +1.0);
}

QMatrix4x4 Camera::getTextMatrix(double w, double h, GLboolean isHorz)
{
    QMatrix4x4 projection;
    projection.ortho(0, float(w), 0, float(h), 0, 1);
    if (! isHorz)
        projection.rotate(90.0, 0.0f, 0.0, 1.0);
    return projection;
}

QMatrix4x4 Camera::getScrnLineMatrix(double w, double h)
{
    QMatrix4x4 projection;
    if (w > 0)
        projection.ortho(0, float(w), 0, float(h), 0, 1);
    else
        projection.ortho(0, float(viewportW), 0, float(viewportH), 0, 1);
    return projection;
}

void Camera::processKeyboard(ECameraMovement direction, double deltaTime)
{
    double velocity = movementSpeed * deltaTime * 0.01;
    //qDebug() << "velocity" << velocity << "direction" << direction;

    switch (direction)
    {
    // 可用于所有视图，仅仅移动相机位置, 用4个箭头和pgwn pgup
    case FORWARD:
        position += front * float(velocity);
        break;
    case BACKWARD:
        position -= front * float(velocity);
        break;

    case LEFT:
        position -= right * float(velocity * orthoW);
        break;
    case RIGHT:
        position += right * float(velocity * orthoW);
        break;
    case UP:
        position += up * float(velocity * orthoH);
        break;
    case DOWN:
        position -= up * float(velocity * orthoH);
        break;

    // 所有视图，左右移动相机并且旋转，用[]，同鼠标左右拖动
    case DRAGLEFT:
        procRotateYaw(velocity*25.0);
        break;
    case DRAGRIGHT:
        procRotateYaw(-velocity*25.0);
        break;

    // 所有视图，上下移动相机并且旋转,用(/9 )/0，同鼠标上下拖动
    case DRAGUP:
        procRotatePitch(velocity*25.0);
        break;
    case DRAGDOWN:
        procRotatePitch(-velocity*25.0);
        break;

    // 所有视图，放大缩小，用 - +/=，同鼠标滚轮
    case ZOOMIN:
        orthoW *= movementSpeed*0.9;
        orthoH *= movementSpeed*0.9;
        break;
    case ZOOMOUT:
        orthoW *= movementSpeed*1.1;
        orthoH *= movementSpeed*1.1;
        break;
    }

    updateCameraVectors();
}


void Camera::procRotateYaw(double dYaw)
{
    QMatrix4x4 m;
    m.rotate(float(-dYaw), 0.0f, 1.0f, 0.0f);
    QVector3D v(position.x(), 0.0, position.z());
    QVector3D v1 = m*v;
    //position -= right * (xoffset / viewportW *orthoW);
    position.setX(v1.x());
    position.setZ(v1.z());
    yaw += dYaw;
    //qDebug() << "procRotateYaw(GLfloat dYaw)" << dYaw;
}

void Camera::procRotatePitch(double dPitch)
{
    QMatrix4x4 m;
    m.rotate(float(dPitch), 1.0f, 0.0f, 0.0f);
    QVector3D v(0.0f, position.y(), position.z());
    QVector3D v1 = m * v;
    //position -= right * (xoffset / viewportW *orthoW);
    position.setY(v1.y());
    position.setZ(v1.z());
    pitch += dPitch;
    //qDebug() << "procRotatePitch(GLfloat dPitch)" << dPitch;
}

void Camera::processMouseMovement(double xoffset, double yoffset, GLboolean constraintPitch)
{
    // do horz roll
    if (abs(xoffset) > abs(yoffset))
    {
        double dYaw = xoffset / viewportW * R2D;
        procRotateYaw(dYaw);
    }

    // do vert roll
    else
    {
        double dPitch = -yoffset / viewportH * R2D;
        procRotatePitch(dPitch);
        //position -= up * (yoffset / viewportH *orthoH);
    }

    updateCameraVectors();

    /*
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitcth += yoffset;

    if (constraintPitch)
    {
        if (pitcth > 89.0f)
            pitcth = 89.0f;
        if (pitcth < -89.0f)
            pitcth = -89.0f;
    }
    updateCameraVectors();
    terrace->setCamera(this);
    */
}

void Camera::processMouseScroll(double yoffset)
{
    int elapse = timeForScrollProc.elapsed();
    if (elapse > 200)
    {
        timeForScrollProc.start();
        return;
    }
    else if (elapse < 60)
        return;
    timeForScrollProc.start();

    double co = 0.8;
    if (yoffset < 0)
        co = 1.2;
    orthoW *= co;
    orthoH *= co;
}

