//----------------------------------------------------------------------------------------------------------------------
//
//  main.cpp
//
//  Created by Pierre Molinaro on 30/09/2017.
//  Copyright © 2017 Pierre Molinaro. All rights reserved.
//
//----------------------------------------------------------------------------------------------------------------------

#include "ACANSettings.h"
#include "Set.h"

//----------------------------------------------------------------------------------------------------------------------

#include <iostream>
using namespace std ;

//----------------------------------------------------------------------------------------------------------------------

static const uint32_t firstTestedBitRate = 1 ; // 1 bit/s
static const uint32_t lastTestedBitRate = 20 * 1000 * 1000 ; // 20 Mbit/s

//----------------------------------------------------------------------------------------------------------------------

static void compute (const uint32_t inWhishedBaudRate) {
  ACANSettings settings (inWhishedBaudRate) ;
  cout << "Desired baud rate: " << settings.mWhishedBitRate << " bit/s"  << endl ;
  cout << "  Divisor : " << (unsigned) settings.mBitRatePrescaler << endl ;
  cout << "  Prop seg: " << (unsigned) settings.mPropagationSegment << endl ;
  cout << "  Segment1: " << (unsigned) settings.mPhaseSegment1 << endl ;
  cout << "  Segment2: " << (unsigned) settings.mPhaseSegment2 << endl ;
  cout << "  RJW     : " << (unsigned) settings.mRJW << endl ;
  cout << "  Sampling: " << (settings.mTripleSampling ? "triple" : "single") << endl ;
  cout << "  Actual baud rate: " << settings.actualBitRate () << " bit/s" << endl ;
  cout << "  ppm: " << settings.ppmFromWishedBitRate () << endl ;
  cout << "  Sample Point: " << settings.samplePointFromBitStart () << "%" << endl ;
  cout << "  Bit setting closed to wished bit rate ok: " << ((settings.ppmFromWishedBitRate () < 1000) ? "yes" : "no") << endl ;
}

//----------------------------------------------------------------------------------------------------------------------

static void exploreAllSettings (void) {
  cout << "Explore all settings" << endl ;
  for (uint32_t br = firstTestedBitRate ; br <= lastTestedBitRate ; br ++) {
    ACANSettings settings (br) ;
    const uint32_t errorCode = settings.CANBitSettingConsistency () ;
    if (errorCode != 0) {
      cout << "Error 0x" << hex << errorCode << " for br : " << dec << br << endl ;
      exit (1) ;
    }
  }
  cout << "  Ok" << endl ;
}

//----------------------------------------------------------------------------------------------------------------------

static std::vector <uint32_t> allPossibleBitRates (void) {
  cout << "All bit rates" << endl ;
  Set allBitRates (lastTestedBitRate) ;
  for (uint32_t br = firstTestedBitRate ; br <= lastTestedBitRate ; br ++) {
    ACANSettings settings (br) ;
    if (settings.mBitSettingOk) {
      allBitRates.insert (br) ;
    }
  }
  const std::vector <uint32_t> result = allBitRates.values () ;
  cout << "  Completed, " << result.size () << " valid settings" << endl ;
  return result ;
}

//----------------------------------------------------------------------------------------------------------------------
//  EXACT SETTINGS
//----------------------------------------------------------------------------------------------------------------------

static std::vector <uint32_t> allExactSettings (void) {
  cout << "All exact bit rates" << endl ;
  Set allExactBitRates (lastTestedBitRate) ;
  for (uint32_t br = firstTestedBitRate ; br <= lastTestedBitRate ; br ++) {
    ACANSettings settings (br, 0) ;
    if (settings.mBitSettingOk) {
      allExactBitRates.insert (br) ;
    }
  }
  const std::vector <uint32_t> result = allExactBitRates.values () ;
//  for (uint32_t i=0 ; i<ioExactSettingSet.count() ; i++) {
//    cout << "  " << ioExactSettingSet.valueAtIndex (i) << " bit/s" << endl ;
//  }
  cout << "  Completed, " << result.size () << " exact settings" << endl ;
  return result ;
}

//----------------------------------------------------------------------------------------------------------------------

static std::vector <uint32_t> exhaustiveSearchOfAllExactSettings (void) {
  cout << "Exact settings ehaustive search" << endl ;
  Set allExactBitRates (lastTestedBitRate) ;
  const uint32_t kCANClockFrequency = 16 *  1000 * 1000 ; // 16 MHz
  for (uint32_t brp = 1 ; brp <= 256 ; brp ++) {
    for (uint32_t TQCount = 5 ; TQCount <= 25 ; TQCount ++) {
      const uint32_t bitRate = kCANClockFrequency / brp / TQCount ;
      const bool exact = (bitRate * brp * TQCount) == kCANClockFrequency ;
      if (exact) {
        allExactBitRates.insert (bitRate) ;
      }
    }
  }
  const std::vector <uint32_t> result = allExactBitRates.values () ;
//  for (uint32_t i=0 ; i<ioExactSettingSet.count() ; i++) {
//    cout << "  " << ioExactSettingSet.valueAtIndex (i) << " bit/s" << endl ;
//  }
  cout << "  Exhaustive search completed, " << result.size () << " exact settings" << endl ;
  return result ;
}

//----------------------------------------------------------------------------------------------------------------------
//   MAIN
//----------------------------------------------------------------------------------------------------------------------

int main (int /* argc */, const char * /* argv */ []) {
//  compute (250 * 1000) ;
  compute (125 * 1000) ;
//  compute (500 * 1000) ;
//  compute (1000 * 1000) ;
//  compute (10 * 1000) ;
//  compute (842 * 1000) ;
//  compute (440 * 1000) ;
//  compute (821 * 1000) ;
//  compute (842 * 1000) ;
  compute (727 * 1000) ;
//  compute (2000) ;
//  compute (20 * 1000 * 1000) ;
//  compute (2509) ;
//--- Explore all settings
  exploreAllSettings () ;
//--- Check valid settings
  allPossibleBitRates () ;
//--- Check all exact settings
  const std::vector <uint32_t> exactBitRates = allExactSettings () ;
  const std::vector <uint32_t> exhaustiveExactBitRates = exhaustiveSearchOfAllExactSettings () ;
  if (exactBitRates != exhaustiveExactBitRates) {
    cout << "  EQUALITY ERROR" << endl ;
    exit (1) ;
  }else{
    for (size_t i=0 ; i<exactBitRates.size () ; i++) {
      cout << "  " << exactBitRates.at (i) << " bit/s" << endl ;
    }
  }
  return 0;
}

//----------------------------------------------------------------------------------------------------------------------

