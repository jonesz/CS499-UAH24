#!/usr/bin/python
import argparse
import os

def fetch_parser():
    """ Build and return an argument parser for the program. """

    parser = argparse.ArgumentParser(
        prog="mkfs"
    )

    parser.add_argument("path")
    return parser

if __name__ == "__main__":
    p
