#include <windows.h> // for XMVerifyCPUSupport
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <iostream>
using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;

// Overload the  "<<" operators so that we can use cout to 
// output XMVECTOR and XMMATRIX objects.
ostream& XM_CALLCONV operator << (ostream& os, FXMVECTOR v)
{
    XMFLOAT4 dest;
    XMStoreFloat4(&dest, v);

    os << "(" << dest.x << ", " << dest.y << ", " << dest.z << ", " << dest.w << ")";
    return os;
}

ostream& XM_CALLCONV operator << (ostream& os, FXMMATRIX m)
{
    for (int i = 0; i < 4; ++i)
    {
        os << XMVectorGetX(m.r[i]) << "\t";
        os << XMVectorGetY(m.r[i]) << "\t";
        os << XMVectorGetZ(m.r[i]) << "\t";
        os << XMVectorGetW(m.r[i]);
        os << endl;
    }
    return os;
}

int main()
{
    // Check support for SSE2 (Pentium4, AMD K8, and above).
    if (!XMVerifyCPUSupport())
    {
        cout << "directx math not supported" << endl;
        return 0;
    }


    const XMMATRIX scalingMatrix = XMMatrixScaling(0.5f, 2.0f, 1.0f);
    const XMMATRIX rotationMatrix = XMMatrixRotationZ(XM_PIDIV4);
    const XMMATRIX translationMatrix = XMMatrixTranslation(3.0f, 0.0f, 2.0f);

    cout << "MatrixS : \n" << scalingMatrix << endl;
    cout << "MatrixR : \n" << rotationMatrix << endl;
    cout << "MatrixT : \n" << translationMatrix << endl;

    const XMMATRIX SRTMatrix = scalingMatrix * rotationMatrix * translationMatrix;

    cout << "MatrixSRT : \n" << SRTMatrix << endl;

    const XMVECTOR maxPoint = XMVectorSet(2.0f, 2.0f, 2.0f, 1.0f);
    const XMVECTOR minPoint = XMVectorSet(-2.0f, -2.0f, -2.0f, 1.0f);

    const XMVECTOR maxSPoint = XMVector3TransformCoord(maxPoint, scalingMatrix);
    const XMVECTOR minSPoint = XMVector3TransformCoord(minPoint, scalingMatrix);

    cout << "Max*S : " << maxSPoint << " / Min*S : " << minSPoint << endl;

    const XMVECTOR maxSRPoint = XMVector3TransformCoord(maxSPoint, rotationMatrix);
    const XMVECTOR minSRPoint = XMVector3TransformCoord(minSPoint, rotationMatrix);

    cout << "MaxS*R : " << maxSRPoint << " / MinS*R : " << minSRPoint << endl;

    XMVECTOR maxSRTPoint = XMVector3TransformCoord(maxSRPoint, translationMatrix);
    XMVECTOR minSRTPoint = XMVector3TransformCoord(minSRPoint, translationMatrix);

    cout << "MaxSR*T : " << maxSRTPoint << " / MinSR*T : " << minSRTPoint << endl;

    maxSRTPoint = XMVector3TransformCoord(maxPoint, SRTMatrix);
    minSRTPoint = XMVector3TransformCoord(minPoint, SRTMatrix);

    cout << "Max*SRT : " << maxSRTPoint << " / Min*SRT : " << minSRTPoint << endl;

    return 0;
}