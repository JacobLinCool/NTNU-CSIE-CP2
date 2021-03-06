typedef enum Language {
    c_ansi,
    c_18,
    cs,
    cpp_17,
    cpp_98,
    dart,
    elixir,
    erlang,
    fortran,
    go,
    js,
    java,
    kotlin,
    lua,
    matlab,
    objective_c,
    php,
    python_2,
    python,
    r,
    ruby,
    rust,
    scala,
    swift,
    visual,
    unknown_language,
} Language;

int language_count = unknown_language;

char* languages[] = {
   "c-ansi",
   "c",
   "c#",
   "cpp",
   "cpp-98",
   "dart",
   "elixir",
   "erlang",
   "fortran",
   "go",
   "javascript",
   "java",
   "kotlin",
   "lua",
   "matlab",
   "objective-c",
   "php",
   "python-2",
   "python",
   "r",
   "ruby",
   "rust",
   "scala",
   "swift",
   "visual-basic",
};

char* keywords[][220] = {
    /* C (ANSI) (32 keywords) */ { "auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while" },
    /* C (C18) (44 keywords) */ { "auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "inline", "int", "long", "register", "restrict", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while", "_Alignas", "_Alignof", "_Atomic", "_Bool", "_Complex", "_Generic", "_Imaginary", "_Noreturn", "_Static_assert", "_Thread_local" },
    /* C# (8.0) (107 keywords) */ { "abstract", "as", "base", "bool", "break", "byte", "case", "catch", "char", "checked", "class", "const", "continue", "decimal", "default", "delegate", "do", "double", "else", "enum", "event", "explicit", "extern", "false", "finally", "fixed", "float", "for", "foreach", "goto", "if", "implicit", "in", "int", "interface", "internal", "is", "lock", "long", "namespace", "new", "null", "object", "operator", "out", "override", "params", "private", "protected", "public", "readonly", "ref", "return", "sbyte", "sealed", "short", "sizeof", "stackalloc", "static", "string", "struct", "switch", "this", "throw", "true", "try", "typeof", "uint", "ulong", "unchecked", "unsafe", "ushort", "using", "virtual", "void", "volatile", "while", "add", "alias", "ascending", "async", "await", "by", "descending", "dynamic", "equals", "from", "get", "global", "group", "into", "join", "let", "nameof", "notnull", "on", "orderby", "partial", "remove", "select", "set", "unmanaged", "value", "var", "when", "where", "yield" },
    /* C++ (C++17) (84 keywords) */ { "alignas", "alignof", "asm", "auto", "bool", "break", "case", "catch", "char", "char16_t", "char32_t", "class", "const", "constexpr", "const_cast", "continue", "decltype", "default", "delete", "do", "double", "dynamic_cast", "else", "enum", "explicit", "export", "extern", "false", "float", "for", "friend", "goto", "if", "inline", "int", "long", "mutable", "namespace", "new", "noexcept", "nullptr", "operator", "private", "protected", "public", "register", "reinterpret_cast", "return", "short", "signed", "sizeof", "static", "static_assert", "static_cast", "struct", "switch", "template", "this", "thread_local", "throw", "true", "try", "typedef", "typeid", "typename", "union", "unsigned", "using", "virtual", "void", "volatile", "wchar_t", "while", "and", "and_eq", "bitand", "bitor", "compl", "not", "not_eq", "or", "or_eq", "xor", "xor_eq" },
    /* C++ (C++98) (74 keywords) */ { "asm", "auto", "bool", "break", "case", "catch", "char", "class", "const", "const_cast", "continue", "default", "delete", "do", "double", "dynamic_cast", "else", "enum", "explicit", "export", "extern", "false", "float", "for", "friend", "goto", "if", "inline", "int", "long", "mutable", "namespace", "new", "operator", "private", "protected", "public", "register", "reinterpret_cast", "return", "short", "signed", "sizeof", "static", "static_cast", "struct", "switch", "template", "this", "throw", "true", "try", "typedef", "typeid", "typename", "union", "unsigned", "using", "virtual", "void", "volatile", "wchar_t", "while", "and", "and_eq", "bitand", "bitor", "compl", "not", "not_eq", "or", "or_eq", "xor", "xor_eq" },
    /* Dart (2.2) (33 keywords) */ { "assert", "break", "case", "catch", "class", "const", "continue", "default", "do", "else", "enum", "extends", "false", "final", "finally", "for", "if", "in", "is", "new", "null", "rethrow", "return", "super", "switch", "this", "throw", "true", "try", "var", "void", "while", "with" },
    /* Elixir (1.10) (15 keywords) */ { "true", "false", "nil", "when", "and", "or", "not", "in", "fn", "do", "end", "catch", "rescue", "after", "else" },
    /* Erlang (23) (27 keywords) */ { "after", "and", "andalso", "band", "begin", "bnot", "bor", "bsl", "bsr", "bxor", "case", "catch", "cond", "div", "end", "fun", "if", "let", "not", "of", "or", "orelse", "receive", "rem", "try", "when", "xor" },
    /* Fortran (Fortran 2008) (103 keywords) */ { "abstract", "allocatable", "allocate", "assign", "associate", "asynchronous", "backspace", "bind", "block", "block data", "call", "case", "class", "close", "codimension", "common", "contains", "contiguous", "continue", "critical", "cycle", "data", "deallocate", "deferred", "dimension", "do", "do concurrent", "elemental", "else", "else if", "elsewhere", "end", "endfile", "endif", "entry", "enum", "enumerator", "equivalence", "error stop", "exit", "extends", "external", "final", "flush", "forall", "format", "function", "generic", "goto", "if", "implicit", "import", "include", "inquire", "intent", "interface", "intrinsic", "lock", "module", "namelist", "non_overridable", "nopass", "nullify", "only", "open", "operator", "optional", "parameter", "pass", "pause", "pointer", "print", "private", "procedure", "program", "protected", "public", "pure", "read", "recursive", "result", "return", "rewind", "rewrite", "save", "select", "sequence", "stop", "submodule", "subroutine", "sync all", "sync images", "sync memory", "target", "then", "unlock", "use", "value", "volatile", "wait", "where", "while", "write" },
    /* Go (1.15) (25 keywords) */ { "break", "case", "chan", "const", "continue", "default", "defer", "else", "fallthrough", "for", "func", "go", "goto", "if", "import", "interface", "map", "package", "range", "return", "select", "struct", "switch", "type", "var" },
    /* JS (ES2020) (38 keywords) */ { "await", "break", "case", "catch", "class", "const", "continue", "debugger", "default", "delete", "do", "else", "enum", "export", "extends", "false", "finally", "for", "function", "if", "import", "in", "instanceof", "new", "null", "return", "super", "switch", "this", "throw", "true", "try", "typeof", "var", "void", "while", "with", "yield" },
    /* Java (SE 14) (51 keywords) */ { "abstract", "assert", "boolean", "break", "byte", "case", "catch", "char", "class", "const", "continue", "default", "do", "double", "else", "enum", "extends", "final", "finally", "float", "for", "if", "goto", "implements", "import", "instanceof", "int", "interface", "long", "native", "new", "package", "private", "protected", "public", "return", "short", "static", "strictfp", "super", "switch", "synchronized", "this", "throw", "throws", "transient", "try", "void", "volatile", "while", "_" },
    /* Kotlin (1.4) (79 keywords) */ { "as", "as?", "break", "class", "continue", "do", "else", "false", "for", "fun", "if", "in", "!in", "interface", "is", "!is", "null", "object", "package", "return", "super", "this", "throw", "true", "try", "typealias", "typeof", "val", "var", "when", "while", "by", "catch", "constructor", "delegate", "dynamic", "field", "file", "finally", "get", "import", "init", "param", "property", "receiver", "set", "setparam", "where", "actual", "abstract", "annotation", "companion", "const", "crossinline", "data", "enum", "expect", "external", "final", "infix", "inline", "inner", "internal", "lateinit", "noinline", "open", "operator", "out", "override", "private", "protected", "public", "reified", "sealed", "suspend", "tailrec", "vararg", "field", "it" },
    /* Lua (5.3) (22 keywords) */ { "and", "break", "do", "else", "elseif", "end", "false", "for", "function", "goto", "if", "in", "local", "nil", "not", "or", "repeat", "return", "then", "true", "until", "while" },
    /* MATLAB (R2020a) (20 keywords) */ { "break", "case", "catch", "classdef", "continue", "else", "elseif", "end", "for", "function", "global", "if", "otherwise", "parfor", "persistent", "return", "spmd", "switch", "try", "while" },
    /* Objective-C (2.0) (85 keywords) */ { "asm", "auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "inline", "int", "long", "register", "restrict", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while", "_Bool", "_Complex", "__block", "Imaginary", "id", "Class", "SEL", "IMP", "BOOL", "nil", "Nil", "YES", "NO", "self", "super", "_cmd", "__strong", "__weak", "__autoreleasing", "__unsafe_unretained", "oneway", "In", "out", "inout", "bycopy", "byref", "@autoreleasepool", "@interface", "@implementation", "@protocol", "@end", "@private", "@protected", "@public", "@package", "@try", "@throw", "@catch()", "@finally", "@property", "@synthesize", "@dynamic", "@class", "@selector()", "@protocol()", "@required", "@optional", "@encode", "@\"string\"", "@synchronized()" },
    /* PHP (7.4) (69 keywords) */ { "__halt_compiler()", "abstract", "and", "array()", "as", "break", "callable", "case", "catch", "class", "clone", "const", "continue", "declare", "default", "die()", "do", "echo", "else", "elseif", "empty()", "enddeclare", "endfor", "endforeach", "endif", "endswitch", "endwhile", "eval()", "exit()", "extends", "final", "finally", "fn", "for", "foreach", "function", "global", "goto", "if", "implements", "include", "include_once", "instanceof", "insteadof", "interface", "isset()", "list()", "namespace", "new", "or", "print", "private", "protected", "public", "require", "require_once", "return", "static", "switch", "throw", "trait", "try", "unset()", "use", "var", "while", "xor", "yield", "yield from" },
    /* Python (2.7) (31 keywords) */ { "and", "as", "assert", "break", "class", "continue", "def", "del", "elif", "else", "except", "exec", "finally", "for", "from", "global", "if", "import", "in", "is", "lambda", "not", "or", "pass", "print", "raise", "return", "try", "while", "with", "yield" },
    /* Python (3.8) (35 keywords) */ { "False", "None", "True", "and", "as", "assert", "async", "await", "break", "class", "continue", "def", "del", "elif", "else", "except", "finally", "for", "from", "global", "if", "import", "in", "is", "lambda", "nonlocal", "not", "or", "pass", "raise", "return", "try", "while", "with", "yield" },
    /* R (4.0) (21 keywords) */ { "...", "..1", "FALSE", "Inf", "NA", "NA_character_", "NA_complex_", "NA_integer_", "NA_real_", "NaN", "NULL", "TRUE", "break", "else", "for", "function", "if", "in", "next", "repeat", "while" },
    /* Ruby (2.7) (41 keywords) */ { "ENCODING", "LINE", "FILE", "BEGIN", "END", "alias", "and", "begin", "break", "case", "class", "def", "defined?", "do", "else", "elsif", "end", "ensure", "false", "for", "if", "in", "module", "next", "nil", "not", "or", "redo", "rescue", "retry", "return", "self", "super", "then", "true", "undef", "unless", "until", "when", "while", "yield" },
    /* Rust (1.46) (53 keywords) */ { "as", "break", "const", "continue", "crate", "else", "enum", "extern", "false", "fn", "for", "if", "impl", "in", "let", "loop", "match", "mod", "move", "mut", "pub", "ref", "return", "self", "Self", "static", "struct", "super", "trait", "true", "type", "unsafe", "use", "where", "while", "async", "await", "dyn", "abstract", "become", "box", "do", "final", "macro", "override", "priv", "typeof", "unsized", "virtual", "yield", "try", "union", "'static" },
    /* Scala (2.13) (40 keywords) */ { "abstract", "case", "catch", "class", "def", "do", "else", "extends", "false", "final", "finally", "for", "forSome", "if", "implicit", "import", "lazy", "macro", "match", "new", "null", "object", "override", "package", "private", "protected", "return", "sealed", "super", "this", "throw", "trait", "try", "true", "type", "val", "var", "while", "with", "yield" },
    /* Swift (5.3) (97 keywords) */ { "associatedtype", "class", "deinit", "enum", "extension", "fileprivate", "func", "import", "init", "inout", "internal", "let", "open", "operator", "private", "protocol", "public", "rethrows", "static", "struct", "subscript", "typealias", "var", "break", "case", "continue", "default", "defer", "do", "else", "fallthrough", "for", "guard", "if", "in", "repeat", "return", "switch", "where", "while", "as", "Any", "catch", "false", "is", "nil", "super", "self", "Self", "throw", "throws", "true", "try", "_", "#available", "#colorLiteral", "#column", "#else", "#elseif", "#endif", "#error", "#file", "#filePath", "#fileLiteral", "#function", "#if", "#imageLiteral", "#line", "#selector", "#sourceLocation", "#warning", "associativity", "convenience", "dynamic", "didSet", "final", "get", "infix", "indirect", "lazy", "left", "mutating", "none", "nonmutating", "optional", "override", "postfix", "precedence", "prefix", "Protocol", "required", "right", "set", "Type", "unowned", "weak", "willSet" },
    /* Visual Basic (2019) (217 keywords) */ { "AddHandler", "AddressOf", "Alias", "And", "AndAlso", "As", "Boolean", "ByRef", "Byte", "ByVal", "Call", "Case", "Catch", "CBool", "CByte", "CChar", "CDate", "CDbl", "CDec", "Char", "CInt", "Class", "CLng", "CObj", "Const", "Continue", "CSByte", "CShort", "CSng", "CStr", "CType", "CUInt", "CULng", "CUShort", "Date", "Decimal", "Declare", "Default", "Delegate", "Dim", "DirectCast", "Do", "Double", "Each", "Else", "ElseIf", "End", "EndIf", "Enum", "Erase", "Error", "Event", "Exit", "False", "Finally", "For", "For Each", "Friend", "Function", "Get", "GetType", "GetXMLNamespace", "Global", "GoSub", "GoTo", "Handles", "If", "If()", "Implements", "Imports", "In", "Inherits", "Integer", "Interface", "Is", "IsNot", "Let", "Lib", "Like", "Long", "Loop", "Me", "Mod", "Module", "MustInherit", "MustOverride", "MyBase", "MyClass", "NameOf", "Namespace", "Narrowing", "New", "Next", "Not", "Nothnig", "NotInheritable", "NotOverridable", "Object", "Of", "On", "Operator", "Option", "Optional", "Or", "OrElse", "Out", "Overloads", "Overridable", "Overrides", "ParamArray", "Partial", "Private", "Property", "Protected", "Public", "RaiseEvent", "ReadOnly", "ReDim", "REM", "RemoveHandler", "Resume", "Return", "SByte", "Select", "Set", "Shadows", "Shared", "Short", "Single", "Static", "Step", "Stop", "String", "Structure", "Sub", "SyncLock", "Then", "Throw", "To", "True", "Try", "TryCast", "TypeOf", "UInteger", "ULong", "UShort", "Using", "Variant", "Wend", "When", "While", "Widening", "With", "WithEvents", "WriteOnly", "Xor", "#Const", "#Else", "#ElseIf", "#End", "#If", "=", "&", "&=", "*", "*=", "/", "/=", "\\", "=", "^", "^=", "+", "+=", "-", "-=", ">>", ">>=", "<<", "<<=", "Aggregate", "Ansi", "Assembly", "Async", "Auto", "Await", "Binary", "Compare", "Custom", "Distinct", "Equals", "Explicit", "From", "Group By", "Group Join", "Into", "IsFalse", "IsTrue", "Iterator", "Join", "Key", "Mid", "Off", "Order By", "Preserve", "Skip", "Skip While", "Strict", "Take", "Take While", "Text", "Unicode", "Until", "Where", "Yield", "#ExternalSource", "#Region" },
};

