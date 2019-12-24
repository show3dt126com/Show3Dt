#include "Global.h"
#include "Scene.h"
#include <QPalette>
#include <QDebug>
#include "Defines.h"

// 标尺，比例尺参数
#define LengendLen  200.0f


// 可以操作3D画面的按键
CameraMoveKey CameraMoveKeyMan::moveKeys[CameraMoveKeyMAX] =
{
    { FORWARD,  Qt::Key_PageUp, 0},
    { BACKWARD, Qt::Key_PageDown, 0},

    { CLOCK,    Qt::Key_Comma, 0},
    { UCLOCK,   Qt::Key_Period, 0},

    { LEFT,     Qt::Key_Left, 0},
    { RIGHT,    Qt::Key_Right, 0},
    { UP,       Qt::Key_Up, 0},
    { DOWN,     Qt::Key_Down, 0},

    { DRAGLEFT, Qt::Key_BracketLeft, 0},
    { DRAGRIGHT, Qt::Key_BracketRight, 0 },

    { DRAGUP,   Qt::Key_0, 0},
    { DRAGDOWN, Qt::Key_9, 0},

    { ZOOMIN,   Qt::Key_Equal, 0},
    { ZOOMOUT,  Qt::Key_Minus, 0}
};

CameraMoveKeyMan::CameraMoveKeyMan()
{
    for (int i=0; i<KEY_MAX; i++)
        moveKeys[i].pressed = GL_FALSE;
}

void CameraMoveKeyMan::procKeyPressed(int key)
{
    for (int i=0; i<KEY_MAX; i++)
    {
        if (moveKeys[i].key == key)
        {
            moveKeys[i].pressed = GL_TRUE;
            break;
        }
    }
}

void CameraMoveKeyMan::procKeyReleased(int key)
{
    for (int i=0; i<KEY_MAX; i++)
    {
        if (moveKeys[i].key == key)
        {
            moveKeys[i].pressed = GL_FALSE;
            break;
        }
    }
}

GLboolean CameraMoveKeyMan::isDoingMove(ECameraMovement move)
{
    for (int i=0; i<KEY_MAX; i++)
    {
        if (moveKeys[i].move == move)
            return moveKeys[i].pressed;
    }
    return GL_FALSE;
}

Scene::Scene(QWidget* parent) : QOpenGLWidget(parent)
{
    init();
    glInited = false;
}

Scene::~Scene()
{

}

int Scene::init()
{
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
    return 1;
}

void Scene::initializeGL()
{
    qDebug() << "initializeGL";
    C = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_0_Core>();

    //ResourceManager::init();
    G.initGL();

    // setMouseTracking(true);

    qDebug() << "\r\nOpenGL Version "
             << (char *)C->glGetString(GL_VENDOR)
             << (char *)C->glGetString(GL_RENDERER)
             << (char *)C->glGetString(GL_VERSION)
             //<< (char *)C->glGetString(GL_EXTENSIONS)
             << "\r\n";

    //C->glEnable(GL_DEPTH_TEST);
    //C->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //C->glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    deltaTime = 0.0;
    lastFrame = 0.0;
    scrLineVPCNum = 0;

    isFirstMouse = GL_TRUE;
    isLeftMousePress = GL_FALSE;
    lastX = GLint(width() / 2.0);
    lastY = GLint(height() / 2.0);

    timeForKeyPressProc.start();

    /************ obj模型 ***********/
    //modelObject = new Model();
    //objModel->init("H:/Study/OpenGLEx/objModelLoading/res/models/biwutai/biwutai.obj");
    //modelObject->init("G:/Projects/SeaSoundView/Model3Ds/huapen/penzi.obj");
    //objModel->init("G:/Projects/SeaSoundView/Model3Ds/camera/file.obj");


    /************ 背景颜色参数调控 ***********/
    C->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    C->glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    C->glEnable(GL_DEPTH_TEST);

    // 激活合适的渲染状态， textShader
    C->glEnable(GL_BLEND);
    C->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //seaSound.makeTriangleFromCutCylinder();

    int coordXY0 = 10;
    addScrLine(VertexPC(coordXY0, coordXY0, 0.0f, 0.9f),
               VertexPC(coordXY0+LengendLen, coordXY0, 0.0f, 0.4f));
    addScrLine(VertexPC(coordXY0+LengendLen, coordXY0-1, 0.0f, 0.4f),
               VertexPC(coordXY0+LengendLen, coordXY0+5.0f, 0.0f, 0.4f));
    addScrLine(VertexPC(coordXY0, coordXY0, 0.0f, 0.9f),
               VertexPC(coordXY0, coordXY0+LengendLen, 0.0f, 0.4f));
    addScrLine(VertexPC(coordXY0-1, coordXY0+LengendLen, 0.0f, 0.4f),
               VertexPC(coordXY0+5.0f, coordXY0+LengendLen, 0.0f, 0.4f));

    pickMode = EPPM_CLICK;
    showPickData = false;

    glInited = true;
}

