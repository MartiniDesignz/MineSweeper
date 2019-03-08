/* -----------------------Description-------------------------------
 * This is a generic minesweeper game that you play in the console.
 * It will ask you for the dimensions of the board at the beginning,
 * then it generates the bomb coords, places the bombs, calculates
 * the numbers around the bombs.  It will then ask for the x and y for
 * your guess.  If you guess a coord of a 0 it reveils all the 0's that it
 * has contact with.  If you guess a coord of a bomb, it displays all
 * the bomb locations and you instantly lose.
 *
 *
 *
 * Created by: martinidesignz
 * Date:       March 7, 2019
 *
*/
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
using namespace std;

//Custom variables (structs)----------------------

struct coord{short int x; short int y;}; // stuct for storing coords

struct board{// stores setup data for the board
    int x;   //height
    int y;   //width
    vector<coord> mines;   //vector for storing mine coords
};

struct endVars{
    bool win;
    bool end;//saves me from adding another var to 3 funcs
};

//constant variables------------------------------

const char bomb = '*';
const char blank = '-';
const int ratioBomb = 5;//ratio for num of bombs | Smaller the number the more bombs

//general fuctions and debug functions-----------------------------


int mod(int x, int y){return (x/y)-x%y;}//get the dividen of x / y without a remainder


void t(){cout<<"test"<<endl;}//func for quick testing stuff


void dispDebug(board bdCon, vector<vector<int>> &bdDisp){//displays the game board for debuging
    cout<<"  ";
    for(int i=0; i<bdCon.y+2; i++){
        for(int p=0; p<bdCon.x+1; p++){
            if (i==0){
                cout<<p<<" ";
            }else if(i==1){
                cout<<"__";
            }else{
                if(p==0){
                    if(i>10){
                        cout<<i-1<<"|";
                    }else{
                        cout<<i-1<<" |";
                    }
                }else{
                    cout<<" "<<bdDisp[p-1][i-2];
                }
            }
        }
        cout<<endl;
    }
}


void printMineCoords(board bdCon){
    for (int i=0; i<bdCon.mines.size()-1; ++i){//prints mine coords for testing
        cout<<bdCon.mines.at(i).x<<" "<<bdCon.mines.at(i).y<<endl;
    }
}

//Game start functions----------------------


void init(board& bd){//ask for the board dimensions and resize the mines vector
    cout<<"Enter board width: ";
    while(true){//enter width
        cin>>bd.x;
        if(bd.x>=5 and bd.x<=100){
            break;
        }else{
            cout<<"Error, enter a number between 5 and 100: ";
        }
    }
    cout<<"Enter board height: ";
    while(true){//enter height
        cin>>bd.y;
        if(bd.y>=5 and bd.y<=100){
            break;
        }else{
            cout<<"Error, enter a number between 5 and 100: ";
        }
    }
    bd.mines.resize(bd.x*bd.y/ratioBomb+1);
}


void bombGen(board& bd){//generate the coords for the bombs
    for (int i=0; i<bd.mines.size()-1; i++){// create x mines
        coord temp;
        bool cont = true;
        while(cont==true){//do until find unocupied location
            temp.x=rand()%bd.x;//create rand x and y
            temp.y=rand()%bd.y;
            for (int p=0; p<bd.mines.size()-1; p++){//check for existing mines
                if (bd.mines.at(p).x==temp.x && bd.mines.at(p).y==temp.y){
                    cont=true;
                    break;//stop if there is already a bomb there
                }else{
                    cont = false;//set cont to false to break the while loop
                }
            }
        }
        bd.mines.at(i)=temp;
    }
}


void checkR(int rx[], int ry[], coord s, board bdCon){//determin the ranges for finding the mines
    //reset ranges
    rx[0]=-1;
    rx[1]=2;
    ry[0]=-1;
    ry[1]=2;
    //deside to change ranges
    if(s.x==0){
        rx[0]=0;
    }else if(s.x==bdCon.x-1){
        rx[1]=1;
    }else{
        rx[0]=-1;
        rx[1]=2;
    }
    if(s.y==0){
        ry[0]=0;
    }else if(s.y==bdCon.y){
        ry[1]=1;
    }else{
        ry[0]=-1;
        ry[1]=2;
    }
}


