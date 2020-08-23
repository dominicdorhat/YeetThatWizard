#ifndef HFYDA2TILEMANAGER_H
#define HFYDA2TILEMANAGER_H

#include "TileManager.h"
#include "SimpleImage.h"

class Hfyda2TileManager : public TileManager {
    
    public:
    // TODO: set tile size
	Hfyda2TileManager(int iTileHeight, int iTileWidth, const int iTileScale, const int iTileRowSize, const int iTileColSize);

	~Hfyda2TileManager(void);

	int getTileScale() { return this->m_iTileScale; }
	int getTileRowSize() { return this->m_iTileRowSize; }
	int getTileColSize() { return this->m_iTileColSize; }

	int getLeftDrawingRegionForMapX(int iMapX) { return iMapX * getTileWidth(); }
	int getRightDrawingRegionForMapX(int iMapX) { return (iMapX + 1) * getTileWidth(); }
	int getTopDrawingRegionForMapY(int iMapY) { return iMapY * getTileHeight(); }
	int getBottomDrawingRegionForMapY(int iMapY) { return (iMapY + 1) * getTileHeight(); }

	void loadMapData(std::string sPath);

	std::vector<std::vector<int>> getMapSchema() const { return m_vvMapSchema; }
	void setTileValueInSchema(int iMapX, int iMapY, int iValue);

	virtual void virtDrawTileAt( 
		BaseEngine* pEngine, 
		DrawingSurface* pSurface,
		int iMapX, int iMapY,
		int iStartPositionScreenX, int iStartPositionScreenY ) const override;

    private:

		int m_iTileScale;
		int m_iTileRowSize;
		int m_iTileColSize;

        SimpleImage oTileSpriteSheet;
		SimpleImage oRedFlagImage;
		SimpleImage oGreenFlagImage;
		std::vector<std::vector<int>> m_vvMapSchema;
};

#endif