import sys
import os

ImgReaderCore = {"src" : "./../ImgReaderCore/src/", "inc" : "./../ImgReaderCore/inc/"}

def build(Project:dict[str:str]):
    clang_pro_cmmand = "clang -o3 -I ./../3rd -I {0} -c {1} -o {2}.obj"
    for define in sys.argv:
        print(define)

    for c_file in os.listdir(Project.get("src")):
        print("# {0}".format(c_file))
        os.system(clang_pro_cmmand.format(Project.get("inc"), Project.get("src") + c_file , "./obj/" + c_file.replace(".cpp","")))

def main():
    build(ImgReaderCore)

main()