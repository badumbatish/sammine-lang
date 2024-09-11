
import lit.formats

config.name = "Sammine Lang end-to-end testsuite"
config.test_format = lit.formats.ShTest(True)

config.suffixes = ['.c']
