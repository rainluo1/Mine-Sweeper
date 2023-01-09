#include <cstdlib>
#include <iostream>
#include <cctype>
#include <ctime>
//self-authored function
void reveal8(char * board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc);
char * makeBoard(std::size_t xdim, std::size_t ydim);
void calculateNeighbors(char * board, std::size_t xdim, std::size_t ydim);
void hideBoard(char * board, std::size_t xdim, std::size_t ydim);
void cleanBoard(char * board);
void printBoard(char * board, std::size_t xdim, std::size_t ydim);
int show(char * board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc);
int flag(char * board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc);
//provided functions below
int main();
bool gameComplete(char * board, std::size_t xdim, std::size_t ydim);
bool game();
bool game();
void startGame(char * & board, std::size_t & xdim, std::size_t & ydim, unsigned int & numgeese);
char getAction();
void actionReveal(char * & board, std::size_t & xdim, std::size_t & ydim, unsigned int & numgeese);
void actionflag(char * board, std::size_t xdim, std::size_t ydim);
std::size_t readSizeT();
std::size_t xdim_max();
std::size_t ydim_max();
char markedBit();
char hiddenBit();
char valueMask();
void spreadGeese(char * board, std::size_t xdim, std::size_t ydim, unsigned int numgeese);
//starte of provided code
int main()
{
  srand(time(NULL));
  game();
  return 0;
}
bool game()
{
  std::size_t xdim {0};
  std::size_t ydim {0};
  unsigned int numgeese {0};
  char * gameBoard {NULL};

  startGame(gameBoard, xdim, ydim, numgeese);

  char currAction {0};
  while(currAction != 'Q')
  {
    switch (currAction)
    {
      case 'S' :  // show
      {
        actionReveal(gameBoard, xdim, ydim, numgeese);
        break;
      }
      case 'M' :  // mark
      {
        actionflag(gameBoard, xdim, ydim);
        break;
      }
      case 'R' :  // restart
      {
        std::cout << "Restarting the game." << std::endl;
        startGame(gameBoard, xdim, ydim, numgeese);
        break;
      }
    }
    printBoard(gameBoard, xdim, ydim);
    if (gameComplete(gameBoard, xdim, ydim))
    {
      std::cout << "You have revealed all the fields without disturbing a goose!" << std::endl;
      std::cout << "YOU WON!!!" << std::endl;
      for (std::size_t reveal_row {0}; reveal_row < ydim; reveal_row++)
      {
        for (std::size_t reveal_col {0}; reveal_col < xdim; reveal_col++)
        {
          gameBoard[reveal_row*xdim + reveal_col] = (gameBoard[reveal_row*xdim + reveal_col] & valueMask());
        }
      }
      printBoard(gameBoard, xdim, ydim);
      std::cout << "Resetting the game board." << std::endl;
      startGame(gameBoard, xdim, ydim, numgeese);
      printBoard(gameBoard, xdim, ydim);
    }
    currAction = getAction();
  }

  cleanBoard(gameBoard);
  return true;
}

void startGame(char * & board, std::size_t & xdim, std::size_t & ydim, unsigned int & numgeese)
{
  std::cout << "Welcome to GeeseSpotter!" << std::endl;
  do {
    std::cout << "Please enter the x dimension (max " << xdim_max() << "): ";
    std::cin >> xdim;
  } while(xdim < 1 || xdim > xdim_max());
  do {
    std::cout << "Please enter the y dimension (max " << ydim_max() << "): ";
    std::cin >> ydim;
  } while(ydim < 1 || ydim > ydim_max());

  std::cout << "Please enter the number of geese: ";
  std::cin >> numgeese;
  while (numgeese > xdim * ydim)
  {
    std::cout << "That's too many geese!" << std::endl;
    std::cout << "Please enter the number of geese: ";
    std::cin >> numgeese;
  }

  cleanBoard(board);
  board = makeBoard(xdim, ydim);
  spreadGeese(board, xdim, ydim, numgeese);
  calculateNeighbors(board, xdim, ydim);
  hideBoard(board, xdim, ydim);
}

char getAction()
{
  char action {0};

  std::cout << "Please enter the action ([S]how, [M]ark, [R]estart, [Q]uit): ";
  std::cin >> action;

  if (islower(action))
    action = toupper(action);

  return action;
}

void actionReveal(char * & board, std::size_t & xdim, std::size_t & ydim, unsigned int & numgeese)
{
  std::size_t reveal_x {0};
  std::size_t reveal_y {0};
  std::cout << "Please enter the x location to show: ";
  std::cin >> reveal_x;
  std::cout << "Please enter the y location to show: ";
  std::cin >> reveal_y;

  if (reveal_x >= xdim || reveal_y >= ydim)
  {
    std::cout << "Location entered is not on the board." << std::endl;
  }
  else if (board[xdim*reveal_y + reveal_x] & markedBit())
  {
    std::cout << "Location is marked, and therefore cannot be revealed." << std::endl;
    std::cout << "Use Mark on location to unmark." << std::endl;
  }
  else if (show(board, xdim, ydim, reveal_x, reveal_y) == 9)
  {
    std::cout << "You disturbed a goose! Your game has ended." << std::endl;
    printBoard(board, xdim, ydim);
    std::cout << "Starting a new game." << std::endl;
    startGame(board, xdim, ydim, numgeese);
  }
}

