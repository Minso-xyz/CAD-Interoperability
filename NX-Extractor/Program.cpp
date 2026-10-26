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
#include <NXOpen/Assemblies_Component.hxx>
#include <NXOpen/Assemblies_ComponentAssembly.hxx>
#include <NXOpen/Body.hxx>
#include <NXOpen/BodyCollection.hxx>
#include <NXOpen/Face.hxx>
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


	// Print the display name of assembly
	Part* part = theSession->Parts()->Work();

	Assemblies::ComponentAssembly* componentAssembly = part->ComponentAssembly();
	Assemblies::Component* root = componentAssembly->RootComponent();

	if (root == nullptr)
	{
		print("Work part is not an assembly!");
		return;
	}

	NXOpen::NXString nameRoot = root->DisplayName();
	print("Assembly root found : " + nameRoot);

	// Root extraction
	print("=== List of children ===");
	std::vector<Assemblies::Component*> rootChildren = root->GetChildren();
	for (auto child : rootChildren)
	{
		print("L " + child->DisplayName());
	}
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

