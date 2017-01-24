#include <string>
#include <iostream>

class Guess {
    private:
        int mWorstCase;
        std::string mCode;
    public:
        Guess();
        Guess(std::string const& code);

        void SetCode(std::string const& code); 
        void SetWorstCase(int worstCase);

        std::string GetCode() const; 
        int GetWorstCase() const;

        friend std::ostream& operator<<(std::ostream& output, const Guess& show);
};


/* Constructor that automatically sets the boolean status to true, to indicate that all
   guesses are possible at the start of a game */
Guess::Guess(std::string const& code){
    mCode = code;
    mWorstCase = 0;
}

//  Constructor to initialise a guess.
Guess::Guess(){
    mCode = "";
    mWorstCase = 0;
}


//Functions to set private variables
// ---------------------------------
void Guess::SetCode(std::string const& code){
    mCode = code;
} 

void Guess::SetWorstCase(int worstCase){
    mWorstCase = worstCase;
}



//Functions to retrieve private variables
// ---------------------------------
int Guess::GetWorstCase() const{
    return mWorstCase;
}

std::string Guess::GetCode() const{
    return mCode;
}



// Overloading the insertion << operator, so that a Guess prints in a user friendly manner.
std::ostream& operator<<(std::ostream& output, const Guess& show){
   for(int i=0; i<show.GetCode().length(); i++){
    if(show.GetCode()[i] == 'B'){output << " Blue"; }
    else if(show.GetCode()[i] == 'G'){output << " Green";}
    else if(show.GetCode()[i] == 'O'){output << " Orange";}
    else if(show.GetCode()[i] == 'P'){output << " Purple";}
    else if(show.GetCode()[i] == 'R'){output << " Red";}
    else{output << " Yellow";}
    }
    return(output);}