
float4 LightVector = float4(1, 0, 0, 0);
float4 FillColor = float4(0.1, 0.2, 0.4, 1);
float4 Viewport = float4(400.0, 300.0, 0.0, 0.0);
uint infoA[] = { 0, 0, 0, 0, 1, 1, 2 };
uint infoB[] = { 1, 1, 2, 0, 2, 1, 2 };
uint infoAd[] = { 2, 2, 1, 1, 0, 0, 0 };
uint infoBd[] = { 2, 2, 1, 2, 0, 2, 1 };

struct GS_INPUT
{
    float4 Pos  : POSITION;
    float4 PosV : TEXCOORD0;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float4 Col : TEXCOORD0;
};

struct PS_INPUT_WIRE
{
    float4 Pos : SV_POSITION;
    float4 Col : TEXCOORD0;
    noperspective float4 EdgeA: TEXCOORD1;
    noperspective float4 EdgeB: TEXCOORD2;
    uint Case : TEXCOORD3;
};

float3 faceNormal(in float3 posA, in float3 posB, in float3 posC)
{
    return normalize(cross(normalize(posB - posA), normalize(posC - posA)));
}

// Compute the final color of a face depending on its facing of the light
float4 shadeFace(in float4 verA, in float4 verB, in float4 verC)
{
    // Compute the triangle face normal in view frame
    float3 normal = faceNormal(verA, verB, verC);

        // Then the color of the face.
        float shade = 0.5*abs(dot(normal, LightVector));

    return float4(FillColor.xyz*shade, 1);
}

float2 projToWindow(in float4 pos)
{
    return float2(Viewport.x*0.5*((pos.x / pos.w) + 1) + Viewport.z,
        Viewport.y*0.5*(1 - (pos.y / pos.w)) + Viewport.w);
}


[maxvertexcount(6)]
void main(line GS_INPUT input[2],
    inout TriangleStream<PS_INPUT> outStream)
{
    PS_INPUT output[4];

    // Shade and colour face.
    output[0].Col = float4(1, 1, 1, 1);//shadeFace(input[0].PosV, input[1].PosV, input[2].PosV);
    output[1].Col = float4(1, 1, 1, 1);
    output[2].Col = float4(1, 1, 1, 1);
    output[3].Col = float4(1, 1, 1, 1);

    float4 p0 = input[0].Pos;
        float4 p1 = input[1].Pos;

        float w0 = p0.w;
    float w1 = p1.w;

    p0.xyz /= p0.w;
    p1.xyz /= p1.w;

    float3 line01 = p1 - p0;
        

        float3 dir1 = normalize(line01);
        

        float3 unit_z = normalize(float3(0, 0, -1));

        float3 normal1 = normalize(cross(unit_z, dir1));

        float width = 0.002;

    float3 dir_offset1 = dir1 * width;
        
        float3 normal_scaled1 = normal1 * width;        

        float3 p0_ex = p0 - dir_offset1;
        float3 p1_ex = p1 + dir_offset1;

        output[0].Pos = float4(p0_ex + normal_scaled1, 1) * w0;

    output[1].Pos = float4(p0_ex - normal_scaled1, 1) * w0;

    output[2].Pos = float4(p1_ex + normal_scaled1, 1) * w1;

    output[3].Pos = float4(p1_ex - normal_scaled1, 1) * w1;


    outStream.Append(output[3]);
    outStream.Append(output[2]);
    outStream.Append(output[1]);
    outStream.Append(output[0]);

    outStream.RestartStrip();

   /* outStream.Append(output[3]);
    outStream.Append(output[2]);
    outStream.Append(output[0]);

    outStream.RestartStrip();*/
}


