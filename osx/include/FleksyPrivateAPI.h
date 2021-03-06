//
//  FleksyPrivateAPI.h
//  FleksyLib
//
//  Created by Vadim Maximov on 3/22/13.
//  Copyright (c) 2013 Syntellia. All rights reserved.
//

#ifndef FleksyLib_FleksyPrivateAPI_h
#define FleksyLib_FleksyPrivateAPI_h

#include <memory>
#include "FLPoint.h"
#include "FleksyListenerInterface.h"
#include "FLFile.h"
#include "FLResourceArchive.h"
#include "FLLanguageData.h"
#include "FLLocale.h"
#include "FLAsyncDawgManager.h"
#include "FLTypingControllerIfc.h"
#include "FLDataCollector.h"

class FLTypingController;
class SystemsIntegrator;

namespace Json {
  class Value;
}

class FleksyAPIpImpl : private FLLocaleAccessor {
private:
  FLLanguageData _lang;
  
  FLResourceArchivePtr resourceArchive;
  void initResources(const FLUnicodeString &keyboardName);
  std::string resourcePath;
  
  FLPoint internalKeyboardSize;
  FLPoint externalKeyboardSize;
  
  FLUnicodeString _defaultKeyboardName;
  std::vector<FLUnicodeString> _keyboardNames;
  
  void loadLanguageData(const Json::Value &root);
  
public:
  FleksyAPIpImpl(FleksyListenerInterface &listener);
  ~FleksyAPIpImpl();
  FleksyListenerInterface &out;
  SystemsIntegrator *fleksy = NULL;
  
  std::shared_ptr<FLTypingControllerIfc> tc;
  std::shared_ptr<FLDataCollector> dataCollector;
  
  FLLanguageData& getLanguageData() { return _lang; }
  
  std::vector<FLUnicodeString> getKeyboardNames() const;
  std::vector<FLUnicodeString> getKeyboardNames(std::shared_ptr<FLFile> langFile) const;
  
  // set this to have preprocessed files generated. Do not add "/" in the end.
  bool isEngineLoaded;
  void setExternalKeyboardSize(FLPoint xy);
  FLPoint transformPoint(FLPoint p, bool inverse);
  FLUnicodeString getResourceDescription();
  void setMaxNumberOfSuggestions(int numberOfSuggestions);
  void postLoadSetup();
  void loadKeyboardData(FLUnicodeString keyboardName);
  void setResourceFLFile(FLFilePtr &file, FLUnicodeString keyboardName = "");
  void setResourcePath(const std::string &path);
  FLFilePtr FLFileForFileName(const char *filename);
  void initialize(bool isEncrypted = true);
  
  FLUnicodeString getLanguagePackVersion(const char *resourceFilePath);
  FLUnicodeString getLanguagePackVersion(FLResourceArchivePtr languagePack);
  bool isValidLanguagePack(const char *resourceFilePath);
  
  kLanguage getLanguage() const;
  
  //Data Collection
  void setDataCollectionDirectory(const std::string &path); // folder/folder/dataFolder (no / at the end)
  void setIsCollectingData(bool isCollectingData);
  void setDataFileHeader(const std::string &header);
  void processEventData(const FLUnicodeString &eventData);
  void writeDataToFile();
  void setIsTracking(bool isTracking);
  /*
   * Compresses and encrypts data to file. Use this to create file for any kind of data.
   * FLUicodeString data - data to write to file
   * string directory - directory where files can be written
   * Directory path is /path/to/directory (no / at the end)
   * Returns a full path to the encrypted data file (<directory>/data.xxx) or empty string if fails to write data to file
   */
  FLUnicodeString writeDataToFile(const FLUnicodeString &data, const FLUnicodeString &directory);
  
  void spaceWithHint(const FLUnicodeString& hint, float length);
  
  std::vector<FLUnicodeString> getWordsInTemporaryDictionary();
  void setWordsInTemporaryDictionary(std::vector<FLUnicodeString> temp_words);
  
  /**
   * Initializes Fleksy for A/B testing. Can be called multiple times
   * to test multiple things at once
   *
   * @param type
   * the type of the A/B test to use.
   *
   * @param mode
   * Selecst the A or B mode to test
   */
  void setupABTesting(FLABTestType type, FLABTestMode mode);
  
  bool warmUpJet();
  
  bool lockJetContents();
  bool unlockJetContents();
  
  /*
   The writeable data directory is the place where the engine can read and write to the disk.
   */
private:
  static std::string _writableDataDirectory;
public:
  static void setWritableDataDirectory(const std::string &directory);
  static std::string getWritableDataDirectory();
  
public:
  static std::unique_ptr<FLAsyncDawgManager> dawgManager;
};

#endif
