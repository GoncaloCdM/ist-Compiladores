#! /usr/bin/python
'''
Convertion from an XML to a DOT script.

This module has been designed to build an overview of an xml stream.
Using the Dot scripting module (part of graphviz software set),
it creates pictures (png, jpg, ...) from xml.

The xml input can come from a file or from a stream (via pipe).

Script inputs:
- deep : how deep the program is describing.
- xmlFile (optional) : the file path of an xml.

Output:
The output is a dot input script.

Example:

1)  cat example.xml | ./XmlToDot.py 3 | dot -Tpng > test.png
2)  ./XmlToDot.py 3 example.xml | dot -Tpng > test.png
'''

from xml.dom.minidom import parse, parseString, Element, Text
from datetime import datetime
import sys
import random

def _getRandomVariableName(length=8):
    return ''.join(chr(97 + random.randint(0, 25)) for i in range(length))

def _parseXmlToDot(node, deep, parent=None):
    if isinstance(node, Text):
        pass
    
    if not isinstance(node, Element):
        return
    
    if deep <= 0:
        return
    
    label = node.tagName
    variable = label.upper() + _getRandomVariableName()
    
    if parent:
        print parent, '--', variable
    
    for child in node.childNodes:
        _parseXmlToDot(child, deep - 1, variable)
    
    print '%s [label=\"%s\"]'%(variable, label)

def parseXmlToDot(doc, deep, root=None):
    '''
    @param doc is an instance of xml.dom.minidom.Document and contain the complete xml
    @param deep is an integer describing the deepest level to look into
    @param root (optional) contains a string which is the start point of the description.
    '''
    print 'graph XmlToDot%s{'%datetime.now().strftime('%Y%m%d')
    print 'rankdir = TB;'
    print 'node [shape=box];'
    if root:
        doc = chRoot(doc, root)
    
    if deep > 0 and doc.hasChildNodes():
        _parseXmlToDot(doc.documentElement, deep)
    
    print '}'

def _usage():
    print '''Usage :
    %s <deep> <<xml file>>
    '''%__file__

if __name__ == '__main__':
    if len(sys.argv) == 1:
        docXml = parseString(sys.stdin.read())
        deep = 100
    elif len(sys.argv) == 2:
        docXml = parse(sys.argv[1])
        deep = 100
    else:
        _usage()
        sys.exit(1)
    
    parseXmlToDot(docXml, int(deep))

