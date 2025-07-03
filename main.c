/// Importing the needed libraries :
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>





/// True stands for 1 and false stands for 0.
#define true 1
#define false 0

/// In this section we are going to set the color of the text in the consol for WINDOWS for a better visibility.
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void setColor(int textColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD newAttributes;

    if (GetConsoleScreenBufferInfo(hConsole, &consoleInfo)) {
        newAttributes = (consoleInfo.wAttributes & 0xF0) | (textColor & 0x0F);
        SetConsoleTextAttribute(hConsole, newAttributes);
    }
}
void setColors(int textColor, int backgroundColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD attributes = (backgroundColor << 4) | (textColor & 0x0F);
    SetConsoleTextAttribute(hConsole, attributes);
}























/// This array contains the two coordinates of the available boxes in which the pieces can be  placed or moved to.
int availableBoxes[][2] = {
    {0, 0}, {0, 8}, {0, 16},
    {7, 3}, {7, 8}, {7, 13},
    {14, 6}, {14, 8}, {14, 10},
    {21, 0}, {21, 3}, {21, 6}, {21, 10}, {21, 13}, {21, 16},
    {28, 6}, {28, 8}, {28, 10},
    {35, 3}, {35, 8}, {35, 13},
    {42, 0}, {42, 8}, {42, 16}
};

/// numavailableBoxes stands for the number of the available boxes which we need when you will be using different loops.
int numavailableBoxes = sizeof(availableBoxes) / sizeof(availableBoxes[0]);

