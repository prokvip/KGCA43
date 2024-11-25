#define MAX_BONE_MATRICES 255 
//#define USE_BONE_DATA_CONSTANTBUFFER 0
struct VS_In
{
	float3 p : POS;
	float3 n : NOR;
	float4 c : COL;
	float2 t : TEX;
	float4 i0 : TEXCOORD1;
	float4 w0 : TEXCOORD2;
	float4 i1 : TEXCOORD3;
	float4 w1 : TEXCOORD4;
	row_major float4x4 mTransform : TRANSFORM;
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
cbuffer DiffuseLight : register(b1)// 상수버퍼
{
	float3   g_vLightDir	: packoffset(c0);
	float1   g_vDistance	: packoffset(c0.w);
	float3   g_vLightPos	: packoffset(c1);
	float1   g_vRadius		: packoffset(c1.w);
};
//#ifdef USE_BONE_DATA_CONSTANTBUFFER
	cbuffer cbAnimMatrices : register (b2)
	{
		matrix m_matConstBoneWorld[MAX_BONE_MATRICES];
	};
//#else
	Buffer<float4> g_texAnimMatrices : register(t1);
	float4x4 FetchBoneTransform(uint iBone)
	{
		float4x4 mret;
		iBone *= 4;
		float4 row1 = g_texAnimMatrices.Load(iBone + 0);
		float4 row2 = g_texAnimMatrices.Load(iBone + 1);
		float4 row3 = g_texAnimMatrices.Load(iBone + 2);
		float4 row4 = g_texAnimMatrices.Load(iBone + 3);
		mret = float4x4(row1, row2, row3, row4);
		return mret;
	}
//#endif

VS_Out VSMain(VS_In vsIn)
{
	VS_Out  vsOut = (VS_Out)0;
	float4 vLocal = float4(vsIn.p.x, vsIn.p.y, vsIn.p.z, 1.0f);
	float4 vAnimVertex = 0;
	float3 vAnimNormal = 0;
	// iNumWeight4 = 1.0f - w[0] + w[1] + w[2]; 	
	for (int iBone = 0; iBone < 8; iBone++)
	{
		uint iBoneIndex = 0;
		float1 fWeight = 0.0f;
		if (iBone < 4)
		{
			iBoneIndex = vsIn.i0[iBone];
			fWeight = vsIn.w0[iBone];			
		}
		else
		{
			iBoneIndex = vsIn.i1[iBone-4];
			fWeight = vsIn.w1[iBone-4];
		}
		float4x4 matBone = m_matConstBoneWorld[iBoneIndex];
		//float4x4 matBone = FetchBoneTransform(iBoneIndex);
		vAnimVertex += mul(vLocal, matBone) * fWeight;
		vAnimNormal += mul(vsIn.n, (float3x3)matBone) * fWeight;
	}

	float4 vInstance = mul(vAnimVertex, vsIn.mTransform);
	float4 vWorld = mul(vInstance, g_matWorld);
	float4 vView  = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);
	vsOut.p = vProj;
	//vsOut.c = vsIn.c;
	//float3 fLightDir = float3(-1, 0.0f, 0);
	float fDotDirection = dot(vsIn.n, -g_vLightDir.xyz);
	float3 vDir = normalize(g_vLightPos.xyz - vWorld.xyz);
	float fDot = dot(vsIn.n, vDir);

	float fDistance = distance(vWorld.xyz, g_vLightPos.xyz);
	fDistance /= g_vDistance;
	fDistance = 1.0f - min(fDistance, 1.0f);

	float4 fAmbientColor = float4(0.0f, 0.0f, 1.0f, 1.0f);
	vsOut.c = fAmbientColor + float4(fDotDirection, fDotDirection, fDotDirection,1.0f) +
		float4(1.0f* fDistance * fDot, 0.0f*fDistance * fDot, 0.0f*fDistance * fDot, 1.0f);
	vsOut.c = float4(vsIn.w0.x, vsIn.w0.y, vsIn.w0.z, 1.0f);
	vsOut.t = vsIn.t;// *10.0f;
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
	psOut.c = psIn.c;// float4(1, 0, 0, 1);
	return psOut;	
}