void actionflag(char * board, std::size_t xdim, std::size_t ydim)
{
  std::size_t mark_x {0};
  std::size_t mark_y {0};
  std::cout << "Please enter the x location to mark: ";
  std::cin >> mark_x;
  std::cout << "Please enter the y location to mark: ";
  std::cin >> mark_y;

  if (mark_x >= xdim || mark_y >= ydim)
  {
    std::cout << "Location entered is not on the board." << std::endl;
  }
  else if (flag(board, xdim, ydim, mark_x, mark_y) == 2)
  {
    std::cout << "Position already revealed, so cannot be marked." << std::endl;
  }
}

std::size_t readSizeT()
{
  return 0;
}

std::size_t xdim_max()
{
  return 60;
}

std::size_t ydim_max()
{
  return 20;
}

char markedBit()
{
  return 0x10;
}

char hiddenBit()
{
  return 0x20;
}

char valueMask()
{
  return 0x0F;
}

void spreadGeese(char * board, std::size_t xdim, std::size_t ydim, unsigned int numgeese)
{
  if (board != NULL)
  {
    for (unsigned int gen_goose {0}; gen_goose < numgeese; gen_goose++)
    {
      std::size_t try_position {0};
      do {
         try_position = rand() % (xdim * ydim);
      } while (board[try_position] != 0);

      board[try_position] = 9;
    }
  }
}
//End of provided code

//begin of my own code
char * makeBoard(std::size_t xdim, std::size_t ydim){
    char *arr{new char [xdim*ydim]{0}};
    return arr;
}

void hideBoard(char * board, std::size_t xdim, std::size_t ydim){ // something is wrong here
    for(int i=0;i<xdim*ydim;i++){
        board[i] = board[i]|hiddenBit(); //'*'
    }
}
//something is wrong here

//To deallocate the given board
void cleanBoard(char * board){
    delete [] board;
    board = nullptr;
}

void printBoard(char * board, std::size_t xdim, std::size_t ydim){
    for(int i=0;i<xdim*ydim;i++){ // 1 meaning marked 
        if((board[i]&hiddenBit()) == hiddenBit()){ // 1 meaning hidden
            if(board[i]&markedBit()){
            std::cout<<'M';
            }
            else
                std::cout<<'*';
        }
        else{
            int ans = 0;
            int mask{1};
            int curNum =1;
            for(int c=0;c<4;c++){
                if(board[i]&mask){
                ans+=curNum;
                }
            mask<<=1;
            curNum= curNum*2;
            }
            std::cout<<ans;
        }
        if((i+1)%xdim==0){
            std::cout<<""<<std::endl;
        }
    }
}
//marks a field if revealed 
int flag(char * board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc){
    int idx = yloc*xdim + xloc;
    bool hide = board[idx]&hiddenBit();
    if((board[idx]^markedBit()) && hide){
        board[idx] = (board[idx]^markedBit()); // change 5th bit to 0
        return 0;
    }
    return 2;// return value of 2 
}    
//compute the amount of goose 
void calculateNeighbors(char * board, std::size_t xdim, std::size_t ydim){
    for(std::size_t i=0;i<xdim*ydim;i++){
        int result{0};
        int xCord = i%xdim;
        int yCord = (i-xCord)/xdim;
        //trying to tackle edge case
        if((board[i]&valueMask())!=9){//if it is not goose, set their numebr
            if(xCord>0 && yCord>0){
                if(board[i-xdim-1]==9) 
                    result++;
            }
            if(yCord>0){
                if(board[i-xdim]==9) 
                    result++;
            }
            if(xCord+1<xdim &&yCord>0){
                if(board[i-xdim+1]==9) 
                    result++;
            }
            if(xCord>0){
                if(board[i-1]==9) 
                    result++;
            }
            if(xCord+1<xdim){
                if(board[i+1]==9) 
                    result++;
            }
            if(xCord >0 && yCord+1<ydim){ 
                if(board[i+xdim-1]==9) 
                    result++;
            }
            if(yCord+1<ydim){
                if(board[i+xdim]==9) 
                    result++;
            }
            if(xCord +1 < xdim && yCord+1<ydim){
                if(board[i+xdim+1]==9) 
                    result++;
            }
            board[i] = result; // change the result of the board
        }
    }
}

