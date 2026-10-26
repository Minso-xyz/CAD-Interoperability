#include <iostream>
#include "GeometryScene.h"
#include "ModelLoader.h"

int main()
{
	ModelLoader loader;
	GeometryScene scene;

	std::unique_ptr<Line> line = loader.ParseLine("0 0 10 10");
	std::unique_ptr<Circle> circle = loader.ParseCircle("5 5 3");

	scene.AddGeometry(std::move(line));
	scene.AddGeometry(std::move(circle));

	scene.PrintGeometries();

	return 0;
}