void Scene::procBBSMessage(BBSMessage bbsMsg)
{

}

void Scene::resizeGL(int w, int h)
{
    //qDebug() << "resizeGL";
    viewRect = QRect(0,0, w, h);
    C->glViewport(0, 0, w, h);

    textProjVert = G.camera.getTextMatrix(w, h, GL_FALSE);
    textProjHorz = G.camera.getTextMatrix(w, h, GL_TRUE);
}

void Scene::paintGL()
{
    C->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //C->glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    C->glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    /*********** 计算两次帧数之间的时间间隔  ***************/
    GLfloat currentFrame = GLfloat(timeForKeyPressProc.elapsed() / 100.0);
    deltaTime = currentFrame - lastFrame;
    //if (deltaTime > 3.0f)
    {
        lastFrame = currentFrame;
        processInput(deltaTime);
    }

    //
    updateGL();

    return;

    if (G.showSky)
    {
        G.shaderSky.bind();
        G.sky.draw();
    }

    if (G.showDem)
    {
        G.shaderDem.bind();
        G.dem.draw();
    }

    // 绘制水声场
    G.shaderCube.bind();
    for (int i=0; i<G.modelMan.models.count(); i++)
    {
        G.modelMan.models[i]->draw();

    }
    //G.modelMan.models.[0].drawSeaSound();

    // 绘制海底
    if (G.showBathy)
    {
        //G.shaderDem.bind();
        //seaSound.drawBathy();
    }

    C->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // 绘制边界线
    G.shaderLine.bind();
    //seaSound.drawLines();

    // 绘制文本信息
    G.shaderText.bind();

    //G.shaderText.setMatrix4f("projection", textProjHorz);
    static int count = 0;
    count ++;
    // QString countS = QString("C={%1}").arg(count);
    //textDrawer.renderText(countS.toLatin1().data(),
    //        100.0f, 20.0f, 0.5f, QVector3D(0.9f, 0.2f, 0.2f));

    QString lengendX;
    double lengendXV = double(LengendLen*G.camera.orthoW/width());
    if (lengendXV > 10000.0)
        lengendX= QString("%1(km)").arg(lengendXV/1000.0, 5, 'f', 0);
    else if (lengendXV > 1000.0)
        lengendX= QString("%1(km)").arg(lengendXV/1000.0, 5, 'f', 1);
    else
        lengendX= QString("%1(m)").arg(lengendXV, 5, 'f', 0);
    G.texter.renderText(lengendX.toLatin1().data(), 20.0f, 15.0f, 0.5f,
                          QVector3D(0.9f, 0.2f, 0.2f));

    if (showPickData)
    {
        //QString mouseXY = QString("X=%1 Y=%2").arg(mposx).arg(mposy);
        char buf[300];
        sprintf(buf, "%d %d P0(%3.1f,%3.1f,%3.1f) P1(%3.1f,%3.1f,%3.1f)",
                mposx, mposy,
                mpos0.x(),mpos0.y(),mpos0.z(),
                mpos1.x(), mpos1.y(), mpos1.z());
        G.texter.renderText(buf, 150.0f, 20.0f, 0.5f, QVector3D(0.9f, 0.9f, 0.2f));
    }

    // 垂直方向，左下角为 0，0，x向上为正，y向右为负
    double lengendYV = double(LengendLen*G.camera.orthoH/height());
    QString lengendY;
    if (lengendYV > 10000.0)
        lengendY = QString("%1(km)").arg(lengendYV/1000.0, 5, 'f', 0);
    else if (lengendYV > 1000.0)
        lengendY = QString("%1(km)").arg(lengendYV/1000.0, 5, 'f', 1);
    else
        lengendY = QString("%1(m)").arg(lengendYV, 5, 'f', 0);
    G.shaderText.setMatrix4f("projection", textProjVert);
    G.texter.renderText(lengendY.toLatin1().data(), 40.0f, -35.0f, 0.5f,
                          QVector3D(0.9f, 0.2f, 0.2f));

    // 绘制图例
    G.shaderLine.bind();
    drawScrLines();

    // 绘制声呐辐射线
    //ResourceManager::getShader("sonar").bind();
    //seaSound.drawSonar();

}

