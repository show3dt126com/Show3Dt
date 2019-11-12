#include "Spirit.h"
#include "Defines.h"
#include "Global.h"

#include <QFile>

//bool loadOBJ(const char *path, QVector<QVector3D> &out_positions,
// QVector<QVector2D> &out_uvs,QVector<QVector3D> &out_normals);
QOpenGLTexture* temp;

Spirit::Spirit()
{
    loaded = false;
}

/*
#define GL_POINTS                         0x0000
#define GL_LINES                          0x0001
#define GL_LINE_LOOP                      0x0002
#define GL_LINE_STRIP                     0x0003
#define GL_TRIANGLES                      0x0004
#define GL_TRIANGLE_STRIP                 0x0005
#define GL_TRIANGLE_FAN                   0x0006
#define GL_QUADS                          0x0007
#define GL_QUAD_STRIP                     0x0008
#define GL_POLYGON
*/

bool Spirit::init(const QString& path)
{
    /********** 更换模型时 删除上一个模型的数据 节约内存 ***********/
    for (int i = 0; i < articels.size(); ++i)
    {
        C->glDeleteBuffers(1, &articels[i].positionVBO);
        C->glDeleteBuffers(1, &articels[i].uvVBO);
        C->glDeleteBuffers(1, &articels[i].normalVBO);
    }
    articels.clear();
    map_materials.clear();

    /*********** 初始化模型数据 ***************/
    bool res = loadSpirit(path);
    if (res)
    {
        bindBufferData();
    }

    loaded = res;
    return res;
}

