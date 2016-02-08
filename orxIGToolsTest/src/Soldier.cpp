#include "Soldier.h"


void Soldier::OnCreate ()
{
}

void Soldier::OnDelete ()
{
}

void Soldier::Update(const orxCLOCK_INFO &_rstInfo)
{
	orxFLOAT m_movementSpeed = 100;
	// Always initialize thy variables
	orxVECTOR speed = orxVECTOR_0;

	if (orxInput_IsActive ("MoveLeft"))
	{
		SetAnim("WalkLeft", orxTRUE);
		speed.fX = -m_movementSpeed;
	}
	else if (orxInput_IsActive ("MoveUp"))				    
	{
		speed.fY = -m_movementSpeed;
	}
	else if (orxInput_IsActive ("MoveRight"))
	{
		SetAnim("WalkRight", orxTRUE);
		speed.fX = m_movementSpeed;
	}
	else if (orxInput_IsActive ("MoveDown"))
	{
		speed.fY = m_movementSpeed;
	}

	SetSpeed (speed, false);
}

orxBOOL Soldier::OnCollide(ScrollObject *_poCollider, const orxSTRING _zPartName, const orxVECTOR &_rvPosition, const orxVECTOR &_rvNormal)
{
	// Add flash effect
	AddFX ("FX-Flash");
	return true;
}