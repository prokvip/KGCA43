#define MAX_BONE_MATRICES 255 
//#define FT_CONSTANTBUFFER 0
//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D g_txDiffuse: register (t0);
SamplerState samLinear: register (s0);
cbuffer cb0: register (b0)
{
	float4x4	g_matWorld	: packoffset(c0);
	float4x4	g_matView	: packoffset(c4);
	float4x4	g_matProj	: packoffset(c8);
	float4  g_MeshColor     : packoffset(c12);
};


#ifdef FT_CONSTANTBUFFER
	cbuffer cbAnimMatrices : register (b1)
	{
		matrix m_matConstBoneWorld[MAX_BONE_MATRICES];
	};
	matrix FetchBoneTransform(uint iBone)
	{
		matrix mret;
		mret = m_matConstBoneWorld[iBone];
		return mret;
	}
#else
	Buffer<float4>     g_BufferBoneWorld : register(t1);
	float4x4 FetchBoneTransform(uint iBone)
	{
		float4x4 mret;
		iBone *= 4;
		float4 row1 = g_BufferBoneWorld.Load(iBone + 0);
		float4 row2 = g_BufferBoneWorld.Load(iBone + 1);
		float4 row3 = g_BufferBoneWorld.Load(iBone + 2);
		float4 row4 = g_BufferBoneWorld.Load(iBone + 3);
		mret = float4x4(row1, row2, row3, row4);
		return mret;
	}
#endif

struct PNCT5_VS_INPUT
{
	float3 p		: POSITION;
	float3 n		: NORMAL;
	float4 c		: COLOR0;
	float2 t		: TEXCOORD0;
	float4 w0		: TEXCOORD1;
	float4 i0		: TEXCOORD2;
	float4 w1		: TEXCOORD3;
	float4 i1		: TEXCOORD4;
};
struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
};
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(PNCT5_VS_INPUT input)//,uniform bool bHalfVector )
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float4 Pos = float4(input.p, 1);
	float3 Norm = input.n;

	float4x4 matMatrix;

	float4 vLocal;
	for (int iBiped = 0; iBiped < input.w1.w; iBiped++)
	{
		uint iBoneIndex = (uint)input.i0[iBiped];
		float fWeight = input.w0[iBiped];

		vLocal = Pos;
		if (iBiped < 4)
		{
			matMatrix = FetchBoneTransform(iBoneIndex);
			output.p += fWeight * mul(vLocal, matMatrix);
			output.n += fWeight * mul(Norm, (float3x3)matMatrix);
		}
		else
		{
			iBoneIndex = (uint)input.i1[iBiped];
			fWeight = input.w1[iBiped];
			matMatrix = FetchBoneTransform(iBoneIndex);
			output.p += fWeight * mul(vLocal, matMatrix);
			output.n += fWeight * mul(Norm, (float3x3)matMatrix);
		}
	}

	output.p = mul(output.p, g_matWorld);
	output.n = normalize(mul(output.n, (float3x3)g_matWorld));// g_matWorldInvTrans));

	output.p = mul(output.p, g_matView);
	output.p = mul(output.p, g_matProj);
	output.t = input.t;
	output.c = input.c;
	return output;
}
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT vIn) : SV_Target
{
	return g_txDiffuse.Sample(samLinear, vIn.t);// *vIn.c;
}

//Matrix m =
//FetchBoneTransform(Input.Bones.x) * Input.Weights.x
//+ FetchBoneTransform(Input.Bones.y) * Input.Weights.y
//+ FetchBoneTransform(Input.Bones.z) * Input.Weights.z
//+ FetchBoneTransform(Input.Bones.w) * Input.Weights.w;
//Output.Pos = mul(Pos, m);
//Output.Norm = mul(Norm, (float3x3)m);
//Output.Tan = mul(Tan, (float3x3)m);

/*uint iBone = Input.Bones.x;
float fWeight = Input.Weights.x;
matrix m = g_mBoneWorld[ iTimeShift*MAX_BONE_MATRICES + iBone ];
Output.Pos += fWeight * mul( pos, m );
Output.Norm += fWeight * mul( norm, m );

iBone = Input.Bones.y;
fWeight = Input.Weights.y;
m = g_mBoneWorld[ iTimeShift*MAX_BONE_MATRICES + iBone ];
Output.Pos += fWeight * mul( pos, m );
Output.Norm += fWeight * mul( norm, m );

iBone = Input.Bones.z;
fWeight = Input.Weights.z;
m = g_mBoneWorld[ iTimeShift*MAX_BONE_MATRICES + iBone ];
Output.Pos += fWeight * mul( pos, m );
Output.Norm += fWeight * mul( norm, m );

iBone = Input.Bones.w;
fWeight = Input.Weights.w;
m = g_mBoneWorld[ iTimeShift*MAX_BONE_MATRICES + iBone ];
Output.Pos += fWeight * mul( pos, m );
Output.Norm += fWeight * mul( norm, m );
*/