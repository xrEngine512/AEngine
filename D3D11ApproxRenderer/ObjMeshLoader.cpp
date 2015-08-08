#include <DirectXMath.h>
#include <ApproxSystemErrors.h>
using namespace DirectX;
#include "ObjMeshLoader.h"
#include "D3D11Common.h"
#include <fstream>
using namespace std;

ObjMeshLoader::ObjMeshLoader(void):buffer(nullptr)
{
}

DWORD ObjMeshLoader::AddVertex(int vertexI, VertexType vertex)
{
	//vertex.color = XMFLOAT4(0.5, 0.5, 0.5, 1);
	int res=-1;
    DWORD ind;
	//����� ������������ �������
	for (DWORD i=0; i<buffer->verticesI; i++)
		if (memcmp(&buffer->vertices[i],&vertex,sizeof(VertexType))==0) res=ind=i;
	//����������
	if (res<0) {
		buffer->vertices[buffer->verticesI++]=vertex; ind=buffer->verticesI-1; }
	return ind;
}
//---------------------------------------------------------------------------
void ObjMeshLoader::AddIndex(DWORD index)
{
    buffer->indices[buffer->indicesI++]=index;
}

int ObjMeshLoader::GetIndexCount()
{
    return buffer->indicesI+1;
}

int ObjMeshLoader::GetVertexCount()
{
    return buffer->verticesI+1;
}

void ObjMeshLoader::Inititalize(ID3D11Device* device,const char* meshFileName)
{
    m_device = device;

    wifstream InFile(meshFileName);
    
    
    if (!InFile) throw ApproxException(L"Error opening obj file", L"ObjMeshLoader");

    buffer = new ModelBuffer;
      // ������� ������
    buffer->verticesI=0;
    buffer->indicesI=0;

    //�������� ���������� ��������� �������� ������, ��� ������ ������ �����
    //��������� � ���������� ������ ��������� ��� ������ � ��������� � ��������� ������
    XMFLOAT3 * Positions=new XMFLOAT3[buffermax];
    XMFLOAT2 * TexCoords=new XMFLOAT2[buffermax];
    XMFLOAT3 * Normals=new XMFLOAT3[buffermax];

    // ������� ��� ��������
    int PositionsI=0;
    int TexCoordsI=0;
    int NormalsI=0;

    // ���� ������ �� �����
    //char fname[256];
    //strcpy(fname,Edit1->Text.c_str());
    WCHAR strCommand[256] = {0};

    for(; ; )
    {
        InFile >> strCommand;
        if( !InFile )
            break;

        if( 0 == wcscmp( strCommand, L"#" ) )
        {
            // �����������
        }
        else if( 0 == wcscmp( strCommand, L"v" ) )
        {
            // ����������
            float x, y, z;
            InFile >> x >> y >> z; float c=1.0f;

            x *= c;
            y *= c;
            z *= c;

            if (m_firstVertex)
            {
                m_MeshBox.left = m_MeshBox.right = x;
                m_MeshBox.bottom = m_MeshBox.top = y;
                m_MeshBox.front = m_MeshBox.back = z;
                m_firstVertex = false;
            }
            else
            {
                if (x > m_MeshBox.right)
                    m_MeshBox.right = x;
                if (x < m_MeshBox.left)
                    m_MeshBox.left = x;                

                if (y > m_MeshBox.top)
                    m_MeshBox.top = y;
                if (y < m_MeshBox.bottom)
                    m_MeshBox.bottom = y;

                if (z>m_MeshBox.back)
                    m_MeshBox.back = z;
                if (z < m_MeshBox.front)
                    m_MeshBox.front = z;
            }
            Positions[PositionsI++]=XMFLOAT3(x,y,z);
        }
        else if( 0 == wcscmp( strCommand, L"vt" ) )
        {
            // ���������� ����������
            float u, v;
            InFile >> u >> v;
			TexCoords[TexCoordsI++]=XMFLOAT2(u,-v);
        }
        else if( 0 == wcscmp( strCommand, L"vn" ) )
        {
            // �������
            float x, y, z;
            InFile >> x >> y >> z;
            Normals[NormalsI++]=XMFLOAT3(x,y,z);
        }
        else if( 0 == wcscmp( strCommand, L"f" ) )
        {
            // Face
            UINT iPosition, iTexCoord, iNormal;
            VertexType vertex;

            for( UINT iFace = 0; iFace < 3; iFace++ )
            {
                ZeroMemory( &vertex, sizeof( VertexType ) );

                // OBJ ������ ���������� ������� � ��������� �� 1
                InFile >> iPosition;
                vertex.position = Positions[ iPosition - 1 ];

                if( '/' == InFile.peek() )
                {
                    InFile.ignore();

                    if( '/' != InFile.peek() )
                    {
                        // �������� ���������� ����������
                        InFile >> iTexCoord;
                        vertex.texture = TexCoords[ iTexCoord - 1 ];
                    }

                    if( '/' == InFile.peek() )
                    {
                        InFile.ignore();

                        // �������� �������
                        InFile >> iNormal;
                        vertex.normal = Normals[ iNormal - 1 ];
                    }
                }

                //��������� ������� � ������
                DWORD index=AddVertex( iPosition, vertex );
                AddIndex( index );

            }
    }
    }
    InFile.close();

    //������� ��������� ��������
    delete[] Positions;
    delete[] TexCoords;
    delete[] Normals;
}

MeshBox ObjMeshLoader::GetMeshBox()
{
    return m_MeshBox;
}

bool ObjMeshLoader::GetBuffers(ID3D11Buffer *&vertexBuffer, ID3D11Buffer *&indexBuffer)
{
    vertexBuffer = D3D11Common::InitializeStaticVertexBuffer(m_device,buffer->vertices, buffer->verticesI + 1);
    if (!vertexBuffer)
        return false;
    indexBuffer = D3D11Common::InitializeIndexBuffer(m_device, buffer->indices, buffer->indicesI + 1);
    if (!indexBuffer)
        return false;

    return true;
}



ObjMeshLoader::~ObjMeshLoader(void)
{
}