int keyword_sizes[] = { 32, 44, 107, 84, 74, 33, 15, 27, 103, 25, 38, 51, 79, 22, 20, 85, 69, 31, 35, 21, 41, 53, 40, 97, 217 };

char* single_line_comment[] = {
   "//",
   "//",
   "//",
   "//",
   "//",
   "//",
   "#",
   "%",
   "!",
   "//",
   "//",
   "//",
   "//",
   "--",
   "%",
   "//",
   "//",
   "#",
   "#",
   "#",
   "#",
   "//",
   "//",
   "//",
   "'",
};

char* multi_line_comment[][2] = {
   { "/*", "*/" },
   { "/*", "*/" },
   { "/*", "*/" },
   { "/*", "*/" },
   { "/*", "*/" },
   { "/*", "*/" },
   { "\"\"\"", "\"\"\"" },
   { NULL, NULL },
   { NULL, NULL },
   { "/*", "*/" },
   { "/*", "*/" },
   { "/*", "*/" },
   { "/*", "*/" },
   { "--[[", "--]]" },
   { "%{", "}%" },
   { "/*", "*/" },
   { "/*", "*/" },
   { "\"\"\"", "\"\"\"" },
   { "\"\"\"", "\"\"\"" },
   { NULL, NULL },
   { "=begin", "=end" },
   { "/*", "*/" },
   { "/*", "*/" },
   { "/*", "*/" },
   { NULL, NULL },
};

int is_keyword(Language lang, char* keyword) {
    for (int i = 0; i < keyword_sizes[lang]; i++) {
        if (strcmp(keywords[lang][i], keyword) == 0) {
            return i;
        }
    }
    return -1;
}
