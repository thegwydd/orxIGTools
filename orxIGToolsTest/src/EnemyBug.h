#pragma once

#include <Scroll/Scroll.h>

enum Direction { NORTH, SOUTH, EAST, WEST, eDIRECTION_TOP };

class EnemyBug : public ScrollObject
{
private:
	//! Called on object creation.
	virtual void OnCreate ();
	//! Called on object deletion
	virtual void OnDelete ();
	//! Called on clock update
	virtual void Update (const orxCLOCK_INFO &_rstInfo);

	//! Direction of movement
	Direction   m_direction;
	//! Speed of movement
	float       m_movementSpeed;
	//! Time since change of direction
	float       m_timeSinceDirectionChange;
	//! Time interval between direction changes
	float       m_directionChangeInterval;
};