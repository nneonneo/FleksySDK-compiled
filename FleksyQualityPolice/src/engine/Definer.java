package engine;

import java.text.Normalizer;
import java.util.ArrayList;

import com.syntellia.fleksy.api.FLEnums;
import com.syntellia.fleksy.api.FLKey;

import utils.DataManager;
import utils.Key;
import utils.Log;
import utils.Word;

public class Definer {
	private final static String TAG = "Definer";
	
	private static boolean caps = false;
	private static ArrayList<Key> keyboard;
	private final static String cappers = ".!?";//new char[]{'.','!','?'};
	private final static String punctuation = ".,?!:;";//new char[]{'.',',','?','!',':',';'};
//	private static final Pattern cleanPattern = Pattern.compile("[a-zA-Z]+-[a-zA-Z'.,?!:;]+|[a-zA-z'.,?!:;]+");
	
	private static Key swipeR;
	private static Key shift;
	
	public Definer(){
		TestEngine.action = "Creating Definer";
		keyboard = new ArrayList<Key>();
		try{
			Log.d("QWERTY");
			for(FLKey key : FleksyEngine.api.getKeymapForKeyboard(FLEnums.FLKeyboardID.FLKeyboardID_QWERTY_UPPER.ordinal())){
				float x = key.x;
				float y = key.y;
				keyboard.add(new Key(key.label,x,y));
			}
			Log.d("Symbols");
			for(FLKey key : FleksyEngine.api.getKeymapForKeyboard(FLEnums.FLKeyboardID.FLKeyboardID_SYMBOLS.ordinal())){
				float x = key.x;
				float y = key.y;
				Key k = new Key(key.label,x,y);
				k.setSymbol();
				keyboard.add(k);
			}
			createPunctuation();
		}catch(Exception e){
			Log.e(TAG, "Error while defining coordinates: " + e);
			e.printStackTrace();
		}
	}
	
	private void createPunctuation(){
		for(Key key : keyboard){
			for(int p = 0; p < punctuation.length();p++){
				char label = punctuation.charAt(p);
				if(label == key.label.charAt(0)){
					Log.d("Punctuation: " + label);
					key.setPunctuation();
				}
			}
		}
		Log.d("Custom");
		swipeR = new Key("r",0,0);
		swipeR.setSpecial(1);
		shift = new Key("s",0,0);
		shift.setSpecial(7);
	}
	
	private static boolean accurate = false;
	private static boolean autoLower = false;
	
	public static ArrayList<Key> redefineText(String text, boolean exact, boolean lowerFirst){
		accurate = exact;
		autoLower = lowerFirst;
		return defineText(text);
	}
	
	public static ArrayList<Key> defineText(String text){
		ArrayList<Key> keys = new ArrayList<Key>();
		TestEngine.action = "Defining text '" + text + "'\n Accurate?: " + accurate;
		try{
			String lines[] = text.split(System.getProperty("line.separator"));
			for(String line : lines){
				String words[] = line.split("[\\s]|[/]");
				int index = -1;
				for(int w = 0; w < words.length; w++){
					Word word = cleanWord(words[w]);
					if(word == null){
					    Log.d("Skipping " + words[w]);
					    continue;
					}else if(!accurate){
						index ++;
						word.setLabel(removeBadApostrophes(word.label));
						Log.d("Compare New: " + word.label + " Old: " + words[w]);
						word.setErr(NoiseMaker.errorMaker(word.label.replace("-", "")));
						DataManager.addWord(removePunctuation(word),word.punk,word.
											error,word.label,containsCappers(word.label));
						word.setLabel(word.label.replace("-", ""));
					}
					keys = createKeys(keys, (word.error ? createErrWord(word) : word.label), index);
				}
			}
		}catch(Exception e){
			Log.e(TAG, "Error while defining text: " + e.getLocalizedMessage());
			e.printStackTrace();
		}
		if(!accurate){ 
			DataManager.displayWordCount(); 
		}
		accurate = false;
		Log.Key(TAG, keys);
		return keys;
	}
	
