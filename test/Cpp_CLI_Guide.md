## Which C++ Command Line Interface?

first know that `cint` is actually `root` because I did this:

```sh
alias cint="cint"
```

## `% cint --help`

```
usage: root [-b B] [-x X] [-e E] [-n N] [-t T] [-q Q] [-l L] [-a A] [-config CONFIG] [-h HELP] [--version VERSION] [--notebook NOTEBOOK] [--web WEB]
            [--web=<type> WEB=<TYPE>] [--web=off WEB=OFF]
            [dir] [data1.root...dataN.root] [file1.C...fileN.C] [file1_C.so...fileN_C.so]

ROOTs Object-Oriented Technologies.

root is an interactive interpreter of C++ code. It uses the ROOT  framework.  For  more information on ROOT, please refer to

An extensive Users Guide is available from that site (see below).

OPTIONS:
  -b                          Run in batch mode without graphics
  -x                          Exit on exceptions
  -e                          Execute the command passed between single quotes
  -n                          Do not execute logon and logoff macros as specified in .rootrc
  -t                          Enable thread-safety and implicit multi-threading (IMT)
  -q                          Exit after processing command line macro files
  -l                          Do not show the ROOT banner
  -a                          Show the ROOT splash screen
  -config                     print ./configure options
  -h, -?, --help              Show summary of options
  --version                   Show the ROOT version
  --notebook                  Execute ROOT notebook
  --web                       Use web-based display for graphics, browser, geometry
  --web=<type>                Use the specified web-based display such as chrome, firefox, qt6
                              For more options see the documentation of TROOT::SetWebDisplay()
  --web=off                   Disable any kind of web-based display
  [dir]                       if dir is a valid directory cd to it before executing
  [data1.root...dataN.root]   Open the given ROOT files; remote protocols (such as http://) are supported
  [file1.C...fileN.C]         Execute the ROOT macro file1.C ... fileN.C
                              Compilation flags as well as macro arguments can be passed, see format in https://root.cern/manual/root_macros_and_shared_libraries/
  [file1_C.so...fileN_C.so]   Load and execute file1_C.so ... fileN_C.so (or .dll if on Windows)
                              They should be already-compiled ROOT macros (shared libraries) or:
                              regular user shared libraries e.g. userlib.so with a function userlib(args)
```

## Splash (about `root`/`cint`/`cling`)

```
% cint
   ------------------------------------------------------------------
  | Welcome to ROOT 6.33.01                        https://root.cern |
  | (c) 1995-2024, The ROOT Team; conception: R. Brun, F. Rademakers |
  | Built for macosxarm64 on Jun 18 2024, 19:03:07                   |
  | From heads/master@v6-31-01-2392-g86a5a71347                      |
  | With Apple clang version 15.0.0 (clang-1500.3.9.4)               |
  | Try '.help'/'.?', '.demo', '.license', '.credits', '.quit'/'.q'  |
   ------------------------------------------------------------------
```


## `root [0] .help`

```
 Cling (C/C++ interpreter) meta commands usage
 All commands must be preceded by a '.', except
 for the evaluation statement { }
 ==============================================================================
 Syntax: .Command [arg0 arg1 ... argN]

   .L <filename>              Load the given file or library
   .(x|X) <filename>[(args)]  Same as .L and runs a function with signature: ret_type filename(args)
   .> <filename>              Redirect command to a given file
      '>' or '1>'             Redirects the stdout stream only
      '2>'                    Redirects the stderr stream only
      '&>' (or '2>&1')        Redirects both stdout and stderr
      '>>'                    Appends to the given file
   .undo [n]                  Unloads the last 'n' inputs lines
   .U <filename>              Unloads the given file
   .(I|include) [path]        Shows all include paths. If a path is given, adds the path to the include paths.
   .O <level>                 Sets the optimization level (0-3) If no level is given, prints the current setting.
   .class <name>              Prints out class <name> in a CINT-like style (one-level). If no name is given, prints out list of all classes.
   .Class <name>              Prints out class <name> in a CINT-like style (all-levels). If no name is given, prints out list of all classes.
   .namespace                 Prints list of all known namespaces
   .typedef <name>            Prints out typedef <name> in a CINT-like style If no name is given, prints out list of all typedefs.
   .files                     Prints names of all included (parsed) files
   .fileEx                    Prints out included (parsed) file statistics as well as a list of their names
   .g <var>                   Prints out information about global variable 'var' – if no name is given, print them all
   .@                         Cancels and ignores the multiline input
   .rawInput [0|1]            Toggle wrapping and printing the execution results of the input
   .dynamicExtensions [0|1]   Toggles the use of the dynamic scopes and the late binding
   .debug <level>             Generates debug symbols (level is optional, 0 to disable)
   .printDebug [0|1]          Toggles the printing of input's corresponding state changes
   .storeState <filename>     Store the interpreter's state to a given file
   .compareState <filename>   Compare the interpreter's state with the one saved in a given file
   .stats [name]              Show stats for internal data structures
                              'ast'  abstract syntax tree stats
                              'asttree [filter]'  abstract syntax tree layout
                              'decl' dump ast declarations
                              'undo' show undo stack
   .T <filePath> <comment>    Generate autoload map
   .trace <repr> <id>         Dump trace of requested respresentation (see .stats arguments for <repr>)
   .help                      Shows this information (also .?)
   .q                         Exit the program


 ROOT special commands.
 ==============================================================================
   .L <filename>[flags]: load the given file with optional flags like + to compile or ++ to force recompile. 
                         Type `.? TSystem::CompileMacro` for a list of all flags. 
                         <filename> can also be a shared library; skip flags.
   .(x|X) <filename>[flags](args) : same as .L <filename>[flags] and runs then a function with signature: ret_type filename(args).
   .credits            : show credits
   .demo               : launch GUI demo
   .forum bug          : ask for help with a bug or crash at the ROOT forum.
   .gh [bug|feature|improvement] : submit a bug report, feature or improvement suggestion
   .help Class::Member : open reference guide for that class member (or .?). Specifying '::Member' is optional.
   .help edit          : show line editing shortcuts (or .?)
   .license            : show license
   .ls                 : list contents of current TDirectory
   .pwd                : show current TDirectory, pad and style
   .quit (or .exit)    : quit ROOT (long form of .q)
   .R [user@]host[:dir] [-l user] [-d dbg] [script] : launch process in a remote host
   .qqq                : quit ROOT – mandatory
   .qqqqq              : exit process immediately
   .qqqqqqq            : abort process
   .which [file]       : show path of macro file
   .![OS_command]      : execute OS-specific shell command
   .!root -?           : print ROOT usage (CLI options)
root [1] 
```