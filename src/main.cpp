#include "../include/ops.h"
#include "../include/compiler.h"
#include "../include/lexer.h"
#include "../include/error.h"
#include "../include/parser.h"
#include "../include/checks.h"
#include "../include/program.h"
#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>

#define exit_on_error(exit_code) if (exit_code != 0) exit(exit_code);

void usage(char* exec_name)
{
	std::cout << "USAGE: " << exec_name << " <FILENAME> <OPTIONS>"        << std::endl;
	std::cout << "OPTIONS:"                                               << std::endl;
	std::cout << "\t-h, --help\tshow help"                                << std::endl;
	std::cout << "\t-f, --fasm\tuse fasm to compile program"              << std::endl;
	std::cout << "\t-n, --nasm\tuse nasm to compile program"              << std::endl;
	std::cout << "\t-u, --unsafe\tcompile without type checking program"  << std::endl;
}

struct Options
{
	ASSEMBLER assembler;
	std::string filename;
	bool help;
	bool unsafe;
};
Options parse_argv(std::vector<std::string> argv)
{
	static_assert(ASSEMBLER_COUNT == 2, "unhandled assemblers in parse_argv()");

	Options options;
	options.assembler = FASM;
	options.filename = "";
	options.help = false;
	options.unsafe = false;

	for (long unsigned int i = 1; i < argv.size(); i++)
	{
		std::string arg = argv.at(i);
		if (arg == "-f" || arg == "--fasm")
			options.assembler = FASM;
		else if (arg == "-n" || arg == "--nasm")
			options.assembler = NASM;
		else if (arg == "-u" || arg == "--unsafe")
			options.unsafe = true;
		else if (arg == "-h" || arg == "--help")
			options.help = true;

		else if (options.filename == "")
			options.filename = arg;
		else
		{
			print_error("more than one file provided for compilation.");
			exit(1);
		}
	}

	return options;
}

int main(int argc, char* argv[]) 
{
	// parse argv
	Options options = parse_argv(std::vector<std::string>(argv, argv + argc));

	if (options.help)
	{
		usage(argv[0]);
		exit(0);
	}

	// no filename passed into argv
	if (options.filename == "")
	{
		usage(argv[0]);
		print_error("no file program for compilation.");
		exit(1);
	}

	// parse file
	std::vector<Token> tokens = tokenize_file(options.filename);
	Program program = parse_tokens(tokens);
	verify_program(program);

	if (!options.unsafe)
		type_check_program(program);

	// compile
	std::cout << "[info] generating asm from '" << options.filename << "'." << std::endl;
	compile_to_asm(program, "out.asm", options.assembler);

	// nasm or fasm
	std::cout << "[info] compiling assembly." << std::endl;

	static_assert(ASSEMBLER_COUNT == 2, "unhandled assemblers in main()");
	if (options.assembler == FASM)
	{
		exit_on_error(std::system("fasm out.asm"));
	}
	else
	{
		exit_on_error(std::system("nasm -felf64 out.asm"));

		// link
		std::cout << "[info] linking object file." << std::endl;
		exit_on_error(std::system("ld -o out out.o"));

		// cleanup
		std::cout << "[info] cleaning up object file" << std::endl;
		exit_on_error(std::system("rm out.o"));
	}

	return 0;
}
