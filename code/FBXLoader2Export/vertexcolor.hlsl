#define MAX_BONE_MATRICES 255 
struct VS_In
{
	float3 p : POS;
	float3 n : NOR;
	float4 c : COL;
	float2 t : TEX;
	float4 i : INDEX;
	float4 w : WEIGHT;
};
struct VS_Out
{
	float4 p : SV_POSITION;// 반드시 float4
	float4 c  :COLOR0;//COLOR1;
	float2 t  :TEXCOORD0; // ~ TEXCOORD15
};
cbuffer FRAME_CB : register(b0)// 상수버퍼
{
	matrix g_matWorld;
	matrix g_matView;
	matrix g_matProj;
};
cbuffer cbAnimMatrices : register (b2)
{
	matrix m_matConstBoneWorld[MAX_BONE_MATRICES];
};

VS_Out VSMain(VS_In vsIn)
{
	VS_Out  vsOut = (VS_Out)0;
	float4 vLocal = float4(vsIn.p.x, vsIn.p.y, vsIn.p.z, 1.0f);

	float4 vAnimVertex = 0;
	float3 vAnimNormal = 0;
	// iNumWeight4 = 1.0f - w[0] + w[1] + w[2]; 	
	for (int iBone = 0; iBone < 4; iBone++)
	{
		uint iBoneIndex = vsIn.i[iBone];
		float1 fWeight = vsIn.w[iBone];
		float4x4 matBone = m_matConstBoneWorld[iBoneIndex];

		vAnimVertex += mul(vLocal, matBone) * fWeight;
		vAnimNormal += mul(vsIn.n, (float3x3)matBone) * fWeight;
	}

	float4 vWorld = mul(vAnimVertex, g_matWorld);
	float4 vView  = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);
	vsOut.p = vProj;
	vsOut.c = vsIn.w;
	vsOut.c.a = 1.0f;
	vsOut.t = vsIn.t;
	return vsOut;
}

Texture2D g_txTexture : register(t0);
SamplerState LinearPoint : register(s0);
SamplerState Point  : register(s1);


struct PS_In
{
	float4 p : SV_POSITION;// 반드시 float4
	float4 c  :COLOR;
	float2 t  :TEXCOORD0;
};
struct PS_Out
{
	float4 c : SV_Target;
};
PS_Out PSMain(PS_In psIn)
{
	PS_Out  psOut = (PS_Out)0;
	float4 pixel = g_txTexture.Sample(LinearPoint, psIn.t);
	//float4 pixel = g_txTexture.Sample(Point, psIn.t);
	pixel.a = 1.0f;
	psOut.c = psIn.c;// float4(1, 0, 0, 1);
	return psOut;	
}