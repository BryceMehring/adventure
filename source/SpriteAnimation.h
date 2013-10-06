#ifndef _SPRTIEANIMATION_
#define _SPRTIEANIMATION_

class SpriteAnimation
{
public:

	SpriteAnimation(unsigned int numTiles, double fps);

	bool Update(double dt);

	void SetFPS(double fps);
	void SetNumTiles(unsigned int numTiles);
	unsigned int GetTile() const;

private:

	double m_fTime;
	double m_fRate;
	unsigned int m_iCurrentTile;
	unsigned int m_iNumTiles;
};

#endif // _SPRTIEANIMATION_
