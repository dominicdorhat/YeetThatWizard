#include "header.h"

#include "Hfyda2TileManager.h"
#include "ImageManager.h"

#include <fstream>

Hfyda2TileManager::Hfyda2TileManager(int tileHeight, int tileWidth, const int iTileScale, const int iTileRowSize, const int iTileColSize)
    : TileManager(tileHeight, tileWidth),
        m_iTileScale(iTileScale),
        m_iTileRowSize(iTileRowSize),
        m_iTileColSize(iTileColSize) { 

    oTileSpriteSheet = ImageManager::get()->loadImage("resources/images/tiles/nature-platformer-tileset-16x16.png", true).resizeTo(112 * m_iTileScale, 176 * m_iTileScale);
    oRedFlagImage = ImageManager::get()->loadImage("resources/images/tiles/flag_red.png", true).resizeTo(16 * m_iTileScale, 16 * m_iTileScale);
    oGreenFlagImage = ImageManager::get()->loadImage("resources/images/tiles/flag_green.png", true).resizeTo(16 * m_iTileScale, 16 * m_iTileScale);
    m_vvMapSchema = std::vector<std::vector<int>>(m_iTileRowSize, std::vector<int>(m_iTileColSize));
}

Hfyda2TileManager::~Hfyda2TileManager() {}

void Hfyda2TileManager::setTileValueInSchema(int iMapX, int iMapY, int iValue) {
    m_vvMapSchema[iMapY][iMapX] = iValue;
}


void Hfyda2TileManager::loadMapData(std::string sPath)  {
    std::fstream oMapFile;
    oMapFile.exceptions(std::fstream::badbit | std::fstream::failbit);

    std::cout << "Attempting to read map data from \"" << sPath << "\""<< std::endl;
    
    try {
        oMapFile.open(sPath);
    
        for(int row = 0; row < m_iTileRowSize; row++) {
            for(int col = 0; col < m_iTileColSize; col++) {
                oMapFile >> m_vvMapSchema[row][col];
                setMapValue(col, row, m_vvMapSchema[row][col]);
            }
        }
        
    } catch (const std::fstream::failure& e){         
        perror(("Exception opening file: " + sPath).c_str());
    }

    oMapFile.close();
}

