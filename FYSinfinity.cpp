/******
Program: FYSinfinity.cpp
Purpose: to sort first-years into their preferred FYS class based on
various demographics such as gender, ethnicity, and learning disability.
Input: a txt file (originally a csv file) with each student's information
Output: a csv file containing the information of the students who have been
assigned to a FYS class and those who have not been assigned to a class
******/


#include <iostream>
#include <fstream>
#include <string>
#include <stack>

using namespace std;

const int CLASS_MAX = 19; //maximum number of students in each class
const int NUM_FYS_CLASSES = 28; //number of FYS classes (changes year to year)
const int MAX_LEARNINGDIS = 2; //maximum number of students with learning disabilities put into each class

struct wheatonFYinfo{ //every student has this information
  string firstName;
  string lastName;
  string studentID;
  string gender;
  string learningDis;
  string ethnicity;
  string choice1, choice2, choice3, choice4, choice5, choice6, choice7, choice8;
  string FYSclass;
};

class FYS{
private:
public:
    int maleCount; //keeps track of the number of males in a given class
    int femaleCount; //keeps track of the number of females in a given class
    int learningDisCount; //keeps track of the number of students with a learning disability in a given class
    int determineChoice(string);
    int nextChoice(string&, wheatonFYinfo);
    FYS();
};

FYS::FYS(){ //initialize counters
    maleCount = 0;
    femaleCount = 0;
    learningDisCount = 0;
}

