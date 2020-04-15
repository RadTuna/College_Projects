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


    XMMATRIX matrixA = XMMatrixSet(
        1.0f, 0.0f, 4.0f, 1.0f,
        -2.0f, 3.0f, 0.0f, -1.0f,
        0.0f, 1.0f, 2.0f, 1.0f,
        2.0f, 0.0f, 1.0f, 2.0f);

    XMMATRIX matrixB = XMMatrixSet(
        1.0f, 0.0f, 1.0f, 2.0f,
        2.0f, -4.0f, 0.0f, -1.0f,
        0.0f, 1.0f, 2.0f, 0.0f,
        1.0f, 2.0f, 1.0f, 1.0f);

    XMMATRIX matrixC = XMMatrixSet(
        1.0f, 0.0f, 1.0f, 0.0f,
        2.0f, 1.0f, 0.0f, 2.0f,
        -2.0f, 3.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 2.0f, -1.0f);

	
    cout << "A * B : \n" << matrixA * matrixB << endl;
    cout << "B * A : \n" << matrixB * matrixA << endl;
    cout << "A * B * C : \n" << matrixA * matrixB * matrixC << endl;

    return 0;
}