bool Spirit::loadSpirit(const QString& path)
{
    qDebug() << "loadOBJ" << path;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "OBJLOADER ERROR::FILE CAN NOT OPEN!" << path;
        file.close();
        return false;
    }

    QTextStream in(&file);
    QString line;//文件流

    QVector<int> positionIndices, uvIndices, normalIndices;
    QVector<QVector3D> temp_positions;
    QVector<QVector2D> temp_uvs;
    QVector<QVector3D> temp_normals;
    QString temp_matName;//材质的名称

    while (!in.atEnd())
    {
        line = in.readLine();//读取一行,还有读取所有readAll();
        QStringList list = line.split(" ", QString::SkipEmptyParts);
        if (list.empty())
            continue;

        //qDebug() << list;
        //处理材质文件，即处理图片纹理
        if (list[0] == "mtllib")
        {
            /******* 1.1 处理材质文件路径 *********/
            //":/models/res/models/huapen/penzi.obj"
            QString mtl_path = path;
            int tempIndex = path.lastIndexOf("/") + 1;
            mtl_path.remove(tempIndex, path.size() - tempIndex);
            //":/models/res/models/huapen/" 得到根目录路径，用来和材质文件名结合，生成正确路径
            //mtl_path += list[1];//得到材料路径":/models/res/models/huapen/penzi.mtl"
            //      qDebug() << mtl_path;

            /******* 1.2 读取材质文件，导入Material类中 *********/
            qDebug() << "mtl_path+list[1]" << mtl_path + list[1];
            QFile mtl_file(mtl_path + list[1]); //正确的材质文件路径
            if (!mtl_file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                qDebug() << "OBJLOADER ERROR::MTL_FILE CAN NOT OPEN!";
                mtl_file.close();
                file.close();
                return false;
            }
            QTextStream mtl_in(&mtl_file);
            QString mtl_line;//读取材质文件流的一行

            Material material;
            QString matName;//材质的名称
            while (!mtl_in.atEnd())
            {
                //读取一行,还有读取所有readAll();
                mtl_line = mtl_in.readLine();
                //以“空格”与“\t”为识别符号，分开字符串
                QStringList mtl_list = mtl_line.split(QRegExp("\\s+"), QString::SkipEmptyParts);

                if (mtl_list.empty())
                    continue;
                if (mtl_list[0] == "newmtl")
                {
                    matName = mtl_list[1];
                    map_materials[matName] = material;
                }
                else if (mtl_list[0] == "Ns")
                {
                    double shininess = mtl_list[1].toDouble();
                    map_materials[matName].shininess = shininess;
                }
                else if (mtl_list[0] == "Ka")
                {
                    double x = mtl_list[1].toDouble();
                    double y = mtl_list[2].toDouble();
                    double z = mtl_list[3].toDouble();
                    QVector3D Ka(x, y, z);
                    map_materials[matName].Ka = Ka;
                }
                else if (mtl_list[0] == "Kd")
                {
                    double x = mtl_list[1].toDouble();
                    double y = mtl_list[2].toDouble();
                    double z = mtl_list[3].toDouble();
                    QVector3D Kd(x, y, z);
                    map_materials[matName].Kd = Kd;
                }
                else if (mtl_list[0] == "Ks")
                {
                    double x = mtl_list[1].toDouble();
                    double y = mtl_list[2].toDouble();
                    double z = mtl_list[3].toDouble();
                    QVector3D Ks(x, y, z);
                    map_materials[matName].Ks = Ks;
                }
                else if (mtl_list[0] == "map_Ka")
                {
                    qDebug() << "loadTexture Ka" << mtl_list[1] << mtl_path + mtl_list[1];
                    Texture2D texture2D;
                    texture2D.generate(mtl_path + mtl_list[1]);
                    mapTextures[mtl_list[1]] = texture2D;
                    map_materials[matName].name_map_Ka = mtl_list[1];
                }
                else if (mtl_list[0] == "map_Kd")
                {
                    qDebug() << "loadTexture Kd" << mtl_list[1] << mtl_path + mtl_list[1];
                    Texture2D texture2D;
                    texture2D.generate(mtl_path + mtl_list[1]);
                    mapTextures[mtl_list[1]] = texture2D;
                    map_materials[matName].name_map_Kd = mtl_list[1];
                }
            }
            /******* 1.2 读取材质文件，导入Material类中 *********/
        }

        else if (list.size() > 1 && list[1] == "object") //扫描寻找object
        {
            if (!articels.empty())
            {
                for (int i = 0; i < positionIndices.size(); i++)
                {
                    //得到索引
                    int posIndex = positionIndices[i];
                    int uvIndex = uvIndices[i];
                    int norIndex = normalIndices[i];

                    //根据索引取值
                    //          QVector3D pos = temp_positions[posIndex-1];
                    //          QVector2D uv = temp_uvs[uvIndex-1];
                    //          QVector3D nor = temp_normals[norIndex-1];
                    //qDebug() << uvIndex;
                    //          //存进object对象
                    //          objects.last().positions.push_back(pos);
                    //          objects.last().uvs.push_back(uv);
                    //          objects.last().normals.push_back(nor);
                    //根据索引取值
                    QVector3D pos = temp_positions[posIndex - 1];
                    articels.last().positions.push_back(pos);

                    QVector3D nor = temp_normals[norIndex - 1];
                    articels.last().normals.push_back(nor);

                    if (uvIndex != 0)
                    {
                        QVector2D uv = temp_uvs[uvIndex - 1];
                        articels.last().uvs.push_back(uv);
                    }
                }
                articels.last().matName = temp_matName;
                positionIndices.clear();
                uvIndices.clear();
                normalIndices.clear();
            }

            Article object;
            articels.push_back(object);//obj模型文件中的第一个object对象，因为一个obj模型可能还有多个object对象
        }

        else if (list[0] == "v")
        {
            double x = list[1].toDouble();
            double y = list[2].toDouble();
            double z = list[3].toDouble();

            QVector3D pos;
            pos.setX(x);
            pos.setY(y);
            pos.setZ(z);
            temp_positions.push_back(pos);
        }

        else if (list[0] == "vt")
        {
            double x = list[1].toDouble();
            double y = list[2].toDouble();

            QVector2D uv;
            uv.setX(x);
            uv.setY(y);
            temp_uvs.push_back(uv);
        }

        else if (list[0] == "vn")
        {
            double x = list[1].toDouble();
            double y = list[2].toDouble();
            double z = list[3].toDouble();

            QVector3D nor;
            nor.setX(x);
            nor.setY(y);
            nor.setZ(z);
            temp_normals.push_back(nor);
        }

        else if (list[0] == "usemtl")
        {
            temp_matName = list[1];
            //qDebug() << list[1];
        }

        else if (list[0] == "f")
        {
            if (list.size() > 4)
            {
                qDebug() << "OBJLOADER ERROR::THE LOADER ONLY SUPPORT THE TRIANGLES MESH!" << endl;
                file.close();
                return false;
            }
            for (int i = 1; i < 4; ++i) //读取处理 f字符后边的 三长串字符，如“f 2396/2442/2376 101/107/111 100/106/110”
            {
                QStringList slist = list[i].split("/");
                int posIndex = slist[0].toInt();
                int uvIndex = slist[1].toInt();
                int norIndex = slist[2].toInt();

                positionIndices.push_back(posIndex);
                uvIndices.push_back(uvIndex);
                normalIndices.push_back(norIndex);
                //qDebug() <<posIndex << " " << uvIndex << " " << norIndex;
            }
        }
    }

    //处理最后一个object
    for (int i = 0; i < positionIndices.size(); i++)
    {
        //得到索引
        int posIndex = positionIndices[i];
        int uvIndex = uvIndices[i];
        int norIndex = normalIndices[i];

        //根据索引取值
        QVector3D pos = temp_positions[posIndex - 1];
        articels.last().positions.push_back(pos);

        QVector3D nor = temp_normals[norIndex - 1];
        articels.last().normals.push_back(nor);

        if (uvIndex != 0)
        {
            QVector2D uv = temp_uvs[uvIndex - 1];
            articels.last().uvs.push_back(uv);
        }
        //qDebug() <<posIndex << " " << uvIndex << " " << norIndex;
    }
    articels.last().matName = temp_matName;

    file.close();
    return true;
}

