import sys
import os

ImgReaderCore = {"src" : "./../ImgReaderCore/src/", "inc" : "./../ImgReaderCore/inc/"}
_3rd_lib_re = "-shared -fPIC -L./windowslib/re/ -llibpng16.lib -llzma.lib -lmimalloc-static.lib -lSDL2-static.lib -ltiff.lib -lturbojpeg.lib -lzlib.lib"

def build(Project:dict[str:str],de_or_re:str):
    clang_pro_cmmand = "clang -std=c++17 -o3 -I ./../3rd -I {0} -c {1} -o {2}{3}.obj"
    for define in sys.argv:
        print(define)
    
    obj_file = ""
    for c_file in os.listdir(Project.get("src")):
        print("# {0}".format(c_file))
        obj_file = obj_file + "./obj/" + c_file.replace(".cpp","") + "{0}.obj ".format(de_or_re)

        os.system(clang_pro_cmmand.format(Project.get("inc"), Project.get("src") + c_file , "./obj/" + c_file.replace(".cpp",""),de_or_re))

    os.system("clang -std=c++17 -o3 -I ./../3rd -I {0} {1} {2} -o ./re/ImgReaderCore.dll".format(Project.get("inc"),_3rd_lib_re,obj_file))

def main():
    build(ImgReaderCore,"re")

main()