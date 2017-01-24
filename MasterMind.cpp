#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm> 
#include "GuessClass.hpp"


//Function prototypes
std::pair<int,int> GetHint(std::string const& a, std::string const& b);
Guess BestGuess(std::vector<Guess> &poss);
void Permute(std::string const& a, int iteratorStart, int iteratorEnd, std::vector<Guess> &poss);


int main(int argc, char const *argv[]) {
    
    std::cout << "\n\n\n*** WELCOME TO MASTERMIND ***" 
            << "\n\nImagine you have some Blue, Green, Orange, Purple, Red, and Yellow balls."
            <<" Arrange some of them into a row."
            <<  "\nThe computer will try to guess your sequence of colours."
            <<  "\nWhen you're ready, type in the length of your sequence (between 0 and 6) and hit RETURN.\n";
    std::cout.flush();
    
    //Assign the user's code length to a variable, and validate the data.
    unsigned int code_length;
    std::cin >> code_length;
    if(std::cin.fail() || code_length == 0 || code_length > 6){std::cout << "The code length must be a positive"
                                                                         << " integer less than 6! Please start the game"
                                                                         << " again.\n"; return 0;}

    std::string colours("BGOPRY");
    std::vector<Guess> poss(pow(6,code_length));

    //Populate the list of possibilities with all the possible code combinations.
    Permute(colours, 0, pow(6,code_length)-1, poss);



    //The computer makes it's first guess.  This consists of 2 colours, each making up approx half of the guess.
    std::string guessOne = "";
    for(int i =0; i< code_length/2 ;i++){guessOne += colours[0];}
    for(int i = code_length/2; i< code_length ;i++){guessOne += colours[1];}
    Guess current(guessOne);

    int k = 0;
    unsigned int exactMatch;
    unsigned int placementError;
    while(k<10 && poss.size()>0){
        // Retrieve and validate the user's hints (i.e. their response to the guess).
        std::cout << "The computer's guess is" << current << ".\n\n";
        std::cout << "Enter the number of exact matches between your code and the guess."
                  << "Then hit RETURN.\n";
        std::cout.flush();
        std::cin >> exactMatch;
        if(std::cin.fail() || exactMatch > code_length)
            {std::cout << "Hints must be positive integers less than the length of the code! Please start the game "
                       << "again.\n";
            return 0;
            }
        std::cout << "Please enter the number of correctly coloured balls that are in the "
                  << "wrong place. Then hit RETURN.\n";
        std::cout.flush();
        std::cin >> placementError;
        if(std::cin.fail() || placementError > code_length)
            {std::cout << "Hints must be positive integers less than the length of the code! Please start the game"
                       << " again.\n";
            return 0;
            }

        //Process the hints.
        std::pair<int,int> Hint;
        if(exactMatch == code_length){
            //Then the computer has won, so let the user know.
            if(k==0)
                {std::cout << "Success!  The correct code has been guessed by the computer in " << k+1 
                           << " try!\n";
                }
            else {std::cout << "Success!  The correct code has been guessed by the computer in " << k+1 << " tries!\n";}
            std::cout.flush();
            break;
        }
        else{
            //Remove any guesses that are inconsistent with the user's hint.
            std::cout << poss.size() << " possiblities have been narrowed down to ";
            for(int i=poss.size()-1; i>-1; i--){
                //If a code doesn't return the user's hint in comparison with the computer's current guess, it can be
                //eliminated.
                Hint = GetHint(current.GetCode(), poss[i].GetCode());
                if(!((exactMatch == Hint.first) && (placementError == Hint.second))){
                    poss.erase(poss.begin() + i);
                }
            }
            //Let the user know the computer's progress.
            std::cout << poss.size() << ".\n\n";
            std::cout.flush();
            //Now calculate the next best guess.
            current = BestGuess(poss);      
        }
        k++;
    }

    // Give feedback to the user when the while loop quits without succeeding to find their code.
    if(poss.size()==0){std::cout << "Contradictory hints have been given!  Please start the game again.\n";}
    if(k==10){
        std::cout << "You won!  The computer couldn't guess your code in less than 10 tries.  Please start "
                  << "the game again.\n";
        }
    return 0;
}











