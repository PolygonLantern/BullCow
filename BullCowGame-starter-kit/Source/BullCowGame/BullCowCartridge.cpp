// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordsList.h"
// #include "UnrealMathUtility.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    
    Isograms = GetValidWords(HiddenWords);
    
    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else
    {
        AssesGuess(Input);
    }
}

void UBullCowCartridge::SetupGame()
{   
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)]; // Every time when using FString, wrap it up in TEXT to decode the string to most of the supported platforms
    bGameOver = false;
    Lives = HiddenWord.Len() * 2;
    PrintLine(TEXT("You have %i lives."), Lives);
    PrintLine(TEXT("Hidden word is: %s"), *HiddenWord);
    ShowWelcomeMessage();
}

void UBullCowCartridge::ShowWelcomeMessage()
{
    // Print Welcome message
    PrintLine(TEXT("Welcome to Cow Bull!"));
    PrintLine(TEXT("Guess the %i letter word."), HiddenWord.Len());
    PrintLine(TEXT("Type a word and press enter to guess..")); 
}

void UBullCowCartridge::AssesGuess(const FString& Guess) //check the player input
{
    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("You guessed the word."));
        EndGame();
        return;
    }
    
    if(Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("The word length is %i\nTry Again!"), HiddenWord.Len());
        PrintLine(TEXT("Lives remaining: %i"), --Lives);
        return;
    }
    else
    {
        PrintLine(TEXT("You have lost life."));
        --Lives;        
    }

    if(!IsIsogram(Guess))
    {
        PrintLine(TEXT("Your guess is not an isogram. \nTry again."));
        --Lives;
        return;
    }
    
    if(Lives <= 0)
    {
        EndGame();
        PrintLine(TEXT("The word was: %s"), *HiddenWord);
    }

    // Show bulls and cows
    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);
}

void UBullCowCartridge::EndGame()
{
    PrintLine(TEXT("Game Over! \nPress enter to play again."));
    bGameOver = true;
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
   for (int32 Index = 0; Index < Word.Len(); Index++)
   {
       for (int32 Comparison = Index + 1; Comparison < Word.Len()-1; Comparison ++)
       {
           if (Word[Index] == Word[Comparison])
           {
               return false;
           }
       }
   }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for(FString Word : WordList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;
    
    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }
        for (int32 HiddenWordIndex = 0; HiddenWordIndex < Guess.Len(); HiddenWordIndex ++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenWordIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}