	private static ArrayList<Key> createKeys(ArrayList<Key> currentKeys, String currentWord, int w){
		boolean punct = false;
		for(int l = 0; l < currentWord.length(); l++){
			String letter = String.valueOf(currentWord.charAt(l));
			boolean found = false;
			for(Key key : keyboard){
				if(key.label.equals(letter.toUpperCase())){
					Log.d(key.label+caps);
					found = true;
					if(shiftCheck(l,w,key,letter) && !key.symbol){
						Log.d(key.label);
						currentKeys.add(shift);
					}
					caps = key.caps;
					punct = key.punctuation;
					currentKeys.add(key);
					break;
				}
			}
			if(!found){
				letter = Normalizer.normalize(letter, Normalizer.Form.NFD);
				letter = letter.replaceAll("[^\\p{ASCII}]", "");
				if(!letter.isEmpty()){
					for(Key key : keyboard){
						if(key.label.equals(letter.toUpperCase())){
							Log.d(key.label+caps);
							found = true;
							if(shiftCheck(l,w,key,letter) && !key.symbol){
								Log.d(key.label);
								currentKeys.add(shift);
							}
							caps = key.caps;
							punct = key.punctuation;
							currentKeys.add(key);
							break;
						}
					}
				}
			}
		}
		if(!accurate && !punct && !caps){ currentKeys.add(swipeR); }
		return currentKeys;
	}
	
	private static boolean containsCappers(String word){
		for(int c = 0; c < cappers.length(); c++){
			char caps = cappers.charAt(c);
			if(word.indexOf(caps) >= 0){
				return true;
			}
		}
		return false;
	}
	
	private static String removeBadApostrophes(String word){
		if(word.endsWith("'")){
			return word.substring(0, word.length()-1);
		}
		if(word.startsWith("'")){
			return word.substring(1, word.length());
		}
		return word;
	}
	
	private static String removePunctuation(Word word){
		if(word.punk){
			return word.label.substring(0, word.label.length()-1);
		}
		return word.label;
	}
	
	private static String createErrWord(Word word){
		String virgin = word.label;
		if(word.punk){
			virgin = virgin.substring(0,virgin.length()-1);
		}
		String dirty = NoiseMaker.makeBadWord(virgin);
		if(word.punk){
			dirty = dirty.concat(word.label.substring(word.label.length()-1));
		}
		return dirty;
	}
	
	private static Word cleanWord(String word){
		
		char[] chars = word.toCharArray();
		
		boolean dash = false;
		boolean hasAlpha = false;
		boolean hasPunct = false;
		boolean apostrophe = false;
		
		
		for(int i = 0; i < chars.length; i++){
			if(hasPunct){ //Double Punctuation
				Log.d(word + " : Double Punctuation");
				DataManager.ignored(word);
				return null; 
			}
			char c = chars[i];
			if(Character.isDigit(c)){ //Contains Digit
				Log.d(word + " : Contains Digit");
				DataManager.ignored(word);
				return null; 
			}
			if(Character.isLetter(c)){ 
				hasAlpha = true;
			}else{
				switch(c){
				case '\'':
					if(apostrophe){ //Multiple Apostrophes
						Log.d(word + " : Multiple Apostrophes");
						DataManager.ignored(word);
						return null; 
					}
					apostrophe = true; 
					break;
				case '-':
					if(dash || apostrophe){ //Unconventional Dash location
						Log.d(word + " : Unconventional Dash location");
						DataManager.ignored(word);
						return null; 
					}
					dash = true; 
					break;
				default:
					if(i == chars.length-1 && punctuation.indexOf(c) >= 0){
						hasPunct = true;
					}else{ //Unknown Punctuation
						Log.d(word + " : Unknown Punctuation");
						DataManager.ignored(word);
						return null; 
					}
					break;
				}
			}
		}
		
		if(!hasAlpha){ //No Letters Found
			Log.d(word + " : No Letters Found");
			DataManager.ignored(word);
			return null; 
		}
		
		return new Word(word, hasPunct, false);
	}
	
//	private static boolean hasAlpha(String unclean){
//		char[] chars = unclean.toCharArray();
//	    boolean hasAlpha = false;
//	    for (char c : chars) {
//	    	hasAlpha = Character.isLetter(c);
//	        if(hasAlpha){ break; }
//	    }
//	    return hasAlpha;
//	}
//	
//	private static boolean hasDigits(String unclean){
//		char[] chars = unclean.toCharArray();
//	    boolean hasDigits = false;
//	    for (char c : chars) {
//	    	hasDigits = Character.isDigit(c);
//	        if(hasDigits){ break; }
//	    }
//	    return hasDigits;
//	}
	
	private static boolean shiftCheck(int l, int w, Key key, String letter){
		if(autoLower){
			Log.d("AutoLower");
			autoLower = false;
			return false;
		}
		if(accurate){
			return (letter.equals(letter.toUpperCase()));
		}else{
			return (((l == 0 && letter.equals(letter.toLowerCase()) && (caps || w == 0)) || 
					(letter.equals(letter.toUpperCase()) && !caps && !(l == 0 && w == 0))) 
					&& !key.punctuation); //Not Punctuation
		}
	}

}
