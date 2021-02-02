import os
env = Environment(ENV = os.environ, CPPPATH = ['include'])

env.Append(LIBS=["png"])
env.Append(CXXFLAGS=["-std=c++11","-g","-Wall","-O3","-I/usr/include/libpng12"])
env.Append(LINKFLAGS=["-L/usr/local/lib"])

env.Program("./ray_tracer", Glob('src/*.cpp'))

