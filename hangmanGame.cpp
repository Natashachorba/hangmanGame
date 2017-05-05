/* simple hangman game
  ______
  |     |
  |     O
  |    \|/
  |     |
  |    / \
  |
__|__
*/
#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <ctime>
#include <map>
#include <fstream>

using namespace std;
typedef map<char,int> myMap;

//reads the file at filePath, adds all words in it to words vector and returns count
int fileReader(vector<string>& words, const string& filePath){
  int count = 0;
  string line;
  ifstream enteredFile (filePath.c_str());
  if (enteredFile.is_open()){
    while (getline(enteredFile,line)){
      words.push_back(line);
      ++count;
    }
    enteredFile.close();
    return count;
  }
  else {
    cout<<"Unable To Open "<<filePath<<endl<<"exiting";
    exit(1);
  }
}

//prints an amount of a man hanging depending upon the number of pieces input
void hangManPrinter(int pieces){
  cout<<"   _____"<<endl;
  cout<<"  |     |"<<endl;
  cout<<"  |";
  if (pieces>0){cout<<"     O";}
  cout<<endl;
  cout<<"  |";
  if (pieces>1){cout<<"    \\|";}
  if (pieces>2){cout<<"/";}
  cout<<endl;
  cout<<"  |";
  if (pieces>3){cout<<"     |";}
  cout<<endl;
  cout<<"  |";
  if (pieces>4){cout<<"    / ";}
  if (pieces>5){cout<<"\\";}
  cout<<endl;
  cout<<"  |"<<endl;
  cout<<"__|__"<<endl;
}

//displays the input word with spaces between each character
void displayWord(const string& word){
  for (int i=0; i<word.length(); i++){
    cout<<word[i]<<' ';
  }
  cout<<endl;
}

/*checks if the input char is in the original word an updates the fake word
as needed. returns the number of times the char appeared in the word
*/
int wordChecker(const string& word, string& fakeWord, char nextLetter){
  int changed = 0;
  for (int i=0;i<word.length();i++){
    if (word[i] == nextLetter){
      fakeWord[i] = nextLetter;
      ++changed;
    }
  }
  displayWord(fakeWord);
  return changed;
}

//returns 1 if win state is reached, 0 otherwise
int checkWin(const string& word){
  for (int i=0; i<word.length();i++){
    if (word[i] == '_'){
      return 0;
    }
  }
  return 1;
}

//runs each session of the hangman game
int playGame(const string& word){
  string fakeWord(word.length(),'_');
  int falseGuesses = 0;
  myMap guessedLetters;
  
  cout<<"Starting a game of hangman!"<<endl;
  displayWord(fakeWord);
  hangManPrinter(falseGuesses);
  while(falseGuesses < 6){
    char guessLetter;
    cout<< "Please guess a letter: ";
    cin>> guessLetter;
    //make sure that the entered letter has not already been used
    while (guessedLetters.find(guessLetter) != guessedLetters.end()){
      cout<<"You already picked "<<char(toupper(guessLetter))<<endl;
      cout<<"Please guess a letter: ";
      cin>> guessLetter;
    }
    //add the new guessed letter to guessedLetters map, then print it
    guessedLetters[guessLetter] = 1;
    cout<<"Guessed Letters: ";
    for (myMap::iterator i =guessedLetters.begin(); i!=guessedLetters.end();i++){
      cout<< char(toupper(i->first))<<" ";
    }
    cout<<endl<<endl;
    //check if used already here then print all guessed letter
    if (wordChecker(word,fakeWord,guessLetter) > 0){
      //check if win & return 1
      if (checkWin(fakeWord)){
        hangManPrinter(falseGuesses);
        return 1;
      }
    }
    else {
      ++falseGuesses;
    }
    hangManPrinter(falseGuesses);
  }
  
  return 0;
}

int main(int argc, char *argv[]){
  //allows for random numbers
  srand(time(NULL));
  vector<string> words;
  string enteredFile = "listOfWords.txt";
  if (argc>1){
    enteredFile=argv[1];
  }
  int wordSize = fileReader(words, enteredFile);
  
  while (wordSize>0){
    string randWord = words[rand()%wordSize];
    
    if(playGame(randWord)){
      cout<<"Congratulations! You guessed right and won!"<<endl;
    }
    else {
      cout<<"So Sorry that you lost! The word was "<<randWord<<endl;
    }
    
    char playAgain;
    cout<<"Do you want to play again? Y/N : ";
    cin >> playAgain;
    if (toupper(playAgain) != 'Y'){break;}
    cout<<endl;
  }
}

