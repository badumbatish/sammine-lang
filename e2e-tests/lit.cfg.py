
import lit.formats
import os
# config.test_source_root = os.path.dirname(__file__)

# right now when e2e is called, this script thought that its in build/e2e-tests
sammine_exe = os.path.abspath("../src/sammine")

config.name = "Sammine Lang end-to-end testsuite"
config.test_format = lit.formats.ShTest(True)

config.substitutions.append(('%sammine_exe%', sammine_exe))
config.suffixes = ['.c', '.mn']
