#pragma once

#include <memory>
#include <string>

#include "Line.h"
#include "Circle.h"
#include "Polygon.h"
#include "GeometryScene.h"

class ModelLoader
{
public:
	std::unique_ptr<Line> ParseLine(const std::string& lineToParse);
	std::unique_ptr<Circle> ParseCircle(const std::string& circleToParse);
	std::unique_ptr<Polygon> ParsePolygon(const std::string& polygonToParse);
	GeometryScene LoadModel(const std::string& filePath);
};