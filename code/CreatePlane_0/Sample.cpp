#include "TCore.h"
#include "TDxObject.h"
class Sample: public TCore{

public:
	void   Init() override
	{
		// ȭ����ǥ��
		// 0,0    -     800,0

		//  |     X      |

		// 0,600  -     800,600        
		TDxObject objScreen; // �ﰢ��
		objScreen.v.resize(3);
		objScreen.v[0] = T_Math::FVector2(0.0f, 0.0f);
		objScreen.v[1] = T_Math::FVector2(800.0f, 0.0f);
		objScreen.v[2] = T_Math::FVector2(800.0f, 600.0f);
		

		// ����ȭ�� ��ġ ��ǥ��(NDC) : (������ǥ�� ���)
		// -1 ~ +1 ������ �����ϴ� ��ǥ��
		// -1,+1   -     +1,+1



		// -1,-1    -     +1,-1     
		/*TDxObject objNDC;
		objNDC.v[0] = T_Math::FVector2(-1.0f, +1.0f);
		objNDC.v[1] = T_Math::FVector2( 1.0f, +1.0f);
		objNDC.v[2] = T_Math::FVector2( 1.0f, -1.0f);
		objNDC.v[3] = T_Math::FVector2(-1.0f, -1.0f);*/
		/*T_Math::FVector2 v1(1,1);
		T_Math::FVector2 v2 = v1;
		T_Math::FVector2 v3 = v1 * 10.0f;
		T_Math::FVector2 v4;
		float fLength1 = v4.Length();
		bool  bNorm = v4.IsZero();
		float fLength2= v1.Length();
		float fLength3 = v3.Length();
		T_Math::FVector2 vNormalize = v1.Normalize();
		float fLength4 = vNormalize.Length();*/
		
		int k = 0;
	}

};
T_GAME_START(800, 600);