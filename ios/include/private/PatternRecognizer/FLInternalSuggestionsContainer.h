//
//  FLInternalSuggestionsContainer.h
//  PatternRecognizer
//
//  Copyright 2011 Syntellia Inc. All rights reserved.
//


#ifndef FL_INTERNAL_SUGGESTIONS_CONTAINER_H
#define FL_INTERNAL_SUGGESTIONS_CONTAINER_H

#include "FLWord.h"

//TODO replace with FLResponse?

using namespace std;

class FLInternalSuggestionsContainer {
private:
  const FLWord* inputWord;

public:
  FLInternalSuggestionsContainer(const FLWord* inputWord);
  const FLWord* getInputWord();
  FLWord* getFirstSuggestion();
  
  double processingTimes[5];
  int sortedElements;
  FLWordList candidatesExact;
  FLWordList candidatesExtra;
  FLWordList candidatesMinus;
};

#endif