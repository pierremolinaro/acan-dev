#!/usr/bin/python
# -*- coding: utf-8 -*-

#------------------------------------------------------------------------------*

import os, sys, subprocess, shutil

#------------------------------------------------------------------------------*

def runCommand (command) :
  s = "+"
  for t in command :
    s += ' "' + t + '"'
  print (s)
  childProcess = subprocess.Popen (command)
  childProcess.wait ()
  if childProcess.returncode != 0 :
    sys.exit (childProcess.returncode)

#------------------------------------------------------------------------------*

def copyFile (sourceFile, destinationDir) :
  runCommand (["cp", sourceFile, destinationDir])

#------------------------------------------------------------------------------*

def compileArduinoSketch (scriptDir, sketch) :
  buildPath = scriptDir + "/build-path/"  + sketch
  if not os.path.exists (buildPath):
    os.makedirs (buildPath)
#---
  command = [
    "/Applications/Arduino.app/Contents/Java/arduino-builder",
    "-quiet",
    "-compile",
    "-logger=machine",
    "-hardware", "/Applications/Arduino.app/Contents/Java/hardware",
    "-hardware", "/Users/pierremolinaro/Library/Arduino15/packages",
    "-hardware", "/Users/pierremolinaro/Documents/Arduino/hardware",
    "-tools", "/Applications/Arduino.app/Contents/Java/tools-builder",
    "-tools", "/Applications/Arduino.app/Contents/Java/hardware/tools/avr",
    "-tools", "/Users/pierremolinaro/Library/Arduino15/packages",
    "-built-in-libraries", "/Applications/Arduino.app/Contents/Java/libraries",
    "-libraries", "/Users/pierremolinaro/Documents/Arduino/libraries",
    "-libraries", scriptDir + "/library-sources",
    "-fqbn=teensy:avr:teensy36:usb=serial,speed=180,opt=oslto,keys=en-us",
    "-ide-version=10805",
    "-build-path", buildPath,
    "-warnings=all",
    scriptDir + "/sample-code/" + sketch + "/" + sketch + ".ino"
  ]
  runCommand (command)

#------------------------------------------------------------------------------*

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
distributionDirectory = scriptDir + "/../GITHUB/acan"
if not os.path.exists (distributionDirectory + "/extras"):
  os.makedirs (distributionDirectory + "/extras")
copyFile (scriptDir + "/documentation-in-latex/acan.pdf", distributionDirectory + "/extras")
#--- Copy files in the distribution directory
copyFile (scriptDir + "/library-sources/ACAN/library.properties", distributionDirectory)
copyFile (scriptDir + "/library-sources/ACAN/keywords.txt", distributionDirectory)
if os.path.exists (distributionDirectory + "/src"):
  shutil.rmtree (distributionDirectory + "/src")
shutil.copytree (scriptDir + "/library-sources/ACAN/src", distributionDirectory + "/src")
if os.path.exists (distributionDirectory + "/examples"):
  shutil.rmtree (distributionDirectory + "/examples")
shutil.copytree (scriptDir + "/sample-code", distributionDirectory + "/examples")

#------------------------------------------------------------------------------*