void Scene::updateGL()
{

    /*
    if (!seaSound.drawPrepared)
        seaSound.prepareVertexArrayObject();
    if (!seaSound.bathyDrawPrepared)
        seaSound.prepareBathyVertexArrayObject();

    if (!skyBox.drawPrepared)
        skyBox.prepareDraw();
    if (!backdropDem.drawPrepared)
        backdropDem.prepareDraw();

    //if (!seaSound.drawSonarLinePrepared)
    //    seaSound.prepareSonarLine();
    if (!seaSound.drawSonarBodyPrepared)
        seaSound.prepareSonarBody();

    // 代表声呐的正20面体
    //seaSound.prepareSonarTriAngle();

    if (isLineMode)
        C->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        C->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //C->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    camera.setViewPort((GLfloat)width(), (GLfloat)height());
    //camera.setPerspectiveOptions(0.1f, 20000.0f, 50.0f);
    //projection = camera.getPerspectiveMatrix();
    waterProj = camera.getOrthoMatrix();
    waterView = camera.getViewMatrix();

    Shader & shaderSky = ResourceManager::getShader("sky").bind();
    shaderSky.setMatrix4f("projection", waterProj);
    shaderSky.setMatrix4f("view", waterView);

    Shader & shaderDem = ResourceManager::getShader("dem").bind();
    shaderDem.setMatrix4f("projection", waterProj);
    shaderDem.setMatrix4f("view", waterView);

    Shader & shaderBulb = ResourceManager::getShader("bulb").bind();
    shaderBulb.setMatrix4f("projection", waterProj);
    shaderBulb.setMatrix4f("view", waterView);

    Shader & shaderSpirit = ResourceManager::getShader("spirit").bind();
    shaderSpirit.setMatrix4f("projection", waterProj);
    shaderSpirit.setMatrix4f("view",waterView);
    shaderSpirit.setVector3f("viewPos", camera.position);

    QMatrix4x4 spiritModel;
    spiritModel.scale(0.4f);//modelScaling*1000.0);
    spiritModel.rotate(40, 1, 0, 0);
    spiritModel.translate(-1700, -1700, 0);
    shaderSpirit.setMatrix4f("model", spiritModel);

    Shader & shaderWater = ResourceManager::getShader("water").bind();
    shaderWater.setMatrix4f("projection", waterProj);
    shaderWater.setMatrix4f("view",waterView);
    shaderWater.setInteger("zebraNum", seaSound.zebraNum);
    //shaderWater.set

    shaderWater.setInteger("colorMapNum", GD.colorMapColorNum);
    shaderWater.setFloatArray("colorMap", &GD.colorMap[0][0],
            GD.colorMapColorNum, 4);
    shaderWater.setInteger("zebraNum", GD.colorZebraNum);
    shaderWater.setFloatArray("colorZebra", &GD.colorZebra[0][0],
            GD.colorZebraNum, 4);

    waterModelView = waterView*waterModel;

    Shader & shaderLine = ResourceManager::getShader("line").bind();
    shaderLine.setMatrix4f("projection", waterProj);
    shaderLine.setMatrix4f("view",waterView);

    Shader & shaderBathy = ResourceManager::getShader("bathy").bind();
    shaderBathy.setMatrix4f("projection", waterProj);
    shaderBathy.setMatrix4f("view",waterView);

    // sonar
    Shader & shaderSonar = ResourceManager::getShader("sonar").bind();
    // 旋转轴，轮流转若干次
    QVector3D rs[3] = {{1,0,0}, {0,1,0}, {0,0,1}};
    static int rsi = 0;
    int rsm = 14;
    rsi = (rsi++) % (rsm*3);
    seaSound.sonarModelMatrix.rotate(6.0f, rs[int( rsi / rsm)]);

    QMatrix4x4 sonarModel;
    if (seaSound.seaSoundAzNum > 0)
        sonarModel.translate(0.0f, -seaSound.sd, 0.0f);
    //sonarModel.translate(0.0f, -100.0f, 0.0f);
    static GLfloat co = 8.0f;
    co *= 1.2f;
    // 让辐射线飞远，然后再开始下一轮
    if (co > 4800.0f)
        co = 10.0f;
    sonarModel.scale(co * camera.orthoW / camera.viewportW);
    if (co < 10.1f)
        sonarModel *= seaSound.sonarModelMatrix;
    //QMatrix4x4 sonarModel = seaSound.sonarModelMatrix;
    shaderSonar.setMatrix4f("model", sonarModel);
    shaderSonar.setMatrix4f("projection", waterProj);
    shaderSonar.setMatrix4f("view",waterView);
    static GLfloat prog = 0.1f;
    prog += 0.05f;
    if (prog > 1.0f)
        prog -= 1.0f;
    //qDebug() << "prog" << prog;
    shaderSonar.setFloat("prog", prog);

    // sonar body
    Shader & shaderSonarBody = ResourceManager::getShader("sonarbody").bind();
    QMatrix4x4 sonarModelBody;
    if (seaSound.seaSoundAzNum > 0)
        sonarModelBody.translate(0.0f, -seaSound.sd, 0.0f);
    //sonarModelBody.translate(0.0f, -100.0f, 0.0f);
    sonarModelBody.scale(10.0f * camera.getZoom());
    shaderSonarBody.setMatrix4f("model", sonarModelBody);
    //QMatrix4x4 projSonarBody;
    //projSonarBody = camera.getOrthoMatrix(100.0f);
    shaderSonarBody.setMatrix4f("projection", waterProj);
    shaderSonarBody.setMatrix4f("view",waterView);
    */
}

