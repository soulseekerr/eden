from setuptools import setup, Extension

PATH_EDEN_PROJECT = r"/Users/soulseeker/Projects/GitHub/eden_project/eden"
module = Extension('edenpy',
      sources=['factorial.cpp', 'main.cpp'],
      include_dirs=[PATH_EDEN_PROJECT, f"{PATH_EDEN_PROJECT}/edencore"],
      library_dirs=[f'{PATH_EDEN_PROJECT}/build/Debug/lib'],
      # libraries=['libedenanalytics.dylib', 'libedencore.dylib'],
      runtime_library_dirs=[f'{PATH_EDEN_PROJECT}/build/Debug/lib']
      )

setup(name='vfeden',
      version='0.2.3',
      description='C++ extension eden library',
      ext_modules=[module])
