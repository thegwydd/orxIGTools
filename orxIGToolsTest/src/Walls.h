#pragma once

#include <Scroll/Scroll.h>


class Walls: public ScrollObject
{
private:
	virtual void    OnCreate ();
	virtual void    OnDelete ();
	virtual orxBOOL OnCollide (ScrollObject *_poCollider, const orxSTRING _zPartName, const orxVECTOR &_rvPosition, const orxVECTOR &_rvNormal);
	virtual void    Update (const orxCLOCK_INFO &_rstInfo);
};