void boardGen(board bdCon, vector<vector<int>>& bd, vector<vector<char>> &bdDisp){
    /* 0 means empty space
     * 9 means bomb
     * nums 1-8 mean the num of bombs in the proximity
    */
    for(int i=0; i<bdCon.y-1; i++){//set the bombs on the board
        for(int p=0; p<bdCon.x-1; p++){
            for(int q=0; q<bdCon.mines.size()-1; q++){
                if (bdCon.mines.at(q).x==p && bdCon.mines.at(q).y ==i){
                    bd[p][i]=9;
                }
            }
        }
    }
    //Create the numbers on the board...............................
    for(int i=0; i<bdCon.y; i++){       //poi y
        for(int p=0; p<bdCon.x; p++){   //poi x
            int asNum=0;
            int rx[2];                  //range of search---
            int ry[2];
            coord s;
            s.x=p;
            s.y=i;
            checkR(rx, ry, s, bdCon);   //set the search ranges
            //check surrounding area................................
            if(bd[p][i]!=9){
                for(int j=ry[0]; j<ry[1]; j++){
                    for(int k=rx[0]; k<rx[1]; k++){
                        if(bd[p+k][i+j]==9){
                            asNum++;
                        }
                    }
                }
                bd[p][i]=asNum;
            }
        }
    }
    for(int i=0; i<bdCon.y; i++){       //sets all spaces of display board to the blank char
        for(int p=0; p<bdCon.x; p++){
            bdDisp[p][i]=blank;
        }
        cout<<endl;
    }
}


//Display functions--------------------------


void disp(board bdCon, vector<vector<char>> &bdDisp){//displays the gameBoard
    cout<<"  ";
    for(int i=0; i<bdCon.y+2; i++){     //loop through y vals
        for(int p=0; p<bdCon.x+1; p++){ //loop through x vals
            if (i==0){
                cout<<p<<" ";
            }else if(i==1){
                cout<<"__";             //top of board > add line
            }else{
                if(p==0){               //edge of board > add line
                    if(i>10){
                        cout<<i-1<<"|";
                    }else{
                        cout<<i-1<<" |";
                    }
                }else{
                    cout<<" "<<bdDisp[p-1][i-2];
                }
            }
        }
        cout<<endl;
    }
}


//end game stuff---------------------------------------

bool gameover(bool win){
    char ag;
    bool again;
    if(win){
        cout<<"\n\n---------------------------You won!---------------------------"<<endl<<endl;
    }else{
        cout<<"\n\n---------------------------You lost!---------------------------"<<endl<<endl;
    }
    cout<<"Do you want to play again(y/n): ";
    while(true){
        cin>>ag;
        if(ag=='y'){
            again=true;
            break;
        }else if(ag=='n'){
            again=false;
            break;
        }else{
            cout<<"Error, please enter y or n: ";
        }
    }
    return again;
}


void dispBombs(vector<vector<char>> &bdDisp, vector<vector<int>> &bd, board& bdCon){
    for(int i=0; i<bdCon.y; i++){//go through all the mine locations and set the display for each coord
        for(int p=0; p<bdCon.x; p++){
            if(bd[p][i]==9){
                bdDisp[p][i]=bomb;
            }
        }
    }
}


void reset(vector<vector<int>> &bd, vector<vector<char>> &bdDisp, board& bdCon){//funtion to reset the bd and the bdDisp
    for(int i=0; i<bdCon.y; i++){//reset every coord
        for(int p=0; p<bdCon.x; p++){
            bdDisp[p][i]=blank;
        }
    }
    for(int i=0; i<bdCon.y; i++){//reset every coord
        for(int p=0; p<bdCon.x; p++){
            bd[p][i]=0;
        }
    }
}


//Action functions------------------------------------------------------------------

void guess(coord& g, board bdCon){//enter your guess coords
    cout<<endl<<"Enter the x of your guess: ";
    while(true){//enter x
        cin>>g.x;
        if(g.x>0 and g.x<bdCon.x+1){
            g.x--;
            break;
        }else{
            cout<<endl<<"Error, enter a number between 1 and "<<bdCon.x<<": ";
        }
    }
    cout<<"Enter the y of your guess: ";
    while(true){//enter y
        cin>>g.y;
        if(g.y>0 and g.y<bdCon.y+1){
            g.y--;
            break;
        }else{
            cout<<endl<<"Error, enter a number between 1 and "<<bdCon.y<<": ";
        }
    }
    cout<<endl<<endl<<endl;
}