int Scene::addScrLine(VertexPC vs, VertexPC ve)
{
    scrLineVPC[scrLineVPCNum++] = vs;
    scrLineVPC[scrLineVPCNum++] = ve;
    return 1;
}

int Scene::drawScrLines()
{
    C->glDisable(GL_DEPTH_TEST);
    C->glLineWidth(2);
    C->glEnable(GL_LINE_SMOOTH);

    GLuint VBO;
    C->glGenBuffers(1, &VBO);
    C->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    C->glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPC)*scrLineVPCNum, scrLineVPC, GL_STATIC_DRAW);

    C->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPC), (void*)0);
    C->glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(VertexPC), (void*)(3 * sizeof(GLfloat)));

    C->glEnableVertexAttribArray(0);
    C->glEnableVertexAttribArray(1);

    C->glDrawArrays(GL_LINES, 0, scrLineVPCNum);
    C->glBindBuffer(GL_ARRAY_BUFFER, 0);
    C->glDeleteBuffers(1, &VBO);

    C->glLineWidth(1);
    C->glEnable(GL_DEPTH_TEST);
    C->glDisable(GL_LINE_SMOOTH);
    return 1;
}

// deltaTime
void Scene::processInput(GLfloat dt)
{
    //qDebug() << "void Scene::processInput(GLfloat dt)" << dt;
    for (int m=0; m<KEY_MAX; m++)
    {
        if (cameraMoveKeyMan.isDoingMove(ECameraMovement(m)))
            G.camera.processKeyboard(ECameraMovement(m), dt);
    }
}

void Scene::pickPointerData(QMouseEvent* event)
{
    /*
    mposx = event->pos().x();
    mposy = viewRect.height() - event->pos().y();

    QVector3D p0(mposx, mposy, 0.0f);
    mpos0 = p0.unproject(waterModelView, waterProj, viewRect);
    QVector3D p1(mposx, mposy, 0.99f);
    mpos1 = p1.unproject(waterModelView, waterProj, viewRect);

    QVector4D posDb;
    int r = seaSound.reflectPointerData(mpos0, mpos1, posDb);

    char buf[300];
    static int c = 0;
    sprintf(buf, "  %d ==> P0(%3.1f,%3.1f,%3.1f) P1(%3.1f,%3.1f,%3.1f) "
                 "r=%d P&DB(%3.1f,%3.1f,%3.1f) = %3.1f\n",
            c++, mpos0.x(), mpos0.y(), mpos0.z(), mpos1.x(), mpos1.y(), mpos1.z(),
            r, posDb.x(), posDb.y(), posDb.z(), posDb.w());
    QString pointerData(buf);

    emit addPointerData(pointerData);
    */
}

//键盘按下事件
void Scene::keyPressEvent(QKeyEvent* event)
{
    int key = event->key();
    cameraMoveKeyMan.procKeyPressed(key);
}

//键盘释放事件
void Scene::keyReleaseEvent(QKeyEvent* event)
{
    int key = event->key();
    cameraMoveKeyMan.procKeyReleased(key);
}