[maxvertexcount(6)]
void main1(triangle GS_INPUT input[3],
    inout TriangleStream<PS_INPUT> outStream)
{
    PS_INPUT output[6];

    // Shade and colour face.
    output[0].Col = float4(1, 1, 1, 1);//shadeFace(input[0].PosV, input[1].PosV, input[2].PosV);
    output[1].Col = float4(1, 1, 1, 1);
    output[2].Col = float4(1, 1, 1, 1);
    output[3].Col = float4(1, 1, 1, 1);
    output[4].Col = float4(1, 1, 1, 1);
    output[5].Col = float4(1, 1, 1, 1);

    float4 p0 = input[0].Pos;
    float4 p1 = input[1].Pos;
    float4 p2 = input[2].Pos;

    float w0 = p0.w;
    float w1 = p1.w;
    float w2 = p2.w;

    p0.xyz /= p0.w;
    p1.xyz /= p1.w;
    p2.xyz /= p2.w;

    float3 line01 = p1 - p0;
    float3 line02 = p2 - p0;
    float3 line03 = p2 - p1;

    float3 dir1 = normalize(line01);
    float3 dir2 = normalize(line02);
    float3 dir3 = normalize(line03);

    float3 unit_z = normalize(float3(0, 0, -1));

    float3 normal1 = normalize(cross(unit_z, dir1));
    float3 normal2 = normalize(cross(unit_z, dir2));
    float3 normal3 = normalize(cross(unit_z, dir3));

    float width = 0.002;

    float3 dir_offset1 = dir1 * width;
    float3 dir_offset2 = dir2 * width;
    float3 dir_offset3 = dir3 * width;

    float3 normal_scaled1 = normal1 * width;
    float3 normal_scaled2 = normal2 * width;
    float3 normal_scaled3 = normal3 * width;

    float3 p0_ex = p0 - dir_offset1;
    float3 p1_ex = p1 + dir_offset1;
    float3 p2_ex = p2 + dir_offset2;

    output[0].Pos = float4(p0_ex + normal_scaled1, 1) * w0;

    output[1].Pos = float4(p0_ex - normal_scaled1, 1) * w0;

    output[2].Pos = float4(p1_ex + normal_scaled3, 1) * w1;

    output[3].Pos = float4(p1_ex - normal_scaled3, 1) * w1;

    output[4].Pos = float4(p2_ex + normal_scaled2, 1) * w2;

    output[5].Pos = float4(p2_ex - normal_scaled2, 1) * w2;


    outStream.Append(output[5]);
    outStream.Append(output[4]);
    outStream.Append(output[3]);
    outStream.Append(output[2]);
    outStream.Append(output[1]);
    outStream.Append(output[0]);
    outStream.RestartStrip();

    /*outStream.Append(output[5]);
    outStream.Append(output[4]);
    outStream.Append(output[3]);
    outStream.Append(output[2]);
    outStream.Append(output[1]);
    outStream.Append(output[0]);
    outStream.RestartStrip();*/
}