/// This array contains the coordinates of every 3 pieces that might form a mill.
int possibleMills[16][3][2] = {
    {{0, 0}, {0, 8}, {0, 16}}, // mill line 0
    {{7, 3}, {7, 8}, {7, 13}}, // mill line 7
    {{14, 6}, {14, 8}, {14, 10}}, // mill line 14
    {{21, 0}, {21, 3}, {21, 6}}, // mill line 21
    {{21, 10}, {21, 13}, {21, 16}}, // mill line 21 (second part)
    {{28, 6}, {28, 8}, {28, 10}}, // mill line 28
    {{35, 3}, {35, 8}, {35, 13}}, // mill line 35
    {{42, 0}, {42, 8}, {42, 16}}, // mill line 42
    {{0, 0}, {21, 0}, {42, 0}}, // mill column 0
    {{0, 8}, {7, 8}, {14, 8}}, // mill column 3
    {{28, 8}, {35, 8}, {42, 8}}, // mill column 6
    {{7, 3}, {21, 3}, {35, 3}}, // Mmill column 8
    {{14, 6}, {21, 6}, {28, 6}}, // mill column 8 (second part)
    {{14, 10}, {21, 10}, {28, 10}}, // mill column 10
    {{7, 13}, {21, 13}, {35, 13}}, // mill column 13
    {{0, 16}, {21, 16}, {42, 16}}  // mill column 16
};
/// Our matrix that represents the board the action will take place on.
char Board[43][17]={
        {'0', '-', '-', '-', '-', '-', '-', '-', '0', '-', '-', '-', '-', '-', '-', '-', '0'},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
        {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
        {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
        {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
        {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
        {'|', ' ', ' ', '0', '-', '-', '-', '-', '0', '-', '-', '-', '-', '0', ' ', ' ', '|'},
        {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
        {'|', ' ', ' ', '|', ' ', ' ', ' ', ' ', '|', ' ', ' ', ' ', ' ', '|', ' ', ' ', '|'},
        {'|', ' ', ' ', '|', ' ', ' ', ' ', ' ', '|', ' ', ' ', ' ', ' ', '|', ' ', ' ', '|'},
        {'|', ' ', ' ', '|', ' ', ' ', ' ', ' ', '|', ' ', ' ', ' ', ' ', '|', ' ', ' ', '|'},
        {'|', ' ', ' ', '|', ' ', ' ', ' ', ' ', '|', ' ', ' ', ' ', ' ', '|', ' ', ' ', '|'},
        {'|', ' ', ' ', '|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|', ' ', ' ', '|'},
        {'|', ' ', ' ', '|', ' ', ' ', '0', '-', '0', '-', '0', ' ', ' ', '|', ' ', ' ', '|'},
        {'|', ' ', ' ', '|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|', ' ', ' ', '|'},
        {'|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|'},
        {'|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|'},
        {'|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|'},
        {'|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|'},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'0', '-', '-', '0', '-', '-', '0', ' ', ' ', ' ', '0', '-', '-', '0', '-', '-', '0'},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|'},
        {'|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|'},
        {'|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|'},
        {'|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|'},
        {'|', ' ', ' ', '|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|', ' ', ' ', '|'},
        {'|', ' ', ' ', '|', ' ', ' ', '0', '-', '0', '-', '0', ' ', ' ', '|', ' ', ' ', '|'},
        {'|', ' ', ' ', '|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|', ' ', ' ', '|'},
        {'|', ' ', ' ', '|', ' ', ' ', ' ', ' ', '|', ' ', ' ', ' ', ' ', '|', ' ', ' ', '|'},
        {'|', ' ', ' ', '|', ' ', ' ', ' ', ' ', '|', ' ', ' ', ' ', ' ', '|', ' ', ' ', '|'},
        {'|', ' ', ' ', '|', ' ', ' ', ' ', ' ', '|', ' ', ' ', ' ', ' ', '|', ' ', ' ', '|'},
        {'|', ' ', ' ', '|', ' ', ' ', ' ', ' ', '|', ' ', ' ', ' ', ' ', '|', ' ', ' ', '|'},
        {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
        {'|', ' ', ' ', '0', '-', '-', '-', '-', '0', '-', '-', '-', '-', '0', ' ', ' ', '|'},
        {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
        {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
        {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
        {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
        {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'0', '-', '-', '-', '-', '-', '-', '-', '0', '-', '-', '-', '-', '-', '-', '-', '0'}
    };

/// This function checks if a piece is protacted by a mill formed in a line.
int isprotectedline(int i, int j,char Board[][17]){
    if (i<0 || i>42 || j<0 || j>16 || Board[i][j]=='0')
        return 0;
    if ((i==0) && (j==0 || j==8 || j==16)){
        if (Board[i][j]==Board[0][0] && Board[0][8]==Board[0][16] && Board[0][0]==Board[0][8])
            return 1;
            else
        return 0;
    }
    if ((i==7) && (j==3 || j==8 || j==13)){
        if (Board[i][j]==Board[7][3] && Board[7][8]==Board[7][13] && Board[7][3]==Board[7][8])
            return 1;
            else
        return 0;
    }
    if ((i==14) && (j==6 || j==8 || j==10)){
        if (Board[i][j]==Board[14][6] && Board[14][8]==Board[14][10] && Board[14][8]==Board[14][6])
            return 1;
            else
        return 0;
    }
    if ((i==21) && (j==0 || j==3 || j==6)){
        if (Board[i][j]==Board[21][0] && Board[21][3]==Board[21][6] && Board[21][3]==Board[21][0])
            return 1;
            else
        return 0;
    }
    if ((i==21) && (j==10 || j==13 || j==16)){
        if (Board[i][j]==Board[21][10] && Board[21][13]==Board[21][16] && Board[21][10]==Board[21][13])
            return 1;
            else
        return 0;
    }
    if ((i==28) && (j==6 || j==8 || j==10)){
        if (Board[i][j]==Board[28][6] && Board[28][8]==Board[28][10] && Board[28][6]==Board[28][8])
            return 1;
            else
        return 0;
    }
    if ((i==35) && (j==3 || j==8 || j==13)){
        if (Board[i][j]==Board[35][3] && Board[35][8]==Board[35][13] && Board[35][13]==Board[35][8])
            return 1;
            else
        return 0;
    }
    if ((i==42) && (j==0 || j==8 || j==16)){
        if (Board[i][j]==Board[42][0] && Board[42][8]==Board[42][16] && Board[42][0]==Board[42][8])
            return 1;
            else
        return 0;
    }
    else
        return 0;
}

/// This function ckecks if a piece is protacted by a mill formed in a column.
int isprotectedcol(int i, int j,char Board[][17]){
    if (i<0 || i>42 || j<0 || j>16 || Board[i][j]=='0')
        return 0;
    if ((j==0) && (i==0 || i==21 || i==42)){
        if (Board[i][j]==Board[0][0] && Board[21][0]==Board[42][0] && Board[21][0]==Board[0][0])
            return 1;
            else
        return 0;
    }
    if ((j==3) && (i==7 || i==21 || i==35)){
        if (Board[i][j]==Board[7][3] && Board[21][3]==Board[35][3] && Board[7][3]==Board[21][3])
            return 1;
            else
        return 0;
    }
    if ((j==6) && (i==14 || i==21 || i==35)){
        if (Board[i][j]==Board[14][6] && Board[21][6]==Board[35][6] && Board[14][6]==Board[21][6])
            return 1;
            else
        return 0;
    }
    if ((j==8) && (i==0 || i==7 || i==14)){
        if (Board[i][j]==Board[0][8] && Board[7][8] == Board[14][8] && Board[7][8]==Board[0][8])
            return 1;
            else
        return 0;
    }
    if ((j==8) && (i==42 || i==35 || i==28)){
        if (Board[i][j]==Board[35][8] && Board[28][8] == Board[42][8] && Board[42][8]==Board[35][8])
            return 1;
            else
        return 0;
    }
    if ((j==10) && (i==21 || i==14 || i==28)){
        if (Board[i][j]==Board[21][10] && Board[14][10] == Board[28][10] && Board[21][10]==Board[28][10])
            return 1;
            else
        return 0;
    }
    if ((j==13) && (i==7 || i==21 || i==35)){
        if (Board[i][j]==Board[7][13] && Board[21][13] == Board[35][13] && Board[35][13]==Board[7][13])
            return 1;
            else
        return 0;
    }
    if ((j==16) && (i==0 || i==21 || i==42)){
        if (Board[i][j]==Board[0][16] && Board[21][16]==Board[42][16] && Board[0][16]==Board[21][16])
           return 1;
           else
        return 0;
    }

    else
        return 0;
}

/// This function checks if the piece can be removed or not.
int IsValidRemove(int i, int j, char Board[][17], int currentPlayer) {
    if (i < 0 || i > 42 || j < 0 || j >= 17) {
        printf("\t\tOUT RANGE\n");
        return 0;
    }
    if (Board[i][j] != '1' && Board[i][j] != '2'){
        printf("\t\EMPTY BOX OR AVAILABLE CHAR\n");
        return 0;
    }
    if (Board[i][j] == '1' && currentPlayer == 1) {
        printf("\t\tYOU CAN'T TAKE OUT YOUR OWN PIECE\n");
        return 0;
    }
    if (Board[i][j] == '2' && currentPlayer == 2) {
        printf("\t\tYOU CAN'T TAKE OUT YOUR OWN PIECE \n");
        return 0;
    }
    if (isprotectedline(i, j, Board)) {
        printf("\t\tTHE PIECE IS PROTECTED BY A MILL( line )\n");
        return 0;
    }
    if (isprotectedcol(i, j, Board)) {
        printf("\t\tTHE PIECE IS PROTECTED BY A MILL( col )\n");
        return 0;
    }
    else
        return 1;
}

/// This function removes the piece.
void takeout(int i,int j,char Board[][17],int currentPlayer){
    if ( i >= 0 && i <= 42 && j >= 0 && j <= 16 )
        Board[i][j]='0';
}

/// This function places a new piece on the board on the given coordinates by the player.
void PlacePiece(int currentPlayer, int i, int j, char Board[][17]) {
    // Check if the move is valid
    if (TheMoveIsValid(i, j, Board) == 1) {
        // Place the piece based on the current player
        if (currentPlayer == 1) {
            Board[i][j] = '1';  // Player 1 places their piece
        } else {
            Board[i][j] = '2';  // Player 2 places their piece
        }
    } else {
        printf("Invalid move.\n");
    }
}

/// This function checks weither the box is empty and the coordinates are available so that the piece can be placed in or moved to.
int TheMoveIsValid( int i, int j, char Board[][17]){
    // verifying if the inderowes are valid, if they are we next verify if the box is empty (0)
    if ( i >= 0 && i <= 42 && j >= 0 && j <= 16 && Board[i][j]=='0')
        return 1;
    else return 0;
}

/// This function shows the available boxes to the current player.
void showavailibleboxes(){
        printf("    [0][0],[0][8],[0][16]\n");
        printf("    [7][3],[7][8],[7][13]\n");
        printf("    [14][6],[14][8],[14][10]\n");
        printf("    [21][0],[21][3],[21][6],[21][10],[21][13],[21][16]\n");
        printf("    [28][6],[28][8],[28][10]\n");
        printf("    [35][3],[35][8],[35][13]\n");
        printf("    [42][0],[42][8],[42][16]\n");
}

/// This function chooses randomly the first player to place a piece.
int Firstmovemaker(){
    srand(time(NULL));
    int randomChoice = (rand() % 2) + 1;
    if (randomChoice==1){
        printf("\t\tTHE_PLAYER_1 WILL MAKE THE FIRTS MOVE ACCORDING TO THE COIN TOSS :))");
        getch();
    }
    else{
        printf("\t\tTHE_PLAYER_2 WILL MAKE THE FIRTS MOVE ACCORDING TO THE COIN TOSS :))");
        getch();
    }
    return randomChoice;
}

/// This function displays the board and we will be calling it after every single turn.
void Display(char Board[][17]) {
    printf("\n");
    char *horizontale="-----";
    char *vertical="  |  ";
    char *box0="  0  ";
    char *box1="  1  ";
    char *box2="  2  ";
    char *space="     ";
    setColors(15,0);
    printf("\t\t\t\t    0             3              6          8        10             13             16\n\n");
    for (int i = 0; i < 43; i++) {
        setColors(15,0);
        printf("\t\t\t\t");
        if(i==0||i==7){
        setColors(15,0);
           printf(" %d",i);
           setColors(15,0);
        }
        if(i==14 || i==21 || i==28 || i==35 || i==42){
        setColors(15,0);
        printf("%d",i);
        setColors(15,0);
        }
        if(i!=0 && i!=7 && i!=14 && i!=21 && i!=28 && i!=35 && i!=42)
          printf("  ");
        for (int j = 0; j < 17; j++) {
            switch (Board[i][j]){
        case '-':
            setColors(15,0);
            printf("%s",horizontale);
            break;
        case '|':
            setColors(15,0);
            printf("%s",vertical);
            break;
        case '0':
            setColors(3,0);
            printf("%s",box0);
            break;
        case '1':
            setColors(4,0);
            printf("%s",box1);
            break;
        case '2':
            setColors(10,0);
            printf("%s",box2);
            break;
        case ' ':
            setColors(15,0);
            printf("%s",space);
            break;
            }
        }
        setColors(15,0);
        printf("\n");

    }
    printf("\n");
}

/// This function is used to check if all the pieces are protected by a mill
int allprotected(int player, char Board[][17]){
    for (int i=0;i<43;i++){
        for (int j=0;j<17;j++){
            if (Board[i][j]=='0'+player && (!isprotectedline(i,j,Board) || !isprotectedcol(i,j,Board)))
               return 0;
        }
    }
    return 1;
}




////////////////////////////////////////////////////////////////////////////////////   PLAYER______PLAYER   /////////////////////////////////////////////////////////////////////////////////////







/// This function takes care of the PLAYER_PLAYER section, it takes into consideration the different rules and functions.
void Player_Player() {
    printf("\t\tHERE IS THE BOARD THE ACTION WILL TAKE PLACE ON \n");
    Display(Board);
    getch();
    printf("\t\tONE OF YOU GUYS WILL BE NAMED THE_PLAYER_1 AND THE OTHER ONE THE_PLAYER_2.\n");
    getch();
    printf("\t\tTHE PLAYER THAT WILL MAKE THE FIRST MOVE IS GOING TO BE CHOSEN RANDOMLY. \n");
    getch();

    // The player who gets things running.
    int currentPlayer = Firstmovemaker();

    // Data
    int i, j, a, b;
    int c = 0;
    int list[3] = {0, 0, 0};
    int millformed[16][3][2];
    memset(millformed, -1, sizeof(millformed));
    printf("\t\tWE WOULD LIKE TO INFORM YOU THAT THE AVAILABLE BOXES HAVE THE FOLLOWING INDEXES :\n");
    showavailibleboxes();
    // THE PLACING PHASE (once the two players have placed 9 pieces each).
    while (c < 18) {
        printf("\t\tTHE_PLAYER_%d, CHOOSE THE BOX YOU WANNA PLACE YOUR PIECE IN ( ROW AND COLON ) : ", currentPlayer);
        int validinput = 0;
        while (!validinput) {
            scanf("%d %d", &i, &j);   // The player chooses the coordinates (i and j).
            if (TheMoveIsValid(i, j, Board)) {
                PlacePiece(currentPlayer, i, j, Board);
                c++;
                Display(Board);
                validinput = 1;
            } else {
                printf("\t\tINVALID MOVE, TRY AGAIN.\n\n");
            }
        }

        // After placing 5 pieces on the board we start checking if a player has formed a mill.
        if (c > 4) {
            int list[3] = {0, 0, 0};
            int a=(currentPlayer==1)?2:1;
            millVerifier(list, Board, currentPlayer, millformed,possibleMills);
            if (list[currentPlayer] > 0 && !allprotected(a,Board)) { // Indeed the player has formed a new mill.
                printf("\t\tTHE_PLAYER_%d, YOU HAVE FORMED A MILL! CHOOSE A PIECE OF YOUR CHALLENGER TO REMOVE : ", currentPlayer);
                int validRemove = 0;
                while (!validRemove) {
                    scanf("%d %d", &a, &b);
                    if (IsValidRemove(a, b, Board, currentPlayer)) {
                        takeout(a, b, Board, currentPlayer); // The piece has been taken out.
                        printf("\n\t\tBOARD AFTER REMOVING THE PIECE :\n\n");
                        Display(Board);
                        validRemove = 1;
                    } else {
                        printf("\t\tINVALID OPTION, CHOOSE AGAIN.\n");// If the piece in question can not be removed, the player is supposed to try again.
                    }
                }
            }
        }
        currentPlayer = (currentPlayer == 1) ? 2 : 1; // After every single turn we the change the player.
    }
    printf("\t\tYOU GUYS ARE DONE WITH PLACING THE NINE PIECES\n\n");// The placing phase just finished.

/// THE MOVING PHASE.
    while (!gameover(currentPlayer, Board)) { // While the game has not finished yet, we repeat :
            if (countpieces(currentPlayer,Board)==3){
            printf("THE_PLAYER_%d YOU ONLY GOT 3 PIECES LEFT YOU ARE FREE TO JUMP\n", currentPlayer);
                int startrow, startcol, endrow, endcol;
                int validfirst = 0;
                printf("\t\tENTER THE COORDINATES OF THE PIECE YOU WANT TO JUMP (ROW COL): \n"); // the piece to jump
                while (!validfirst) {
                    scanf("%d %d", &startrow, &startcol);
                    if (validecoordinatesstart(startrow, startcol, Board, currentPlayer)) {
                        printf("\t\tENTER THE COORDINATES OF THE TARGET POSITION (ROW COL): \n"); // the target box
                        int validsecond = 0;
                        while (!validsecond) {
                            scanf("%d %d", &endrow, &endcol);
                            if (Board[endrow][endcol]=='0') { // the only condition so that the jump happens is that the target box is empty.
                                    Jumppiece(currentPlayer, startrow, startcol, endrow, endcol, Board);
                                    printf("\t\tTHE BOARD AFTER JUMPING THE PIECE:\n");
                                    Display(Board);
                                    int list[3] = {0, 0, 0};
                                    millVerifier(list, Board, currentPlayer, millformed, possibleMills); // we check for new mills.
                                    validsecond = 1;
                                    int a=(currentPlayer==1)?2:1;
                                    if (list[currentPlayer] > 0 && !allprotected(a,Board)) { // a new mill formed.
                                        int list1[3]={0,0,0};
                                        millVerifier(list1, Board,a,millformed,possibleMills);
                                        if ( countpieces(a,Board)==3 && list1[a]>0 ){ // but the challenger has only 3 pieces left and they form a mill.
                                            printf("\t\tPLAYER_%d, YOU HAVE FORMED A MILL! BUT UNFORTUNATELLY YOU CAN NOI TAKE A PIECE OF THE MACHINE OUT BECAUSE IT ONLY GOT 3 LEFT AND THEY FORM A MILL");
                                            break;
                                        }else{ // if not
                                        printf("\t\tPLAYER_%d, YOU HAVE FORMED A MILL! CHOOSE A PIECE OF YOUR CHALLENGER TO REMOVE: ", currentPlayer);
                                        int validRemove = 0;
                                        while (!validRemove) {
                                            scanf("%d %d", &i, &j);
                                            if (IsValidRemove(i, j, Board, currentPlayer)) {
                                                takeout(i, j, Board, currentPlayer);
                                                printf("\n\t\tBOARD AFTER REMOVING THE PIECE:\n\n");
                                                Display(Board);
                                                validRemove = 1;
                                            } else {
                                                printf("\t\tINVALID OPTION, CHOOSE AGAIN.\n");
                                            }
                                        }
                                    }

                            }
                            } else {
                                printf("\t\tINVALID OPTION, CHOOSE AGAIN.\n");
                            }
                        }
                        validfirst = 1;
                    } else {
                        printf("\t\tINVALID OPTION, CHOOSE AGAIN.\n");
                    }
            }
            }
    else {
        printf("\t\tPLAYER_%d, IT'S YOUR TURN TO MOVE.\n", currentPlayer);
        int startrow, startcol, endrow, endcol;
        int validfirst = 0;
        printf("\t\tENTER THE COORDINATES OF THE PIECE YOU WANT TO MOVE (ROW COL): \n"); // The player enters the coordinates of the piece to move.
        while (!validfirst) {
            scanf("%d %d", &startrow, &startcol);
            if (validecoordinatesstart(startrow, startcol, Board, currentPlayer)) {
                printf("\t\tENTER THE COORDINATES OF THE TARGET POSITION (ROW COL): \n"); // The player enters the coordinates of the target box.
                int validsecond = 0;
                while (!validsecond) {
                    scanf("%d %d", &endrow, &endcol);
                    if (validecoordinatesend(endrow, endcol, Board)) {
                        if (Jumppieceavailable(currentPlayer, startrow, startcol, endrow, endcol, Board)) { // we check weither the piece can be moved to the target box or not.
                            Jumppiece(currentPlayer, startrow, startcol, endrow, endcol, Board); // if yes, then we do the moving.
                            printf("\t\t THE BOARD AFTER JUMPING THE PIECE : \n");
                            Display(Board);
                            int list[3]={0,0,0};
                            millVerifier(list, Board, currentPlayer, millformed,possibleMills); // we check weither a new mill has been formed.
                            validsecond = 1;
                            int a=(currentPlayer==1)?2:1;
                            if (list[currentPlayer] > 0 && !allprotected(a,Board)) { // a new mill formed.
                                int list1[3]={0,0,0};
                                millVerifier(list1, Board,a,millformed,possibleMills);
                                if ( countpieces(a,Board)==3 && list1[a]>0 ){ // in the moving phase, we can not take out a piece of a player who only has 3 left and they form a mill.
                                    currentPlayer= (currentPlayer==1)?2:1;
                                    printf("        YOU CAN NOT TAKE OUT ANY PIECE OF YOU CHALLENGER BECAUSE HE ONLY GOT 3 LEFT AND THEY ARE ALL A PART OF A MILL.\n");
                                    break;
                                }
                              else{
                                printf("\t\tTHE_PLAYER_%d, YOU HAVE FORMED A MILL! CHOOSE A PIECE OF YOUR CHALLENGER TO REMOVE : ", currentPlayer);
                                int validRemove = 0;
                                while (!validRemove) {
                                    scanf("%d %d", &i, &j);
                                    if (IsValidRemove(i, j, Board, currentPlayer)) {
                                        takeout(i, j, Board, currentPlayer); // the piece has been taken out.
                                        printf("\n\t\tBOARD AFTER REMOVING THE PIECE :\n\n");
                                        Display(Board);
                                        validRemove = 1;
                                    } else {
                                        printf("\t\tINVALID OPTION, CHOOSE AGAIN.\n"); // choose another piece to take out.
                                    }
                                }
                            }
                            }
                        }
                    }
                    else {
                        printf("\t\tINVALID OPTION, CHOOSE AGAIN.\n"); // choose an available target.
                    }
                }
                validfirst = 1;
            } else {
                printf("\t\tINVALID OPTION, CHOOSE AGAIN.\n"); // choose a different piece to move.
            }
        }
    }
    currentPlayer = (currentPlayer == 1) ? 2 : 1; // changing the player.
        }
        printf("\n\t\t\t   GAME OVER!!!!!! THE_PLAYER_%d HAS WON ! \n",3-currentPlayer); // game over
        Again();
 }

/// This function checks if the coordinates of the piece to move are available.
int validecoordinatesstart(int i,int j, char Board[][17], int currentPlayer){
     if ( i >= 0 && i <= 42 && j >= 0 && j <= 16 && Board[i][j]=='0'+ currentPlayer && CanPiecemove(i,j,Board, currentPlayer))
         return 1;
     return 0;
}

/// This function checks if the coordinates of the target box are available.
int validecoordinatesend(int i,int j, char Board[][17]){
     if ( i >= 0 && i <= 42 && j >= 0 && j <= 16 &&  Board[i][j]=='0')
         return 1;
        else
        return 0;
}

/// This function checks weither the current box and the target one are connected or not.
int areNodesConnected(int a, int b, int c, int d, char Board[][17]) {
    if (a == 0 && b == 0) {
        return ((c == 0 && d == 8) || (c == 21 && d == 0));
    } else if (a == 0 && b == 8) {
        return ((c == 0 && d == 0) || (c == 0 && d == 16) || (c == 7 && d == 8));
    } else if (a == 0 && b == 16) {
        return ((c == 0 && d == 8) || (c == 21 && d == 16));
    } else if (a == 7 && b == 3) {
        return ((c == 7 && d == 8) || (c == 21 && d == 3));
    } else if (a == 7 && b == 8) {
        return ((c == 0 && d == 8) || (c == 7 && d == 3) || (c == 7 && d == 13) || (c == 14 && d == 8));
    } else if (a == 7 && b == 13) {
        return ((c == 7 && d == 8) || (c == 21 && d == 13));
    } else if (a == 14 && b == 6) {
        return ((c == 14 && d == 8) || (c == 21 && d == 6));
    } else if (a == 14 && b == 8) {
        return ((c == 7 && d == 8) || (c == 14 && d == 6) || (c == 14 && d == 10));
    } else if (a == 14 && b == 10) {
        return ((c == 14 && d == 8) || (c == 21 && d == 10));
    } else if (a == 21 && b == 0) {
        return ((c == 0 && d == 0) || (c == 21 && d == 3) || (c == 42 && d == 0));
    } else if (a == 21 && b == 3) {
        return ((c == 7 && d == 3) || (c == 21 && d == 0) || (c == 21 && d == 6) || (c == 35 && d == 3));
    } else if (a == 21 && b == 6) {
        return ((c == 14 && d == 6) || (c == 21 && d == 3) || (c == 28 && d == 6));
    } else if (a == 21 && b == 10) {
        return ((c == 14 && d == 10) || (c == 21 && d == 13) || (c == 28 && d == 10));
    } else if (a == 21 && b == 13) {
        return ((c == 7 && d == 13) || (c == 21 && d == 10) || (c == 21 && d == 16) || (c == 35 && d == 13));
    } else if (a == 21 && b == 16) {
        return ((c == 0 && d == 16) || (c == 21 && d == 13) || (c == 42 && d == 16));
    } else if (a == 28 && b == 6) {
        return ((c == 21 && d == 6) || (c == 28 && d == 8));
    } else if (a == 28 && b == 8) {
        return ((c == 28 && d == 6) || (c == 28 && d == 10) || (c == 35 && d == 8));
    } else if (a == 28 && b == 10) {
        return ((c == 21 && d == 10) || (c == 28 && d == 8));
    } else if (a == 35 && b == 3) {
        return ((c == 21 && d == 3) || (c == 35 && d == 8));
    } else if (a == 35 && b == 8) {
        return ((c == 28 && d == 8) || (c == 35 && d == 3) || (c == 35 && d == 13) || (c == 42 && d == 8));
    } else if (a == 35 && b == 13) {
        return ((c == 21 && d == 13) || (c == 35 && d == 8));
    } else if (a == 42 && b == 0) {
        return ((c == 42 && d == 8) || (c == 21 && d == 0));
    } else if (a == 42 && b == 8) {
        return ((c == 42 && d == 0) || (c == 42 && d == 16) || (c == 35 && d == 8));
    } else if (a == 42 && b == 16) {
        return ((c == 42 && d == 8) || (c == 21 && d == 16));
    }
     return 0;
}

/// This function checks if a piece is surrounded by other pieces.
int isPieceSurrounded(int a, int b, char Board[][17], char opponentPiece) {
    if (a == 0 && b == 0) {
        return (Board[0][8] != '0' && Board[21][0] != '0');
    } else if (a == 0 && b == 8) {
        return (Board[0][0] != '0' && Board[0][16] != '0' && Board[7][8] != '0');
    } else if (a == 0 && b == 16) {
        return (Board[0][8] != '0' && Board[21][16] != '0');
    } else if (a == 7 && b == 3) {
        return (Board[7][8] != '0' && Board[21][3] != '0');
    } else if (a == 7 && b == 8) {
        return (Board[0][8] != '0' && Board[7][3] != '0' &&
                Board[7][13] != '0' && Board[14][8] != '0');
    } else if (a == 7 && b == 13) {
        return (Board[7][8] != '0' && Board[21][13] != '0');
    } else if (a == 14 && b == 6) {
        return (Board[14][8] != '0' && Board[21][6] != '0');
    } else if (a == 14 && b == 8) {
        return (Board[7][8] != '0' && Board[14][6] != '0' &&
                Board[14][10] != '0');
    } else if (a == 14 && b == 10) {
        return (Board[14][8] != '0' && Board[21][10] != '0');
    } else if (a == 21 && b == 0) {
        return (Board[0][0] != '0' && Board[21][3] != '0' && Board[42][0] != '0');
    } else if (a == 21 && b == 3) {
        return (Board[7][3] != '0' && Board[21][0] != '0' && Board[21][6] != '0' && Board[35][3] != '0');
    } else if (a == 21 && b == 6) {
        return (Board[14][6] != '0' && Board[21][3] != '0' && Board[21][10] != '0' && Board[28][6] != '0');
    } else if (a == 21 && b == 10) {
        return (Board[14][10] != '0' && Board[28][10] != '0' && Board[21][13] != '0');
    } else if (a == 21 && b == 13) {
        return (Board[7][13] != '0' && Board[21][10] != '0' && Board[21][16] != '0' && Board[35][13] != '0');
    } else if (a == 21 && b == 16) {
        return (Board[0][16] != '0' && Board[21][13] != '0' && Board[42][16] != '0');
    } else if (a == 28 && b == 6) {
        return (Board[21][6] != '0' && Board[28][8] != '0');
    } else if (a == 28 && b == 8) {
        return ( Board[28][6] != '0' &&
                Board[28][10] != '0' && Board[35][8] != '0');
    } else if (a == 28 && b == 10) {
        return (Board[21][10] != '0' && Board[28][8] != '0');
    } else if (a == 35 && b == 3) {
        return (Board[21][3] != '0' && Board[35][8] != '0');
    } else if (a == 35 && b == 8) {
        return (Board[28][8] != '0' && Board[35][3] != '0' &&
                Board[35][13] != '0' && Board[42][8] != '0');
    } else if (a == 35 && b == 13) {
        return (Board[21][13] != '0' && Board[35][8] != '0');
    } else if (a == 42 && b == 0) {
        return (Board[42][8] != '0' && Board[21][0] != '0');
    } else if (a == 42 && b == 8) {
        return (Board[42][0] != '0' && Board[42][16] != '0' && Board[35][8] != '0');
    } else if (a == 42 && b == 16) {
        return (Board[42][8] != '0' && Board[21][16] != '0');
    }

    return 0; // La pi�ce n'est pas entour�e
}

/// This function check weither the piece can move or not.
int CanPiecemove(int a, int b, char Board[][17], int currentPlayer) {
    char opponentPiece = (currentPlayer == 1) ? '2' : '1';
    return !isPieceSurrounded(a, b, Board, opponentPiece);
}

/// This funtions makes sure the move is available.
int Jumppieceavailable(int currentPlayer, int a, int b, int c, int d, char Board[][17]) {
    char playerPiece = (currentPlayer == 1) ? '1' : '2';
    if (Board[a][b] != playerPiece || Board[c][d] != '0' || !CanPiecemove(a,b,Board, currentPlayer)) {
        return 0;
    }
    return areNodesConnected(a, b, c, d, Board);
}

/// This function does the moving or jumping.
void Jumppiece(int currentPlayer,int startrow,int startcol,int endrow,int endcol, char Board[][17]){
    if (currentPlayer==1){
        Board[startrow][startcol]='0';
        Board[endrow][endcol]='1';
    }
    if (currentPlayer==2){
        Board[startrow][startcol]='0';
        Board[endrow][endcol]='2';
    }
}

/// This function counts the pieces of the player.
int countpieces(int currentPlayer, char Board[][17]){
    int c1=0,c2=0;
    for (int i=0;i<43;i++){
        for (int j=0;j<17;j++){
                if (Board[i][j]=='1')
                    c1++;
                if (Board[i][j]=='2')
                    c2++;
        }
    }
    if (currentPlayer==1)
        return c1;
    if (currentPlayer==2)
        return c2;
}

/// This function checks if the game is over or not yet.
int gameover(int currentPlayer, char Board[][17]) {
    if (countpieces(currentPlayer, Board) <= 2) {
        return 1;
    }
    for (int i = 0; i < 42; i++) {
        for (int j = 0; j < 17; j++) {
            if (Board[i][j] == (currentPlayer + '0')) {
                for (int a = 0; a < 42; a++) {
                    for (int b = 0; b < 17; b++) {
                        if (Jumppieceavailable(currentPlayer, i, j, a, b, Board)) {
                            return 0;
                        }
                    }
                }
            }
        }
    }
    return 1;
}
void Play(){
    printf("\n\n\t\t--> PLAY \n\n");
    printf("\t\t      1 - PLAY 1 V 1 \n");
    printf("\t\t      2 - PLAY 1 V COMPUTER 0 \n");
    printf("\t\t      3 - PLAY 1 V COMPUTER 1 \n\n");
    int b;
    printf("\t  WHAT IS YOUR CHOICE : ");
    scanf("%d",&b);
    if( b==1)
       Player_Player();
    if (b==2)
        PlayerVsMachine0();
    if (b==3)
        PlayerVsMachine1();
    while (b!=1 && b!=2 && b!=3){
       printf("\t\tINVALID OPTION, TRY AGAIN!\n\n");
       scanf("%d",&b);
    }
}

/// This function verifies if a player has formed a new mill or not.
void millVerifier(int list[3], char Board[][17], int player, int millformed[][3][2], int possibleMills[16][3][2]) {
    for (int i = 0; i < 16; i++) {
        int row1 = possibleMills[i][0][0], col1 = possibleMills[i][0][1];
        int row2 = possibleMills[i][1][0], col2 = possibleMills[i][1][1];
        int row3 = possibleMills[i][2][0], col3 = possibleMills[i][2][1];
        if (Board[row1][col1] == '0' + player &&
            Board[row2][col2] == '0' + player &&
            Board[row3][col3] == '0' + player) {
                if         ((
            millformed[i][0][0] != row1 || millformed[i][0][1] != col1 ||
            millformed[i][1][0] != row2 || millformed[i][1][1] != col2 ||
            millformed[i][2][0] != row3 || millformed[i][2][1] != col3)){
                list[player]++;
    }
            }
    }
    for (int i=0;i<16;i++){
        millformed[i][0][0]=-1;millformed[i][0][1]=-1;
        millformed[i][1][0]=-1;millformed[i][1][1]=-1;
        millformed[i][2][0]=-1;millformed[i][2][1]=-1;
    }
    for (int i=0;i<16;i++){
        int row1 = possibleMills[i][0][0], col1 = possibleMills[i][0][1];
        int row2 = possibleMills[i][1][0], col2 = possibleMills[i][1][1];
        int row3 = possibleMills[i][2][0], col3 = possibleMills[i][2][1];
        if (Board[row1][col1]!='0' && Board[row1][col1] == Board[row2][col2] &&
            Board[row1][col1] == Board[row3][col3]){
                millformed[i][0][0] = row1;
                millformed[i][0][1] = col1;
                millformed[i][1][0] = row2;
                millformed[i][1][1] = col2;
                millformed[i][2][0] = row3;
                millformed[i][2][1] = col3;
            }
        }
}

/// This function is used to print the instructions.
void Instructions(){
    printf("  Rules of the game:\n");
    printf("    1. The game is played between two players, each having 9 pieces.\n");
    getch();
    printf("    2. The game consists of two phases:\n");
    printf("       a. **Placing pieces**: Players take turns placing their pieces on empty positions.\n");
    printf("          - If a player forms a 'mill' (three pieces in a row), they can remove an opponent's piece.\n");
    printf("       b. **Moving pieces**: After all pieces are placed, players can move their pieces to adjacent positions.\n");
    printf("          - A mill can still be formed, allowing removal of an opponent's piece.\n");
    getch();
    printf("    3. The objective is to reduce the opponent to 2 pieces or block all their moves, but if they were left with only three pieces they can move to any empty position. \n\n");
    printf("\n");
    getch();
    printf("  Board layout:\n");
    printf("    - The board has three concentric squares connected by lines.\n");
    printf("    - Pieces can be placed or moved only at the intersections or along the lines.\n\n");
    printf("\n");
    getch();
    printf("  How to play:\n");
    printf("    1. On your turn, enter the row and column number where you want to place/move your piece.\n");
    printf("    2. Invalid moves will be rejected, and you'll need to try again.\n");
    printf("    3. Be strategic! Forming mills and blocking your opponent are key to winning.\n\n");
    printf("  Special notes:\n");
    getch();
    printf("    - You cannot remove an opponent's piece that is part of a mill unless no other pieces are available.\n");
    printf("    - Once a player is reduced to 3 pieces, they enter the 'flying phase' and can move freely.\n");
    printf("\n  Let the game begin! May the best player win!\n");
}

/// This function is used to quit the game.
void Quit(){
     printf("\n\t\tSEE YOU!\n");
}

/// This function shows the MENU.
void Menu(){
    printf("\t\t--> MENU \n\n");
    printf("\t\t      1 - PLAY \n");
    printf("\t\t      2 - GAME INSTRUCTIONS \n");
    printf("\t\t      3 - QUIT :( \n\n");
    int a;
    printf("\t WHAT IS YOUR CHOICE : ");
    scanf("%d",&a);
    while (a!=1 && a!=2 && a!=3){
        printf("\t\tINVALID OPTION, TRY AGAIN! : ");
        scanf("%d",&a);
    }
    if (a==1)
        Play();
    if (a==2){
        Instructions();
        printf("\t\t      1 - PLAY \n");
        printf("\t\t      2 - QUIT :( \n\n");
        int b;
        printf("\t WHAT IS YOUR CHOICE : ");
        scanf("%d",&b);
        while (b!=1 && b!=2){
            printf("\t\tINVALID OPTION, TRY AGAIN! : ");
            scanf("%d",&b);
        }
            if (b==1)
               Play();
            if (b==2)
               Quit();

    }

    if (a==3)
        Quit();
}

/// The Introduction to the game.
void introduction(){
    setColors(15,0);
    printf("\n\n\t\t\t\t********  THE NINE MEN'S MORRIS GAME  ********\n\n");
    getch();
    printf("\tIT'S SO NICE TO HAVE, CHOOSE ONE OF THE FOLLOWING OPTIONS!\n\n");
    getch();
    Menu();

}









/////////////////////////////////////////////////////////////////////   PLAYER____________MACHINE0   /////////////////////////////////////////////////////////////////////









/// This function is used to if a piece can be taken out.
int IsValidRemovemachine(int i, int j, char Board[][17], int currentPlayer) {
    if (i < 0 || i > 42 || j < 0 || j >= 17) {
        return 0;
    }
    if (Board[i][j] != '1' && Board[i][j] != '2'){
        return 0;
    }
    if (Board[i][j] == '1' && currentPlayer == 1) {
        return 0;
    }
    if (Board[i][j] == '2' && currentPlayer == 2) {
        return 0;
    }
    if (isprotectedline(i, j, Board)) {
        return 0;
    }
    if (isprotectedcol(i, j, Board)) {
        return 0;
    }
    else
        return 1;
}



void PlayerVsMachine0() {
    printf("\t\tHERE IS THE BOARD THE ACTION WILL TAKE PLACE ON \n");
    Display(Board);
    getch();
    printf("\t\tYOU WILL BE NAMED THE_PLAYER_1 AND THE MACHINE WILL BE NAMED THE_PLAYER_2.\n");
    getch();
    printf("\t\tTHE PLAYER THAT WILL MAKE THE FIRST MOVE IS GOING TO BE CHOSEN RANDOMLY. \n");
    getch();
    int currentPlayer = Firstmovemaker();
    int i, j, a, b;
    int c = 0;
    int list[3] = {0, 0, 0};
    int millformed[16][3][2];
    memset(millformed, -1, sizeof(millformed));
    getch();
    printf("\t\t\nWE WOULD LIKE TO INFORM YOU THAT THE AVAILABLE BOXES HAVE THE FOLLOWING INDEXES :\n");
    showavailibleboxes();
    srand(time(NULL));

    // THE PLACING PHASE :
    while (c < 18) {
        if (currentPlayer == 1) {
            printf("\t\tPLAYER_1, CHOOSE THE BOX YOU WANNA PLACE YOUR PIECE IN (ROW AND COL): ");
            int validinput = 0;
            while (!validinput) {
                scanf("%d %d", &i, &j);
                if (TheMoveIsValid(i, j, Board)) {
                    PlacePiece(currentPlayer, i, j, Board);
                    c++;
                    Display(Board);
                    validinput = 1;
                } else {
                    printf("\t\tINVALID MOVE, TRY AGAIN.\n\n");
                }
            }
        } else {
            printf("\t\tMACHINE (PLAYER_2) IS MAKING ITS MOVE...\n");
    int validinput = 0;{
    while (!validinput) {
        int randomIndex = rand() % numavailableBoxes; // The machine chooses a random number from 0 to numavailableBoxes-1
        i = availableBoxes[randomIndex][0]; // the first coordinate
        j = availableBoxes[randomIndex][1]; // the second coordinate
        if (TheMoveIsValid(i, j, Board)) {
            PlacePiece(currentPlayer, i, j, Board);
            c++;
            Display(Board);
            validinput = 1;
        }
    }
  }
        }
        if (c > 4) {
            int list[3] = {0, 0, 0};
            millVerifier(list, Board, currentPlayer, millformed, possibleMills);
            int a=(currentPlayer==1)?2:1;
            if (list[currentPlayer] > 0 && !allprotected(a,Board)) { // the machine has formed a new mill.
                if (currentPlayer == 1) {
                    int list1[3]={0,0,0};
                    millVerifier(list1, Board,a,millformed,possibleMills);
                    if ( countpieces(a,Board)==3 && list1[a]>0){  // the challenger has only 3 pieces left and the form a mill.
                        currentPlayer= (currentPlayer==1)?2:1;
                        printf("        YOU CAN NOT TAKE OUT ANY PIECE OF THE MACHINE BECAUSE IT ONLY GOT 3 LEFT AND THEY ARE ALL A PART OF A MILL.\n");
                        break;
                    }
                    else{
                    printf("\t\tPLAYER_1, YOU HAVE FORMED A MILL! CHOOSE A PIECE OF YOUR CHALLENGER TO REMOVE: ");
                    int validRemove = 0;
                    while (!validRemove) {
                        scanf("%d %d", &a, &b);
                        if (IsValidRemove(a, b, Board, currentPlayer)) {
                            takeout(a, b, Board, currentPlayer); // the piece is being taken out.
                            printf("\n\t\tBOARD AFTER REMOVING THE PIECE:\n\n");
                            Display(Board);
                            validRemove = 1;
                        } else {
                            printf("\t\tINVALID OPTION, CHOOSE AGAIN.\n");
                        }
                    }
                }
                }
                else { // the machine's turn.
                    int list1[3]={0,0,0};
                    millVerifier(list1, Board,a,millformed,possibleMills); // the machine formed a new mill .
                    if ( countpieces(a,Board)>3 || (countpieces(a,Board)==3 && list1[a]==0)){
                    printf("\t\tMACHINE (PLAYER_2) HAS FORMED A MILL AND IS REMOVING A PIECE...\n");
                    int validRemove = 0;
                    while (!validRemove) {
                        int randomIndex = rand() % numavailableBoxes; // Again the machine chooses a piece randomly.
                        a = availableBoxes[randomIndex][0];
                        b = availableBoxes[randomIndex][1];
                        if (IsValidRemovemachine(a, b, Board, currentPlayer)) { // but now the piece has to be owned by the player one so it can be removed.
                            takeout(a, b, Board, currentPlayer);
                            printf("\n\t\tBOARD AFTER REMOVING THE PIECE:\n\n");
                            Display(Board);
                            validRemove = 1;
                        }
                    }
                    }
                }
            }
        }
        currentPlayer = (currentPlayer == 1) ? 2 : 1;  // we change the player.
    }

    printf("\t\tYOU ARE DONE WITH PLACING THE NINE PIECES.\n\n"); // THE PLACING PHASE
    while (!gameover(currentPlayer, Board)) { // while the game isn't over yet , we repeat :
            if (currentPlayer == 1) { // player 1 turn
                if (countpieces(currentPlayer,Board)==3){  // if the player 1 has only 3 pieces left.
                printf("THE_PLAYER_1 YOU ONLY GOT 3 PIECES LEFT YOU ARE FREE TO JUMP\n");
                int startrow, startcol, endrow, endcol;
                int validfirst = 0;
                printf("\t\tENTER THE COORDINATES OF THE PIECE YOU WANT TO JUMP (ROW COL): \n"); // the piece to jump
                while (!validfirst) {
                    scanf("%d %d", &startrow, &startcol);
                    if (validecoordinatesstart(startrow, startcol, Board, currentPlayer)) {
                        printf("\t\tENTER THE COORDINATES OF THE TARGET POSITION (ROW COL): \n"); // the target box
                        int validsecond = 0;
                        while (!validsecond) {
                            scanf("%d %d", &endrow, &endcol);
                            if (Board[endrow][endcol]=='0') { // the only condition so that the jump happens is that the target box is empty.
                                    Jumppiece(currentPlayer, startrow, startcol, endrow, endcol, Board);
                                    printf("(%d,%d),", startrow, startcol);
                                    printf("(%d,%d)", endrow, endcol);
                                    printf("\t\tTHE BOARD AFTER JUMPING THE PIECE:\n");
                                    Display(Board);
                                    int list[3] = {0, 0, 0};
                                    millVerifier(list, Board, currentPlayer, millformed, possibleMills); // we check for new mills.
                                    validsecond = 1;
                                    int a=(currentPlayer==1)?2:1;
                                    if (list[currentPlayer] > 0 && !allprotected(a,Board)) { // a new mill formed.
                                        int list1[3]={0,0,0};
                                        millVerifier(list1, Board,a,millformed,possibleMills);
                                        if ( countpieces(a,Board)==3 && list1[a]>0 ){ // but the challenger has only 3 pieces left and they form a mill.
                                            printf("\t\tPLAYER_1, YOU HAVE FORMED A MILL! BUT UNFORTUNATELLY YOU CAN NOI TAKE A PIECE OF THE MACHINE OUT BECAUSE IT ONLY GOT 3 LEFT AND THEY FORM A MILL");
                                            break;
                                        }else{ // if not
                                        printf("\t\tPLAYER_1, YOU HAVE FORMED A MILL! CHOOSE A PIECE OF YOUR CHALLENGER TO REMOVE: ");
                                        int validRemove = 0;
                                        while (!validRemove) {
                                            scanf("%d %d", &i, &j);
                                            if (IsValidRemove(i, j, Board, currentPlayer)) {
                                                takeout(i, j, Board, currentPlayer);
                                                printf("\n\t\tBOARD AFTER REMOVING THE PIECE:\n\n");
                                                Display(Board);
                                                validRemove = 1;
                                            } else {
                                                printf("\t\tINVALID OPTION, CHOOSE AGAIN.\n");
                                            }
                                        }
                                    }

                            }
                            } else {
                                printf("\t\tINVALID OPTION, CHOOSE AGAIN.\n");
                            }
                        }
                        validfirst = 1;
                    } else {
                        printf("\t\tINVALID OPTION, CHOOSE AGAIN.\n");
                    }
                }
                }else{ // The player 1 has more than 3 pieces.
                printf("\t\tPLAYER_1, IT'S YOUR TURN TO MOVE.\n");
                int startrow, startcol, endrow, endcol;
                int validfirst = 0;
                printf("\t\tENTER THE COORDINATES OF THE PIECE YOU WANT TO MOVE (ROW COL): \n");
                while (!validfirst) {
                    scanf("%d %d", &startrow, &startcol);
                    if (validecoordinatesstart(startrow, startcol, Board, currentPlayer)) {
                        printf("\t\tENTER THE COORDINATES OF THE TARGET POSITION (ROW COL): \n");
                        int validsecond = 0;
                        while (!validsecond) {
                            scanf("%d %d", &endrow, &endcol);
                            if (validecoordinatesend(endrow, endcol, Board)) {
                                if (Jumppieceavailable(currentPlayer, startrow, startcol, endrow, endcol, Board)) {
                                    Jumppiece(currentPlayer, startrow, startcol, endrow, endcol, Board);
                                    printf("\t\tTHE BOARD AFTER JUMPING THE PIECE:\n");
                                    Display(Board);
                                    int list[3] = {0, 0, 0};
                                    millVerifier(list, Board, currentPlayer, millformed, possibleMills);
                                    validsecond = 1;
                                    int a=(currentPlayer==1)?2:1;
                                    if (list[currentPlayer] > 0 && !allprotected(a,Board)) {
                                        int list1[3]={0,0,0};
                                        millVerifier(list1, Board,a,millformed,possibleMills);
                                        if ( countpieces(a,Board)==3 && list1[a]>0 ){
                                            currentPlayer= (currentPlayer==1)?2:1;
                                            printf("\t\tPLAYER_1, YOU HAVE FORMED A MILL! BUT UNFORTUNATELLY YOU CAN NOI TAKE A PIECE OF THE MACHINE OUT BECAUSE IT ONLY GOT 3 LEFT AND THEY FORM A MILL");
                                            break;
                                        }else{
                                        printf("\t\tPLAYER_1, YOU HAVE FORMED A MILL! CHOOSE A PIECE OF YOUR CHALLENGER TO REMOVE: ");
                                        int validRemove = 0;
                                        while (!validRemove) {
                                            scanf("%d %d", &i, &j);
                                            if (IsValidRemove(i, j, Board, currentPlayer)) {
                                                takeout(i, j, Board, currentPlayer);
                                                printf("\n\t\tBOARD AFTER REMOVING THE PIECE:\n\n");
                                                Display(Board);
                                                validRemove = 1;
                                            } else {
                                                printf("\t\tINVALID OPTION, CHOOSE AGAIN.\n");
                                            }
                                        }
                                    }
                                }
                            }
                            } else {
                                printf("\t\tINVALID OPTION, CHOOSE AGAIN.\n");
                            }
                        }
                        validfirst = 1;
                    } else {
                        printf("\t\tINVALID OPTION, CHOOSE AGAIN.\n");
                    }
                }
                }
            } else { // the machine's turn
                if (countpieces(currentPlayer, Board) == 3) { // it only has 3 pieces left ( the free jumping phase)
                    printf("\t\tMACHINE (PLAYER_2) HAS ONLY 3 PIECES LEFT ? IT IS FREE TO JUMP...\n");
                    int list2[9][2]={{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}}; // an array to store the pieces of the machine
                int list3[9][2]={{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}}; // an array to store the empty boxes
                int i2 = 0, i3 = 0;
                for (int j = 0; j < 43; j++) {
                    for (int k = 0; k < 17; k++) {
                        if (Board[j][k] == '2') {
                            list2[i2][0] = j;
                            list2[i2][1] = k;
                            i2++; //we fill in the first array.
                        }
                         if (Board[j][k] == '0') {
                            list3[i3][0] = j;
                            list3[i3][1] = k;
                            i3++;// we fill in the second array.
                        }
                    }
                }
                int validMove = 0;
                while (!validMove) {
                    srand(time(NULL));
                    int randomindex = rand() % i2;   // random index based on the number of elements in list2 (i2).
                    int startrow = list2[randomindex][0];
                    int startcol =list2[randomindex][1];
                    if (validecoordinatesstart(startrow, startcol, Board, currentPlayer)){
                       int randomindex = rand() % i3; // random index based on the number of elements in list3 (i3).
                       int endrow = list3[randomindex][0];
                       int endcol =list3[randomindex][1];
                       if (validecoordinatesend(endrow, endcol, Board)){
                        printf("(%d,%d)", startrow, startcol);
                        printf("(%d,%d)", endrow, endcol);
                        Jumppiece(currentPlayer, startrow, startcol, endrow, endcol, Board); // we do the jumping.
                        printf("\t\tTHE BOARD AFTER THE MACHINE'S MOVE:\n");
                        Display(Board);
                        int list[3] = {0, 0, 0};
                        millVerifier(list, Board, currentPlayer, millformed, possibleMills); // check for new mills
                        validMove = 1;
                        int a=(currentPlayer==1)?2:1;
                           if (list[currentPlayer] > 0 && !allprotected(a,Board)) {
                                int list2[3]={0,0,0};
                                millVerifier(list2, Board, currentPlayer, millformed, possibleMills);
                                if (countpieces(a,Board)>3 || (countpieces(a,Board)==3 && list[a]==0)){ // avoiding the case in which the challenger has 3 pieces left and they form a mill.
                                printf("\t\tMACHINE (PLAYER_2) HAS FORMED A MILL AND IS REMOVING A PIECE...\n");
                                int validRemove = 0;
                                while (!validRemove) {
                                    int randomIndex = rand() % numavailableBoxes; // a piece to remove randomly
                                    a =availableBoxes[randomIndex][0];
                                    b =availableBoxes[randomIndex][1];
                                    if (IsValidRemovemachine(a, b, Board, currentPlayer)) {
                                       takeout(a, b, Board, currentPlayer);
                                       printf("\n\t\tBOARD AFTER REMOVING THE PIECE:\n\n");
                                       Display(Board);
                                       validRemove = 1;
                                }
                            }
                                }
                        }

                }
                    }
            }

        }else{ // the machine has more than 3 pieces left.
                printf("\t\tMACHINE (PLAYER_2) IS MAKING ITS MOVE...\n");
                int list2[9][2]={{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};
                int list3[9][2]={{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};
                int i2 = 0, i3 = 0;
                for (int j = 0; j < 43; j++) {
                    for (int k = 0; k < 17; k++) {
                        if (Board[j][k] == '2') {
                            list2[i2][0] = j;
                            list2[i2][1] = k;
                            i2++;
                        }
                         if (Board[j][k] == '0') {
                            list3[i3][0] = j;
                            list3[i3][1] = k;
                            i3++;
                        }
                    }
                }
                int validMove = 0;
                while (!validMove) {
                    srand(time(NULL));
                    int randomindex = rand() % i2;
                    int startrow = list2[randomindex][0];
                    int startcol =list2[randomindex][1];
                    if (validecoordinatesstart(startrow, startcol, Board, currentPlayer)){
                       int randomindex = rand() % i3;
                       int endrow = list3[randomindex][0];
                       int endcol =list3[randomindex][1];
                       if (validecoordinatesend(endrow, endcol, Board)){
                        if (Jumppieceavailable(currentPlayer, startrow, startcol, endrow, endcol, Board)) {
                        printf("(%d,%d)", startrow, startcol);
                        printf("(%d,%d)", endrow, endcol);
                        Jumppiece(currentPlayer, startrow, startcol, endrow, endcol, Board);
                        printf("\t\tTHE BOARD AFTER THE MACHINE'S MOVE:\n");
                        Display(Board);
                        int list[3] = {0, 0, 0};
                        millVerifier(list, Board, currentPlayer, millformed, possibleMills);
                        validMove = 1;
                            int a=(currentPlayer==1)?2:1;
                            if (list[currentPlayer] > 0 && !allprotected(a,Board)) {
                                int list2[3]={0,0,0};
                                millVerifier(list2, Board, currentPlayer, millformed, possibleMills);
                                if (countpieces(a,Board)>3 || (countpieces(a,Board)==3 && list[a]==0)){
                                printf("\t\tMACHINE (PLAYER_2) HAS FORMED A MILL AND IS REMOVING A PIECE...\n");
                                int validRemove = 0;
                                while (!validRemove) {
                                    int randomIndex = rand() % numavailableBoxes;
                                    a =availableBoxes[randomIndex][0];
                                    b =availableBoxes[randomIndex][1];
                                    if (IsValidRemovemachine(a, b, Board, currentPlayer)) {
                                       takeout(a, b, Board, currentPlayer);
                                       printf("\n\t\tBOARD AFTER REMOVING THE PIECE:\n\n");
                                       Display(Board);
                                       validRemove = 1;
                                }
                            }
                                }
                        }
                    }
                }
                    }
            }
        }
            }
        currentPlayer = (currentPlayer == 1) ? 2 : 1;
    }
    printf("\n\t\t\t   GAME OVER!!!!!! THE_PLAYER_%d HAS WON ! \n",3-currentPlayer); // game over
    Again();
}






/////////////////////////////////////////////////////////////////////     PLAYER_______MACHINE1      //////////////////////////////////////////////////////////////////////////





/// USEFUL FUNCTION FOR THE FREE JUMPING PHASE :

// This  function is used by the machine to block a mill formation in a LINE.
bool movepieceline(char Board[][17], int player, int row, int col1, int col2, int col3, int list[][2]) {
    int other=(player==1)?2:1;
    if (Board[row][col1] == Board[row][col2] && Board[row][col1] == '0' + other && Board[row][col3] == '0') {
        for (int i=0;i<8;i++){
                Jumppiece(player,list[i][0],list[i][1],row,col3, Board);
                break;
        }
        return true;
    }
    if (Board[row][col1] == Board[row][col3] && Board[row][col1] == '0' + other && Board[row][col2] == '0') {
        for (int i=0;i<8;i++){
                Jumppiece(player,list[i][0],list[i][1],row,col2, Board);
                break;
        }
        return true;
    }
    if (Board[row][col2] == Board[row][col3] && Board[row][col2] == '0' + other && Board[row][col1] == '0') {
        for (int i=0;i<8;i++){
                Jumppiece(player,list[i][0],list[i][1],row,col1, Board);
                break;
        }
        return true;
    }
    return false;
}

// This  function is used by the machine to block a mill formation in a COLUMN.
bool movepiececolumn(char Board[][17], int player, int col, int row1, int row2, int row3, int list[][2]) {
    int other=(player==1)?2:1;
    if (Board[row1][col] == Board[row2][col] && Board[row1][col] == '0' + other && Board[row3][col] == '0') {
        for (int i=0;i<8;i++){
                Jumppiece(player,list[i][0],list[i][1],row3,col, Board);
                break;
        }
        return true;
    }
    if (Board[row1][col] == Board[row3][col] && Board[row1][col] == '0' + other && Board[row2][col] == '0') {
        for (int i=0;i<8;i++){
                Jumppiece(player,list[i][0],list[i][1],row2,col, Board);
                break;
        }
        return true;
    }
    if (Board[row2][col] == Board[row3][col] && Board[row2][col] == '0' + other && Board[row1][col] == '0') {
        for (int i=0;i<8;i++){
                Jumppiece(player,list[i][0],list[i][1],row1,col, Board);
                break;
        }
        return true;
    }
    return false;
}



// This  function is used by the machine to complete a mill formation in a LINE.
bool completmillline(char Board[][17], int player, int row, int col1, int col2, int col3, int list[][2]) {
    if (Board[row][col1] == Board[row][col2] && Board[row][col1] == '0' + player && Board[row][col3] == '0') {
        for (int i=0;i<8;i++){
                Jumppiece(player,list[i][0],list[i][1],row,col3, Board);
                break;
        }
        return true;
    }
    if (Board[row][col1] == Board[row][col3] && Board[row][col1] == '0' + player && Board[row][col2] == '0') {
        for (int i=0;i<8;i++){
                Jumppiece(player,list[i][0],list[i][1],row,col2, Board);
                break;
        }
        return true;
    }
    if (Board[row][col2] == Board[row][col3] && Board[row][col2] == '0' + player && Board[row][col1] == '0') {
        for (int i=0;i<8;i++){
                Jumppiece(player,list[i][0],list[i][1],row,col1, Board);
                break;
        }
        return true;
    }
    return false;
}

// This  function is used by the machine to complete a mill formation in a COLUMN.
bool completmillcolumn(char Board[][17], int player, int col, int row1, int row2, int row3, int list[][2]) {
    if (Board[row1][col] == Board[row2][col] && Board[row1][col] == '0' + player && Board[row3][col] == '0') {
        for (int i=0;i<8;i++){
                Jumppiece(player,list[i][0],list[i][1],row3,col, Board);
                break;
        }
        return true;
    }
    if (Board[row1][col] == Board[row3][col] && Board[row1][col] == '0' + player && Board[row2][col] == '0') {
        for (int i=0;i<8;i++){
                Jumppiece(player,list[i][0],list[i][1],row2,col, Board);
                break;
        }
        return true;
    }
    if (Board[row2][col] == Board[row3][col] && Board[row2][col] == '0' + player && Board[row1][col] == '0') {
        for (int i=0;i<8;i++){
                Jumppiece(player,list[i][0],list[i][1],row1,col, Board);
                break;
        }
        return true;
    }
    return false;
}

// This  function is used by the machine to complete a mill formation.
bool completemill(char Board[][17], int player, int list[][2]) {
    if (completmillline(Board, player, 0, 0, 8, 16,list)) return true;
    if (completmillline(Board, player, 7, 3, 8, 13,list)) return true;
    if (completmillline(Board, player, 14, 6, 8, 10,list)) return true;
    if (completmillline(Board, player, 21, 0, 3, 6,list)) return true;
    if (completmillline(Board, player, 21, 10, 13, 16,list)) return true;
    if (completmillline(Board, player, 28, 6, 8, 10,list)) return true;
    if (completmillline(Board, player, 35, 3, 8, 13,list)) return true;
    if (completmillline(Board, player, 42, 0, 8, 16,list)) return true;
    if (completmillcolumn(Board, player, 0, 0, 21, 42,list)) return true;
    if (completmillcolumn(Board, player, 3, 7, 21, 35,list)) return true;
    if (completmillcolumn(Board, player, 6, 14, 21, 28,list)) return true;
    if (completmillcolumn(Board, player, 8, 0, 7, 14,list)) return true;
    if (completmillcolumn(Board, player, 8, 28, 35, 42,list)) return true;
    if (completmillcolumn(Board, player, 10, 14, 21, 28,list)) return true;
    if (completmillcolumn(Board, player, 13, 7, 21, 35,list)) return true;
    if (completmillcolumn(Board, player, 16, 0, 21, 42,list)) return true;

    return false; // Aucun moulin à compléter
}



// This function is used by the machine to place a piece near one of it's own pieces in a LINE.
bool movenearpieceline(char Board[][17], int player, int list[][2]) {
    int rows[] = {0, 7, 14, 21, 21, 28, 35, 42};
    int cols[][3] = {
        {0, 8, 16},  // Ligne 0
        {3, 8, 13},  // Ligne 7
        {6, 8, 10},  // Ligne 14
        {0, 3, 6},   // Ligne 21 (gauche)
        {10, 13, 16},// Ligne 21 (droite)
        {6, 8, 10},  // Ligne 28
        {3, 8, 13},  // Ligne 35
        {0, 8, 16}   // Ligne 42
    };

    for (int i = 0; i < 8; i++) {
        int row = rows[i];
        if (Board[row][cols[i][0]] == '0' + player && Board[row][cols[i][1]] == '0' && Board[row][cols[i][2]] == '0') {
            for (int i=0;i<8;i++){
                Jumppiece(player,list[i][0],list[i][1],row,cols[i][2], Board);
                break;
        }
            return true;
        }
        if (Board[row][cols[i][1]] == '0' + player && Board[row][cols[i][0]] == '0' && Board[row][cols[i][2]] == '0') {
            for (int i=0;i<8;i++){
                Jumppiece(player,list[i][0],list[i][1],row,cols[i][0], Board);
                break;
        }
            return true;
        }
        if (Board[row][cols[i][2]] == '0' + player && Board[row][cols[i][0]] == '0' && Board[row][cols[i][1]] == '0') {
            for (int i=0;i<8;i++){
                Jumppiece(player,list[i][0],list[i][1],row,cols[i][0], Board);
                break;
        }
            return true;
        }
    }
    return false;
}

// This function is used by the machine to place a piece near one of it's own pieces in a COLUMN.
bool movepiecenearcolumn(char Board[][17], int player, int list[][2]) {
    int cols[] = {0, 3, 6, 8, 10, 13, 16};
    int rows[][3] = {
        {0, 21, 42},  // Colonne 0
        {7, 21, 35},  // Colonne 3
        {14, 21, 28}, // Colonne 6
        {0, 7, 14},   // Colonne 8 (haut)
        {28, 35, 42}, // Colonne 8 (bas)
        {14, 21, 28}, // Colonne 10
        {7, 21, 35},  // Colonne 13
        {0, 21, 42}   // Colonne 16
    };

    for (int i = 0; i < 8; i++) {
        int col = cols[i];
        if (Board[rows[i][0]][col] == '0' + player && Board[rows[i][1]][col] == '0' && Board[rows[i][2]][col] == '0') {
            for (int i=0;i<8;i++){
                Jumppiece(player,list[i][0],list[i][1],rows[i][2],col, Board);
                break;
        }
            return true;
        }
        if (Board[rows[i][1]][col] == '0' + player && Board[rows[i][0]][col] == '0' && Board[rows[i][2]][col] == '0') {
                        for (int i=0;i<8;i++){
                Jumppiece(player,list[i][0],list[i][1],rows[i][0],col, Board);
                break;
        }
            return true;
        }
        if (Board[rows[i][2]][col] == '0' + player && Board[rows[i][0]][col] == '0' && Board[rows[i][1]][col] == '0') {
                        for (int i=0;i<8;i++){
                Jumppiece(player,list[i][0],list[i][1],rows[i][0],col, Board);
                break;
        }
            return true;
        }
    }
    return false;
}


// This function is used by the machine to place a piece in a stratigic box if it's empty.
bool movePieceStrategically(char Board[][17], int player, int list[][2]) {
    int strategicRows[] = {0,0,7,7,14,14, 28,28,35, 35,42,42};
    int strategicCols[] = {0,16,3, 13,6, 10,6,10,3,13,0,16};
    for (int i = 0; i < 5; i++) {
        if (Board[strategicRows[i]][strategicCols[i]] == '0') {
            for (int i=0;i<8;i++){
                Jumppiece(player,list[i][0],list[i][1],strategicRows[i],strategicCols[i], Board);
                break;

            return true;
        }
    }
    }
    for (int row = 0; row < 43; row++) {
        for (int col = 0; col < 17; col++) {
            if (Board[row][col] == '0') {
                for (int i=0;i<8;i++){
                Jumppiece(player,list[i][0],list[i][1],row,col, Board);
                break;
                return true;
            }
        }
    }
  }
}

// This function is used by the machine to do the free jumping.
bool movepiecemachine(char Board[][17],int player,int list[][2]){
    if (completemill(Board,player,list)) return true;
    if (movepieceline(Board, player, 0, 0, 8, 16,list)) return true;
    if (movepieceline(Board, player, 7, 3, 8, 13,list)) return true;
    if (movepieceline(Board, player, 14, 6, 8, 10,list)) return true;
    if (movepieceline(Board, player, 21, 0, 3, 6,list)) return true;
    if (movepieceline(Board, player, 21, 10, 13, 16,list)) return true;
    if (movepieceline(Board, player, 28, 6, 8, 10,list)) return true;
    if (movepieceline(Board, player, 35, 3, 8, 13,list)) return true;
    if (movepieceline(Board, player, 42, 0, 8, 16,list)) return true;
    if (movepiececolumn(Board, player, 0, 0, 21, 42,list)) return true;
    if (movepiececolumn(Board, player, 3, 7, 21, 35,list)) return true;
    if (movepiececolumn(Board, player, 6, 14, 21, 28,list)) return true;
    if (movepiececolumn(Board, player, 8, 0, 7, 14,list)) return true;
    if (movepiececolumn(Board, player, 8, 28, 35, 42,list)) return true;
    if (movepiececolumn(Board, player, 10, 14, 21, 28,list)) return true;
    if (movepiececolumn(Board, player, 13, 7, 21, 35,list)) return true;
    if (movepiececolumn(Board, player, 16, 0, 21, 42,list)) return true;
    if (movenearpieceline(Board,player,list)) return true;
    if (movepiecenearcolumn(Board,player,list)) return true;
    if (movePieceStrategically(Board,player,list)) return true;
    return false;
}




/// USEFUL FUNCTIONS IN THE MOVING PHASE : (Same functions as the free jumping phase but we check if the move is available)

bool movepieceline1(char Board[][17], int player, int row, int col1, int col2, int col3, int list[][2]) {
    int other=(player==1)?2:1;
    if (Board[row][col1] == Board[row][col2] && Board[row][col1] == '0' + other && Board[row][col3] == '0') {
        for (int i=0;i<8;i++){
                if (Jumppieceavailable(player,list[i][0],list[i][1],row,col3,Board)){
                Jumppiece(player,list[i][0],list[i][1],row,col3, Board);
                break;
        }
        }
        return true;
    }
    if (Board[row][col1] == Board[row][col3] && Board[row][col1] == '0' + other && Board[row][col2] == '0') {
        for (int i=0;i<8;i++){
                if (Jumppieceavailable(player,list[i][0],list[i][1],row,col2,Board)){
                Jumppiece(player,list[i][0],list[i][1],row,col2, Board);
                break;
        }
        }
        return true;
    }
    if (Board[row][col2] == Board[row][col3] && Board[row][col2] == '0' + other && Board[row][col1] == '0') {
        for (int i=0;i<8;i++){
                if (Jumppieceavailable(player,list[i][0],list[i][1],row,col1,Board)){
                Jumppiece(player,list[i][0],list[i][1],row,col1, Board);
                break;
                }
        }
        return true;
    }
    return false;
}
bool movepiececolumn1(char Board[][17], int player, int col, int row1, int row2, int row3, int list[][2]) {
    int other=(player==1)?2:1;
    if (Board[row1][col] == Board[row2][col] && Board[row1][col] == '0' + other && Board[row3][col] == '0') {
        for (int i=0;i<8;i++){
                if (Jumppieceavailable(player,list[i][0],list[i][1],row3,col,Board)){
                Jumppiece(player,list[i][0],list[i][1],row3,col, Board);
                break;
        }
        }
        return true;
    }
    if (Board[row1][col] == Board[row3][col] && Board[row1][col] == '0' + other && Board[row2][col] == '0') {
        for (int i=0;i<8;i++){
                if (Jumppieceavailable(player,list[i][0],list[i][1],row2,col,Board)){
                Jumppiece(player,list[i][0],list[i][1],row2,col, Board);
                break;
        }
        }
        return true;
    }
    if (Board[row2][col] == Board[row3][col] && Board[row2][col] == '0' + other && Board[row1][col] == '0') {
        for (int i=0;i<8;i++){
                if (Jumppieceavailable(player,list[i][0],list[i][1],row1,col,Board)){
                Jumppiece(player,list[i][0],list[i][1],row1,col, Board);
                break;
                }
        }
        return true;
    }
    return false;
}
bool completmillline1(char Board[][17], int player, int row, int col1, int col2, int col3, int list[][2]) {
    if (Board[row][col1] == Board[row][col2] && Board[row][col1] == '0' + player && Board[row][col3] == '0') {
        for (int i=0;i<8;i++){
                if (Jumppieceavailable(player,list[i][0],list[i][1],row,col3,Board)){
                Jumppiece(player,list[i][0],list[i][1],row,col3, Board);
                break;
                }
        }
        return true;
    }
    if (Board[row][col1] == Board[row][col3] && Board[row][col1] == '0' + player && Board[row][col2] == '0') {
        for (int i=0;i<8;i++){
                if (Jumppieceavailable(player,list[i][0],list[i][1],row,col2,Board)){
                Jumppiece(player,list[i][0],list[i][1],row,col2, Board);
                break;
        }
        }
        return true;
    }
    if (Board[row][col2] == Board[row][col3] && Board[row][col2] == '0' + player && Board[row][col1] == '0') {
        for (int i=0;i<8;i++){
                if (Jumppieceavailable(player,list[i][0],list[i][1],row,col1,Board)){
                Jumppiece(player,list[i][0],list[i][1],row,col1, Board);
                break;
        }
        }
        return true;
    }
    return false;
}
bool completmillcolumn1(char Board[][17], int player, int col, int row1, int row2, int row3, int list[][2]) {
    if (Board[row1][col] == Board[row2][col] && Board[row1][col] == '0' + player && Board[row3][col] == '0') {
        for (int i=0;i<8;i++){
                if (Jumppieceavailable(player,list[i][0],list[i][1],row3,col,Board)){
                Jumppiece(player,list[i][0],list[i][1],row3,col, Board);
                break;
        }
        }
        return true;
    }
    if (Board[row1][col] == Board[row3][col] && Board[row1][col] == '0' + player && Board[row2][col] == '0') {
        for (int i=0;i<8;i++){
                if (Jumppieceavailable(player,list[i][0],list[i][1],row2,col,Board)){
                Jumppiece(player,list[i][0],list[i][1],row2,col, Board);
                break;
                }
        }
        return true;
    }
    if (Board[row2][col] == Board[row3][col] && Board[row2][col] == '0' + player && Board[row1][col] == '0') {
        for (int i=0;i<8;i++){
                if (Jumppieceavailable(player,list[i][0],list[i][1],row1,col,Board)){
                Jumppiece(player,list[i][0],list[i][1],row1,col, Board);
                break;
        }
        }
        return true;
    }
    return false;
}
bool completemill1(char Board[][17], int player, int list[][2]) {
    if (completmillline1(Board, player, 0, 0, 8, 16,list)) return true;
    if (completmillline1(Board, player, 7, 3, 8, 13,list)) return true;
    if (completmillline1(Board, player, 14, 6, 8, 10,list)) return true;
    if (completmillline1(Board, player, 21, 0, 3, 6,list)) return true;
    if (completmillline1(Board, player, 21, 10, 13, 16,list)) return true;
    if (completmillline1(Board, player, 28, 6, 8, 10,list)) return true;
    if (completmillline1(Board, player, 35, 3, 8, 13,list)) return true;
    if (completmillline1(Board, player, 42, 0, 8, 16,list)) return true;
    if (completmillcolumn1(Board, player, 0, 0, 21, 42,list)) return true;
    if (completmillcolumn1(Board, player, 3, 7, 21, 35,list)) return true;
    if (completmillcolumn1(Board, player, 6, 14, 21, 28,list)) return true;
    if (completmillcolumn1(Board, player, 8, 0, 7, 14,list)) return true;
    if (completmillcolumn1(Board, player, 8, 28, 35, 42,list)) return true;
    if (completmillcolumn1(Board, player, 10, 14, 21, 28,list)) return true;
    if (completmillcolumn1(Board, player, 13, 7, 21, 35,list)) return true;
    if (completmillcolumn1(Board, player, 16, 0, 21, 42,list)) return true;

    return false; // Aucun moulin à compléter
}

bool movenearpieceline1(char Board[][17], int player, int list[][2]) {
    int rows[] = {0, 7, 14, 21, 21, 28, 35, 42};
    int cols[][3] = {
        {0, 8, 16},  // Ligne 0
        {3, 8, 13},  // Ligne 7
        {6, 8, 10},  // Ligne 14
        {0, 3, 6},   // Ligne 21 (gauche)
        {10, 13, 16},// Ligne 21 (droite)
        {6, 8, 10},  // Ligne 28
        {3, 8, 13},  // Ligne 35
        {0, 8, 16}   // Ligne 42
    };

    for (int i = 0; i < 8; i++) {
        int row = rows[i];
        if (Board[row][cols[i][0]] == '0' + player && Board[row][cols[i][1]] == '0' && Board[row][cols[i][2]] == '0') {
            for (int i=0;i<8;i++){
                    if (Jumppieceavailable(player,list[i][0],list[i][1],row,cols[i][2],Board)){
                Jumppiece(player,list[i][0],list[i][1],row,cols[i][2], Board);
                break;
        }
            }
            return true;
        }
        if (Board[row][cols[i][1]] == '0' + player && Board[row][cols[i][0]] == '0' && Board[row][cols[i][2]] == '0') {
            for (int i=0;i<8;i++){
                    if (Jumppieceavailable(player,list[i][0],list[i][1],row,cols[i][0],Board)){
                Jumppiece(player,list[i][0],list[i][1],row,cols[i][0], Board);
                break;
        }
            }
            return true;
        }
        if (Board[row][cols[i][2]] == '0' + player && Board[row][cols[i][0]] == '0' && Board[row][cols[i][1]] == '0') {
            for (int i=0;i<8;i++){
                    if (Jumppieceavailable(player,list[i][0],list[i][1],row,cols[i][1],Board)){
                Jumppiece(player,list[i][0],list[i][1],row,cols[i][0], Board);
                    break;
        }
            }
            return true;
        }
    }
    return false;
}
bool movepiecenearcolumn1(char Board[][17], int player, int list[][2]) {
    int cols[] = {0, 3, 6, 8, 10, 13, 16};
    int rows[][3] = {
        {0, 21, 42},  // Colonne 0
        {7, 21, 35},  // Colonne 3
        {14, 21, 28}, // Colonne 6
        {0, 7, 14},   // Colonne 8 (haut)
        {28, 35, 42}, // Colonne 8 (bas)
        {14, 21, 28}, // Colonne 10
        {7, 21, 35},  // Colonne 13
        {0, 21, 42}   // Colonne 16
    };

    for (int i = 0; i < 8; i++) {
        int col = cols[i];
        if (Board[rows[i][0]][col] == '0' + player && Board[rows[i][1]][col] == '0' && Board[rows[i][2]][col] == '0') {
            for (int i=0;i<8;i++){
                    if (Jumppieceavailable(player,list[i][0],list[i][1],rows[i][2],col,Board)){
                Jumppiece(player,list[i][0],list[i][1],rows[i][2],col, Board);
                break;
        }
            }
            return true;
        }
        if (Board[rows[i][1]][col] == '0' + player && Board[rows[i][0]][col] == '0' && Board[rows[i][2]][col] == '0') {
                for (int i=0;i<8;i++){
                        if (Jumppieceavailable(player,list[i][0],list[i][1],rows[i][0],col,Board)){
                Jumppiece(player,list[i][0],list[i][1],rows[i][0],col, Board);
                        }
                break;
        }
            return true;
        }
        if (Board[rows[i][2]][col] == '0' + player && Board[rows[i][0]][col] == '0' && Board[rows[i][1]][col] == '0') {
                for (int i=0;i<8;i++){
                        if (Jumppieceavailable(player,list[i][0],list[i][1],rows[i][0],col,Board)){
                Jumppiece(player,list[i][0],list[i][1],rows[i][0],col, Board);
                break;
        }
                }
            return true;
        }
    }
    return false;
}
bool movePieceStrategically1(char Board[][17], int player, int list[][2]) {
    int strategicRows[] = {0,0,7,7,14,14, 28,28,35, 35,42,42};
    int strategicCols[] = {0,16,3, 13,6, 10,6,10,3,13,0,16};
    for (int i = 0; i < 12; i++) {
        if (Board[strategicRows[i]][strategicCols[i]] == '0') {
            for (int i=0;i<8;i++){
                    if (Jumppieceavailable(player,list[i][0],list[i][1],strategicRows[i],strategicCols[i],Board)){
                Jumppiece(player,list[i][0],list[i][1],strategicRows[i],strategicCols[i], Board);
                break;
                }
            }
            return true;
        }
    }
    for (int row = 0; row < 43; row++) {
        for (int col = 0; col < 17; col++) {
            if (Board[row][col] == '0') {
                for (int i=0;i<8;i++){
                if (Jumppieceavailable(player,list[i][0],list[i][1],row,col,Board)){
                Jumppiece(player,list[i][0],list[i][1],row,col, Board);
                break;
                }
                }
                return true;
            }
        }
    }
    return false;
}
bool basicmove(char Board[][17], int player,int list[][2]){
     for (int k=0;k<8;k++){
        for (int i=0;i<43;i++){
            for(int j=0;j<17;j++){
                if (Jumppieceavailable(player,list[i][0],list[i][1],i,j,Board)){
                    Jumppiece(player,list[i][0],list[i][1],i,j,Board);
                    return true;
                }
            }
        }
     }
}


// This function is used by the machine to do the moving.
bool movepiecemachine1(char Board[][17],int player,int list[][2]){
    if (completemill1(Board,player,list)) return true;
    if (movepieceline1(Board, player, 0, 0, 8, 16,list)) return true;
    if (movepieceline1(Board, player, 7, 3, 8, 13,list)) return true;
    if (movepieceline1(Board, player, 14, 6, 8, 10,list)) return true;
    if (movepieceline1(Board, player, 21, 0, 3, 6,list)) return true;
    if (movepieceline1(Board, player, 21, 10, 13, 16,list)) return true;
    if (movepieceline1(Board, player, 28, 6, 8, 10,list)) return true;
    if (movepieceline1(Board, player, 35, 3, 8, 13,list)) return true;
    if (movepieceline1(Board, player, 42, 0, 8, 16,list)) return true;
    if (movepiececolumn1(Board, player, 0, 0, 21, 42,list)) return true;
    if (movepiececolumn1(Board, player, 3, 7, 21, 35,list)) return true;
    if (movepiececolumn1(Board, player, 6, 14, 21, 28,list)) return true;
    if (movepiececolumn1(Board, player, 8, 0, 7, 14,list)) return true;
    if (movepiececolumn1(Board, player, 8, 28, 35, 42,list)) return true;
    if (movepiececolumn1(Board, player, 10, 14, 21, 28,list)) return true;
    if (movepiececolumn1(Board, player, 13, 7, 21, 35,list)) return true;
    if (movepiececolumn1(Board, player, 16, 0, 21, 42,list)) return true;
    if (movenearpieceline1(Board,player,list)) return true;
    if (movepiecenearcolumn1(Board,player,list)) return true;
   if (movePieceStrategically1(Board,player,list)) return true;
    return false;
}







////// USEFUL FUNCTIONS IN THE PLACING PHASE :

// This  function is used by the machine to block a mill formation in a LINE.
bool tryPlacePieceInLine(char Board[][17], int player, int row, int col1, int col2, int col3) {
    int other=(player==1)?2:1;
    if (Board[row][col1] == Board[row][col2] && Board[row][col1] == '0' + other && Board[row][col3] == '0') {
        PlacePiece(player, row, col3, Board);
        return true;
    }
    if (Board[row][col1] == Board[row][col3] && Board[row][col1] == '0' + other && Board[row][col2] == '0') {
        PlacePiece(player, row, col2, Board);
        return true;
    }
    if (Board[row][col2] == Board[row][col3] && Board[row][col2] == '0' + other && Board[row][col1] == '0') {
        PlacePiece(player, row, col1, Board);
        return true;
    }
    return false;
}

// This  function is used by the machine to block a mill formation in a COLUMN.
bool tryPlacePieceInColumn(char Board[][17], int player, int col, int row1, int row2, int row3) {
    int other=(player==1)?2:1;
    if (Board[row1][col] == Board[row2][col] && Board[row1][col] == '0' + other && Board[row3][col] == '0') {
        PlacePiece(player, row3, col, Board);
        return true;
    }
    if (Board[row1][col] == Board[row3][col] && Board[row1][col] == '0' + other && Board[row2][col] == '0') {
        PlacePiece(player, row2, col, Board);
        return true;
    }
    if (Board[row2][col] == Board[row3][col] && Board[row2][col] == '0' + other && Board[row1][col] == '0') {
        PlacePiece(player, row1, col, Board);
        return true;
    }
    return false;
}


// This function is used by the machine to place a piece in a stratigic box if it's empty.
bool placePieceStrategically(char Board[][17], int player) {
    int strategicRows[] = {0,0,7,7,14,14, 28,28,35, 35,42,42};
    int strategicCols[] = {0,16,3, 13,6, 10,6,10,3,13,0,16};
    for (int i = 0; i < 12; i++) {
        if (Board[strategicRows[i]][strategicCols[i]] == '0') {
            PlacePiece(player, strategicRows[i], strategicCols[i], Board);
            return true;
        }
    }
    for (int row = 0; row < 43; row++) {
        for (int col = 0; col < 17; col++) {
            if (Board[row][col] == '0') {
                PlacePiece(player, row, col, Board);
                return true;
            }
        }
    }
    return false;
}


// This  function is used by the machine to complete a mill formation in a LINE.
bool tryCompleteMillInLine(char Board[][17], int player, int row, int col1, int col2, int col3) {
    if (Board[row][col1] == Board[row][col2] && Board[row][col1] == '0' + player && Board[row][col3] == '0') {
        PlacePiece(player, row, col3, Board);
        return true;
    }
    if (Board[row][col1] == Board[row][col3] && Board[row][col1] == '0' + player && Board[row][col2] == '0') {
        PlacePiece(player, row, col2, Board);
        return true;
    }
    if (Board[row][col2] == Board[row][col3] && Board[row][col2] == '0' + player && Board[row][col1] == '0') {
        PlacePiece(player, row, col1, Board);
        return true;
    }
    return false;
}

// This  function is used by the machine to complete a mill formation in a COLUMN.
bool tryCompleteMillInColumn(char Board[][17], int player, int col, int row1, int row2, int row3) {
    if (Board[row1][col] == Board[row2][col] && Board[row1][col] == '0' + player && Board[row3][col] == '0') {
        PlacePiece(player, row3, col, Board);
        return true;
    }
    if (Board[row1][col] == Board[row3][col] && Board[row1][col] == '0' + player && Board[row2][col] == '0') {
        PlacePiece(player, row2, col, Board);
        return true;
    }
    if (Board[row2][col] == Board[row3][col] && Board[row2][col] == '0' + player && Board[row1][col] == '0') {
        PlacePiece(player, row1, col, Board);
        return true;
    }
    return false;
}

// This  function is used by the machine to complete a mill formation.
bool tryCompleteMill(char Board[][17], int player) {
    // Vérification des lignes pour compléter un moulin
    if (tryCompleteMillInLine(Board, player, 0, 0, 8, 16)) return true;
    if (tryCompleteMillInLine(Board, player, 7, 3, 8, 13)) return true;
    if (tryCompleteMillInLine(Board, player, 14, 6, 8, 10)) return true;
    if (tryCompleteMillInLine(Board, player, 21, 0, 3, 6)) return true;
    if (tryCompleteMillInLine(Board, player, 21, 10, 13, 16)) return true;
    if (tryCompleteMillInLine(Board, player, 28, 6, 8, 10)) return true;
    if (tryCompleteMillInLine(Board, player, 35, 3, 8, 13)) return true;
    if (tryCompleteMillInLine(Board, player, 42, 0, 8, 16)) return true;

    // Vérification des colonnes pour compléter un moulin
    if (tryCompleteMillInColumn(Board, player, 0, 0, 21, 42)) return true;
    if (tryCompleteMillInColumn(Board, player, 3, 7, 21, 35)) return true;
    if (tryCompleteMillInColumn(Board, player, 6, 14, 21, 28)) return true;
    if (tryCompleteMillInColumn(Board, player, 8, 0, 7, 14)) return true;
    if (tryCompleteMillInColumn(Board, player, 8, 28, 35, 42)) return true;
    if (tryCompleteMillInColumn(Board, player, 10, 14, 21, 28)) return true;
    if (tryCompleteMillInColumn(Board, player, 13, 7, 21, 35)) return true;
    if (tryCompleteMillInColumn(Board, player, 16, 0, 21, 42)) return true;

    return false; // Aucun moulin à compléter
}


// This function is used by the machine to place a piece near one of it's own pieces in a LINE.
bool tryPlaceNearSinglePieceInLine(char Board[][17], int player) {
    // Tous les cas de lignes à vérifier
    int rows[] = {0, 7, 14, 21, 21, 28, 35, 42};
    int cols[][3] = {
        {0, 8, 16},  // Ligne 0
        {3, 8, 13},  // Ligne 7
        {6, 8, 10},  // Ligne 14
        {0, 3, 6},   // Ligne 21 (gauche)
        {10, 13, 16},// Ligne 21 (droite)
        {6, 8, 10},  // Ligne 28
        {3, 8, 13},  // Ligne 35
        {0, 8, 16}   // Ligne 42
    };

    for (int i = 0; i < 8; i++) {
        int row = rows[i];
        if (Board[row][cols[i][0]] == '0' + player && Board[row][cols[i][1]] == '0' && Board[row][cols[i][2]] == '0') {
            PlacePiece(player, row, cols[i][1], Board);
            return true;
        }
        if (Board[row][cols[i][1]] == '0' + player && Board[row][cols[i][0]] == '0' && Board[row][cols[i][2]] == '0') {
            PlacePiece(player, row, cols[i][0], Board);
            return true;
        }
        if (Board[row][cols[i][2]] == '0' + player && Board[row][cols[i][0]] == '0' && Board[row][cols[i][1]] == '0') {
            PlacePiece(player, row, cols[i][1], Board);
            return true;
        }
    }
    return false;
}

// This function is used by the machine to place a piece near one of it's own pieces in a COLUMN.
bool tryPlaceNearSinglePieceInColumn(char Board[][17], int player) {
    int cols[] = {0, 3, 6, 8, 10, 13, 16};
    int rows[][3] = {
        {0, 21, 42},  // Colonne 0
        {7, 21, 35},  // Colonne 3
        {14, 21, 28}, // Colonne 6
        {0, 7, 14},   // Colonne 8 (haut)
        {28, 35, 42}, // Colonne 8 (bas)
        {14, 21, 28}, // Colonne 10
        {7, 21, 35},  // Colonne 13
        {0, 21, 42}   // Colonne 16
    };

    for (int i = 0; i < 8; i++) {
        int col = cols[i];
        if (Board[rows[i][0]][col] == '0' + player && Board[rows[i][1]][col] == '0' && Board[rows[i][2]][col] == '0') {
            PlacePiece(player, rows[i][1], col, Board);
            return true;
        }
        if (Board[rows[i][1]][col] == '0' + player && Board[rows[i][0]][col] == '0' && Board[rows[i][2]][col] == '0') {
            PlacePiece(player, rows[i][0], col, Board);
            return true;
        }
        if (Board[rows[i][2]][col] == '0' + player && Board[rows[i][0]][col] == '0' && Board[rows[i][1]][col] == '0') {
            PlacePiece(player, rows[i][1], col, Board);
            return true;
        }
    }
    return false;
}


// This function is used by the machine to do the placing.
bool intelligenceplacing(char Board[][17], int currentPlayer) {
    // Vérification des lignes possibles pour bloquer un moulin
    int other=currentPlayer;
    if (tryCompleteMill(Board, currentPlayer)) return true;
    if (tryPlacePieceInLine(Board, other, 0, 0, 8, 16)) return true;
    if (tryPlacePieceInLine(Board, other, 7, 3, 8, 13)) return true;
    if (tryPlacePieceInLine(Board, other, 14, 6, 8, 10)) return true;
    if (tryPlacePieceInLine(Board, other, 21, 0, 3, 6)) return true;
    if (tryPlacePieceInLine(Board, other, 21, 10, 13, 16)) return true;
    if (tryPlacePieceInLine(Board, other, 28, 6, 8, 10)) return true;
    if (tryPlacePieceInLine(Board, other, 35, 3, 8, 13)) return true;
    if (tryPlacePieceInLine(Board, other, 42, 0, 8, 16)) return true;

    // Vérification des colonnes possibles pour bloquer un moulin
    if (tryPlacePieceInColumn(Board, other, 0, 0, 21, 42)) return true;
    if (tryPlacePieceInColumn(Board, other, 3, 7, 21, 35)) return true;
    if (tryPlacePieceInColumn(Board, other, 6, 14, 21, 28)) return true;
    if (tryPlacePieceInColumn(Board, other, 8, 0, 7, 14)) return true;
    if (tryPlacePieceInColumn(Board, other, 8, 28, 35, 42)) return true;
    if (tryPlacePieceInColumn(Board, other, 10, 14, 21, 28)) return true;
    if (tryPlacePieceInColumn(Board, other, 13, 7, 21, 35)) return true;
    if (tryPlacePieceInColumn(Board, other, 16, 0, 21, 42)) return true;
    if (tryPlaceNearSinglePieceInLine(Board, currentPlayer)) return true ;
    if (tryPlaceNearSinglePieceInColumn(Board, currentPlayer)) return true ;
    if (placePieceStrategically(Board, currentPlayer)) return true ;
}




/// USEFUL FUNCTIONS TO TAKEOUT PIECES :


// Takeout one the pieces that are about to form a mill in LINE.
bool lookforpiecestoremoveinLine(char Board[][17], int player, int row, int col1, int col2, int col3){
    int other=(player==1)?2:1;
    if (Board[row][col1] == Board[row][col2] && Board[row][col1] == '0' + other && Board[row][col3] == '0') {
        if (!isprotectedline(row,col1,Board))
        takeout(row, col1, Board,other);
        else if (!isprotectedline(row,col2,Board))
        takeout(row, col2, Board,other);
        return true;
    }
    if (Board[row][col1] == Board[row][col3] && Board[row][col1] == '0' + other && Board[row][col2] == '0') {
        if (!isprotectedline(row,col1,Board))
        takeout(row, col1, Board,other);
        else if (!isprotectedline(row,col2,Board))
        takeout(row, col3, Board,other);
        return true;

    }
    if (Board[row][col2] == Board[row][col3] && Board[row][col2] == '0' + other && Board[row][col1] == '0') {
        if (!isprotectedline(row,col1,Board))
        takeout(row, col3, Board,other);
        else if (!isprotectedline(row,col2,Board))
        takeout(row, col2, Board,other);
        return true;
    }
    return false;
}

// Takeout one the pieces that are about to form a mill in COLUMN.
bool lookforpiecestoremoveinColumn(char Board[][17], int player, int row1, int row2, int row3, int col){
    int other=(player==1)?2:1;
    if (Board[row1][col] == Board[row2][col] && Board[row1][col] == '0' + other && Board[row3][col] == '0') {
        if (!isprotectedcol(row1,col,Board))
        takeout(row1, col, Board,other);
    else if (!isprotectedcol(row2,col,Board))
        takeout(row2, col, Board,other);
        return true;
    }
    if (Board[row1][col] == Board[row3][col] && Board[row1][col] == '0' + other && Board[row2][col] == '0') {
        if (!isprotectedcol(row1,col,Board))
        takeout(row1, col, Board,other);
        else if (!isprotectedcol(row3,col,Board))
        takeout(row3, col, Board,other);
        return true;
    }
    if (Board[row2][col] == Board[row3][col] && Board[row2][col] == '0' + other && Board[row1][col] == '0') {
        if (!isprotectedcol(row3,col,Board))
        takeout(row3, col, Board,other);
        else if (!isprotectedcol(row2,col,Board))
        takeout(row2, col, Board,other);
        return true;
    }
    return false;
}

// Takeout one of the pieces that does not allow me to complet a mill in a line

bool lookforpiecestoremoveinLine1(char Board[][17], int player, int row, int col1, int col2, int col3){
    int other=(player==1)?2:1;
    if (Board[row][col1] == Board[row][col2] && Board[row][col1] == '0' + player && Board[row][col3] == '0'+other) {
        if (!isprotectedline(row,col3,Board))
        takeout(row, col3, Board,other);
        return true;
    }
    if (Board[row][col1] == Board[row][col3] && Board[row][col1] == '0' + player && Board[row][col2] == '0'+other) {
        if (!isprotectedline(row,col2,Board))
        takeout(row, col2, Board,other);
        return true;

    }
    if (Board[row][col2] == Board[row][col3] && Board[row][col2] == '0' + player && Board[row][col1] == '0'+other) {
        if (!isprotectedline(row,col1,Board))
        takeout(row, col1, Board,other);
        return true;
    }
    return false;
}

// takout one of the pieces that does not allow me to complet a mill in a column
bool lookforpiecestoremoveinColumn1(char Board[][17], int player, int row1, int row2, int row3, int col){
    int other=(player==1)?2:1;
    if (Board[row1][col] == Board[row2][col] && Board[row1][col] == '0' + player && Board[row3][col] == '0'+other) {
        if (!isprotectedcol(row3,col,Board))
        takeout(row3, col, Board,other);
        return true;
    }
    if (Board[row1][col] == Board[row3][col] && Board[row1][col] == '0' + player && Board[row2][col] == '0'+other) {
        if (!isprotectedcol(row1,col,Board))
        takeout(row2, col, Board,other);
        return true;
    }
    if (Board[row2][col] == Board[row3][col] && Board[row2][col] == '0' + player && Board[row1][col] == '0'+other) {
        if (!isprotectedcol(row3,col,Board))
        takeout(row1, col, Board,other);
        return true;
    }
    return false;
}



// if not then i takeout one of them.
bool removeone(int player, char Board[][17]){
    int other=(player==1)?2:1;
    for (int i=0;i<43;i++){
        for (int j=0;j<17;j++){
            if (Board[i][j]=='0'+other && !isprotectedline(i,j,Board) && !isprotectedcol(i,j,Board)){
                takeout(i,j,Board,player);
                return true;
            }
        }
    }
}

// This function is used by the machine to take a piece out.
bool lookforpiecetoremove(char Board[][17], int player){
    if (lookforpiecestoremoveinLine(Board, player, 0, 0, 8, 16)) return true;
    if (lookforpiecestoremoveinLine(Board, player, 7, 3, 8, 13)) return true;
    if (lookforpiecestoremoveinLine(Board, player, 14, 6, 8, 10)) return true;
    if (lookforpiecestoremoveinLine(Board, player, 21, 0, 3, 6)) return true;
    if (lookforpiecestoremoveinLine(Board, player, 21, 10, 13, 16)) return true;
    if (lookforpiecestoremoveinLine(Board, player, 28, 6, 8, 10)) return true;
    if (lookforpiecestoremoveinLine(Board, player, 35, 3, 8, 13)) return true;
    if (lookforpiecestoremoveinLine(Board, player, 42, 0, 8, 16)) return true;
    if (lookforpiecestoremoveinColumn(Board, player, 0, 0, 21, 42)) return true;
    if (lookforpiecestoremoveinColumn(Board, player, 3, 7, 21, 35)) return true;
    if (lookforpiecestoremoveinColumn(Board, player, 6, 14, 21, 28)) return true;
    if (lookforpiecestoremoveinColumn(Board, player, 8, 0, 7, 14)) return true;
    if (lookforpiecestoremoveinColumn(Board, player, 8, 28, 35, 42)) return true;
    if (lookforpiecestoremoveinColumn(Board, player, 10, 14, 21, 28)) return true;
    if (lookforpiecestoremoveinColumn(Board, player, 13, 7, 21, 35)) return true;
    if (lookforpiecestoremoveinColumn(Board, player, 16, 0, 21, 42)) return true;
    if (lookforpiecestoremoveinLine1(Board, player, 0, 0, 8, 16)) return true;
    if (lookforpiecestoremoveinLine1(Board, player, 7, 3, 8, 13)) return true;
    if (lookforpiecestoremoveinLine1(Board, player, 14, 6, 8, 10)) return true;
    if (lookforpiecestoremoveinLine1(Board, player, 21, 0, 3, 6)) return true;
    if (lookforpiecestoremoveinLine1(Board, player, 21, 10, 13, 16)) return true;
    if (lookforpiecestoremoveinLine1(Board, player, 28, 6, 8, 10)) return true;
    if (lookforpiecestoremoveinLine1(Board, player, 35, 3, 8, 13)) return true;
    if (lookforpiecestoremoveinLine1(Board, player, 42, 0, 8, 16)) return true;
    if (lookforpiecestoremoveinColumn1(Board, player, 0, 0, 21, 42)) return true;
    if (lookforpiecestoremoveinColumn1(Board, player, 3, 7, 21, 35)) return true;
    if (lookforpiecestoremoveinColumn1(Board, player, 6, 14, 21, 28)) return true;
    if (lookforpiecestoremoveinColumn1(Board, player, 8, 0, 7, 14)) return true;
    if (lookforpiecestoremoveinColumn1(Board, player, 8, 28, 35, 42)) return true;
    if (lookforpiecestoremoveinColumn1(Board, player, 10, 14, 21, 28)) return true;
    if (lookforpiecestoremoveinColumn1(Board, player, 13, 7, 21, 35)) return true;
    if (lookforpiecestoremoveinColumn1(Board, player, 16, 0, 21, 42)) return true;
    if (removeone(player,Board)) return true;
    return false;
}






void PlayerVsMachine1() {
    printf("\t\tHERE IS THE BOARD THE ACTION WILL TAKE PLACE ON \n");
    Display(Board);
    getch();
    printf("\t\tYOU WILL BE NAMED THE_PLAYER_1 AND THE MACHINE WILL BE NAMED THE_PLAYER_2.\n");
    getch();
    printf("\t\tTHE PLAYER THAT WILL MAKE THE FIRST MOVE IS GOING TO BE CHOSEN RANDOMLY. \n");
    getch();
    int currentPlayer = Firstmovemaker();  // Choisir aléatoirement le premier joueur
    int i, j, a, b;
    int c = 0;  // Compteur de tours pour le placement des pièces
    int millformed[16][3][2];  // Tableau pour suivre les moulins formés
    memset(millformed, -1, sizeof(millformed));  // Initialiser à -1
    getch();
    printf("\t\t\nWE WOULD LIKE TO INFORM YOU THAT THE AVAILABLE BOXES HAVE THE FOLLOWING INDEXES :\n");
    showavailibleboxes();
    srand(time(NULL));
    // THE PLACING PHASE :
    while (c < 18) {
        if (currentPlayer == 1) {
            printf("\t\tPLAYER_1, CHOOSE THE BOX YOU WANNA PLACE YOUR PIECE IN (ROW AND COL): ");
            int validinput = 0;
            while (!validinput) {
                scanf("%d %d", &i, &j);
                if (TheMoveIsValid(i, j, Board)) {
                    PlacePiece(currentPlayer, i, j, Board);
                    c++;
                    Display(Board);
                    validinput = 1;
                } else {
                    printf("\t\tINVALID MOVE, TRY AGAIN.\n\n");
                }
            }
        } else {
            printf("\t\tMACHINE (PLAYER_2) IS MAKING ITS MOVE...\n");
            intelligenceplacing(Board, currentPlayer);
            c++;
            Display(Board);
        }
        if (c > 4) {
            if (currentPlayer==1){
            int list[3] = {0, 0, 0};
            millVerifier(list, Board,currentPlayer,millformed,possibleMills);
            int a=(currentPlayer==1)?2:1;
            if (list[currentPlayer] > 0 && !allprotected(a,Board)) {
                if (list[currentPlayer]>0){
                    printf("\t\tPLAYER_1, YOU HAVE FORMED A MILL! CHOOSE A PIECE OF YOUR CHALLENGER TO REMOVE: ");
                    int validRemove = 0;
                    while (!validRemove) {
                        scanf("%d %d", &a, &b);
                        if (IsValidRemove(a, b, Board, currentPlayer)) {
                            takeout(a, b, Board, currentPlayer);
                            printf("\n\t\tBOARD AFTER REMOVING THE PIECE:\n\n");
                            Display(Board);
                            validRemove = 1;
                        } else {
                            printf("\t\tINVALID OPTION, CHOOSE AGAIN.\n");
                        }
                    }

                }
            }
            }
                else{
                int list[3] = {0, 0, 0};
                millVerifier(list, Board,currentPlayer,millformed,possibleMills);
                int a=(currentPlayer==1)?2:1;
                if (list[currentPlayer] > 0 && !allprotected(a,Board)) {
                    printf("\t\tMACHINE (PLAYER_2) HAS FORMED A MILL AND IS REMOVING A PIECE...\n");
                    lookforpiecetoremove(Board, currentPlayer);
                    printf("\n\t\tBOARD AFTER REMOVING THE PIECE:\n\n");
                    Display(Board);
                }
                }
            }

        // change the player
        currentPlayer = (currentPlayer == 1) ? 2 : 1;
    }
    printf("\t\tYOU ARE DONE WITH PLACING THE NINE PIECES.\n\n");

    // THE MOVING PHASE :
    while (!gameover(currentPlayer, Board)) {
            if (currentPlayer == 1) {
                if (countpieces(currentPlayer, Board) == 3) {
                printf("THE_PLAYER_1 YOU ONLY GOT 3 PIECES LEFT YOU ARE FREE TO JUMP\n");
                int startrow, startcol, endrow, endcol;
                int validfirst = 0;
                printf("\t\tENTER THE COORDINATES OF THE PIECE YOU WANT TO JUMP (ROW COL): \n");
                while (!validfirst) {
                    scanf("%d %d", &startrow, &startcol);
                    if (validecoordinatesstart(startrow, startcol, Board, currentPlayer)) {
                        printf("\t\tENTER THE COORDINATES OF THE TARGET POSITION (ROW COL): \n");
                        int validsecond = 0;
                        while (!validsecond) {
                            scanf("%d %d", &endrow, &endcol);
                            if (Board[endrow][endcol]=='0') {
                                    Jumppiece(currentPlayer, startrow, startcol, endrow, endcol, Board);
                                    printf("\t\tTHE BOARD AFTER JUMPING THE PIECE:\n");
                                    Display(Board);
                                    int list[3] = {0, 0, 0};
                                    millVerifier(list, Board, currentPlayer, millformed, possibleMills);
                                    validsecond = 1;
                                    int a=(currentPlayer==1)?2:1;
                                    if (list[currentPlayer] > 0 && !allprotected(a,Board)) {
                                        int list1[3]={0,0,0};
                                        millVerifier(list1, Board,a,millformed,possibleMills);
                                        if ( countpieces(a,Board)==3 && list1[a]>0 ){
                                            printf("\t\tPLAYER_1, YOU HAVE FORMED A MILL! BUT UNFORTUNATELLY YOU CAN NOI TAKE A PIECE OF THE MACHINE OUT BECAUSE IT ONLY GOT 3 LEFT AND THEY FORM A MILL");
                                            break;
                                        }else{
                                        printf("\t\tPLAYER_1, YOU HAVE FORMED A MILL! CHOOSE A PIECE OF YOUR CHALLENGER TO REMOVE: ");
                                        int validRemove = 0;
                                        while (!validRemove) {
                                            scanf("%d %d", &i, &j);
                                            if (IsValidRemove(i, j, Board, currentPlayer)) {
                                                takeout(i, j, Board, currentPlayer);
                                                printf("\n\t\tBOARD AFTER REMOVING THE PIECE:\n\n");
                                                Display(Board);
                                                validRemove = 1;
                                            } else {
                                                printf("\t\tINVALID OPTION, CHOOSE AGAIN.\n");
                                            }
                                        }
                                    }

                            }
                            } else {
                                printf("\t\tINVALID OPTION, CHOOSE AGAIN.\n");
                            }
                        }
                        validfirst = 1;
                    } else {
                        printf("\t\tINVALID OPTION, CHOOSE AGAIN.\n");
                    }
                }
                }


                if (countpieces(currentPlayer, Board) != 3) {
                printf("\t\tPLAYER_1, IT'S YOUR TURN TO MOVE.\n");
                int startrow, startcol, endrow, endcol;
                int validfirst = 0;
                printf("\t\tENTER THE COORDINATES OF THE PIECE YOU WANT TO MOVE (ROW COL): \n");
                while (!validfirst) {
                    scanf("%d %d", &startrow, &startcol);
                    if (validecoordinatesstart(startrow, startcol, Board, currentPlayer)) {
                        printf("\t\tENTER THE COORDINATES OF THE TARGET POSITION (ROW COL): \n");
                        int validsecond = 0;
                        while (!validsecond) {
                            scanf("%d %d", &endrow, &endcol);
                            if (validecoordinatesend(endrow, endcol, Board)) {
                                if (Jumppieceavailable(currentPlayer, startrow, startcol, endrow, endcol, Board)) {
                                    Jumppiece(currentPlayer, startrow, startcol, endrow, endcol, Board);
                                    printf("\t\tTHE BOARD AFTER JUMPING THE PIECE:\n");
                                    Display(Board);
                                    int list[3] = {0, 0, 0};
                                    millVerifier(list, Board, currentPlayer, millformed, possibleMills);
                                    validsecond = 1;
                                    int a=(currentPlayer==1)?2:1;
                                    if (list[currentPlayer] > 0 && !allprotected(a,Board)) {
                                        int list1[3]={0,0,0};
                                        millVerifier(list1, Board,a,millformed,possibleMills);
                                        if ( countpieces(a,Board)==3 && list1[a]>0 ){
                                            printf("\t\tPLAYER_1, YOU HAVE FORMED A MILL! BUT UNFORTUNATELLY YOU CAN NOI TAKE A PIECE OF THE MACHINE OUT BECAUSE IT ONLY GOT 3 LEFT AND THEY FORM A MILL");
                                            break;
                                        }else{
                                        printf("\t\tPLAYER_1, YOU HAVE FORMED A MILL! CHOOSE A PIECE OF YOUR CHALLENGER TO REMOVE: ");
                                        int validRemove = 0;
                                        while (!validRemove) {
                                            scanf("%d %d", &i, &j);
                                            if (IsValidRemove(i, j, Board, currentPlayer)) {
                                                takeout(i, j, Board, currentPlayer);
                                                printf("\n\t\tBOARD AFTER REMOVING THE PIECE:\n\n");
                                                Display(Board);
                                                validRemove = 1;
                                            } else {
                                                printf("\t\tINVALID OPTION, CHOOSE AGAIN.\n");
                                            }
                                        }
                                    }
                    }
                            }
                            } else {
                                printf("\t\tINVALID OPTION, CHOOSE AGAIN.\n");
                            }
                        }
                        validfirst = 1;
                    } else {
                        printf("\t\tINVALID OPTION, CHOOSE AGAIN.\n");
                    }
                }
                }
            } else {
                // Tour de l'IA avancée
                if (countpieces(currentPlayer, Board) == 3) {
                    printf("\t\tMACHINE (PLAYER_2) HAS ONLY 3 PIECES LEFT ? IT IS FREE TO JUMP...\n");
                    getch();
                    int list2[9][2]={{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};
                int list3[9][2]={{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};
                int i2 = 0, i3 = 0;
                for (int j = 0; j < 43; j++) {
                    for (int k = 0; k < 17; k++) {
                        if (Board[j][k] == '2') {
                            list2[i2][0] = j;
                            list2[i2][1] = k;
                            i2++;
                        }
                         if (Board[j][k] == '0') {
                            list3[i3][0] = j;
                            list3[i3][1] = k;
                            i3++;
                        }
                    }
                }
                movepiecemachine(Board,currentPlayer,list2);
                printf("\t\tTHE BOARD AFTER THE MACHINE'S MOVE:\n");
                Display(Board);
                int list[3] = {0, 0, 0};
                millVerifier(list, Board, currentPlayer, millformed, possibleMills);
                int a=(currentPlayer==1)?2:1;
                if (list[currentPlayer] > 0 && !allprotected(a,Board)) {
                    int list2[3]={0,0,0};
                    millVerifier(list2, Board, a, millformed, possibleMills);
                    if (countpieces(a,Board)>3 || (countpieces(a,Board)==3 && list[a]==0)){
                        printf("\t\tMACHINE (PLAYER_2) HAS FORMED A MILL AND IS REMOVING A PIECE...\n");
                        lookforpiecetoremove(Board, currentPlayer);
                        printf("\n\t\tBOARD AFTER REMOVING THE PIECE:\n\n");
                        Display(Board);
                    }
               }
        }
        if (countpieces(currentPlayer, Board) != 3) {
                printf("\t\tMACHINE (PLAYER_2) IS MAKING ITS MOVE...\n");
                int list2[9][2]={{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};
                int list3[9][2]={{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};
                int i2 = 0, i3 = 0;
                for (int j = 0; j < 43; j++) {
                    for (int k = 0; k < 17; k++) {
                        if (Board[j][k] == '2') {
                            list2[i2][0] = j;
                            list2[i2][1] = k;
                            i2++;
                        }
                         if (Board[j][k] == '0') {
                            list3[i3][0] = j;
                            list3[i3][1] = k;
                            i3++;
                        }
                    }
                }
                movepiecemachine1(Board,currentPlayer,list2);
                printf("\t\tTHE BOARD AFTER THE MACHINE'S MOVE:\n");
                Display(Board);
                int list[3] = {0, 0, 0};
                millVerifier(list, Board, currentPlayer, millformed, possibleMills);
                int a=(currentPlayer==1)?2:1;
                if (list[currentPlayer] > 0 && !allprotected(a,Board)) {
                    int list2[3]={0,0,0};
                    millVerifier(list2, Board, a, millformed, possibleMills);
                    if (countpieces(a,Board)>3 || (countpieces(a,Board)==3 && list[a]==0)){
                        printf("\t\tMACHINE (PLAYER_2) HAS FORMED A MILL AND IS REMOVING A PIECE...\n");
                        lookforpiecetoremove(Board, currentPlayer);
                        printf("\n\t\tBOARD AFTER REMOVING THE PIECE:\n\n");
                        Display(Board);
                    }
               }
        }
    }
    currentPlayer = (currentPlayer == 1) ? 2 : 1;
    }
    printf("\n\t\t\t   GAME OVER!!!!!! THE_PLAYER_%d HAS WON ! \n",3-currentPlayer);
    Again();
}




void Again(){
    printf("\t\t      1 - PLAY AGAIN \n");
    printf("\t\t      2 - QUIT :( \n\n");
    int a;
    scanf("%d",&a);
    if (a==1)
        Play_again();
    if (a==2)
        Quit();
    while (a!=1 && a!=2){
       printf("\t\tINVALID OPTION, TRY AGAIN!\n\n");
       scanf("%d",&a);
    }
}

void Play_again(){
    printf("\n\n\t\t--> PLAY AGAIN \n\n");
    printf("\t\t      1 - PLAY 1 V 1 \n");
    printf("\t\t      2 - PLAY 1 V COMPUTER 0 \n");
    printf("\t\t      3 - PLAY 1 V COMPUTER 1 \n\n");
    int b;
    printf("\t  WHAT IS YOUR CHOICE : ");
    scanf("%d",&b);
    if( b==1)
       Player_Player();
    if (b==2)
        PlayerVsMachine0();
    if (b==3)
        PlayerVsMachine1();
    while (b!=1 && b!=2 && b!=3){
       printf("\t\tINVALID OPTION, TRY AGAIN!\n\n");
       scanf("%d",&b);
    }
    }


int main()
{
    introduction();
    return 0;
}