void Scene::mouseMoveEvent(QMouseEvent* event)
{
    if (pickMode == EPPM_MOVE)
        pickPointerData(event);

    if (! isLeftMousePress)
        return;
    GLint xpos = event->pos().x();
    GLint ypos = event->pos().y();

    if (isFirstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        isFirstMouse = GL_FALSE;
        return;
    }

    GLint xoffset = xpos - lastX;
    GLint yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;
    //G.camera.processMouseMovement(xoffset, yoffset);
}

void Scene::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) //注意是button（）不是buttons（）；
    {
        if (pickMode == EPPM_CLICK)
            pickPointerData(event);
        isLeftMousePress = GL_TRUE;
        setMouseTracking(true);
    }
}

void Scene::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) //注意是button（）不是buttons（）；
    {
        setMouseTracking(false);
        isLeftMousePress = GL_FALSE;
        isFirstMouse = GL_TRUE;
    }
}

void Scene::wheelEvent(QWheelEvent* event)
{
    QPoint offset = event->angleDelta();
    G.camera.processMouseScroll(offset.y());
}

/*
void Scene::setBestLookIso3D()
{
    if (seaSound.seaSoundAzNum == 0)
        return;

    GLfloat rrMax = seaSound.rrMax*GD.RR_UNIT;
    QRect r = geometry();
    int w = r.width();
    int h  =r.height();
    camera.setCameraPos(0.0f, rrMax*0.57f*1.5f, rrMax*1.5f);
    camera.setOrthoOptions(rrMax*1.5f*w/h, rrMax*1.5f, 1.0f, 1000000.0f);
    //amera.setWorldUp(QVector3D(0.0f, 1.0f, 0.0f));
    camera.setCameraAttitude(330.0f, -90.0f, 0.0f);
}

void Scene::setBestLookDown()
{
    if (seaSound.seaSoundAzNum == 0)
        return;

    GLfloat rrMax = seaSound.rrMax*GD.RR_UNIT;
    QRect r = geometry();
    int w = r.width();
    int h  =r.height();

    camera.setCameraPos(0.0f, rrMax*1.5f, 0.0f);
    camera.setOrthoOptions(rrMax*2.2f*w/h, rrMax*2.2f, 1.0f, 1000000.0f);
    //camera.setWorldUp(QVector3D(0.0f, 0.0f, -1.0f));
    camera.setCameraAttitude(-90.0f, -90.0f, 0.0f);
}

void Scene::setBestLookSide()
{
    if (seaSound.seaSoundAzNum == 0)
        return;

    double z = seaSound.rrMax*GD.RR_UNIT;
    if (seaSound.rdMax*GD.RD_UNIT > z)
        z = seaSound.rdMax*GD.RD_UNIT;
    QRect r = geometry();
    int w = r.width();
    int h  =r.height();

    camera.setCameraPos(0.0, -seaSound.rdMax*GD.RD_UNIT*0.5, z*1.5);
    camera.setOrthoOptions(z*1.1, z*1.1*h/w, 1.0, 1000000.0);
    //camera.setWorldUp(QVector3D(0.0f, 1.0f, 0.0f));
    camera.setCameraAttitude(0.0f, -90.0f, 0.0f);
}

// 水平的绕心旋转，保持指向
void Scene::horzRotate(GLfloat deg)
{

}

int Scene::addScrLine(VertexPC vs, VertexPC ve)
{
    scrLineVPC[scrLineVPCNum++] = vs;
    scrLineVPC[scrLineVPCNum++] = ve;
    return 1;
}

int Scene::drawScrLines()
{
    C->glDisable(GL_DEPTH_TEST);
    C->glLineWidth(2);
    C->glEnable(GL_LINE_SMOOTH);

    GLuint VBO;
    C->glGenBuffers(1, &VBO);
    C->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    C->glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPC)*scrLineVPCNum, scrLineVPC, GL_STATIC_DRAW);

    C->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPC), (void*)0);
    C->glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(VertexPC), (void*)(3 * sizeof(GLfloat)));

    C->glEnableVertexAttribArray(0);
    C->glEnableVertexAttribArray(1);

    C->glDrawArrays(GL_LINES, 0, scrLineVPCNum);
    C->glBindBuffer(GL_ARRAY_BUFFER, 0);
    C->glDeleteBuffers(1, &VBO);

    C->glLineWidth(1);
    C->glEnable(GL_DEPTH_TEST);
    C->glDisable(GL_LINE_SMOOTH);
    return 1;
}



*/
