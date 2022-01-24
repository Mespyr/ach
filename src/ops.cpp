#include "../include/ops.h"

bool is_builtin_word(std::string word)
{
    static_assert(OP_COUNT == 56, "unhandled op types in type_check_program()");
    std::vector<std::string> builtin_words = {
        "dump",
        "+", "-", "*", "/",
        "=", ">", "<", ">=", "<=", "!=", "not", "and", "or", 
        "pop", "dup", "swp", "rot", "over",
        "read8", "write8", "read16", "write16", "read32", "write32", "read64", "write64",
        "argv", "argc",
        ">>", "<<", "orb", "andb",
        "syscall1", "syscall2", "syscall3", "syscall4", "syscall5", "syscall6",
        "offset", "reset",
        "while", "do", "if", "else", "end", "def", "in", "const", "@include"
    };

    for (std::string w : builtin_words)
        if (w == word)
            return true;

    return false;
};
