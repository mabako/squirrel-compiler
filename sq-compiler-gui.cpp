#include <sstream>
#include <string>
#include <squirrel.h>
#include <sqstdio.h>
#include <Windows.h>

bool bKnownError = false;

void compile_error_handler(HSQUIRRELVM v, const SQChar* desc, const SQChar* source, SQInteger line, SQInteger column)
{
	bKnownError = true;

	std::stringstream out;
	out << "Error in " << source << " on line " << line << " column " << column << ": " << desc << std::endl;
	MessageBox(NULL, out.str().c_str(), "Squirrel Compiler", MB_OK | MB_ICONERROR);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	std::string infile(lpCmdLine);
	std::stringstream out;

	if (infile.length() < 3)
	{
		OPENFILENAME ofn;
		char szFile[260];

		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = szFile;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = "Squirrel Files (*.nut)\0*.nut\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetOpenFileName(&ofn) == TRUE)
			infile = ofn.lpstrFile;
		else
			return 1;
	}

	// open vm
	HSQUIRRELVM v = sq_open(1024);
	if (!v) {
		MessageBox(NULL, "Error: Could not open Squirrel VM", "Squirrel Compiler", MB_OK | MB_ICONERROR);
		return 2;
	}

	// set compile error handler
	sq_setcompilererrorhandler(v, compile_error_handler);

	// compile source file
	if (SQ_FAILED(sqstd_loadfile(v, infile.c_str(), SQTrue))) {
		if (!bKnownError)
		{
			out << "Error: Could not compile source file " << infile << std::endl;
			MessageBox(NULL, out.str().c_str(), "Squirrel Compiler", MB_OK | MB_ICONERROR);
		}

		sq_close(v);

		return 3;
	}

	// serialize closure containing the source
	std::string outfile(infile.c_str());
	outfile = outfile.substr(0, outfile.length() - 4) + ".out";
	if (SQ_FAILED(sqstd_writeclosuretofile(v, outfile.c_str()))) {
		MessageBox(NULL, "Error: Could not serialize closure", "Squirrel Compiler", MB_OK | MB_ICONERROR);
	}
	
	// clean up.
	sq_close(v);
	
	return 0;
}