void zero(coord g, vector<vector<int>> &bd, vector<vector<char>> &bdDisp, board bdCon){//function for if a zero is found
    vector<coord> found(100000000); //it just has to be huge
    int rx[2];                      //range of search(x), (y)
    int ry[2];
    int i=0;
    vector<vector<int>> used(bdCon.x, vector<int>(bdCon.y, 0));//0=not used, 1 is used
    found.at(0)=g;                  //set first 0 point to be the guess since the guess has been determined to be zero
    int run=1;
    do{
        coord inter;                //temp holder
        inter=found.at(i);
        checkR(rx, ry, inter, bdCon);
        used[inter.x][inter.y]=1;       //add current zero to used list
        for(int j=ry[0]; j<ry[1]; j++){ //search around poi
            for(int k=rx[0]; k<rx[1]; k++){
                bdDisp[inter.x+k][inter.y+j]=char(bd[inter.x+k][inter.y+j]+48);//uncover spaces next to 0's
                if(k==0 and j==0){      //dont want to count the poi again
                }else{
                    if(bd[inter.x+k][inter.y+j]==0 and used[inter.x+k][inter.y+j]==0){
                        found.at(run).x=inter.x+k;  //add new zero to list
                        found.at(run).y=inter.y+j;
                        run++;                      //adds to the total runs
                    }
                }
            }
        }
        i++;
    }while(i<run);
}


bool checkWin(vector<vector<char>> &bdDisp, board& bdCon){
    // If the number of blanks is equal to the number of bombs generated
    // then the user wins!
    int i=0;
    bool w=false;
    for(int j=0; j<bdCon.y; j++){   //counts the num of blanks
        for(int k=0; k<bdCon.x; k++){
            if(bdDisp[k][j]==blank){
                i++;
            }
        }
    }
    if(i<=bdCon.mines.size()){      //compares to num of mines
        w=true;
    }else{
        w=false;
    }
    return w;
}



void action(coord g, vector<vector<int>> &bd, vector<vector<char>> &bdDisp, board& bdCon, endVars& e){
    //hub for controling all the actions depending on what the user guesses
    if(bd[g.x][g.y]==9){//user guessed a mine location
        dispBombs(bdDisp, bd, bdCon);
        disp(bdCon, bdDisp);
        e.end=true;
        e.win=false;
    }else if(bd[g.x][g.y]==0){//user guessed a zero location
        zero(g, bd, bdDisp, bdCon);
    }else if(checkWin(bdDisp, bdCon)){//Check to see if the user has won
        e.end=true;
        e.win=true;
    }else{
        bdDisp[g.x][g.y]=char(bd[g.x][g.y]+48);//other wise just display the number
    }
}


//----------------------------------Main------------------------------------------

int main(){
    srand(time(NULL));//Seed the rand num gen
    board bdCon;//define board Constructor
    coord g;//define var for holding guess coord
    endVars e;

    e.win=false;
    e.end=false;
    init(bdCon);//ask for size and set the size
    bombGen(bdCon);//generate bomb locations and store them in the board construct

    vector<vector<int>> bd(bdCon.y, vector<int>(bdCon.x, 0));//define 2d vector for game board with size | This holds all the needed info
    vector<vector<char>> bdDisp(bdCon.y, vector<char>(bdCon.x, blank));//define 2d vector for game board with size | this is displayed

    boardGen(bdCon, bd, bdDisp);//generate the game board
    disp(bdCon, bdDisp);//display the initial game board

    while(true){
        guess(g, bdCon);
        action(g, bd, bdDisp, bdCon, e);
        if(e.end){
            if(gameover(e.win)){//reset everything............
                init(bdCon);
                bombGen(bdCon);//regenerate bomb locations
                reset(bd, bdDisp, bdCon);
                boardGen(bdCon, bd, bdDisp);
                disp(bdCon, bdDisp);
                e.end=false;
            }else{
                cout<<"\n\n\n               Thanks for playing!      "<<endl<<endl<<endl<<endl;
                break;
            }
        }else{
            disp(bdCon, bdDisp);
        }
    }
    return 0;
}
