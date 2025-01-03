#include "pch.h"
#include "CSpecialSlot.h"

CSpecialSlot::CSpecialSlot()
{
}

void CSpecialSlot::Initialize()
{
}

int CSpecialSlot::Update()
{
	return 0;
}

void CSpecialSlot::Late_Update()
{
}

void CSpecialSlot::Render(HDC hDC)
{
	Image* image(nullptr);
	Graphics graphics(hDC);
	image = Image::FromFile(L"../MoonlighterAssets/Ui/Bag_slot_Outline.png");
	switch (m_OutfitType)
	{
	case HELMET:
		graphics.DrawImage(image, 527 , 229, 0, 0, 54, 54, UnitPixel);
		break;
	case ARMOR:
		graphics.DrawImage(image, 527, 288, 0, 0, 54, 54, UnitPixel);
		break;
	case BOOTS:
		graphics.DrawImage(image, 527, 345, 0, 0, 54, 54, UnitPixel);
		break;
	case WEAPON1:
		graphics.DrawImage(image, 595, 157, 0, 0, 54, 54, UnitPixel);
		break;
	case WEAPON2:
		graphics.DrawImage(image, 734, 157, 0, 0, 54, 54, UnitPixel);
		break;
	case POTION:
		graphics.DrawImage(image, 584, 288, 0, 0, 54, 54, UnitPixel);
		break;
	case OUTFIT_END:
		break;
	default:
		break;
	}
	delete image;
}

void CSpecialSlot::Release()
{
}
