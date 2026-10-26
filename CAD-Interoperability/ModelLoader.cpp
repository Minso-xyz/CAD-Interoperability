#include <sstream>
#include <fstream>
#include "ModelLoader.h"
#include <iostream>

std::unique_ptr<Line> ModelLoader::ParseLine(const std::string& lineToParse)
{
    std::stringstream ss(lineToParse);   //lineToParse;   // 0 0 10 10 (Line)

    std::string entityName;
    double startX;
    double startY;
    double endX;
    double endY;

    ss >> entityName;
    ss >> startX >> startY >> endX >> endY;

    Point2D start { startX, startY };
    Point2D end{ endX, endY };

    std::unique_ptr<Line> line = std::make_unique<Line>(start, end);

    return line;
}

std::unique_ptr<Circle> ModelLoader::ParseCircle(const std::string& circleToParse)
{
    std::stringstream ss(circleToParse);

    std::string entityName;
    double centerX;
    double centerY;
    double radius;

    ss >> entityName;
    ss >> centerX >> centerY >> radius;

    Point2D center{ centerX, centerY };

    std::unique_ptr<Circle> circle = std::make_unique<Circle>(center, radius);

    return circle;
}

GeometryScene ModelLoader::LoadModel(const std::string& filePath)
{
    GeometryScene scene;

    std::fstream file(filePath);
    std::string line;

    while (std::getline(file, line))
    {
        std::unique_ptr<Geometry> geometry;
        std::string entityName;

        std::stringstream ss(line);
        ss >> entityName;

        if (entityName == "LINE")
        {
            geometry = ParseLine(line);
        }
        else if (entityName == "CIRCLE")
        {
            geometry = ParseCircle(line);
        }

        /*switch (entityName) {
        case "LINE":
            geometry = ParseLine(line);
            break;
        case "CIRCLE":
            geometry = ParseCircle(line);
            break;
        }*/

        if (geometry != nullptr)
        {
            scene.AddGeometry(std::move(geometry));
        }
    }
    return scene;
}