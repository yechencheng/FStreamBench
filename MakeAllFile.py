import os
import sys
import fnmatch
import subprocess

MakeCluster = "Makefile.cluster"

def MyWork(path):
    #GetLLVMCode(path)
    proc = subprocess.Popen("/Users/apple/Project/benchmarks/FStreamBench/DotParser " + path + "/stream-graph.dot", stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
    (out, err) = proc.communicate()
    print(out)

def GetLLVMCode(path):
    if not os.path.exists(MakeCluster):
        print "\tMake ERROR : " + path
        return False
    
    proc = subprocess.Popen("make -f " + MakeCluster + " clean", stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
    (out, err) = proc.communicate()    
    
    file = open(MakeCluster)
    try:
        content = file.readlines()
    finally:
        file.flush()
        file.close()
    os.remove(MakeCluster)
    content[2] = "CXX = clang++\n"
    content[3] = "CCFLAGS = -emit-llvm\n"
    
    file = open(MakeCluster, 'w+')
    file.writelines(content)
    file.flush()
    file.close()
    
    proc = subprocess.Popen("make -f " + MakeCluster, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
    (out, err) = proc.communicate()
    
    return 
    
def CaseMakefile(path):
    for file in os.listdir(path):
        if os.path.isfile(file):
            os.system("cp " + os.path.abspath(file) + " " + path + "/build")
    path += "/build"
    os.chdir(path)
    proc = subprocess.Popen("make", stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
    (out, err) = proc.communicate()
    
    MyWork(path)
    try:
        os.rename("combined_threads.o", os.path.basename(os.path.abspath(path + "/../../")) + ".bc")
    except:
        print "\tNo combined_threads.o : " + path
    return 
    
def CaseNoMake(path) :
    SrcList = fnmatch.filter(os.listdir(path), "*.str")
    
    path += "/build"
    for file in SrcList:
        os.system("cp " + os.path.abspath(file) + " " + path)
    os.chdir(path)
    for file in SrcList:
        proc = subprocess.Popen("strc " + file, stderr=subprocess.PIPE, stdout=subprocess.PIPE, shell=True)
        (out, err) = proc.communicate()
        MyWork(path)
        try:
            os.rename("combined_threads.o", os.path.splitext(file)[0] + ".bc")    
        except:
            print "\tRename Error : " + path
    return

path = sys.argv[1]
print "Path to Process : " + path
os.chdir(path)
path = os.getcwd()

root = path

for dir in os.listdir(root) :    
    path = root + "/" + dir
    if not os.path.isdir(path):  #must be directory
        continue;
    path += "/streamit"
    if not os.path.exists(path):    #must has streamit dir
        continue;
    print "Processing : " + path
    os.chdir(path)
    if not os.path.exists(path + "/build"):
        system("mkdir build")
    if os.path.exists("./Makefile"):    # case 1
        CaseMakefile(path)
    else: #case 2
        CaseNoMake(path)        