bool gameComplete(char * board, std::size_t xdim, std::size_t ydim){
    for(std::size_t i =0;i<xdim*ydim;i++){
        if((board[i]&valueMask())!=9){ // if not goose -> check if it has been revealed 
            if(board[i]&hiddenBit()){ // if still hiding return false
                return false;
            }
            if(board[i]&markedBit()){ //if marked a non-goose return false
                return false;
            }
        }
    }
    return true;
}
//if marked return 1 or already revealled return 2 if geese return 9 
int show(char * board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc){
   // if number amount of geese is 0 -> show 8 surrrounding. 
    int idx = xdim*yloc+ xloc;
    if(board[idx]&markedBit()){
        return 1;
    }
    if(!(board[idx]&hiddenBit())){
        return 2;
    }
    if(((board[idx]&valueMask())==9)){
        board[idx]-=32;
        return 9;
    }
    if(board[idx]&hiddenBit()){
        board[idx]= board[idx]^hiddenBit(); //produce 0
        //account for 0 surrounding geese 
        if(board[idx]==0){
            int i = idx;
            int xCord = i%ydim;
            int yCord = (i-xCord)/ydim;
            if(xCord >0 && yCord>0){ //up left
                if((board[idx - xdim -1]&valueMask()) == 0){
                    reveal8(board, xdim, ydim, xloc-1,yloc-1);
                }
            }
            if(yCord>0){ //up
                
                if((board[idx - xdim]&valueMask()) == 0){
                    reveal8(board, xdim, ydim, xloc,yloc-1);
                }
            }
            if(yCord>0 && xCord+1<xdim){ //up right
                
                if((board[idx - xdim+1]&valueMask()) == 0){
                    reveal8(board, xdim, ydim, xloc+1,yloc-1);
                }
            }
            if(xCord >0){ //left
                
                if((board[idx - 1]&valueMask()) == 0){
                    reveal8(board, xdim, ydim, xloc-1,yloc);
                }
            } 
            if(xCord+1 <xdim){ //riht
                
                if((board[idx + 1]&valueMask()) == 0){
                    reveal8(board, xdim, ydim, xloc+1,yloc);
                }
            }
            if(xCord>0 && yCord+1<ydim){ //down left
                
                if((board[idx +xdim - 1]&valueMask()) == 0){
                    reveal8(board, xdim, ydim, xloc-1,yloc+1);
                }
            }
            if(yCord+1<ydim){ //down
                
                if((board[idx +xdim]&valueMask()) == 0){
                    reveal8(board, xdim, ydim, xloc,yloc+1);
                }
            }
            if(xCord+1<xdim && yCord+1<ydim){ //down left
                if((board[idx +xdim + 1]&valueMask()) == 0){
                    reveal8(board, xdim, ydim, xloc+1,yloc+1);
                }
            }
        }
        return 0;
    }
    return 9090;
 }
 void reveal8(char * board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc){
    int idx = xdim*yloc+ xloc;
    if(!(board[idx]&hiddenBit())) return; //if it is not hidden, return.
    board[idx] = board[idx]^hiddenBit();
    if((board[idx]&valueMask())==0 ){
            int i = idx;
            int xCord = i%ydim;
            int yCord = (i-xCord)/ydim;
            if(xCord >0 && yCord>0){ //up left
                if(board[idx - xdim-1]&hiddenBit())
                    if((board[idx - xdim-1]&valueMask()) == 0){
                        reveal8(board, xdim, ydim, xloc-1,yloc-1);
                    }
            }
            if(yCord>0){ //up
                if(board[idx - xdim]&hiddenBit()){
                    if((board[idx - xdim]&valueMask()) == 0)
                        reveal8(board, xdim, ydim, xloc,yloc-1);
                }
            }
            if(yCord>0 && xCord+1<xdim){ //up right
                if(board[idx - xdim+1]&hiddenBit()){
                    if((board[idx - xdim+1]&valueMask()) == 0)
                        reveal8(board, xdim, ydim, xloc+1,yloc-1);
                }
            }
            if(xCord >0){ //left
                if(board[idx -1]&hiddenBit()){
                    if((board[idx -1]&valueMask()) == 0)
                        reveal8(board, xdim, ydim, xloc-1,yloc);
                }
            } 
            if(xCord+1 <xdim){ //right
                if(board[idx+1]&hiddenBit()){
                    if((board[idx +1]&valueMask()) == 0)
                        reveal8(board, xdim, ydim, xloc+1,yloc);
                }
            }
            if(xCord>0 && yCord+1<ydim){ //down left
                if(board[idx + xdim-1]&hiddenBit()){
                    if((board[idx + xdim-1]&valueMask()) == 0)
                        reveal8(board, xdim, ydim, xloc-1,yloc+1);
                }
            }
            if(yCord+1<ydim){ //down
                if(board[idx + xdim]&hiddenBit()){
                    if((board[idx +xdim]&valueMask()) == 0)
                        reveal8(board, xdim, ydim, xloc,yloc+1);
                }
            }
            if(xCord+1<xdim && yCord+1<ydim){ //down left
                if(board[idx + xdim+1]&hiddenBit()){
                    if((board[idx + xdim+1]&valueMask()) == 0)
                        reveal8(board, xdim, ydim, xloc+1,yloc+1);
                }
            }
        }
 }