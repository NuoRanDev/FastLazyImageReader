import sys
import os
import shutil

SetMacro = ["READ_ONLY", "NOT_ADD_WINDOW_SHOW_MODULES"]
ImgReaderCore = {"src" : "./../ImgReaderCore/src/", "inc" : "./../ImgReaderCore/inc/"}

def Build_RE(Project:dict[str:str], MACROS:list[str], project_name:str, libs_name:str):
    re_cl64 = "/c /permissive- /ifcOutput \"/x64Release/\" /GS /GL /W3 /Gy /Zc:wchar_t /I\"{0}\" /I\"./../3rd\" /Gm- /O2 /sdl /Zc:inline /fp:precise /errorReport:prompt /WX- /Zc:forScope /Gd /Oi /MD /std:c++20 /FC /Fa\"x64Release/\" /EHsc /nologo /FS /Fo\"x64Release/\"  /diagnostics:column /utf-8 /D \"NODEBUG\" /D \"IMGREADER_EXPORTS\" /D \"_WINDOWS\" /D \"_USRDLL\" /D \"_WINDLL\" /D \"_UNICODE\" /D \"UNICODE\" {1} {2}"
    re_link="/DLL /OUT:\"./x64Release/{0}.dll\" /OPT:ICF /OPT:REF /LIBPATH:\"./windowslib/re/\" /LIBPATH:{1} /MANIFEST /LTCG:incremental /NXCOMPAT /DYNAMICBASE {2} {3} /IMPLIB:\"./x64Release/{0}.lib\" /MACHINE:X64 /SUBSYSTEM:CONSOLE /MANIFESTUAC:NO /ERRORREPORT:PROMPT /NOLOGO /TLBID:1"

    re_obj_file_path = "./x64Release/"
    re_out_obj_file = ""
    cpp_file_path = ""

    print("FIND C FILE")
    for c_file in os.listdir(Project.get("src")):
        print("# {0}".format(c_file))
        obj_file_item = c_file.replace(".cpp","") + ".obj"
        re_out_obj_file= re_out_obj_file + " \"" + obj_file_item + "\" "
        cpp_file_path = cpp_file_path + "\"" + Project.get("src") + c_file+"\" "
    print("\n\n STRAT BUILD \n\n")
    M = ""
    for MACRO in MACROS:
        M = M.join(" /D \"{0}\"".format(MACRO))
    os.system("cl " + re_cl64.format(Project.get("inc") ,cpp_file_path,M))

    print("LINK FILE")
    os.system("link " + re_link.format(project_name,"\"" + re_obj_file_path + "\"" , re_out_obj_file, libs_name))

    print("REMOVE FILE")
    all_export_file = os.listdir(re_obj_file_path)
    for files in all_export_file:
        if(files != project_name + ".dll" and files != project_name + ".lib" and files != project_name + ".exp"):
            print("REMOVE FILE :{0}".format(re_obj_file_path + files))
            os.remove(re_obj_file_path + files)
    print("Build {0} IS DONE".format(project_name))

def main():
    ImgReaderCore_lib=" winmm.lib version.lib Imm32.lib Setupapi.lib libpng16.lib lzma.lib mimalloc-static.lib SDL2-static.lib tiff.lib turbojpeg.lib zlib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib "
    Build_RE(ImgReaderCore, SetMacro,"ImgReaderCore",ImgReaderCore_lib)
    shutil.copytree("./../ImgReaderCore/inc/", "./x64Release/inc/ImgReaderCore")
    print("INCLUDE FILE MOVE TO :{0}".format("./x64Release/inc/"))

    fs = open("./x64Release/inc/Lazyimg.hpp", "w")
    LazyimgStruct = "#ifndef _INC_LAZY_IMG_HPP\n#define _INC_LAZY_IMG_HPP\n\n#include \"ImgReaderCore/LazyImg.h\"\n\n{0}\n#endif // _INC_LAZY_IMG_HPP IS EOF"
    macro_str = ""
    for item in SetMacro:
        macro_str = macro_str + "#define " + item + "\n"
    fs.write(LazyimgStruct.format(macro_str))
    fs.close()

main()