int main(){

    ifstream inFP;
    ofstream outFP;

    string fileName;

    wheatonFYinfo info; //instance of wheatonFYinfo struct to access its data members

    cout << "Enter file name containing list of Wheaton first-years and their choices:" << endl;
    cin >> fileName;
    inFP.open(fileName.c_str());

    while(inFP.fail()){
            inFP.clear();
            cout << "FILENAME IS INCORRECT, TRY AGAIN." << endl;
            cin >> fileName;
            inFP.open(fileName.c_str());
    }

    if (inFP){ //if file is found

        //create same number of instances as number of FYS classes for that year
        stack<wheatonFYinfo> FYSclass[NUM_FYS_CLASSES]; //makes stacks of type wheatonFYinfo struct
        FYS FYSclassInfo[NUM_FYS_CLASSES]; //makes instances to use for each stack made above

        stack<wheatonFYinfo> unassignedFYclass; //make an instance for students that were not assigned to any class

        //reads and skips over headers
        getline(inFP, info.lastName, '\t');
        getline(inFP, info.firstName, '\t');
        getline(inFP, info.studentID, '\t');
        getline(inFP, info.gender, '\t');
        getline(inFP, info.learningDis, '\t');
        getline(inFP, info.ethnicity, '\t');
        getline(inFP, info.choice1, '\t');
        getline(inFP, info.choice2, '\t');
        getline(inFP, info.choice3, '\t');
        getline(inFP, info.choice4, '\t');
        getline(inFP, info.choice5, '\t');
        getline(inFP, info.choice6, '\t');
        getline(inFP, info.choice7, '\t');
        getline(inFP, info.choice8, '\n');


        getline(inFP, info.lastName, '\t');

        int pushCounter1 = 0; //keeps track of number of unassigned students
        int pushCounter2 = 0; //keeps track of number of assigned students

        while (info.lastName != "DONE."){ //indicates end of file being read
            getline(inFP, info.firstName, '\t');
            getline(inFP, info.studentID, '\t');
            getline(inFP, info.gender, '\t');
            getline(inFP, info.learningDis, '\t');

            bool learnDis;
            if (info.learningDis != ""){ //non-empty slot indicates a student who has a learning disability
                learnDis = true;
            }
            else{
                learnDis = false;
            }

            getline(inFP, info.ethnicity, '\t');
            getline(inFP, info.choice1, '\t');
            getline(inFP, info.choice2, '\t');
            getline(inFP, info.choice3, '\t');
            getline(inFP, info.choice4, '\t');
            getline(inFP, info.choice5, '\t');
            getline(inFP, info.choice6, '\t');
            getline(inFP, info.choice7, '\t');
            getline(inFP, info.choice8, '\n');

            bool found = false;

            string tempchoice;
            tempchoice = info.choice1;
            int choice;
            choice=FYSclassInfo[0].determineChoice(tempchoice); //makes each choice represent the FYS stack that was chosen
                                                                //THESE CRNs WILL HAVE TO BE EDITED EACH YEAR


            while(!found && choice != -1){ //while student is not assigned to a class (and we haven't gone through all of the class choices they had)

                if(FYSclass[choice].size() < CLASS_MAX){ //check if given FYS class is full yet
                    if((learnDis && FYSclassInfo[choice].learningDisCount < MAX_LEARNINGDIS) || learnDis == false ){ //check if student has a learning disability & number of those students already in class is less than 2
                        if(FYSclass[choice].size() == (CLASS_MAX - 4)){ //checks gender
                            if(info.gender == "M" && FYSclassInfo[choice].femaleCount == 0){  //need more females, so males can't join class
                                FYSclassInfo[0].nextChoice(tempchoice,info); //updates tempchoice
                                choice=FYSclassInfo[0].determineChoice(tempchoice); //store student's next choice in choice
                            }
                            else if(info.gender == "F" && FYSclassInfo[choice].maleCount == 0){ //need more males, so females can't join class
                                FYSclassInfo[0].nextChoice(tempchoice,info); //updates tempchoice
                                choice=FYSclassInfo[0].determineChoice(tempchoice); //store student's next choice in choice
                            }
                            else{ //need more of gender being read in that class
                                    //or gender count is good so they can join class

                                //add to gender counters
                                if(info.gender == "M")
                                    FYSclassInfo[choice].maleCount++;
                                else{
                                    FYSclassInfo[choice].femaleCount++;
                                }
                                if(learnDis){
                                    FYSclassInfo[choice].learningDisCount++;
                                }
                                found = true;
                            }
                        }
                        else if(FYSclass[choice].size() == (CLASS_MAX - 3)){ //checks gender
                            if(info.gender == "M" && FYSclassInfo[choice].femaleCount == 1){  //need more females, so males can't join class
                                FYSclassInfo[0].nextChoice(tempchoice,info); //updates tempchoice
                                choice=FYSclassInfo[0].determineChoice(tempchoice); //store student's next choice in choice
                            }
                            else if(info.gender == "F" && FYSclassInfo[choice].maleCount == 1){ //need more males, so females can't join class
                                FYSclassInfo[0].nextChoice(tempchoice,info); //updates tempchoice
                                choice=FYSclassInfo[0].determineChoice(tempchoice); //store student's next choice in choice
                            }
                            else{ //need more of gender being read in that class
                                    //or gender count is good so they can join class

                                //add to counters
                                if(info.gender == "M")
                                    FYSclassInfo[choice].maleCount++;
                                else{
                                    FYSclassInfo[choice].femaleCount++;
                                }
                                if(learnDis){
                                    FYSclassInfo[choice].learningDisCount++;
                                }
                                found = true;
                            }
                        }
                        else if(FYSclass[choice].size() == (CLASS_MAX - 2)){ //checks gender
                            if(info.gender == "M" && FYSclassInfo[choice].femaleCount == 2){  //need more females, so males can't join class
                                FYSclassInfo[0].nextChoice(tempchoice,info); //updates tempchoice
                                choice=FYSclassInfo[0].determineChoice(tempchoice); //store student's next choice in choice
                            }
                            else if(info.gender == "F" && FYSclassInfo[choice].maleCount == 2){ //need more males, so females can't join class
                                FYSclassInfo[0].nextChoice(tempchoice,info); //updates tempchoice
                                choice=FYSclassInfo[0].determineChoice(tempchoice); //store student's next choice in choice
                            }
                            else{ //need more of gender being read in that class
                                    //or gender count is good so they can join class

                                //add to counters
                                if(info.gender == "M")
                                    FYSclassInfo[choice].maleCount++;
                                else{
                                    FYSclassInfo[choice].femaleCount++;
                                }
                                if(learnDis){
                                    FYSclassInfo[choice].learningDisCount++;
                                }
                                found = true;
                            }
                        }
                        else if(FYSclass[choice].size() == (CLASS_MAX - 1)){ //checks gender
                            if(info.gender == "M" && FYSclassInfo[choice].femaleCount == 3){  //need more females, so males can't join class
                                FYSclassInfo[0].nextChoice(tempchoice,info); //updates tempchoice
                                choice=FYSclassInfo[0].determineChoice(tempchoice); //store student's next choice in choice
                            }
                            else if(info.gender == "F" && FYSclassInfo[choice].maleCount == 3){ //need more males, so females can't join class
                                FYSclassInfo[0].nextChoice(tempchoice,info); //updates tempchoice
                                choice=FYSclassInfo[0].determineChoice(tempchoice); //store student's next choice in choice
                            }
                            else{ //need more of gender being read in that class
                                    //or gender count is good so they can join class

                                //add to counters
                                if(info.gender == "M")
                                    FYSclassInfo[choice].maleCount++;
                                else{
                                    FYSclassInfo[choice].femaleCount++;
                                }
                                if(learnDis){
                                    FYSclassInfo[choice].learningDisCount++;
                                }
                                found = true;
                            }
                        }
                        else if(FYSclass[choice].size() == CLASS_MAX){ //checks gender
                            if(info.gender == "M" && FYSclassInfo[choice].femaleCount == 4){  //need more females, so males can't join class
                                FYSclassInfo[0].nextChoice(tempchoice,info); //updates tempchoice
                                choice=FYSclassInfo[0].determineChoice(tempchoice); //store student's next choice in choice
                            }
                            else if(info.gender == "F" && FYSclassInfo[choice].maleCount == 4){ //need more males, so females can't join class
                                FYSclassInfo[0].nextChoice(tempchoice,info); //updates tempchoice
                                choice=FYSclassInfo[0].determineChoice(tempchoice); //store student's next choice in choice


                            }
                            else{ //need more of gender being read in that class
                                    //or gender count is good so they can join class

                                //add to counters
                                if(info.gender == "M")
                                    FYSclassInfo[choice].maleCount++;
                                else{
                                    FYSclassInfo[choice].femaleCount++;
                                }
                                if(learnDis){
                                    FYSclassInfo[choice].learningDisCount++;
                                }
                                found = true;
                            }
                        }
                        else{
                            //add to counters
                            if(info.gender == "M")
                                FYSclassInfo[choice].maleCount++;
                            else{
                                FYSclassInfo[choice].femaleCount++;
                            }
                            if(learnDis){
                                FYSclassInfo[choice].learningDisCount++;
                            }
                            found = true;
                        }
                    }
                    else{
                        FYSclassInfo[0].nextChoice(tempchoice,info); //updates tempchoice

                        choice=FYSclassInfo[0].determineChoice(tempchoice);

                    }
                } //if loops end

                else{
                    FYSclassInfo[0].nextChoice(tempchoice,info); //updates tempchoice

                    choice=FYSclassInfo[0].determineChoice(tempchoice); //store student's next choice in choice
                }
            } //while loop ends

            if(choice == -1){ //students who did not fill out all of their FYS preferences or
                                //could not be sorted properly for some other reason
                unassignedFYclass.push(info); //put students in unassigned stack and manually sort
                pushCounter1++; //increment counter keeping track of unassigned students
            }
            else{
                FYSclass[choice].push(info); //put students in corresponding FY class
                pushCounter2++; //increment counter keeping tack of assigned students
            }
            getline(inFP, info.lastName, '\t');
        }

        cout << "Unassigned students: " << pushCounter1 << endl; //display number of unassigned students for reference
        cout << "Assigned students: " << pushCounter2 << endl; //display number of assigned students for reference

        outFP.open("FYSresults.csv"); //creates final csv file with results

        outFP << "FYS" << ',' << "LAST NAME " << ',' << "FIRST NAME" << ',' << "ID" << ','<<"GENDER"<<','<<"LEARNING DISABILITY"<<','<<"ETHNICITY"<<'\n'; //headers for output file
        for (int choice = 0;choice<NUM_FYS_CLASSES;choice++){
            outFP << "FYS " << choice+1 << '\n'; //add 1 to choice to properly display FYS number (make it start at FYS 1)
            while(!FYSclass[choice].empty()){ //check that given FYS class is not empty before popping out students
                outFP << "" << ',' << FYSclass[choice].top().lastName << ',' << FYSclass[choice].top().firstName << ',' <<FYSclass[choice].top().studentID << ','<< FYSclass[choice].top().gender<<','<<FYSclass[choice].top().learningDis<<','<<FYSclass[choice].top().ethnicity<<'\n'; //access and display each student's info
                FYSclass[choice].pop(); //pop topmost student in order to access the next student's information
            }
        }

        outFP << "FYS unassigned" << '\n'; //display unassigned students
        while(!unassignedFYclass.empty()){
            outFP << "" << ',' << unassignedFYclass.top().lastName << ',' << unassignedFYclass.top().firstName << ',' <<unassignedFYclass.top().studentID << ',' << unassignedFYclass.top().gender<< ','<<unassignedFYclass.top().learningDis<< ','<<unassignedFYclass.top().ethnicity<< '\n';
            unassignedFYclass.pop(); //pop topmost student in unassigned stack in order to access the next student's information
        }

        inFP.close(); //close the input file
        outFP.close(); //close the output file
    }

    return 0;
}

