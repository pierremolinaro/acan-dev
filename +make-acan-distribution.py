#!/usr/bin/python3
# -*- coding: utf-8 -*-
#-----------------------------------------------------------------------------------------

import os, sys, subprocess, shutil

#-----------------------------------------------------------------------------------------

def runCommand (command) :
  s = "+"
  for t in command :
    s += ' "' + t + '"'
  print (s)
  childProcess = subprocess.Popen (command)
  childProcess.wait ()
  if childProcess.returncode != 0 :
    sys.exit (childProcess.returncode)

#-----------------------------------------------------------------------------------------

def copyFile (sourceFile, destinationDir) :
  runCommand (["cp", sourceFile, destinationDir])

#-----------------------------------------------------------------------------------------

def compileArduinoSketch (scriptDir, sketch) :
  buildPath = scriptDir + "/build-path/"  + sketch
  if not os.path.exists (buildPath):
    os.makedirs (buildPath)
#---
  command = [
    "arduino-cli",
    "compile",
    "--libraries", "/Users/pierremolinaro/Documents/Arduino-dev/libraries",
    "-b=teensy:avr:teensy36",
#     "-build-path", buildPath,
    "--warnings=all",
    scriptDir + "/sample-code/" + sketch
  ]
  runCommand (command)

#-----------------------------------------------------------------------------------------

#--- Get script absolute path
scriptDir = os.path.dirname (os.path.abspath (sys.argv [0]))
os.chdir (scriptDir)
#--- Compile sketches
compileArduinoSketch (scriptDir, "ReceivePrimaryFilters")
compileArduinoSketch (scriptDir, "LoopBackDemo")
compileArduinoSketch (scriptDir, "ReceivePrimarySecondaryFilters")
compileArduinoSketch (scriptDir, "SendReceiveRemoteFrames")
compileArduinoSketch (scriptDir, "Teensy36Test")
#--- Compile latex doc
runCommand ([scriptDir + "/documentation-in-latex/-build.command"])
distributionDirectory = scriptDir + "/../acan"
if not os.path.exists (distributionDirectory + "/extras"):
  os.makedirs (distributionDirectory + "/extras")
copyFile (scriptDir + "/documentation-in-latex/acan.pdf", distributionDirectory + "/extras")
#--- Copy files in the distribution directory
copyFile (scriptDir + "/library-sources/library.properties", distributionDirectory)
copyFile (scriptDir + "/library-sources/keywords.txt", distributionDirectory)
if os.path.exists (distributionDirectory + "/src"):
  shutil.rmtree (distributionDirectory + "/src")
shutil.copytree (scriptDir + "/library-sources/src", distributionDirectory + "/src")
if os.path.exists (distributionDirectory + "/examples"):
  shutil.rmtree (distributionDirectory + "/examples")
shutil.copytree (scriptDir + "/sample-code", distributionDirectory + "/examples")

#-----------------------------------------------------------------------------------------