[maxvertexcount(3)]
void GS(
    triangle GS_INPUT input[3],
    inout TriangleStream< PS_INPUT_WIRE > outStream)
{
    PS_INPUT_WIRE output;


    // Compute the case from the positions of point in space.
    output.Case = (input[0].Pos.z < 0) * 4 + (input[1].Pos.z < 0) * 2 + (input[2].Pos.z < 0);

    // If case is all vertices behind viewpoint (case = 7) then cull.
    if (output.Case == 7) return;

    // Shade and colour face just for the "all in one" technique.
    output.Col = float4(1, 1, 1, 1);//shadeFace(input[0].PosV, input[1].PosV, input[2].PosV);

    // Transform position to window space
    float2 points[3];
    points[0] = projToWindow(input[0].Pos);
    points[1] = projToWindow(input[1].Pos);
    points[2] = projToWindow(input[2].Pos);

    // If Case is 0, all projected points are defined, do the
    // general case computation
    if (output.Case == 0)
    {
        output.EdgeA = float4(0, 0, 0, 0);
        output.EdgeB = float4(0, 0, 0, 0);

        // Compute the edges vectors of the transformed triangle
        float2 edges[3];
        edges[0] = points[1] - points[0];
        edges[1] = points[2] - points[1];
        edges[2] = points[0] - points[2];

        // Store the length of the edges
        float lengths[3];
        lengths[0] = length(edges[0]);
        lengths[1] = length(edges[1]);
        lengths[2] = length(edges[2]);

        // Compute the cos angle of each vertices
        float cosAngles[3];
        cosAngles[0] = dot(-edges[2], edges[0]) / (lengths[2] * lengths[0]);
        cosAngles[1] = dot(-edges[0], edges[1]) / (lengths[0] * lengths[1]);
        cosAngles[2] = dot(-edges[1], edges[2]) / (lengths[1] * lengths[2]);

        // The height for each vertices of the triangle
        float heights[3];
        heights[1] = lengths[0] * sqrt(1 - cosAngles[0] * cosAngles[0]);
        heights[2] = lengths[1] * sqrt(1 - cosAngles[1] * cosAngles[1]);
        heights[0] = lengths[2] * sqrt(1 - cosAngles[2] * cosAngles[2]);

        float edgeSigns[3];
        edgeSigns[0] = (edges[0].x > 0 ? 1 : -1);
        edgeSigns[1] = (edges[1].x > 0 ? 1 : -1);
        edgeSigns[2] = (edges[2].x > 0 ? 1 : -1);

        float edgeOffsets[3];
        edgeOffsets[0] = lengths[0] * (0.5 - 0.5*edgeSigns[0]);
        edgeOffsets[1] = lengths[1] * (0.5 - 0.5*edgeSigns[1]);
        edgeOffsets[2] = lengths[2] * (0.5 - 0.5*edgeSigns[2]);

        output.Pos = (input[0].Pos);
        output.EdgeA[0] = 0;
        output.EdgeA[1] = heights[0];
        output.EdgeA[2] = 0;
        output.EdgeB[0] = edgeOffsets[0];
        output.EdgeB[1] = edgeOffsets[1] + edgeSigns[1] * cosAngles[1] * lengths[0];
        output.EdgeB[2] = edgeOffsets[2] + edgeSigns[2] * lengths[2];
        outStream.Append(output);

        output.Pos = (input[1].Pos);
        output.EdgeA[0] = 0;
        output.EdgeA[1] = 0;
        output.EdgeA[2] = heights[1];
        output.EdgeB[0] = edgeOffsets[0] + edgeSigns[0] * lengths[0];
        output.EdgeB[1] = edgeOffsets[1];
        output.EdgeB[2] = edgeOffsets[2] + edgeSigns[2] * cosAngles[2] * lengths[1];
        outStream.Append(output);

        output.Pos = (input[2].Pos);
        output.EdgeA[0] = heights[2];
        output.EdgeA[1] = 0;
        output.EdgeA[2] = 0;
        output.EdgeB[0] = edgeOffsets[0] + edgeSigns[0] * cosAngles[0] * lengths[2];
        output.EdgeB[1] = edgeOffsets[1] + edgeSigns[1] * lengths[1];
        output.EdgeB[2] = edgeOffsets[2];
        outStream.Append(output);

        outStream.RestartStrip();
    }
    // Else need some tricky computations
    else
    {
        // Then compute and pass the edge definitions from the case
        output.EdgeA.xy = points[infoA[output.Case]];
        output.EdgeB.xy = points[infoB[output.Case]];

        output.EdgeA.zw = normalize(output.EdgeA.xy - points[infoAd[output.Case]]);
        output.EdgeB.zw = normalize(output.EdgeB.xy - points[infoBd[output.Case]]);

        // Generate vertices
        output.Pos = (input[0].Pos);
        outStream.Append(output);

        output.Pos = (input[1].Pos);
        outStream.Append(output);

        output.Pos = (input[2].Pos);
        outStream.Append(output);

        outStream.RestartStrip();
    }
}