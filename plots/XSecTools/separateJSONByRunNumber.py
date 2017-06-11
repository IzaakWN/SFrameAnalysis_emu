#! /usr/bin/env python

from argparse import ArgumentParser
import os, sys, time, re
from math import sqrt, pow, floor, ceil

argv = sys.argv
description = '''This script make plots.'''
parser = ArgumentParser(prog="plotter",description=description,epilog="Succes!")
parser.add_argument('file', metavar='FILE', #type=int, nargs='+',
                    help='a JSON txt file to split')
# parser.add_argument( "-s", "--category", dest="category", type=int, default=-1, action='store',
#                      metavar="CATEGORY", help="run only for this category of selection and cuts" )
# parser.add_argument( "-c", "--channel", dest="channel", default="", action='store',
#                      metavar="CHANNEL", help="run only for this channel" )
# parser.add_argument( "-e", "--etau", dest="etau", default=False, action='store_true',
#                      help="run only for the etau channel" )
# parser.add_argument( "-m", "--mutau", dest="mutau", default=False, action='store_true',
#                      help="run only for the mutau channel" )
# parser.add_argument( "-l", "--list", dest="list", default=False, action='store_true',
#                      help="list all available categories" )
args = parser.parse_args()



class period(object):
    """Class of a run period."""

    def __init__(self, name, start, end):
        self.name   = name
        self.end    = end
        self.start  = start
    
    def contains(runNumber):
        if self.start =< runNumber <= self.end:
            return True
        return False



def main():
    """Main function."""
    
    file = open(args.file,'r')
    pattern = re.compile("\"(\d+)\"")
    
    periods = [ 
                period("B",200000,300000)
                period("C",200000,300000)
                ]
    
    for i, line in enumerate(file):
        found = pattern.search(line)
        if found:
            runNumber = found.group(1)
            print ">>> found >%s<" % (runNumber)
            if runNumber.isdigit():
                print ">>> is a digit"
            else:
                print ">>> Waring! Found run number is not a digit"
        else:
            print ">>> Warning! No run number found on line %d: >%s<" % (i,line.replace("\n",""))
            continue
        
    


if __name__ == '__main__':
    print
    main()
    print ">>>\n>>> Done with this, son.\n"