void Hfyda2TileManager::virtDrawTileAt( 
		BaseEngine* pEngine, 
		DrawingSurface* pSurface,
		int iMapX, int iMapY,
		int iStartPositionScreenX, int iStartPositionScreenY ) const {
    switch(getMapValue(iMapX, iMapY)) {

        // [PLATFORMS]
        case 0:
        case 25:
        case 26:
        case 27:
            // pEngine->drawBackgroundRectangle(iStartPositionScreenX, iStartPositionScreenY, iStartPositionScreenX + this->m_iTileWidth, iStartPositionScreenY +  this->m_iTileHeight, 0x000000);
            // pEngine->drawBackgroundRectangle(iStartPositionScreenX + 2, iStartPositionScreenY + 2, (iStartPositionScreenX + this->m_iTileWidth) - 4, (iStartPositionScreenY +  this->m_iTileHeight) - 4, 0x00a5eb);
            break;
        case 1: // middle of single platform            
            oTileSpriteSheet.renderImageWithMask(pSurface, 63*m_iTileScale, 0, iStartPositionScreenX, iStartPositionScreenY, 16*m_iTileScale, 16*m_iTileScale, 0x000000);
            break;
        case 2: // left-end of single platform
            oTileSpriteSheet.renderImageWithMask(pSurface, 48*m_iTileScale, 0, iStartPositionScreenX, iStartPositionScreenY, 16*m_iTileScale, 16*m_iTileScale, 0x000000);
            break;
        case 3: // end of single platform
            oTileSpriteSheet.renderImageWithMask(pSurface, 80*m_iTileScale, 0, iStartPositionScreenX, iStartPositionScreenY, 16*m_iTileScale, 16*m_iTileScale, 0x000000);   
            break;

        case 4: // middle of platform
            oTileSpriteSheet.renderImageWithMask(pSurface, 16*m_iTileScale, 0, iStartPositionScreenX, iStartPositionScreenY, 16*m_iTileScale, 16*m_iTileScale, 0x000000);   
            break;
        case 5: // left-end of platform
            oTileSpriteSheet.renderImageWithMask(pSurface, 0, 0, iStartPositionScreenX, iStartPositionScreenY, 16*m_iTileScale, 16*m_iTileScale, 0x000000);   
            break;
        case 6: // right-end of platform
            oTileSpriteSheet.renderImageWithMask(pSurface, 32*m_iTileScale, 0, iStartPositionScreenX, iStartPositionScreenY, 16*m_iTileScale, 16*m_iTileScale, 0x000000);   
            break;
        case 7: // middle of dirt platform
            oTileSpriteSheet.renderImageWithMask(pSurface, 16*m_iTileScale, 16*m_iTileScale, iStartPositionScreenX, iStartPositionScreenY, 16*m_iTileScale, 16*m_iTileScale, 0x000000);   
            break;
        case 8: // left end of dirt platform
            oTileSpriteSheet.renderImageWithMask(pSurface, 0, 16*m_iTileScale, iStartPositionScreenX, iStartPositionScreenY, 16*m_iTileScale, 16*m_iTileScale, 0x000000);   
            break;
        case 9: // right end of dirt platform
            oTileSpriteSheet.renderImageWithMask(pSurface, 32*m_iTileScale, 16*m_iTileScale, iStartPositionScreenX, iStartPositionScreenY, 16*m_iTileScale, 16*m_iTileScale, 0x000000);   
            break;

        case 10: // middle of brick platform
            oTileSpriteSheet.renderImageWithMask(pSurface, 16*m_iTileScale, 96*m_iTileScale, iStartPositionScreenX, iStartPositionScreenY, 16*m_iTileScale, 16*m_iTileScale, 0x000000);   
            break;
        case 11: // left end of brick platform
            oTileSpriteSheet.renderImageWithMask(pSurface, 0, 96*m_iTileScale, iStartPositionScreenX, iStartPositionScreenY, 16*m_iTileScale, 16*m_iTileScale, 0x000000);   
            break;
        case 12: // right end of brick platform
            oTileSpriteSheet.renderImageWithMask(pSurface, 32*m_iTileScale, 96*m_iTileScale, iStartPositionScreenX, iStartPositionScreenY, 16*m_iTileScale, 16*m_iTileScale, 0x000000);   
            break;


        
        // [ETC.]
        case 20: // middle of ladder
            oTileSpriteSheet.renderImageWithMask(pSurface, 0, 64*m_iTileScale, iStartPositionScreenX, iStartPositionScreenY, 16*m_iTileScale, 16*m_iTileScale, 0x000000);   
            // pEngine->drawBackgroundRectangle(iStartPositionScreenX, iStartPositionScreenY, iStartPositionScreenX + this->m_iTileWidth, iStartPositionScreenY +  this->m_iTileHeight, 0x000000);
            // pEngine->drawBackgroundRectangle(iStartPositionScreenX + 4, iStartPositionScreenY + 4, (iStartPositionScreenX + this->m_iTileWidth) - 4, (iStartPositionScreenY +  this->m_iTileHeight) - 4, 0x00a5eb);
            break;
        case 21: // top of ladder
            oTileSpriteSheet.renderImageWithMask(pSurface, 63*m_iTileScale, 0, iStartPositionScreenX, iStartPositionScreenY, 16*m_iTileScale, 16*m_iTileScale, 0x000000);
            oTileSpriteSheet.renderImageWithMask(pSurface, 0, 48*m_iTileScale, iStartPositionScreenX, iStartPositionScreenY, 16*m_iTileScale, 16*m_iTileScale, 0x000000);   
            // pEngine->drawBackgroundRectangle(iStartPositionScreenX, iStartPositionScreenY, iStartPositionScreenX + this->m_iTileWidth, iStartPositionScreenY +  this->m_iTileHeight, 0x000000);
            // pEngine->drawBackgroundRectangle(iStartPositionScreenX + 4, iStartPositionScreenY + 4, (iStartPositionScreenX + this->m_iTileWidth) - 4, (iStartPositionScreenY +  this->m_iTileHeight) - 4, 0x00a5eb);
            break;
        case 22: // bottom of ladder
            oTileSpriteSheet.renderImageWithMask(pSurface, 0, 80*m_iTileScale, iStartPositionScreenX, iStartPositionScreenY, 16*m_iTileScale, 16*m_iTileScale, 0x000000);   
            // pEngine->drawBackgroundRectangle(iStartPositionScreenX, iStartPositionScreenY, iStartPositionScreenX + this->m_iTileWidth, iStartPositionScreenY +  this->m_iTileHeight, 0x000000);
            // pEngine->drawBackgroundRectangle(iStartPositionScreenX + 4, iStartPositionScreenY + 4, (iStartPositionScreenX + this->m_iTileWidth) - 4, (iStartPositionScreenY +  this->m_iTileHeight) - 4, 0x00a5eb);
            break;

        case 23: // enemy spawn point
            oTileSpriteSheet.renderImageWithMask(pSurface, 64*m_iTileScale, 16*m_iTileScale, iStartPositionScreenX, iStartPositionScreenY, 16*m_iTileScale, 16*m_iTileScale, 0x000000);   
            break;

        case 24: // yellow orb
            oTileSpriteSheet.renderImageWithMask(pSurface, 80*m_iTileScale, 112*m_iTileScale, iStartPositionScreenX, iStartPositionScreenY, 16*m_iTileScale, 16*m_iTileScale, 0x000000);
            break;
        case 28: // purple orb
            oTileSpriteSheet.renderImageWithMask(pSurface, 96*m_iTileScale, 112*m_iTileScale, iStartPositionScreenX, iStartPositionScreenY, 16*m_iTileScale, 16*m_iTileScale, 0x000000);
            break;

        case 30: // red flag
            oRedFlagImage.renderImageWithMask(pSurface, 0, 0, iStartPositionScreenX, iStartPositionScreenY, 16*m_iTileScale, 16*m_iTileScale, 0x000000);
            break;
        case 31: // green flag
            oGreenFlagImage.renderImageWithMask(pSurface, 0, 0, iStartPositionScreenX, iStartPositionScreenY, 16*m_iTileScale, 16*m_iTileScale, 0x000000);
            break;
            
    }   

}