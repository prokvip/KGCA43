//1 > FXC : error X3501 : 'main' : entrypoint not found
//1 >
//1 > compilation failed; no code produced
struct VS_In
{
	float3 p : POS;
	float3 n : NOR;
	float4 c : COL;
	float2 t : TEX;
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

VS_Out VSMain(VS_In vsIn)
{
	VS_Out  vsOut = (VS_Out)0;
	float4 vLocal = float4(vsIn.p.x, vsIn.p.y, vsIn.p.z, 1.0f);
	float4 vWorld = mul(vLocal, g_matWorld);
	float4 vView  = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);
	vsOut.p = vProj;
	vsOut.c = vsIn.c;
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
	psOut.c =  pixel * psIn.c;
	return psOut;	
}