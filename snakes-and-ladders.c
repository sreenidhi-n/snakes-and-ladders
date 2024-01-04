// Snakes and Ladders on Terminal (Command-Line) because GUI messed us up
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

typedef struct snake_ladder{
    int length;
    int beg;
	int end;  // Starting and ending of the snake/ladder
}snake_ladder;

typedef struct token{
    char name[50]; // Player Name
    int pos; // token position
}token;

void instructions()
{
    printf("INSTRUCTIONS: \n");
    printf("Each player's token starts on square 1 and goes in an increasin order to square 100, encountering snakes and ladders on the way.\n");
    printf("Take it in turns to roll the dice. Move your counter forward the number of spaces shown on the dice.\n");
    printf("If your token lands at the bottom of a ladder, you can move up to the top of the ladder.\n");
    printf("If your token lands on the head of a snake, you must slide down to the bottom of the snake.\n");
    printf("The first player to get to the last square (100) is the winner.\n");
    printf("Most importantly, remember to have fun!\n");
}

int isExist(snake_ladder item, snake_ladder arr[], int size)  // To prevent overlapping of any structures (snakes or ladders)
{
    int flag = -1;
    for(int i=0;i<size;i++)
    {
        if(item.beg== arr[i].beg || item.end==arr[i].beg)
        {
            flag = 1;
            return flag;
        }
        return flag;
    }
}

void snake(snake_ladder snakes[],int size)  //Snakes are placed differently every game.
{
    for(int i=0;i<size;i++)
    {
        do
        {
            snakes[i].beg = (rand() % 89) + 11; // So that the snake wont land on 100 or land within the first 10 squares.
            snakes[i].length = ((rand() % (snakes[i].beg - 10)) + 10); // must be less than the beginning square
            snakes[i].end = snakes[i].beg - snakes[i].length; // Where the snake would end.

        }while(isExist(snakes[i],snakes,i)>0);
    }
}

void ladder(snake_ladder ladders[], snake_ladder snakes[], int size)  // Ladders, like snakes, are placed differenly each game.
{
    int len;
    for(int i=0;i<size;i++)
    {
        do		
        {
            ladders[i].beg = (rand() % 89) + 2;  //Starts around square 11, length is atleast 2
            len = 99 - ladders[i].beg;
			if (len < 10) {
				ladders[i].length = 9;
			}
			else if (len == 10) {
				ladders[i].length = 10;
			}
			else {
				ladders[i].length = ((rand() % (len - 9) + 10)); // Ladder can be anywhere from 10-89 in length

            }
            ladders[i].end = ladders[i].beg + ladders[i].length;
    	}while((isExist(ladders[i],ladders,i)>0)||(isExist(ladders[i], snakes, 7) > 1));
	}
}

void Snake_Display(snake_ladder snakes[]) {
	printf("Snakes at squares: ");
	for (int i = 0;i < 7;i++) { // We are taking 7 snakes for the board
		
		printf("%d - %d, ", snakes[i].beg, snakes[i].end);
	}
}

void Ladder_Display(snake_ladder ladders[]) {
	printf("\nLadders at squares: ");
    for (int i = 0;i < 7;i++) { // We are considering 7 ladders to balance the game
		printf("%d - %d, ", ladders[i].beg, ladders[i].end);
	}
	printf("\n");
}

int winCondition(int position) {  // Did you win?
	int flag;
	if (position >= 100) flag = 1; 
	else {
		flag = 0; 
	}
	return flag;
}

void player_disp(token *tokens, int count)  // Show positions of players. 
{
    for(int i=0;i<count;i++)
    {
        printf("\nPlayer %d is: %s\n",(i+1),(tokens+i)->name);
        printf("Position of player %d: %d\n",(i+1),(tokens+i)->pos);
    }
	return;
}

int Snake(int position, snake_ladder snakes[]) {  // To check if the token landed on a snake and display appropriate message.
    int pos;
    pos = position;
	for (int i = 0;i<7;i++) {
		if (pos == snakes[i].beg) {
			printf("You landed on a Snake at Position: %d\nNew Position: %d\n",snakes[i].beg,snakes[i].end);
			pos = snakes[i].end; 
		}
	}
	return pos;
}