//For a given pair of code guess, find the Mastermind hint. 
std::pair<int,int> GetHint(std::string const& a, std::string const& b){

    int n = a.length();
    // Find the number of exact matches between the codes.
    int hint[n];  //An array to record the matches between the 2 codes.
    for(int i=0; i<n; i++){
        if(a[i] == b[i]){
            //Denote an exact match by a 2 in the hint recorder.
            hint[i] = 2;
        }
        else{hint[i] = 0;}
    }

    // Excluding the exact matches and repeats, find the number of correct colours with the wrong 
    //  placement between the codes.
    for(int i=0; i<n; i++){
        //if an exact match hasn't been recorded within between the seqs, test for placement mismatches.
        if(hint[i] != 2){
            for(int j=0; j<n; j++){
                //If the ball in the second code is as yet unmatched, but now matches, denote this with a 1.
                if(hint[j] == 0 & a[i] == b[j]){
                    hint[j] = 1;
                    //Exit the for loop as soon as the first match has been made.
                    break;
                }
            }
        }
    }


    //Now count the number of exact matches and placement errors.
    int exactMatch = 0;
    int placementError = 0;
    for(int i=0; i<n; i++){
        if(hint[i] == 2){ exactMatch++; }
        else if(hint[i] == 1){placementError++;}
    }

    return std::make_pair(exactMatch, placementError);

}




//Populate the vector poss with a list of guesses consisting of all posible codes of length n.
//Done recursively by setting each letter of the code in turn.
void Permute(std::string const& a, int iteratorStart, int iteratorEnd, std::vector<Guess> &poss){
    //To split the length of poss into m equal sections, where m is the number of colours, 
    //first find the length of each section.
    int n = (iteratorEnd - iteratorStart + 1)/a.length();
    //For each section, append one of the colours to the end of the code.
    for(int i=0; i<a.length(); i++){
        for(int j=n*i+iteratorStart ; j<n*(i+1)+iteratorStart; j++){
            poss[j].SetCode(poss[j].GetCode() += a[i]);
        }
        //Pass the subvector consisting of all entries ending with a[i] back to Permute.
        if(n!=1){
            Permute(a, n*i+iteratorStart, n*(i+1)+iteratorStart, poss);
        }
    }
}




//Find the computer's next best guess, in accordance with Knuth's Algorithm.
Guess BestGuess(std::vector<Guess> &poss){
    std::pair<int,int> Hint;
    int eliminationsCount;
    int exactMatch;
    int placementError;
    int mostEliminations = 0;
    //Each possibility is a candidate for the computer's next guess.
    for(int GuessCandidate = 0; GuessCandidate<poss.size(); GuessCandidate++){
        //Consider the case that poss[UserCode] is the correct colour sequence.
        int worstCaseEliminations = poss.size();
        for(int UserCode = 0; UserCode<poss.size(); UserCode++){
            //Then the user would return the hint given below.
            Hint = GetHint(poss[GuessCandidate].GetCode(), poss[UserCode].GetCode());
            exactMatch = Hint.first;
            placementError = Hint.second;
            //In this scenario, any guess that didn't provide the same hint against the
            //candidate could be elinmated.  The number of disregarded guesses is found below.
            for(int i=poss.size(); i>-1; i--){
                eliminationsCount = 0;
                Hint = GetHint(poss[GuessCandidate].GetCode(), poss[i].GetCode());
                if(!((exactMatch == Hint.first) && (placementError == Hint.second))){
                    eliminationsCount++;
                }
            }
            //In the worse case scenario, the minimum of the eliminationsCounts can be ruled
            //out as the solution. This is:
            worstCaseEliminations = std::min(worstCaseEliminations, eliminationsCount);
        }
        //Set this as the worst case scenario for the guess.
        poss[GuessCandidate].SetWorstCase(worstCaseEliminations);
        //At each step, find the best 'worst case scenario' encountered so far:
        mostEliminations = std::max(mostEliminations, worstCaseEliminations);
    }

    //All scenarios have been considered.
    //Now return the first entry that will provide the mostEliminations, as calculated above..
    for(int i=0; i<poss.size(); i++){
        if(poss[i].GetWorstCase() == mostEliminations){return poss[i];}
    }
    return poss[0];
}