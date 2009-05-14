import FWCore.ParameterSet.Config as cms


# VBFHWW2l2nuSource = cms.PSet(
# fileNames = cms.untracked.vstring(

readFiles = cms.untracked.vstring()

VBFHWW2l2nuSource = cms.PSet(
  fileNames = readFiles
)

readFiles.extend( [


