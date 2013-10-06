#include "SpriteAnimation.h"

SpriteAnimation::SpriteAnimation(unsigned int numTiles, double fps) : m_fTime(0.0), m_iCurrentTile(0), m_iNumTiles(numTiles)
{
	SetFPS(fps);
}

bool SpriteAnimation::Update(double dt)
{
	bool bWrap = false;

	m_fTime += dt;

	if(m_fTime > m_fRate)
	{
		m_iCurrentTile++;

		if(m_iCurrentTile >= m_iNumTiles)
		{
			m_iCurrentTile = 0;
			bWrap = true;
		}

		m_fTime = 0;
	}

	return bWrap;
}

void SpriteAnimation::SetFPS(double fps)
{
	m_fRate = 1.0 / fps;
}

void SpriteAnimation::SetNumTiles(unsigned int numTiles)
{
	m_iNumTiles = numTiles;
	m_iCurrentTile = 0;
}

unsigned int SpriteAnimation::GetTile() const
{
	return m_iCurrentTile;
}