int FYS::nextChoice(string &tempchoice, wheatonFYinfo info){ //method updates tempchoice in order to store their next choice in it
    if(tempchoice == info.choice1)
        tempchoice = info.choice2;
    else if (tempchoice == info.choice2)
        tempchoice = info.choice3;
    else if (tempchoice == info.choice3)
        tempchoice = info.choice4;
    else if (tempchoice == info.choice4)
        tempchoice = info.choice5;
    else if (tempchoice == info.choice5)
        tempchoice = info.choice6;
    else if (tempchoice == info.choice6)
        tempchoice = info.choice7;
    else if (tempchoice == info.choice7)
        tempchoice = info.choice8;
    else
        tempchoice = "-1";
}

int FYS::determineChoice(string tempchoice){ //method takes CRN as input and converts it to a choice number to be returned to the original call
    //makes each choice represent the FYS stack that was chosen
    //THESE CRNs WILL HAVE TO BE EDITED EACH YEAR
    int choice;

     if(tempchoice == "10502"){
        choice = 0; //always start at 0
        return choice;
    }
    else if(tempchoice == "10537"){
        choice = 1;
        return choice;
    }
    else if(tempchoice == "10504"){
        choice = 2;
        return choice;
    }
    else if(tempchoice == "10505"){
        choice = 3;
        return choice;
    }
    else if(tempchoice == "10506"){
        choice = 4;
        return choice;
    }
    else if(tempchoice == "10507"){
        choice = 5;
        return choice;
    }
    else if(tempchoice == "10508"){
        choice = 6;
        return choice;
    }
    else if(tempchoice == "10509"){
        choice = 7;
        return choice;
    }
    else if(tempchoice == "10510"){
        choice = 8;
        return choice;
    }
    else if(tempchoice == "10511"){
        choice = 9;
        return choice;
    }
    else if(tempchoice == "10512"){
        choice = 10;
        return choice;
    }
    else if(tempchoice == "10513"){
        choice = 11;
        return choice;
    }
    else if(tempchoice == "10514"){
        choice = 12;
        return choice;
    }
    else if(tempchoice == "10515"){
        choice = 13;
        return choice;
    }
    else if(tempchoice == "10516"){
        choice = 14;
        return choice;
    }
    else if(tempchoice == "10517"){
        choice = 15;
        return choice;
    }
    else if(tempchoice == "10518"){
        choice = 16;
        return choice;
    }
    else if(tempchoice == "10519"){
        choice = 17;
        return choice;
    }
    else if(tempchoice == "10520"){
        choice = 18;
        return choice;
    }
    else if(tempchoice == "10521"){
        choice = 19;
        return choice;
    }
    else if(tempchoice == "10522"){
        choice = 20;
        return choice;
    }
    else if(tempchoice == "10523"){
        choice = 21;
        return choice;
    }
    else if(tempchoice == "10524"){
        choice = 22;
        return choice;
    }
    else if(tempchoice == "10525"){
        choice = 23;
        return choice;
    }
    else if(tempchoice == "10526"){
        choice = 24;
        return choice;
    }
    else if(tempchoice == "10527"){
        choice = 25;
        return choice;
    }
    else if(tempchoice == "10528"){
        choice = 26;
        return choice;
    }
    else if(tempchoice == "10541"){
        choice = 27;
        return choice;
    }
    else{
        choice = -1; //-1 refers to the unassigned stack class
        return choice;
    }
}
