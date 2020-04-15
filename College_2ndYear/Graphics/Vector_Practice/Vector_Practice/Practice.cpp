
#include <windows.h> // for XMVerifyCPUSupport
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <iostream>
using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;

// Overload the  "<<" operators so that we can use cout to 
// output XMVECTOR objects.
ostream& XM_CALLCONV operator << (ostream& os, FXMVECTOR v)
{
    XMFLOAT3 dest;
    XMStoreFloat3(&dest, v);

    os << "(" << dest.x << ", " << dest.y << ", " << dest.z << ")";
    return os;
}

int main()
{
    XMVECTOR vectorA = XMVectorSet(3.0f, 3.0f, 1.0f, 0.0f);
    XMVECTOR vectorB = XMVectorReplicate(1.0f);
    XMVECTOR vectorC = XMVectorSet(2.0f, 0.0f, 3.0f, 0.0f);
    XMVECTOR vectorD = XMVectorSet(1.0f, 2.0f, 0.0f, 0.0f);

    XMVECTOR scalar1 = XMVectorReplicate(3.0f);
    XMVECTOR answer1 = XMVectorMultiply(scalar1, vectorA);
    cout << "Q1 : " << answer1 << endl;

    XMVECTOR answer2 = XMVector3Dot(vectorA, vectorB);
    cout << "Q2 : " << XMVectorGetX(answer2) << endl;

    XMVECTOR answer3 = XMVector3Cross(vectorA, vectorC);
    cout << "Q3 : " << answer3 << endl;

    XMVECTOR answer4 = XMVector3Dot(vectorB, vectorC);
    cout << "Q4 : " << XMVectorGetX(answer4) << endl;

    XMVECTOR answer5 = XMVector3Cross(vectorC, vectorD);
    cout << "Q5 : " << answer5 << endl;

    XMVECTOR answer6 = XMVector3Dot(vectorA, vectorD);
    cout << "Q6 : " << XMVectorGetX(answer6) << endl;

    XMVECTOR scalar2 = XMVectorReplicate(2.0f);
    XMVECTOR answer7 = XMVectorMultiply(scalar2, vectorD);
    cout << "Q7 : " << answer7 << endl;

    const float radianAngle1 = XMVectorGetX(XMVector3AngleBetweenVectors(vectorA, vectorD));
    cout << "Q8 : " << XMConvertToDegrees(radianAngle1) << endl;

    XMVECTOR vectorU = XMVectorSet(2.0f, 1.0f, 2.0f, 0.0f);
    XMVECTOR vectorV = XMVectorSet(-2.0f, 2.0f, 1.0f, 0.0f);
    const float radianAngle2 = XMVectorGetX(XMVector3AngleBetweenVectors(vectorU, vectorV));
    cout << "Q9 : " << XMConvertToDegrees(radianAngle2) << endl;

    XMVECTOR u = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
    XMVECTOR n = XMVector3Normalize(u);

    float LU = XMVectorGetX(XMVector3Length(n));

    cout << LU << endl;
    if (LU == 1.0f)
        cout << "Length 1" << endl;
    else
        cout << "Length not 1" << endl;

    float powLU = powf(LU, 1.0e6f);
    cout << "LU^(10^6) = " << powLU << endl;
}
