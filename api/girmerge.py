#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#       girmerge.py - this file is part of Geany, a fast and lightweight IDE
#
#       Copyright 2008-2011 Nick Treleaven <nick.treleaven<at>btinternet.com>
#       Copyright 2008-2011 Enrico Tröger <enrico(dot)troeger(at)uvena(dot)de>
#
#       This program is free software; you can redistribute it and/or modify
#       it under the terms of the GNU General Public License as published by
#       the Free Software Foundation; either version 2 of the License, or
#       (at your option) any later version.
#
#       This program is distributed in the hope that it will be useful,
#       but WITHOUT ANY WARRANTY; without even the implied warranty of
#       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#       GNU General Public License for more details.
#
#       You should have received a copy of the GNU General Public License
#       along with this program; if not, write to the Free Software
#       Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#


# g-ir-compiler has some issues with some .gir files so the merging must be done in a slightly
# intelligent way. Note that the merge is only necessary in the first place because g-ir-compiler
# can only compile one .gir into one .typelib

import sys
from xml.dom import Node
import copy
import xml.dom.minidom as dom;
from optparse import OptionParser

out_fd = sys.stdout

def push(x):
	out_fd.write(str(x).strip() + "\n")

def debug(x):
	sys.stderr.write(str(x).strip() + "\n")


if __name__ == "__main__":

	parser = OptionParser("Usage: %prog [options] GIR-files...")
	parser.add_option("-o", "--output", action="store",
						help="Write output to FILE", metavar="FILE")
	(options, args) = parser.parse_args()

	if len(args) < 1:
		parser.print_help()
		exit(1)

	if options.output is not None:
		out_fd = open(options.output, "w+")

	document = dom.parse(args[0])
	maindoc = document.documentElement

	# get some element refs for ordered insertion
	# g-ir-compiler crashes when <include> tag is inserted after namespace
	# so this should take care of inserting <include> at (before) existing includes
	# fwiw: duplicates don't seem to be a problem
	insertionPoints = {}
	for child in maindoc.childNodes:
		if child.nodeType == Node.ELEMENT_NODE:
			# always append namespace
			if child.tagName != "namespace":
				if child.tagName not in insertionPoints:
					insertionPoints[child.tagName] = child

	for f in args[1:]:
		doc = dom.parse(f).documentElement
		for child in doc.childNodes:
			if child.nodeType == Node.ELEMENT_NODE:
				if child.tagName in insertionPoints:
					insert = insertionPoints[child.tagName]
					maindoc.insertBefore(child, insert)
					maindoc.insertBefore(document.createTextNode("\n"), insert)
				else:
					maindoc.appendChild(child)
					maindoc.appendChild(document.createTextNode("\n"))

	# workaround for g-ir-compiler failiqng to merge two <namespace> tags with the same
	# name into the typelib. the expr below makes getElementsByTagName() non-recursive
	# WARNING: this assumes all <namespace> have the same name
	namespaces = list(filter(lambda e: e.parentNode == maindoc, maindoc.getElementsByTagName("namespace")))
	# use the one with the highest version
	highestv = 0.0
	highest = namespaces[0]
	allchilds = []

	# find the "best" <namespace> that is finally used
	for ns in namespaces:
		allchilds.extend(ns.childNodes)
		if ns.hasAttribute("version"):
			v = float(ns.getAttribute("version"))
			if v > highestv:
				highestv = v
				highest = ns

	# clear childs for the target <namespace>, will be re-added
	for c in highest.childNodes:
		highest.removeChild(c)

	# add all childs back, including those from other <namespace>
	for c in allchilds:
		highest.appendChild(c)

	# finally remove other <namespace> from the document
	for c in namespaces:
		if c is not highest:
			maindoc.removeChild(c)

	push(document.toxml())
