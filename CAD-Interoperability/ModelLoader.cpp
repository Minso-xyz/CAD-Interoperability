#include <sstream>
#include "ModelLoader.h"

std::unique_ptr<Line> ModelLoader::ParseLine(const std::string& lineToParse)
{
    std::stringstream ss(lineToParse);   //lineToParse;   // 0 0 10 10 (Line)

    double startX;
    double startY;
    double endX;
    double endY;

    ss >> startX >> startY >> endX >> endY;

    Point2D start { startX, startY };
    Point2D end{ endX, endY };

    std::unique_ptr<Line> line = std::make_unique<Line>(start, end);

    return line;
}

std::unique_ptr<Circle> ModelLoader::ParseCircle(const std::string& circleToParse)
{
    std::stringstream ss(circleToParse);

    double centerX;
    double centerY;
    double radius;

    ss >> centerX >> centerY >> radius;

    Point2D center{ centerX, centerY };

    std::unique_ptr<Circle> circle = std::make_unique<Circle>(center, radius);

    return circle;
}