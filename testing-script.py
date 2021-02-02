#!/usr/bin/python

# ./test-script.py

import os,re,shutil,random,subprocess,threading

case_dir='./tests/cases'
sol_dir='./tests/solutions'

# Set up scratch space for testing
dir="tests"

# Check for cheating
token='TOKEN'+str(random.randrange(100000,999999))

if subprocess.call('scons',cwd='.')!=0:
    print("FAIL: Did not compile")
    exit()

def run_command_with_timeout(cmd, timeout_sec):
    proc = subprocess.Popen(cmd,cwd=dir)
    proc_thread = threading.Thread(target=proc.communicate)
    proc_thread.start()
    proc_thread.join(timeout_sec)
    if proc_thread.is_alive():
        try:
            proc.kill()
        except OSError, e:
            return True
        return False
    return True

pass_cases = 0
total = 0
hashed_tests={}
diff_parse=re.compile('diff: (.*)')
test_cmd=['../ray_tracer', '-i', 'file.txt', '-s', 'file.png', '-o', token+'.txt']

ignore_line=re.compile('^\s*(#|$)')
test_line=re.compile('^(\S+)\s+(\S+)\s*$')
gs=0
try:
    gs=open(dir+'/testing-scheme.txt', 'r')
except:
    print("FAIL: could not open testing scheme.")
    exit()

for line in gs.readlines():
    if ignore_line.search(line):
        continue
    g=test_line.search(line)
    if not g:
        print("Unrecognized command: "+line)
        exit()
    max_error=float(g.groups()[0])
    file=g.groups()[1]

    pass_error = 0
    pass_time = 0
    if not hashed_tests.has_key(file):
        timeout = 10
        shutil.copyfile(case_dir+'/'+file+".txt", dir+"/file.txt")
        shutil.copyfile(sol_dir+'/'+file+".png", dir+"/file.png")
        if not run_command_with_timeout(test_cmd, timeout):
            hashed_tests[file]="TIMEOUT"
        else:
            try:
                report = dir+'/'+token+'.txt'
                results_file=open(report)
                d=diff_parse.match(results_file.readline())
                results_file.close()
                if os.path.isfile(report):
                    os.remove(report) # remove the diff file
                if d: d=float(d.groups()[0])
                hashed_tests[file]=d
            except:
                hashed_tests[file]=None

    d=hashed_tests[file]
    if d=="TIMEOUT":
        print("FAIL: (%s) Test timed out."%file)
    elif d==None:
        print("FAIL: (%s) Program failed to report statistics."%file)
    else:
        if d>max_error:
            print("FAIL: (%s) Too much error. Actual: %g  Max: %g."%(file,d,max_error))
        else:
            print("PASS: (%s) diff %g vs %g."%(file,d,max_error))
            pass_cases += 1
    total += 1

print("Pass/Total: %g / %g" %(pass_cases, total))
