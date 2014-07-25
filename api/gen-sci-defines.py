import sys;
from optparse import OptionParser

sys.path.append("../scintilla/scripts")

import Face
import HFacer


out_fd = sys.stdout

def push(x):
	out_fd.write(x.strip() + "\n")

if __name__ == "__main__":

	parser = OptionParser("Usage: %prog [options] path/to/Scintilla.iface...")
	parser.add_option("-o", "--output", action="store",
						help="Write output to FILE", metavar="FILE")
	(options, args) = parser.parse_args()

	if len(args) < 1:
		parser.print_help()
		exit(1)

	if options.output is not None:
		out_fd = open(options.output, "w+")

	# TODO: check input file for existence
	f = Face.Face()
	f.ReadFromFile("../scintilla/include/Scintilla.iface")
	defines = HFacer.printHFile(f)
	defines += HFacer.printLexHFile(f)

	for line in defines:
		#~ parts = line.split()
		#~ push(parts[0] + " SCINTILLA_" + parts[1] + " " + parts[2])
		push(line)
