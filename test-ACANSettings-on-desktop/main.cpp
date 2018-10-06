//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
//
//  main.cpp
//
//  Created by Pierre Molinaro on 30/09/2017.
//  Copyright © 2017 Pierre Molinaro. All rights reserved.
//
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

#include "ACANSettings.h"
#include "Set.h"

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

#include <iostream>
using namespace std ;

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

static const uint32_t firstTestedBitRate = 1 ; // 1 bit/s
static const uint32_t lastTestedBitRate = 20 * 1000 * 1000 ; // 20 Mbit/s

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

//static void compute (const uint32_t inWhishedBaudRate) {
//  ACANSettings settings (inWhishedBaudRate) ;
//  cout << "Desired baud rate: " << settings.mWhishedBitRate << " bit/s"  << endl ;
//  cout << "  Divisor : " << (unsigned) settings.mBitRatePrescaler << endl ;
//  cout << "  Prop seg: " << (unsigned) settings.mPropagationSegment << endl ;
//  cout << "  Segment1: " << (unsigned) settings.mPhaseSegment1 << endl ;
//  cout << "  Segment2: " << (unsigned) settings.mPhaseSegment2 << endl ;
//  cout << "  RJW     : " << (unsigned) settings.mRJW << endl ;
//  cout << "  Sampling: " << (settings.mTripleSampling ? "triple" : "single") << endl ;
//  cout << "  Actual baud rate: " << settings.actualBitRate () << " bit/s" << endl ;
//  cout << "  ppm: " << settings.ppmFromWishedBitRate () << endl ;
//  cout << "  Sample Point: " << settings.samplePointFromBitStart () << "%" << endl ;
//  cout << "  Bit setting closed to wished bit rate ok: " << ((settings.ppmFromWishedBitRate () < 1000) ? "yes" : "no") << endl ;
//}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

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

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

static void allCorrectSettings (Set <uint32_t> & ioValidSettingSet) {
  cout << "All valid settings" << endl ;
  for (uint32_t br = firstTestedBitRate ; br <= lastTestedBitRate ; br ++) {
    ACANSettings settings (br) ;
    if (settings.mBitSettingOk) {
      ioValidSettingSet.insert (br) ;
    }
  }
  cout << "  Completed, " << ioValidSettingSet.count () << " valid settings" << endl ;
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
//  EXACT SETTINGS
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

static void allExactSettings (Set <uint32_t> & ioExactSettingSet) {
  cout << "All exact settings" << endl ;
  for (uint32_t br = firstTestedBitRate ; br <= lastTestedBitRate ; br ++) {
    ACANSettings settings (br, 0) ;
    if (settings.mBitSettingOk) {
      ioExactSettingSet.insert (br) ;
    }
  }
  cout << "  Completed, " << ioExactSettingSet.count () << " exact settings" << endl ;
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

static void exhaustiveSearchOfAllExactSettings (Set <uint32_t> & ioExactSettingSet) {
  const uint32_t kCANClockFrequency = 16 *  1000 * 1000 ; // 16 MHz
  for (uint32_t brp = 1 ; brp <= 256 ; brp ++) {
    for (uint32_t TQCount = 5 ; TQCount <= 25 ; TQCount ++) {
      const uint32_t bitRate = kCANClockFrequency / brp / TQCount ;
      const bool exact = (bitRate * brp * TQCount) == kCANClockFrequency ;
      if (exact) {
        ioExactSettingSet.insert (bitRate) ;
      }
    }
  }
  cout << "  Exhaustive search completed, " << ioExactSettingSet.count () << " exact settings" << endl ;
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
//   MAIN
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

int main (int /* argc */, const char * /* argv */ []) {
//  compute (250 * 1000) ;
//  compute (125 * 1000) ;
//  compute (500 * 1000) ;
//  compute (1000 * 1000) ;
//  compute (10 * 1000) ;
//  compute (842 * 1000) ;
//  compute (440 * 1000) ;
//  compute (821 * 1000) ;
//  compute (842 * 1000) ;
//  compute (727 * 1000) ;
//  compute (2000) ;
//  compute (20 * 1000 * 1000) ;
//  compute (2509) ;
//--- Explore all settings
  exploreAllSettings () ;
//--- Check valid settings
  Set <uint32_t> validSettingSet ;
  allCorrectSettings (validSettingSet) ;
//--- Check all exact settings
  Set <uint32_t> exactSettingSet ;
  allExactSettings (exactSettingSet) ;
  Set <uint32_t> exhaustiveExactSettingSet ;
  exhaustiveSearchOfAllExactSettings (exhaustiveExactSettingSet) ;
  if (exactSettingSet != exhaustiveExactSettingSet) {
    cout << "  EQUALITY ERROR" << endl ;
    exit (1) ;
  }else{
    for (size_t i=0 ; i<exactSettingSet.count () ; i++) {
      cout << "  " << exactSettingSet.valueAtIndex (i) << " bit/s" << endl ;
    }
  }
  return 0;
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