int Ladder(int position, snake_ladder ladders[]) {   // To check if the token landed on a ladder and diplay appropriate message.
	int pos;
    pos = position;
	for (int i = 0;i<7;i++) {
		if (position == ladders[i].beg) {
			printf("You landed on a Ladder at Position: %d\nNew Position: %d\n", ladders[i].beg, ladders[i].end);
			pos = ladders[i].end;
		}
	}
	return pos; 
}

void SaveAndExitGame(int count, int player, token* players, snake_ladder snakes[], snake_ladder ladders[]) {
	FILE *reference = fopen("reference.txt", "w"); 
	int i;
	FILE *file;

	if(reference!=NULL) fclose(reference);  // Checking if the file-pointer is not null.

	file = fopen("reference.txt", "w"); 

	fprintf(file, "%d\n", count);  // Number of players  
	fprintf(file, "%d\n", player); // Gets current player

	//player names and position
	for (i = 0;i < count;i++) {

		fprintf(file, "%s %d\n", (players + i)->name, (players + i)->pos); 
	}

	//Positions of snakes and ladders
	for (i = 0;i < 7;i++) {

		fprintf(file, "%d %d %d\n", (snakes + i)->beg, (snakes + i)->length, (snakes + i)->end);
	}

	for (i = 0;i < 7;i++) {

		fprintf(file, "%d %d %d\n", (ladders + i)->beg, (ladders + i)->length, (ladders + i)->end);
	}
	fclose(file);
}

int LoadGame(int *count, int *player, token* players, snake_ladder snakes[], snake_ladder ladders[]) 
{
	FILE *reference = fopen("reference.txt", "r");
	int i;
	if (reference == NULL) {
		printf("No Files Currently Stored\n");
		return -1;
	}
	printf("File Opened\n");
	if (reference != NULL) {
		fscanf(reference, "%d", count);	
		fscanf(reference, "%d", player); 
		for (i = 0; i < *count;i++) {
		
			fscanf(reference, "%s", (players + i)->name);
			fscanf(reference, "%d", &(players + i)->pos);

		}
		player_disp(players, *count);
		for (i = 0;i < 7;i++) {

			fscanf(reference, "%d %d %d", &(snakes + i)->beg, &(snakes + i)->length, &(snakes + i)->end);
		
		}
		for (i = 0;i < 7;i++) {

			fscanf(reference, "%d %d %d", &(ladders + i)->beg, &(ladders + i)->length, &(ladders + i)->end);
			
		}
		fclose(reference);
	}
	return 1; 
}


void board(int position, char name[])
{
    int temp,temp1,res;
    int flag = 0;
	system("cls");
    printf("-------------------------------------------\n");
	printf("\tSnakes and Ladders!\n");
	printf("-------------------------------------------\n");
    for(int i=10;i>0;i--)
    {
        temp = i-1;
        if(flag%2==0)
        {
            temp1=0;
            for(int j=10;j>=1;j--)
            {
                res = (i*j)+(temp*temp1++);
                if(position==res)
                {
                    printf("%s\t",name);
                }
                else
                {
                    printf("%d\t",res);
                }
            }
            flag++;
        }
        else
        {
            temp1=9;
			for(int j=1;j<=10;j++)
			{
				res=(i*j)+(temp*temp1--);

				if(position==res)
					printf("%s\t",name);
				else
					printf("%d\t",res);
			}
			flag++;
        }
        printf("\n\n");
    }
}

void player_name(token *tokens, int count)   // Player names
{
    for(int i=0;i<count;i++)
    {
        printf("Enter player %d's name: ",i+1);
        scanf("%s",(tokens + i)->name);
        (tokens+i)->pos = 1; 
    }
}

int dice_roll() 
{
	int roll;
    roll =  (rand()%6) + 1;
	return roll;
}