void Spirit::bindBufferData()
{
    for (int i = 0; i < articels.size(); ++i)
    {
        C->glGenBuffers(1, &articels[i].positionVBO);
        C->glBindBuffer(GL_ARRAY_BUFFER, articels[i].positionVBO);
        //    qDebug() << objects[i].positions.size();
        C->glBufferData(GL_ARRAY_BUFFER, articels[i].positions.size() * sizeof(QVector3D),
                           &articels[i].positions[0], GL_STATIC_DRAW);

        C->glGenBuffers(1, &articels[i].uvVBO);
        C->glBindBuffer(GL_ARRAY_BUFFER, articels[i].uvVBO);
        C->glBufferData(GL_ARRAY_BUFFER, articels[i].uvs.size() * sizeof(QVector2D),
                           &articels[i].uvs[0], GL_STATIC_DRAW);

        C->glGenBuffers(1, &articels[i].normalVBO);
        C->glBindBuffer(GL_ARRAY_BUFFER, articels[i].normalVBO);
        C->glBufferData(GL_ARRAY_BUFFER, articels[i].normals.size() * sizeof(QVector3D),
                           &articels[i].normals[0], GL_STATIC_DRAW);
        // qDebug() << i << " : " << objects[i].positionVBO << " " << objects[i].uvVBO << " " << objects[i].normalVBO;
    }
}

void Spirit::draw(GLboolean isOpenLighting)
{
    G.shaderSpirit.bind();
    G.shaderSpirit.setBool("isOpenLighting", isOpenLighting);

    //qDebug() << "Spirit::draw" << articels.size();
    for (int i = 0; i < articels.size(); ++i)
    {
        //

        G.shaderSpirit.setVector3f("material.Ka", map_materials[articels[i].matName].Ka);
        if (!map_materials.empty())
        {
            C->glActiveTexture(GL_TEXTURE0);
            QString mapKa = map_materials[articels[i].matName].name_map_Ka;
            if (mapKa != "")
                mapTextures[mapKa].bind();

            if (isOpenLighting)
            {
                //开启光照效果
                C->glActiveTexture(GL_TEXTURE1);
                QString mapKd = map_materials[articels[i].matName].name_map_Kd;
                if (mapKd != "")
                    mapTextures[mapKd].bind();

                G.shaderSpirit.setVector3f("material.Kd", map_materials[articels[i].matName].Kd);
                G.shaderSpirit.setVector3f("material.Ks", map_materials[articels[i].matName].Ks);
                G.shaderSpirit.setFloat("material.shininess", map_materials[articels[i].matName].shininess);
            }
        }

        C->glEnableVertexAttribArray(0);
        C->glBindBuffer(GL_ARRAY_BUFFER, articels[i].positionVBO);
        C->glVertexAttribPointer(
            0,                  // attribute
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );

        C->glEnableVertexAttribArray(1);
        C->glBindBuffer(GL_ARRAY_BUFFER, articels[i].uvVBO);
        C->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        C->glEnableVertexAttribArray(2);
        C->glBindBuffer(GL_ARRAY_BUFFER, articels[i].normalVBO);
        C->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        //绘图
        C->glDrawArrays(GL_TRIANGLES, 0, articels[i].positions.size());
    }
}
