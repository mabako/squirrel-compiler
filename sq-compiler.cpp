#include <iostream>
#include <squirrel.h>
#include <sqstdio.h>

void compile_error_handler(HSQUIRRELVM v, const SQChar* desc, const SQChar* source, SQInteger line, SQInteger column)
{
	std::cout << "Error in " << source << ":" << line << ":" << column << ", " << desc << std::endl;
}

int main(int argc, char** argv)
{
	if (argc < 3) {
		std::cout << "Usage: " << argv[0] << " [source file] [destination file]" << std::endl;
		return 1;
	}

	// open vm
	HSQUIRRELVM v = sq_open(1024);
	if (!v) {
		std::cout << "Error: Could not open Squirrel VM" << std::endl;
		return 2;
	}

	// set compile error handler
	sq_setcompilererrorhandler(v, compile_error_handler);

	// compile source file
	if (SQ_FAILED(sqstd_loadfile(v, argv[1], SQTrue))) {
		std::cout << "Error: Could not compile source file " << argv[1] << std::endl;
		sq_close(v);
		return 3;
	}

	// serialize closure containing the source
	if (SQ_FAILED(sqstd_writeclosuretofile(v, argv[2]))) {
		std::cout << "Error: Could not serialize closure" << std::endl;
	}

	// clean up.
	sq_close(v);
	
	return 0;
}