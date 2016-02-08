#include "Walls.h"


void Walls::OnCreate ()
	{
	}

void Walls::OnDelete ()
	{
	}

void Walls::Update(const orxCLOCK_INFO &_rstInfo)
	{
	}

orxBOOL Walls::OnCollide(ScrollObject *_poCollider, const orxSTRING _zPartName, const orxVECTOR &_rvPosition, const orxVECTOR &_rvNormal)
	{
	return true;
	}