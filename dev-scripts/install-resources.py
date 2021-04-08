#!/usr/bin/env python3

from xml.etree.ElementTree import Element, SubElement, tostring
from xml.dom import minidom
from os import listdir, path
from sys import argv

RESOURCES_PATCHES = argv[2].split(",")

REC_FILE_NAME = argv[1]

ROOT = "/".join(path.abspath(path.dirname(__file__)).split("/")[0:-1])

rec = Element('RCC')


def prettify(elem):
    rough_string = tostring(elem, 'utf-8')
    reparsed = minidom.parseString(rough_string)
    return reparsed.toprettyxml(indent="    ")


for rec_dir in RESOURCES_PATCHES:
    rec_child = SubElement(rec, "qresource")
    rec_child.set("prefix", "/" + rec_dir)

    files = listdir(ROOT + "/" + rec_dir)

    for file in files:
        if "." not in file:
            continue

        rec_file = SubElement(rec_child, "file")
        rec_file.set("alias", file.split(".")[0])
        rec_file.text = rec_dir + "/" + file

with open(ROOT + "/" + REC_FILE_NAME, "w") as file:
    file.write(prettify(rec))
