//NX_Extractor
//
// Mandatory UF Includes
#include <uf.h>
#include <uf_object_types.h>

// Internal Includes
#include <NXOpen/ListingWindow.hxx>
#include <NXOpen/NXMessageBox.hxx>
#include <NXOpen/UI.hxx>
 
// Internal+External Includes
#include <NXOpen/Annotations.hxx>
#include <NXOpen/Annotations_Dimension.hxx>
#include <NXOpen/Annotations_DimensionCollection.hxx>
#include <NXOpen/Assemblies_Component.hxx>
#include <NXOpen/Assemblies_ComponentAssembly.hxx>
#include <NXOpen/Body.hxx>
#include <NXOpen/BodyCollection.hxx>
#include <NXOpen/Face.hxx>
#include <NXOpen/Edge.hxx>
#include <NXOpen/Features_BodyFeature.hxx>
#include <NXOpen/Line.hxx>
#include <NXOpen/NXException.hxx>
#include <NXOpen/NXObject.hxx>
#include <NXOpen/Part.hxx>
#include <NXOpen/PartCollection.hxx>
#include <NXOpen/Session.hxx>

// Std C++ Includes
#include <iostream>
#include <sstream>

using namespace NXOpen;
using std::string;
using std::exception;
using std::stringstream;
using std::endl;
using std::cout;
using std::cerr;

	 	

//------------------------------------------------------------------------------
// NXOpen c++ test class 
//------------------------------------------------------------------------------
class NXExtractor
{
	// class members
public:
	static Session *theSession;

	static UI *theUI;


	NXExtractor();
	~NXExtractor();

	void do_it();
	void print(const NXString &);
	void print(const string &);
	void print(const char*);

	void PrintComponentTree(Assemblies::Component* component, int depth);
	void PrintBRepTree(Part* part);
	void PrintFeatureTree(Part* part);
	void PrintFeatureExpressions(Part* part);
	void PrintDimensions(Part* part);
	std::string FaceTypeToString(Face::FaceType faceType);
	std::string EdgeTypeToString(Edge::EdgeType edgeType);
  

private:
	BasePart* workPart, *displayPart;

 
	NXMessageBox *mb;
	ListingWindow *lw;
	LogFile *lf;

};

//------------------------------------------------------------------------------
// Initialize static variables
//------------------------------------------------------------------------------
Session *(NXExtractor::theSession) = NULL;

UI *(NXExtractor::theUI) = NULL;


//------------------------------------------------------------------------------
// Constructor 
//------------------------------------------------------------------------------
NXExtractor::NXExtractor()
{

	// Initialize the NX Open C++ API environment
	NXExtractor::theSession = NXOpen::Session::GetSession();

	NXExtractor::theUI = UI::GetUI();
  
	mb = theUI->NXMessageBox();
	lw = theSession->ListingWindow();
	lf = theSession->LogFile();

	workPart = theSession->Parts()->BaseWork();
	displayPart = theSession->Parts()->BaseDisplay();
}

//------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------
NXExtractor::~NXExtractor()
{

}

//------------------------------------------------------------------------------
// Print string to listing window or stdout
//------------------------------------------------------------------------------
void NXExtractor::print(const NXString& msg)
{

	if (!lw->IsOpen()) lw->Open();
	lw->WriteLine(msg);

}

void NXExtractor::print(const string& msg)
{

	if (!lw->IsOpen()) lw->Open();
	lw->WriteLine(msg);

}

void NXExtractor::print(const char* msg)
{

	if (!lw->IsOpen()) lw->Open();
	lw->WriteLine(msg);

}

  

//------------------------------------------------------------------------------
 //Do something
//------------------------------------------------------------------------------
void NXExtractor::do_it()
{
	print("=== NX Extractor ===");

	// Print the name of work part
	if (workPart == nullptr)
	{
		print("No work part is open!");
		return;
	}

	NXOpen::NXString nameWorkPart = workPart->Name();
	print("Work part found : " + nameWorkPart);
	print("");

	// Print the display name of assembly
	Part* part = theSession->Parts()->Work();

	Assemblies::ComponentAssembly* componentAssembly = part->ComponentAssembly();
	Assemblies::Component* root = componentAssembly->RootComponent();

	//if (root == nullptr)
	//{
	//	print("Work part is not an assembly!");
	//	return;
	//}

	//NXOpen::NXString nameRoot = root->DisplayName();
	//print("Assembly root found : " + nameRoot);

	//// Root extraction
	//print("=== List of children ===");
	//std::vector<Assemblies::Component*> rootChildren = root->GetChildren();
	//for (auto child : rootChildren)
	//{
	//	print("L " + child->DisplayName());
	//}
	



	//PrintComponentTree(root, 1);

	//PrintBRepTree(part);

	//PrintFeatureTree(part);

	//PrintFeatureExpressions(part);

	PrintDimensions(part);
}

