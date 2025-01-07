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
	case OUTFIT_POTION:
		graphics.DrawImage(image, 584, 288, 0, 0, 54, 54, UnitPixel);
		break;
	case OUTFIT_END:
		break;
	default:
		break;
	}

	if (item.itemId != ITEM_END) {
		switch (m_OutfitType)
		{
		case HELMET:
			break;
		case ARMOR:
			break;
		case BOOTS:
			break;
		case WEAPON1:
			image = Image::FromFile(item.pImageUrl);
			graphics.DrawImage(image, 598, 160, 0, 0, 48, 48, UnitPixel);
			break;
		case WEAPON2:
			image = Image::FromFile(item.pImageUrl);
			graphics.DrawImage(image, 737, 160, 0, 0, 48, 48, UnitPixel);
			break;
		case OUTFIT_POTION:
			image = Image::FromFile(item.pImageUrl);
			graphics.DrawImage(image, 587, 291, 0, 0, 48, 48, UnitPixel);
			break;
		case OUTFIT_ITEM:
			break;
		case OUTFIT_END:
			break;
		default:
			break;
		}
	}
	else {
		switch (m_OutfitType)
		{
		case HELMET:
			image = Image::FromFile(L"../MoonlighterAssets/Ui/Character_helmet.png");
			graphics.DrawImage(image, 525, 227, 0, 0, 60, 60, UnitPixel);
			break;
		case ARMOR:
			image = Image::FromFile(L"../MoonlighterAssets/Ui/Character_armor.png");
			graphics.DrawImage(image, 525, 286, 0, 0, 48, 48, UnitPixel);
			break;
		case BOOTS:
			image = Image::FromFile(L"../MoonlighterAssets/Ui/Character_boots.png");
			graphics.DrawImage(image, 525, 343, 0, 0, 48, 48, UnitPixel);
			break;
		case WEAPON1:
			image = Image::FromFile(L"../MoonlighterAssets/Ui/Character_sword.png");
			graphics.DrawImage(image, 593, 155, 0, 0, 48, 48, UnitPixel);
			break;
		case WEAPON2:
			image = Image::FromFile(L"../MoonlighterAssets/Ui/Character_sword.png");
			graphics.DrawImage(image, 732, 155, 0, 0, 48, 48, UnitPixel);
			break;
		case OUTFIT_POTION:
			image = Image::FromFile(L"../MoonlighterAssets/Ui/Character_potion.png");
			graphics.DrawImage(image, 582, 286, 0, 0, 48, 48, UnitPixel);
			break;
		case OUTFIT_ITEM:
			break;
		case OUTFIT_END:
			break;
		default:
			break;
		}
	}

	delete image;
}

void CSpecialSlot::Release()
{
}
