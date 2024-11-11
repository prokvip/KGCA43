//1 > FXC : error X3501 : 'main' : entrypoint not found
//1 >
//1 > compilation failed; no code produced
struct VS_In
{
	float3 p : POS;
	float3 n : NOR;
	float4 c : COL;
	float2 t : TEX;
	uint  vertexID : SV_VertexID;
};
struct VS_Out
{
	float4 p : SV_POSITION;// 반드시 float4
	float4 c  :COLOR0;//COLOR1;
	float3 t  :TEXCOORD0; // ~ TEXCOORD15
	uint   i  : INDEX;
};
cbuffer FRAME_CB : register(b0)// 상수버퍼
{
	matrix g_matWorld;
	matrix g_matView;
	matrix g_matProj;
};

VS_Out VSMain(VS_In vsIn)
{
	VS_Out  vsOut = (VS_Out)0;
	float4 vLocal = float4(vsIn.p.x, vsIn.p.y, vsIn.p.z, 1.0f);
	float4 vWorld = mul(vLocal, g_matWorld);
	float4 vView  = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);
	vProj.xyzw = float4(vProj.x, vProj.y, vProj.z, vProj.z);
	vsOut.p = vProj;
	vsOut.c = vsIn.c;
	uint i = vsIn.vertexID / (uint)4;
	vsOut.t = float3(vsIn.t, (float)i);
	//vsOut.t = normalize(vLocal.xyz);
	//vsOut.t.z =  vsIn.vertexID /4;
	vsOut.i = i;
	return vsOut;
}

static const int g_iNumTexture = 6;

Texture2D g_txTexture : register(t0);
// 6 size
Texture2D      g_txTextureArray[g_iNumTexture] : register(t1); // 2d(u,v)
Texture2DArray g_txArray: register(t2); // 3d( u,v, index)
TextureCube    g_txCube : register(t3); // 3d( u,v,w)

SamplerState Linear : register(s0);
SamplerState Point  : register(s1);


struct PS_In
{
	float4 p : SV_POSITION;// 반드시 float4
	float4 c  :COLOR;
	float3 t  :TEXCOORD0;	
	uint   i : INDEX;
	uint  primitiveID : SV_PrimitiveID;
};
struct PS_Out
{
	float4 c : SV_Target;
};
float4 GetTextureColor(uint index, float2 uv)
{
	float4 color  = float4(1, 1, 1, 1);
	switch (index)
	{
	case 0: color = g_txTextureArray[0].Sample(Point, uv); break;
	case 1: color= g_txTextureArray[1].Sample(Point, uv); break;
	case 2: color =g_txTextureArray[2].Sample(Point, uv); break;
	case 3: color =g_txTextureArray[3].Sample(Point, uv); break;
	case 4: color =g_txTextureArray[4].Sample(Point, uv); break;
	case 5: color =g_txTextureArray[5].Sample(Point, uv); break;
	}
	return color;
}
PS_Out PSMain(PS_In psIn)
{	
	//uint iIndex = psIn.primitiveID / 2;
	PS_Out  psOut = (PS_Out)0;
	psOut.c = GetTextureColor((uint)psIn.i, psIn.t.xy);
	//psOut.c = GetTextureColor(iIndex, psIn.t.xy);
	//psOut.c = g_txCube.Sample(Linear, psIn.t);
	return psOut;
}
////PS_Out PSMain(PS_In psIn)
////{
////	uint iIndex = psIn.primitiveID / 2;
////	PS_Out  psOut = (PS_Out)0;
////	psOut.c = GetTextureColor(iIndex, psIn.t);
////	return psOut;	
////}