//------------------------------------------------------------------------------
// Entry point(s) for unmanaged internal NXOpen C/C++ programs
//------------------------------------------------------------------------------
  //  Explicit Execution
extern "C" DllExport void ufusr(char* parm, int* returnCode, int rlen)
{
	try
	{
		// Create NXOpen C++ class instance
		NXExtractor* theNXExtractor;
		theNXExtractor = new NXExtractor();
		theNXExtractor->do_it();
		delete theNXExtractor;
	}
	catch (const NXException & e1)
	{
		UI::GetUI()->NXMessageBox()->Show("NXException", NXOpen::NXMessageBox::DialogTypeError, e1.Message());
	}
	catch (const exception & e2)
	{
		UI::GetUI()->NXMessageBox()->Show("Exception", NXOpen::NXMessageBox::DialogTypeError, e2.what());
	}
	catch (...)
	{
		UI::GetUI()->NXMessageBox()->Show("Exception", NXOpen::NXMessageBox::DialogTypeError, "Unknown Exception.");
	}
}
                                       
//------------------------------------------------------------------------------
// Unload Handler
//------------------------------------------------------------------------------
extern "C" DllExport int ufusr_ask_unload()
{
  // Unloads the image when NX session terminates
	//return (int)Session::LibraryUnloadOptionAtTermination;   // unload the dll when NX is closed
	return (int)Session::LibraryUnloadOptionImmediately;   // unload the dll immediately
  
}


//------------------------------------------------------------------------------
// Custom Functions
//------------------------------------------------------------------------------

void NXExtractor::PrintComponentTree(Assemblies::Component* component, int depth)
{
	if (component == nullptr)
	{
		return;
	}

	std::string indent(depth * 2, ' ');  // to print the assembly hierarchy better

	// Extract the position & orientation
	NXOpen::Point3d position;
	NXOpen::Matrix3x3 orientation;
	component->GetPosition(&position, &orientation);

	print(indent + component->DisplayName());  // display name
	print(indent + " L Journal Identifier : " + component->JournalIdentifier());  // Journal Identifier

	// position
	print(indent + " L Position : (" 
		+ std::to_string(position.X) + ","
	+ std::to_string(position.Y) + ", "
	+ std::to_string(position.Z) + ")");

	// orientation
	// to-do (after EulerAngles conversion)

	// Children count
	std::vector<Assemblies::Component*> children = component->GetChildren();
	print(indent + " L Children Count : " + std::to_string(children.size()));

	// Suppressed (boolean)
	print(indent + " L Suppressed : " + std::to_string(component->IsSuppressed()));

	print("");

	for (Assemblies::Component* child : children)
	{
		PrintComponentTree(child, depth + 1);
	}
}

void NXExtractor::PrintBRepTree(Part* part)
{
	BodyCollection* bodies = part->Bodies();
	int bodyIndex = 0;

	for (BodyCollection::iterator iter = bodies->begin(); iter != bodies->end(); ++iter)
	{
		Body* body = *iter;
		bodyIndex++;

		print("Body [" + std::to_string(bodyIndex) + "]");

		std::vector<Face*> faces = body->GetFaces();

		print(" L Faces :" + std::to_string(faces.size()));

		for (int i=0; i< faces.size();i++)
		{
			std::string faceType = FaceTypeToString(faces[i]->SolidFaceType());
			print("  L Face [" + std::to_string(i)+"]: " + faceType);

			std::vector<Edge*> edges = faces[i]->GetEdges();
			print("   L Edges : " + std::to_string(edges.size()));

			for (int j = 0; j < edges.size(); j++)
			{
				std::string edgeType = EdgeTypeToString(edges[j]->SolidEdgeType());
				print("    L Edge [" + std::to_string(j) + "]: " + edgeType);
			}
		}
	}
}

