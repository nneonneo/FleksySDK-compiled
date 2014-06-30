//
//  SystemsIntegrator.h
//  Fleksy
//
//  Created by Kostas Eleftheriou on 7/30/11.
//  Copyright 2011 Syntellia Inc. All rights reserved.
//

#ifndef __FleksySDK__FLSystemIntegrator__
#define __FleksySDK__FLSystemIntegrator__

#include <string>
#include <vector>
#include <array>
#include "FleksyUtilities.h"
#include "FLKeyTapRecognizer.h"

class FLTapsToWords;
typedef std::shared_ptr<FLTapsToWords> FLTapsToWordsPtr;

class FLRequest {
public:
  FLUnicodeString hint;
  FLUnicodeString previousToken1;
  FLUnicodeString previousToken2;
  std::vector<FLUnicodeString> platformSuggestions;
  bool debug;
  double requestTime;
  std::vector<FLPoint> points;
  //////////////////////////////////////////
  //~FLRequest();
  
  FLRequest(const std::vector<FLPoint> &_points, const FLUnicodeString &previousToken1, const FLUnicodeString &previousToken2, const FLUnicodeString hint, bool debug = false);
};

class SystemsIntegrator {
  
private:
  bool blindMode = false;
  FLFoundWordsVector _foundWordsVector;
  
public:
  SystemsIntegrator();
  ~SystemsIntegrator();

  FleksyUtilities* utils = NULL;
  
  std::vector<FLUnicodeString> noChangeWords;
  
  FLTapsToWordsPtr tapsToWords;
  
  inline const FLFoundWordsVector &getFoundWords() const {
    return _foundWordsVector;
  }
  
  std::vector<FLUnicodeString> getCandidatesForRequest(FLRequest &request);
  
  void loadKeyboardData(FLFilePtr &keyboardFile, FLFilePtr &commonFile);
  void loadDictionary(const std::string& tag, void* data, size_t dataLength, const FLUnicodeString& delimiter, kWordlistType type, bool isEncrypted);
  
  void loadDictionary(FLFilePtr &f, const FLUnicodeString& delimiter, kWordlistType type, bool isEncrypted);
  
  void postload();
  
  FLAddWordResult addUserWord(FLUnicodeStringPtr &word);
  bool removeUserWord(const FLUnicodeString& word);
  bool wordExists(const FLUnicodeString& word, bool allowLowerCase);
  
  void pointsFromLetters(const FLUnicodeString& letters, FLPoint points[]);
  FLPoint getPointForChar(FLUnicodeString c, FLKeyboardID keyboard);
    
  std::shared_ptr<FLKeyboard> getKeyboard();

  // Settings
  bool setSpaceBreaksEnabled(bool value); // returns new value. If return value does not match argument some error occurred
  bool getSpaceBreaksEnabled();
  bool setSpellingCorrectionEnabled(bool value); // returns new value. If return value does not match argument some error occurred
  bool getSpellingCorrectionEnabled();
  void setBlindMode(bool value);
  
  void setSettingTransformLayerWeight(float weight);
  void setSettingShapeLayerWeight(float weight);
  void setSettingContextLayerWeight(float weight);
  void setSettingPlatformLayerWeight(float weight);
  
  float getSettingShapeLayerWeight();
  float getSettingTransformLayerWeight();
  float getSettingContextLayerWeight();
  float getSettingPlatformLayerWeight();
  
  void setBayesianBlending(bool useBayesian);

  FLUnicodeString getVersion();
};

#endif
