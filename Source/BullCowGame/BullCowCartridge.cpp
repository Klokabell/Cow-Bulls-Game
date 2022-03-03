#include "BullCowCartridge.h"
#include "HiddenWordList.h"


void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    Isograms = GetValidWords(WordList); //Sets sets the return from ValidWords array to be Isograms for neatness

    StartGame();

    PrintLine(TEXT("The word is %s"), *HiddenWord);
}



void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{

    WordLetters = HiddenWord.Len();
    GuessLetters = PlayerInput.Len();
    

    if(bGameOver) //Check if the game has ended
        {
            ClearScreen();
            StartGame();
        }

    else //Check the guess is correct
        {
            GuessWork(PlayerInput);
        }   
}



void UBullCowCartridge::StartGame()
    {
        bGameOver = false;
        HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() -1)]; //Set the HiddenWord to come from the ValidWords array
        Lives = HiddenWord.Len();


        PrintLine(TEXT("Bon fackin jour and welcome to \n'Bull Cows the Movie Novelisation: \nThe Official Game'\n"));
        PrintLine(TEXT("The word has %i letters so you get %i lives. FUN!"), HiddenWord.Len(), Lives);
        PrintLine(TEXT("To begin, type your first guess and then press Enter"));
    }



void UBullCowCartridge::EndGame()
    {
        bGameOver = true;
        ClearScreen();
        PrintLine(TEXT("The word was %s "), *HiddenWord);
        PrintLine(TEXT("To play again, press Enter!")); 
    }



void UBullCowCartridge::GuessWork(const FString& Guess)
    {
        if(Guess == HiddenWord)
            {
                PrintLine(TEXT("You guessed correctly!"));
                EndGame();
                return;
            }

        if (GuessLetters != WordLetters) // Check the number of letters is correct
            {
                PrintLine(TEXT("This isn't the right amount of letters, idiot \n There are %i letters"), WordLetters);
                return;
            }
            
        if (!IsIsogram(Guess))
            {
                PrintLine(TEXT("No repeating letters!"));
                return;
            }

        else
            {
                if(Lives > 0) // Check if there are lives left
                {
                    PrintLine(TEXT("Nope! One down, a billion to go!"));
                    PrintLine(TEXT("You have %i live(s) left."), --Lives);

                    FBullCowCount Score = GetBullCows(Guess);

                    //PrintLine(TEXT("You have %i Bulls and %i cows"), Bulls, Cows);


                    PrintLine(TEXT("You got %i Bulls and %i Cows here"), Score.Bulls, Score.Cows);
                        if(Score.Bulls > 0)
                            {   
                                PrintLine(TEXT("You got these ones totally right"));
                                for(int32 i = 0; i < Score.BullLetters.Num(); i++)
                                    {
                                        PrintLine(TEXT("%c"), Score.BullLetters[i]);
                                    }
                            }

                    return;
                }

            else
                {
                    EndGame();
                }      
            }
    }



bool UBullCowCartridge::IsIsogram(const FString& Word) const
    {    
        for (int32 Index = 0; Index < Word.Len(); Index++)
        {
            for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
                {
                    if (Word[Index] == Word[Comparison])
                        {
                            return false;
                        }
                }                
        }
        
        return true;
    }



TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& Words) const
    {
        
        TArray<FString> ValidWords;

        for(FString Wordies : Words) 
            {
                if(Wordies.Len() >= 4 && Wordies.Len() <= 8)
                {   
                    if(IsIsogram(Wordies))
                        {
                            ValidWords.Emplace(Wordies);
                        }
                }
            }
            return ValidWords;
    }


FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
    {
        FBullCowCount Count;

        //for when every index "Guess" is the same as index "Hidden", BullCount ++


        for(int32 GIndex = 0; GIndex < Guess.Len(); GIndex++)
            {
                if(Guess[GIndex] == HiddenWord[GIndex])
                    {
                        Count.Bulls++;
                        Count.BullLetters.Emplace(Guess[GIndex]);
                        continue;
                    }

                for(int32 HIndex = 0; HIndex < Guess.Len(); HIndex++)
                    {
                        if(HiddenWord[HIndex] == Guess[GIndex])
                            {
                                Count.Cows++;
                                break;
                            }
                    }
            }
        return Count;
    }


