#include "object.h"

bool Object::initialize(int Index, D2D1_POINT_2F pos, int rot, D2D1_SIZE_F siz, D2D1_SIZE_F scal, int health, int maxhealth, D2D1_RECT_U Texture, D2D1_POINT_2F rotaxis, UINT TextureType)
{
	index = Index;
	Position = pos;
	Size = siz;
	Texcord = Texture;
	Rotation = rot;
	Health = health;
	MaxHealth = maxhealth;
	Axis = rotaxis;
	TextType = TextureType;
	Scale = scal;
	Selected = false;
	StartCorner.x = Position.x - (Axis.x * (Texcord.right - Texcord.left) * Scale.width);
	StartCorner.y = Position.y - (Axis.y * (Texcord.bottom - Texcord.top) * Scale.height);
	Size.height;
	return true;
}

D2D1_RECT_U Object::GetTexture()
{
	return Texcord;
}

int Object::GetRotation()
{
	return Rotation;
}

D2D1_POINT_2F Object::GetPosition()
{
	return Position;
}

D2D1_POINT_2F Object::GetStartCorner()
{
	return StartCorner;
}

UINT Object::GetTextureType()
{
	return TextType;
}

void Object::SetFractions(UINT input)
{
	Fraction = input;
}

UINT Object::GetFraction()
{
	return Fraction;
}

void Object::SetPosition(D2D1_POINT_2F Input)
{
	Position = Input;
	StartCorner.x = Position.x - (Axis.x * (Texcord.right - Texcord.left) * Scale.width);
	StartCorner.y = Position.y - (Axis.y * (Texcord.bottom - Texcord.top) * Scale.height);
}

D2D1_SIZE_F Object::GetSize()
{
	return Size;
}

D2D1_SIZE_F Object::GetScale()
{
	return Scale;
}

int Object::GetMaxHealth()
{
	return MaxHealth;
}

double Object::GetHealth()
{
	return Health;
}

bool Object::IsMovable()
{
	return Movable;
}

UINT Object::GetUnitType()
{
	return UnitType;
}