void gameplay(token *players, snake_ladder snakes[], snake_ladder ladders[], int count, int currentPlayer) {
	int choice;
	int flag=0;
	int diceRoll;
	int Player = 0;
	currentPlayer = Player;
	int exitChoice;
	do {
		printf("1. Roll Dice\n");
		printf("2. Exit\n");
		scanf("%d", &choice);
		switch (choice) {
		case 1:
			diceRoll = dice_roll();
			(players + Player)->pos += diceRoll;
			board((players+Player)->pos,(players+Player)->name);
			Snake_Display(snakes);
			Ladder_Display(ladders);
			printf("\n");
			printf("You rolled: %d\n", diceRoll);
			printf("%s is now at position : %d\n", (players + Player)->name, (players + Player)->pos);
			(players + Player)->pos = Ladder((players + Player)->pos, ladders);
            (players + Player)->pos = Snake((players + Player)->pos, snakes);
			flag = winCondition((players + Player)->pos); 
			if (flag > 0) 
            {
				printf("\n%s IS THE WINNER!! \n", (players + Player)->name);
				return;
			}	
			if (Player == (count - 1)) {
				Player = 0;				
			}
			else{
				Player++;
			} 

			break;
		case 2:
			printf("Would you Like to Save Before Quitting?\n");
			printf("1. Save File\n");
			printf("2. Quit Without Saving\n");
			scanf("%d", &exitChoice);
			switch (exitChoice) {
			case 1:
				SaveAndExitGame(count,currentPlayer,players,snakes,ladders);
				exit(0);
				break;
			case 2:
				printf("Thanks for playing!\n");
				exit(0);
				break;
			default:
				printf("Invalid Choice\n");
				break;
			}
		break;
		}
	}while(choice!=0);
}
int GetPlayerCount() {
	int count;
	FILE *file;
	FILE *reference = fopen("reference.txt", "r");

	if (reference != NULL) {		
		file = fopen("reference.txt", "r"); 
		if (file != NULL) {
			fscanf(file, "%d", &count);
			fclose(file);
			return count; //returns the player count
			fclose(file);
		}
	}
}

void main() {
	token *players;
	int playerCount,choice; 
	int currentPlayer = 0;
	snake_ladder snakes[7]; 
	snake_ladder ladders[7];
	int loaded;
	FILE* fileChoice = fopen("reference.txt","r");
	do {
		printf("-------------------------------------------\n");
		printf("\tSnakes and Ladders!\n");
		printf("-------------------------------------------\n");
		printf("%-14s New Game\n", "1. ");
		printf("%-14s Instructions\n", "2. ");
		printf("%-14s Load an existing game\n", "3. ");
		printf("%-14s Exit Game\n", "4. ");
		printf("-------------------------------------------\n");

		scanf("%d", &choice);
		switch (choice) {
		case 1:
			do {
				printf("How many players? Minimum - 2 Maximum - 6\n");
				scanf("%d", &playerCount);
			} while (playerCount> 6 || playerCount<2);
			players = (token *)malloc(playerCount*sizeof(token)); 
			player_name(players,playerCount);
			// generate the board
			snake(snakes, 7);
			ladder(ladders, snakes, 7);
			Snake_Display(snakes);
			Ladder_Display(ladders);
			//start game
			gameplay(players, snakes, ladders,playerCount,currentPlayer);
			break;
		case 2:
			instructions();
			break;
		case 3:
			printf("Loading your game...\n");
			playerCount = GetPlayerCount(fileChoice);
			players = (token*) malloc(playerCount*sizeof(token));
			loaded=LoadGame(&playerCount, &currentPlayer, players, snakes, ladders);
			if (loaded > 0) {
				printf("\nGame Loaded\n");
				Snake_Display(snakes);
				Ladder_Display(ladders);
				gameplay(players, snakes, ladders, playerCount, currentPlayer);
			}
			break;
		case 4:
			printf("\nThanks for Playing!!\n");
            exit(0);
			break;
		}
	} while (choice != 0); 
	getch();
}

// Future development:
// Save and exit the existing game, load the saved game - attempting this using file handling. DONE
// Researching methods to retain same board - display and clear messages accordingly. DONE
// Any other suggestions to better the project are appreciated :)