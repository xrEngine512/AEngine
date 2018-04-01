#include "ObjMeshLoader.h"
#include "AABB.h"
#include "IObjectFactory.h"

#include <ApproxSystemErrors.h>
#include <algorithm>
#include <fstream>

using namespace std;
using namespace experimental;
using namespace Math;


struct {
    std::vector<DefaultVertex> vertices;
    std::vector<index_t> indices;
} buffer;


inline void AddVertex(DefaultVertex vertex) {
	//Поиск существующей вершины
    auto it = find(buffer.vertices.begin(), buffer.vertices.end(), vertex);

	//Добавление
    if(it == buffer.vertices.end()) {
        buffer.vertices.push_back(vertex);
        it = buffer.vertices.end() - 1;
    }
    buffer.indices.push_back(static_cast<index_t>(it - buffer.vertices.begin()));
}
//---------------------------------------------------------------------------
IGeometryBufferPtr ObjMeshLoader::Load(const string_view &meshFileName)
{
    auto file = ifstream(meshFileName.data());

    if (!file)
        throw approx_exception("Error opening obj file", "ObjMeshLoader");

    //Создание временного хранилища входящих данных, как только данные будут
    //загружены в подходящий формат скопируем эти данные в вершинный и индексный буферы
    auto positions = vector<AVector>(1024);
    auto texCoords = vector<AVector2>(1024);
    auto normals = vector<AVector3>(1024);
    auto aabb = AABB();

    // Ввод данных из файла
    string str_command = "";

    while (true)
    {
        file >> str_command;
        if( !file )
            break;

        if(str_command == "#")
        {
            // Комментарий
        }
        else if(str_command == "v")
        {
            // Координаты
            float x, y, z;
            file >> x >> y >> z; float c=1.0f;

            x *= c;
            y *= c;
            z *= c;

            if (x > aabb.right)
                aabb.right = x;
            else if (x < aabb.left)
                aabb.left = x;

            if (y > aabb.top)
                aabb.top = y;
            else if (y < aabb.bottom)
                aabb.bottom = y;

            if (z > aabb.back)
                aabb.back = z;
            else if (z < aabb.front)
                aabb.front = z;

			positions.push_back(AVector(x, y, z, 1.0f));
        }
        else if(str_command == "vt")
        {
            float u, v;
            file >> u >> v;
			texCoords.push_back(AVector2(u,-v));
        }
        else if(str_command == "vn")
        {
            float x, y, z;
            file >> x >> y >> z;
            normals.push_back(AVector3(x,y,z));
        }
        else if(str_command == "f")
        {
            // Face
            uint32_t iPosition, iTexCoord, iNormal;
            DefaultVertex vertex;

            for( uint32_t iFace = 0; iFace < 3; iFace++ )
            {
                file >> iPosition;
                vertex.data.position = positions[ iPosition - 1 ];

                if( '/' == file.peek() )
                {
                    file.ignore();

                    if( '/' != file.peek() )
                    {
                        file >> iTexCoord;
                        vertex.data.texture = texCoords[ iTexCoord - 1 ];
                    }

                    if( '/' == file.peek() )
                    {
                        file.ignore();

                        file >> iNormal;
                        vertex.data.normal = normals[ iNormal - 1 ];
                    }
                }

                AddVertex(vertex);
            }
        }
    }
    file.close();

    return getObjectFactory()->createGeometryBuffer(move(buffer.vertices), move(buffer.indices), aabb);
}

IMeshLoader& getMeshLoader() {
    static ObjMeshLoader instance;
    return instance;
}
