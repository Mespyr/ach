#include "../include/ops.h"

bool is_builtin_word(std::string word)
{
	static_assert(OP_COUNT == 59, "unhandled op types in type_check_program()");
	std::vector<std::string> builtin_words = {
		"dump",
		"+", "-", "*", "/",
		"=", ">", "<", ">=", "<=", "!=", "not", "and", "or",
		"pop", "dup", "swp", "rot", "over",
		"@8", "!8", "@16", "!16", "@32", "!32", "@64", "!64",
		"argv", "argc",
		">>", "<<",
		"syscall0", "syscall1", "syscall2", "syscall3", "syscall4", "syscall5", "syscall6",
		"offset", "reset",
		"while", "do", "if", "else", "end", "def", "in", "const", "memory", "set", "include"
	};

	for (std::string w : builtin_words)
		if (w == word)
			return true;

	return false;
};
