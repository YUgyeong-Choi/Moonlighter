#include "pch.h"
#include "CVisitor.h"

CVisitor::CVisitor():m_eCurPattern(CVisitor::END), m_ePrePattern(CVisitor::END)
{
}

void CVisitor::Initialize()
{

}

int CVisitor::Update()
{
    return 0;
}

void CVisitor::Late_Update()
{
}

void CVisitor::Render(HDC hDC)
{
}

void CVisitor::Release()
{
}
