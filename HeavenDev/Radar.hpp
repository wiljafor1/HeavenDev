#pragma once
#include "Includes.h"

class Radar : public Singleton<Radar>
{
public:

	Vector Radar::WorldToMap(Vector &worldpos, Vector m_MapOrigin, float m_fMapScale)
	{
		Vector offset(worldpos.x - m_MapOrigin.x, worldpos.y - m_MapOrigin.y, 0);
		offset.x /= m_fMapScale;
		offset.y /= -m_fMapScale;
		return offset;
	}
};