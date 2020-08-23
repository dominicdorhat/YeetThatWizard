#ifndef COORDINATESHIFT_H
#define COORDINATESHIFT_H

#include "header.h"
#include "CoordinateMapping.h"
#include "SimpleImage.h"


class CoordinateMappingFlip : public CoordinateMapping {
    public:
        CoordinateMappingFlip() { };

        // very simple but gets the job done, I just need to flip a static image anyways
        virtual bool mapCoordinates(double& x, double& y, const SimpleImage& image) override
        {
            x = image.getWidth() - x;
            return true;
        }
};

#endif