void NXExtractor::PrintFeatureTree(Part* part)
{
	BodyCollection* bodies = part->Bodies();
	int bodyIndex = 0;

	for (BodyCollection::iterator iter = bodies->begin(); iter != bodies->end(); ++iter)
	{
		Body* body = *iter;
		bodyIndex++;

		print("Body [" + std::to_string(bodyIndex) + "]");

		std::vector<NXOpen::Features::Feature*> features = body->GetFeatures();

		print(" L Features :" + std::to_string(features.size()));

		for (int i = 0; i < features.size(); i++)
		{
			NXOpen::NXString featureType = features[i]->FeatureType();
			NXOpen::NXString featureName = features[i]->GetFeatureName();
			print("  L Feature [" + std::to_string(i) + "]");
			print("    Type : " + featureType);
			print("    Name : " + featureName);
		}
	}
}

void NXExtractor::PrintFeatureExpressions(Part* part)
{
	print("=== NX Expression ===");

	BodyCollection* bodies = part->Bodies();

	for (BodyCollection::iterator iter = bodies->begin(); iter != bodies->end(); ++iter)
	{
		Body* body = *iter;

		std::vector<NXOpen::Features::Feature*> features = body->GetFeatures();

		for (int i = 0; i < features.size(); i++)
		{
			std::vector<Expression*> expressions = features[i]->GetExpressions();

			NXOpen::NXString featureType = features[i]->FeatureType();
			print("Feature [" + std::to_string(i) + "] : " + featureType);

			for (int j = 0; j < expressions.size(); j++)
			{
				NXOpen::NXString name = expressions[j]->Name();
				NXOpen::NXString formula = expressions[j]->GetFormula();
				double value = expressions[j]->Value();

				print(" Expression [" + std::to_string(j) + "]");
				print("  L Name : " + name);
				print("  L Formula : " + formula);
				print("  L Value : " + std::to_string(value));
				print("");
			}
		}
	}
}

void NXExtractor::PrintDimensions(Part* part)
{
	NXOpen::Annotations::DimensionCollection* dimensions = part->Dimensions();
	int dimIndex = 0;

	for (NXOpen::Annotations::DimensionCollection::iterator iter = dimensions->begin(); iter != dimensions->end(); ++iter)
	{
		NXOpen::Annotations::Dimension* dimension = *iter;
		dimIndex++;

		// value
		double value = dimension->ComputedSize();

		// text
		std::vector<NXOpen::NXString> mainText;
		std::vector<NXOpen::NXString> dualText;
		dimension->GetDimensionText(mainText, dualText);

		// measurement type
		NXOpen::Annotations::Dimension::MeasurementTypes dimType = dimension->GetMeasurementType();

		// Associated object
		std::vector<NXOpen::NXObject*> obj = dimension->GetUserAttributeSourceObjects();

		print("Dimension [" + std::to_string(dimIndex) + "]");
		print(" L Value : " + std::to_string(value));
		print(" L Text : " + mainText[0]);
		print(" L Type : " + std::to_string(dimType));
	}

	print("Total Dimensions : " + std::to_string(dimIndex));
}

std::string NXExtractor::FaceTypeToString(Face::FaceType faceType)
{
	switch (faceType)
	{
	case Face::FaceTypePlanar:
		return "Plane";

	case Face::FaceTypeCylindrical:
		return "Cylinder";

	case Face::FaceTypeConical:
		return "Cone";

	case Face::FaceTypeSpherical:
		return "Sphere";

	case Face::FaceTypeSurfaceOfRevolution:
		return "Surface of Revolution";

	case Face::FaceTypeParametric:
		return "Parametric Surface";

	case Face::FaceTypeBlending:
		return "Blend";

	case Face::FaceTypeOffset:
		return "Offset Surface";

	case Face::FaceTypeSwept:
		return "Swept Surface";

	case Face::FaceTypeRubber:
		return "Rubber";

	default:
		return "Undefined";
	}
}

std::string NXExtractor::EdgeTypeToString(Edge::EdgeType edgeType)
{
	switch (edgeType)
	{
	case Edge::EdgeTypeLinear:
		return "Line";

	case Edge::EdgeTypeCircular:
		return "Circle";

	case Edge::EdgeTypeElliptical:
		return "Ellipse";

	case Edge::EdgeTypeSpline:
		return "Spline";

	case Edge::EdgeTypeIntersection:
		return "Intersection Curve";

	case Edge::EdgeTypeSpCurve:
		return "SP Curve";

	case Edge::EdgeTypeTrimmedCurve:
		return "Trimmed Curve";

	case Edge::EdgeTypeConvergent:
		return "Convergent";

	case Edge::EdgeTypeForeign:
		return "Foreign";

	case Edge::EdgeTypeConstantParameter:
		return "Constant Parameter";

	case Edge::EdgeTypeRubber:
		return "Rubber";

	default:
		return "Undefined";
	}
}