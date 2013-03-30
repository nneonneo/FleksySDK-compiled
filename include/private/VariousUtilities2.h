//
//  VariousUtilities2.h
//  Fleksy
//
//  Created by Kostas Eleftheriou on 12/7/12.
//  Copyright (c) 2012 Syntellia Inc. All rights reserved.
//

#include <FLString.h>
#include <FLPoint.h>

#include "TimeFunctions.h"

#include <string>
#include <vector>

using namespace std;

namespace VariousUtilities2 {

  FLString& trim(FLString &s);
  
  int getCharacterLineType(const FLString& line);
  void readCharacterLine(const FLString& line, FLPoint* dictionary, FLString* alphabet);

  FLChar getNearestCharForPoint(FLPoint& target, FLPoint* keys);
  
  FLString getStringFromFile(const string& filename, bool isEncrypted);
};
