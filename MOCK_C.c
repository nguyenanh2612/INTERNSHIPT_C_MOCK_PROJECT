/**************************************************************************************
 * FPT Software Academy - 2024 
 * Class: HCM24_CPL_EMB_01 
 * Project name: MOCK C 
 * Author: Tran Quang Nguyen Anh
 * 
 * Requirement: magic number game
 **************************************************************************************/

/**************************************************************************************
 *                                USED LIBRARY
 **************************************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/************************************************************************************************
 *                                 DEFINE VARIABLE
 ***********************************************************************************************/
/**
 * @file
 * @brief Describes global variables and related definitions.
 */

/** @def RESET
 *  @brief Reset all attributes (text color, background color, etc.) to default.
 *  Use this after applying text color or background color to return to default settings.
 */
#define RESET   "\033[0m"

/** @def RED
 *  @brief Set text color to red.
 */
#define RED     "\033[31m"

/** @def GREEN
 *  @brief Set text color to green.
 */
#define GREEN   "\033[32m"

/** @def YELLOW
 *  @brief Set text color to yellow.
 */
#define YELLOW  "\033[33m"

/** @def BLUE
 *  @brief Set text color to blue.
 */
#define BLUE    "\033[34m"

/** @def MAGENTA
 *  @brief Set text color to magenta.
 */
#define MAGENTA "\033[35m"

/** @def CYAN
 *  @brief Set text color to cyan.
 */
#define CYAN    "\033[36m"

/** @def WHITE
 *  @brief Set text color to white.
 */
#define WHITE   "\033[37m"

/** @def BG_WHITE
 *  @brief Set background color to white.
 */
#define BG_WHITE "\033[48;5;255m"

/**
 * @def LENGTH_NUMBER
 * @brief The length of the magic number and the input string number for comparison.
 * @details Used for comparing the magic number and the input string number.
 */
#define LENGTH_NUMBER  6 

/**
 * @def LENGTH_STRING_MAX
 * @brief The maximum length of the user name input string.
 */
#define LENGTH_STRING_MAX  20

/**
 * @struct User
 * @brief Structure to hold user information.
 * @details Contains user name, total guess count, right guess count and record time of play match.
 */
typedef struct {
    char userName[LENGTH_STRING_MAX+2];
    int totalGuess;
    int rightGuess;
    float timeRecord; 
} User;

/**
 * @struct player_table
 * @brief Structure to hold 10 highest player 
 * @details Contains player name, lucky ratio, time record. 
 */
typedef struct {
    char playerName[10][LENGTH_STRING_MAX+1];
    float luckyRatio[10]; 
    float timeRecord[10]; 
} player_table;

/**
 * @var magic_number
 * @brief Stores the random string number.
 * @details A character array with a length of LENGTH_NUMBER + 1 (6 digits + 1 null terminator).
 */
char g_magic_number[LENGTH_NUMBER + 1];  

/**
 * @var input_number
 * @brief Stores the input string number for comparison.
 * @details A character array with a length of LENGTH_NUMBER + 2 (7 digits + 1 null terminator + 1 extra character if needed).
 */
char g_input_number[LENGTH_NUMBER + 2];

/**
 * @var common_char
 * @brief Stores the char in common.
 * @details A character array with a length of LENGTH_NUMBER + 1 (6 digits + 1 null terminator).
 */
char g_common_char[LENGTH_NUMBER+1];

/**
 * @brief Array storing the magic numbers for the last 10 users.
 *
 * This array holds the magic numbers guessed by the last 10 users. Each element is a string
 * with a length of LENGTH_NUMBER + 1 to accommodate the null terminator.
 */
char g_magic_number_old[10][LENGTH_NUMBER + 1]; 

/**
 * @brief Array storing the common character sequences for the last 10 users.
 *
 * This array holds the common character sequences for the last 10 users. Each element is a string
 * with a length of LENGTH_NUMBER + 1 to accommodate the null terminator.
 */
char g_common_char_old[10][LENGTH_NUMBER + 1];

/**
 * @brief Array indicating if the last 10 users guessed the magic number correctly. (completing guess task flag)
 *
 * This array holds integer values indicating whether each of the last 10 users guessed
 * the magic number correctly. A value of 1 indicates a correct guess all digits, while 0 indicates
 * an incorrect guess.
 */
int  g_is_all_correct_string[10];

/**
 * @brief Global flag indicating administrative status.
 *
 * This global variable is used to check whether the current user has administrative privileges.
 * The value of `g_check_admin` is typically set by the authentication or authorization system.
 * 
 * @note The variable should be set to a non-zero value if the user is an administrator,
 *       and to zero if the user does not have administrative privileges.
 */
int g_check_admin; 

/************************************************************************************************
 *                                 DEFINE FUNCTION
 ***********************************************************************************************/

/**
 * @brief Displays the user interface menu for the Guess Game.
 *
 * This function prints the welcome message, menu options, and a special admin
 * message if the user is an admin. The printed text includes colored ASCII art.
 *
 * @param user The User structure containing the user's information. The function
 *             checks if the user's name is "nhathanh" to display the admin message.
 */
void printf_interface(User user); 

/**
 * @brief Prompts the user to enter a request and validates the input.
 *
 * This function prompts the user to enter a request, validates the input to ensure
 * it is a single character between '1' and '3'. If the input is invalid, it will
 * prompt the user to enter the request again until a valid input is received.
 *
 * @param userRequest A character array to store the user's request. The array must have a size of at least 3 characters.
 */
void enter_request(char userRequest[3]); 

/**
 * @brief Inputs the user name.
 * @param user Pointer to the User struct.
 * @return Integer status code (1 for valid input, 0 for invalid input).
 */
int input_user_name(User* user); 

/**
 * @brief Creates a random 6-digit number (magic number).
 * @details Generates a random number with 6 digits and stores it in the global variable `magic_number`.
 */
void random_6_digits_number(void); 

/**
 * @brief Inputs a 6-digit number string for comparison.
 * @details Prompts the user to input a 6-digit number. The input is then compared with the magic number.
 * @return Integer status code (1 for valid input, 0 for invalid input).
 */
int input_6_digits_number(void);

/**
 * @brief Comparing magic number with input number. 
 * @details Determining common chars and their positions.
 * @param user Pointer to the User struct.
 * @return Integer status code (1 for all corrects, 0 for incorrect)
 */
int compare_2_string(User* user); 

/**
 * @brief Updates the player table with the current user's score.
 * 
 * @details This function calculates the user's luck ratio (RightGuess/TotalGuess)
 *          and compares it with the existing players in the `player_table`. If the
 *          user's ratio qualifies for the top 10 players, it inserts the user's
 *          information into the correct position, shifting lower-ranked players down.
 *
 * @param user Pointer to the User struct containing the current user's information.
 * @param top_players Pointer to the player struct containiung the 10 highest players information 
 */
void update_player_table(User* user, player_table* top_players);

/**
 * @brief Saves the top 10 players' information to a text file.
 * 
 * @details This function writes the names and lucky ratios of the top 10 players
 *          from the `player_table` to a file named "top_players.txt".
 *
 * @param top_players Pointer to the player_table struct containing the top players' information.
 */
void save_player_table_to_file(const player_table* top_players);

/**
 * @brief Reads the top 10 players' information from a text file and stores it in the player_table.
 * 
 * @details This function opens the file named "top_players.txt", skips the first line, 
 *          reads each subsequent line character by character, and manually parses the player names 
 *          and their lucky ratios. It stores this information in the provided `player_table` structure.
 *
 * @param top_players Pointer to the player_table struct where the data will be stored.
 * @return Integer status code (1 for success, 0 for failure).
 */
int read_player_table_from_file(player_table* top_players);

/**
 * @brief Prints the top 10 players from the given player table.
 *
 * This function reads the top 10 players from the provided player table and prints
 * their names, lucky ratios, and time records.
 *
 * @param top_players A pointer to a player_table structure containing the top players' information.
 */
void print_high_score(player_table *top_players); 

/**
 * @brief Saves the list of users to a file named "log.txt".
 *
 * This function opens the "log.txt" file in write mode and writes the details
 * of each user in the provided users array to the file. The file is overwritten
 * if it already exists. Each user's details are written in a formatted manner,
 * and the file is closed after writing.
 *
 * @param users An array of User structures containing user details.
 */
void save_user_list_to_file(User users[]); 

/**
 * @brief Saves a single user to the list of users and updates the file.
 *
 * This function inserts a new user at the beginning of the users array,
 * shifts existing users down to make room, and removes the oldest user
 * if the list exceeds 10 users. It also updates associated arrays for
 * magic numbers, common characters, and correctness status. Finally, it
 * saves the updated list to a file.
 *
 * @param users An array of User structures containing the current list of users.
 * @param user The new User structure to be added to the list.
 * @param isAllCorrect An integer indicating whether the user's guess was correct all .
 */
void save_user_to_file(User users[],User user, int isAllCorrect); 

/**
 * @brief Load user list from the log file.
 * 
 * This function reads user information from a log file and stores it in the provided array of `User` structures.
 * It also clears the previous game state and checks if the input user has any incomplete games.
 * 
 * @param users Array of `User` structures where the loaded user information will be stored.
 * @param user The `User` structure representing the current user.
 * @return int Index of the current user with an incomplete game if found, otherwise -1.
 */
int load_user_list_from_file(User users[], User user);

/**
 * @brief Unit test function to enter and print user's request.
 *
 * This function tests the `enter_request` function by prompting the user to enter a request,
 * storing it in a static array, and then printing the entered request to verify its correctness.
 *
 * The function clears the request buffer, invokes the `enter_request` function to accept user input,
 * and then prints the request to the console for verification.
 *
 * @note The `enter_request` function should take a char array as input and populate it with the user's request.
 */
void ut_enter_request(void); 

/**
 * @brief Unit test function to input and validate user's name.
 *
 * This function tests the `input_user_name` function by prompting the user to enter a name,
 * storing it in a User struct, and then printing the validation results.
 *
 * The function clears the user name buffer, invokes the `input_user_name` function to accept user input,
 * checks if the input is valid and if the string length is within the maximum allowed length,
 * and then prints the validation results to the console for verification.
 *
 * @note The `input_user_name` function should take a User struct pointer as input and populate it with the user's name.
 */
void ut_input_user_name(void); 

/**
 * @brief Unit test function to generate and print a random 6-digit number.
 *
 * This function tests the `random_6_digits_number` function by generating a random 6-digit number,
 * storing it in a static character array, and then printing the generated number to verify its correctness.
 *
 * The function invokes the `random_6_digits_number` function to generate the random number,
 * copies the global magic number to a local variable for printing, and then clears the global magic number.
 *
 * @note The `random_6_digits_number` function should generate a 6-digit number and store it in a global variable `g_magic_number`.
 */
void ut_random_6_digits_number(void);

/**
 * @brief Unit test function to input and validate a 6-digit number.
 *
 * This function tests the `input_6_digits_number` function by prompting the user to input a 6-digit number,
 * validating the input, and then printing the validation result to verify its correctness.
 *
 * The function invokes the `input_6_digits_number` function to accept and validate the user's input,
 * prints whether the input was valid or if the game should stop, and then clears the global input number.
 *
 * @note The `input_6_digits_number` function should validate a 6-digit number entered by the user and return:
 *       1 if the input is valid,
 *      -1 if the game should stop.
 */
void ut_input_6_digits_number(void);

/**
 * @brief Unit test function to compare two strings and print the results.
 *
 * This function tests the `compare_2_string` function by comparing a predefined magic number
 * with different input numbers, storing the results, and then printing the comparison results to verify correctness.
 *
 * The function sets predefined values for the global magic number and input number,
 * invokes the `compare_2_string` function to compare these strings,
 * prints the result indicating whether all numbers are guessed correctly,
 * and the common characters between the strings.
 *
 * @note The `compare_2_string` function should compare the global magic number (`g_magic_number`) 
 *       with the global input number (`g_input_number`) and return:
 *       1 if all characters match,
 *       0 otherwise.
 */
void ut_compare_2_string(void);

/**
 * @brief Unit test function to load, read, save, and print top player data from/to a file.
 *
 * This function tests the sequence of operations for managing top player data by performing the following steps:
 * 1. Reading the player table from a file.
 * 2. Printing the high scores.
 * 3. Updating the player table with a new player's data.
 * 4. Saving the updated player table back to the file.
 * 5. Printing the high scores again to verify the update.
 *
 * The function initializes a `User` struct with a sample username, performs file operations
 * using `read_player_table_from_file`, `print_high_score`, `update_player_table`, and `save_player_table_to_file`,
 * and prints the high scores before and after updating to ensure the data is correctly managed.
 *
 * @note The `read_player_table_from_file` function should read player data from a file and populate the `player_table`.
 * @note The `print_high_score` function should display the current high scores from the `player_table`.
 * @note The `update_player_table` function should update the player table with the data from the `User` struct.
 * @note The `save_player_table_to_file` function should save the updated player table back to the file.
 */
void ut_load_read_save_print_top_file(void);

/**
 * @brief Unit test for saving and reading data of file log.
 *
 * This function tests the `save_user_to_file` and `load_user_list_from_file` functions
 * by loading a user list from a file, modifying a user's data, saving the data back to the file, 
 * and then instructing the user to check the file for correctness.
 * 
 * @note the `save_user_to_file` functions should save the user to the file
 * @note the `load_user_list_from_file` function should read users data from a file
 */
void ut_save_and_load_file(void);

/**************************************************************************************
 *                                MAIN PROGRAM
 **************************************************************************************/
int main(void)
{   

    /****************************************
    *           Clear varible               *
    *****************************************/
    /*Create a user instance*/
    User user;
    User users[10]; 

    /*Clear all variables of struct*/
    user.totalGuess = 0;
    user.rightGuess = 0;
    user.timeRecord = 0;
    memset(user.userName,'\0',sizeof(user.userName)); 

    /*Create a top player instance*/
    player_table top_players; 

    /*Clear all variables of struct*/
    for (int i = 0; i < 10; i++) {
        memset(top_players.playerName[i], '\0', LENGTH_STRING_MAX+1);
        top_players.luckyRatio[i] = 0.0f;
        top_players.timeRecord[i] = 0.0f; 
    } 

    /*clear struct*/
    for (int i = 0; i < 10; i++) {
        memset(users[i].userName, '\0', sizeof(users[i].userName));
        users[i].rightGuess = 0;
        users[i].totalGuess = 0;
        users[i].timeRecord = 0.0f;
    }

    /*clear common_char_old + magic_number_old*/
    memset(g_common_char_old,'\0', sizeof(g_common_char_old)); 
    memset(g_magic_number_old,'\0',sizeof(g_magic_number_old));

    /*Check the input valid*/
    int isValid = 0; 

    /*Check magic number is determined*/
    int isAllCorrect = 0; 

    /*Stop game*/
    int stopGame = 0; 

    /*Storing time of guessing action*/
    time_t startTime, endTime; 

    /*Store position taken users struct*/
    int userPostionString = -1;

    /*Determine initial time*/
    int initCheck = 0; 

    /*avoid random data in the first time running program*/
    for(int i = 0; i <= 10; i++)
    {
        save_user_to_file(users,user,0); 
    }

    /*Request compare*/
    char requestComapre = '3'; 
    /****************************************
    *               PROGRAM                 *
    *****************************************/

   /*Store the user's request*/
    char userRequest[3];

    do
    { 

    /*Clear StopGame*/
    stopGame = 0; 

    /*Check admin account*/
    g_check_admin = (strcmp(user.userName,"nhathanh") == 0); 

    /*Print interface game*/
    printf_interface(user);  

    /*Enter request */
    enter_request(userRequest); 

    switch (g_check_admin)
    {
    case 1: 
    {
        requestComapre = '9' ;
        switch (userRequest[0])
        {
        case '1':
        {
            ut_enter_request();
            break;
        }
        case '2':
        {
            ut_input_user_name();
            break; 
        }
        case '3':
        {
            ut_random_6_digits_number();
            break;
        }
        case '4': 
        {
            ut_input_6_digits_number();
            break;
        }
        case '5': 
        {
            ut_compare_2_string();
            break;
        }
        case '6': 
        {
            ut_load_read_save_print_top_file();
            break; 
        }
        case '7':
        {
            ut_save_and_load_file(); 
            break; 
        }
        case '8':
        {
            memset(user.userName,'\0',sizeof(user.userName));
            break; 
        }
        case '9':
            break;
        }  
        break; 
    }
    default: 
    {
        requestComapre = '3';
        switch (userRequest[0])
        {
        /*Create account*/
        case '1':
        {
            /*Input user name*/
            do
            {
                isValid = input_user_name(&user);
            } while (isValid == 0);

            break; 
        }

        /*Start game*/
        case '2': 
        {
            /*Check user has account yet*/
            if(strlen(user.userName) == 0)
            {
                /*clear isValid*/
                isValid = 0; 
                printf("You dont't have account.Cannot start the game\n"); 
                break; 
            }

            /*Load log.txt and compare user_name*/ 
            userPostionString = load_user_list_from_file(users,user);

            /*Clear for new user*/
            user.totalGuess = 0;
            user.rightGuess = 0;
            user.timeRecord = 0.0f; 

            /*Check account had not been finished game before yet*/
            if(userPostionString != -1)
            {
                printf("Your account had not finnished the last game yet.\n");
            }

            printf(GREEN"\n                                                    GAME START\n"RESET);

            /*Create magic number*/
            random_6_digits_number();

            /*clear common_char string*/
            memset(g_common_char,'\0',sizeof(g_common_char));
            memset(g_common_char,'_',LENGTH_NUMBER);  

            /*Update data of last play*/
            if(userPostionString != -1)
            {
                user = users[userPostionString]; 
                user.totalGuess -= 1; 
                strcpy(g_magic_number,g_magic_number_old[userPostionString]); 
                strcpy(g_common_char,g_common_char_old[userPostionString]); 
                isAllCorrect = 0; 
            }

            /*Print for fast checking*/ 
            printf("%s\n", g_magic_number);

            /*Capture start time*/
            startTime = time(NULL);   

            /*Guess magic number*/
            do
            {
                /*Increase guess count*/
                user.totalGuess++;

                /*Guess time*/
                printf("%d.", user.totalGuess);

                /*Clear isValid*/
                isValid = 0; 

                /*Enter the input string number*/
                do
                {
                    /*Clear StopGame*/
                    stopGame = 0;

                    /*Enter input*/
                    isValid = input_6_digits_number();

                    if (isValid == -1) // If user entered "quit"
                    {
                        /*Capture end guessing action */
                        endTime = time(NULL);   
                        user.timeRecord = difftime(endTime,startTime); 

                        /*Save to log file*/
                        save_user_to_file(users,user,isAllCorrect);  
                        stopGame = 1; 
                        initCheck = 1; 
                        break; 
                    }
                } while (isValid == 0); 

                /*Stop game*/
                if (stopGame)
                    break;

                /*Compare*/
                isAllCorrect = compare_2_string(&user);

            } while (isAllCorrect == 0);

            /*Stop game*/
            if (stopGame)
                break;

            /*Capture end guessing action */
            endTime = time(NULL);   

            user.timeRecord += difftime(endTime,startTime); 

            /*read player table*/
            if (initCheck)
                read_player_table_from_file(&top_players);

            /* Update the player table */
            update_player_table(&user,&top_players);

            /*Save to file*/
            save_player_table_to_file(&top_players);

            /*Save to log file*/
            save_user_to_file(users, user, isAllCorrect);

                printf("\n                                 /\\_/\\  (   \n");
                printf("                                ( ^.^ ) _)  \n");    
                printf("                                  \"/  (    \n");    
                printf("                                ( | | )     \n");    
                printf("                               (__d b__)    ");    


            printf("CONGRATULATION!\n");
            printf("                         Do you want to see the 10 highest table(y/Y. YES n/N. No): "); 

            char printRequest[3];
            isValid = 0; 

            do {
                /*CLear print_request*/
                memset(printRequest, '\0',3);

                /* Read user input*/
                fgets(printRequest, sizeof(printRequest), stdin);

                /* Check for valid input (y/Y or n/N)*/
                if (printRequest[0] == 'y' || printRequest[0] == 'Y') 
                {

                    print_high_score(&top_players);
                    isValid = 1; // Valid input, exit the loop
                } 
                else if (printRequest[0] == 'n' || printRequest[0] == 'N') 
                {
                    isValid = 1; // Valid input, exit the loop
                } else 
                {
                    printf(RED"                         Invalid input. Please enter y/Y for YES or n/N for NO: "RESET);
                }

                if(strlen(printRequest) > 2)
                {
                    /* Clear cache */
                    while (getchar() != '\n');
                }
            } while (!isValid);

            /*Clear all variables of struct for new player*/
            user.totalGuess = 0;
            user.rightGuess = 0;
            user.timeRecord = 0.0f; 
            memset(user.userName,'\0',sizeof(user.userName));  

            initCheck = 1; 
            break;
        }

        /*Exit*/
        case '3':
        {
            break;; 
        }
        default:
            break;
        }
        break; 
    }
    }
    }while(userRequest[0] != requestComapre); 

    return 0; 
}

/**************************************************************************************
 *                             EXECUTION FUNCTION
 **************************************************************************************/

/**************************************************************************************
 *                              PRINT_INTERFACE
 **************************************************************************************/
void printf_interface(User user)
{
    printf(BLUE"                                  ___  __  __  ____  ___     ___    __    __  __  ____ \n" RESET);
    printf(GREEN"                                 / __)(  )(  )( ___)/ __)   / __)  /__\\  (  \\/  )( ___)\n" RESET);
    printf(YELLOW"                                ( (_-. )(__)(  )__) \\__\\   ( (_-. /(__)\\  )    (  )__) \n" RESET);
    printf(RED"                                 \\___/(______)(____)(___/   \\___/(__)(__)(_/\\/\\_)(____)\n\n" RESET);

    printf("                                        Welcome to Gues Game by tranquangnguyenanh.\n\n");

    if(g_check_admin)
    {
        printf("\n                                            UNIT TEST FOR ADMIN\n");
        printf("                                        1. UT_ENTER_REQUEST\n");
        printf("                                        2. UT_INPUT_USER_NAME\n");
        printf("                                        3. UT_RANDOM_6_DIGITS_NUMBER\n"); 
        printf("                                        4. UT_INPUT_6_DIGITS_NUMBER\n");
        printf("                                        5. UT_COMPARE_2_STRING\n");
        printf("                                        6. UT_INTERRACT_WITH_TOP_PLAYER_FILE\n");
        printf("                                        7. UT_INTERRACT_WITH_LOG_FILE\n");
        printf("                                        8. EXIT\n");
        printf("                                        9. STOP PROGRAM\n"); 
    }
    else
    {
        printf("                                                        MENU\n");
        printf("                                                1. CREATE ACCOUNT\n");
        printf("                                                2.      PLAY\n");
        printf("                                                3.      EXIT\n"); 
    }
}

/**************************************************************************************
 *                              ENTER_REQUEST
 **************************************************************************************/
void enter_request(char userRequest[3])
{
    int isValid  = 0; 
    do
    {
        /* Clear request */
        memset(userRequest,'\0', 3);

        /* Enter request */
        printf("Enter your request: ");
        if (fgets(userRequest, 3, stdin) != NULL)
        {
            /* Calculate the length of request string */
            size_t dataLength = strlen(userRequest);

            /* Remove the newline character if present */
            if (dataLength > 0 && userRequest[dataLength - 1] == '\n')
            {
                userRequest[dataLength - 1] = '\0';
                dataLength--;
            }

            switch(g_check_admin)
            {
            case 1:
            {
                /* Validate input */
                if (dataLength == 1 && userRequest[0] >= '1' &&  userRequest[0] <= '9')
                {
                    isValid = 1;
                }
                else
                {
                    printf(RED"Invalid request\n"RESET);

                    /*Clear cache*/
                    if(dataLength > 1 )
                        while (getchar() != '\n');
                }
                break; 
            }
            default: 
            {
                /* Validate input */
                if (dataLength == 1 && userRequest[0] >= '1' &&  userRequest[0] <= '3')
                {
                    isValid = 1;
                }
                else
                {
                    printf(RED"Invalid request\n"RESET);

                    /*Clear cache*/
                    if(dataLength > 1 )
                        while (getchar() != '\n');
                }
                break; 
            }
            }
            
        }
        else
        {
        }
    } while (isValid == 0);
}

/**************************************************************************************
 *                              INPUT_USER_NAME
 **************************************************************************************/
int input_user_name(User* user)
{
    /*Check String is valid or invalid*/
    /*1: valid  ; 0: invalid*/
    int isValid = 1;

    /*Print request*/
    printf("User name: ");

    /*Clear string before type*/
    memset(user->userName, '\0', sizeof(user->userName));

    /*Input string*/
    if (fgets(user->userName, sizeof(user->userName), stdin) != NULL)
    {
        /*Calculate the length of user_name string*/
        size_t dataLength = strlen(user->userName);

        /*Delete the downline char*/
        if (dataLength > 0 && user->userName[dataLength-1] == '\n')
        {
            user->userName[dataLength-1] = '\0'; 
            dataLength--; 
        }

        /*Check length of user name*/
        if (dataLength > LENGTH_STRING_MAX)
        {
            isValid = 0;
            printf(RED,"The length exceeds the allowed maximum of %d characters.\n", LENGTH_STRING_MAX,RESET);

            /*Clear cache */
            while (getchar() != '\n'); 
        }
        else
        {
            /*Check the string user_name is valid*/
            for (int i = 0; user->userName[i] != '\0'; i++)
            {
                if (!isalnum(user->userName[i]))
                {
                    isValid = 0; 
                    break; 
                }
            }
        }

        /*Double check*/
        if (isValid)
        {
           /*Finish the task*/
        }
        else
        {
            printf(RED"User name is not valid\nPlease enter the user name again\n"RESET); 
        }
    }
    else 
    {}

    return isValid; 
}

/**************************************************************************************
 *                            RANDOM_6_DIGIT_NUMBER
 **************************************************************************************/
void random_6_digits_number(void)
{
    /*Clear string*/
    memset(g_magic_number, '\0', sizeof(g_magic_number));

    /*Restart the time CPU*/
    srand(time(NULL));

    /*Create magic number*/
    for (int i = 0; i < LENGTH_NUMBER; i++)
    {
        g_magic_number[i] = '0' + (rand() % 10); 
    }

    /*Null set at the EOS (end of string)*/
    g_magic_number[LENGTH_NUMBER] = '\0'; 
}

/**************************************************************************************
 *                            INPUT_6_DIGIT_NUMBER
 **************************************************************************************/
int input_6_digits_number(void)
{
    /*Check String is valid or invalid*/
    /*1: valid  ; 0: invalid*/
    int isValid = 1;

    /*Clear string*/
    memset(g_input_number, '\0', sizeof(g_input_number));

    /*Print request*/
    printf("Enter the number(quit for stop game): "); 

    /*Enter 6 digit number*/
    if (fgets(g_input_number, sizeof(g_input_number), stdin) != NULL)
    {
        /*Calculate the length of input_number string*/
        size_t dataLength = strlen(g_input_number);

        /*Delete the downline char*/
        if (dataLength > 0 && g_input_number[dataLength-1] == '\n')
        {
            g_input_number[dataLength-1] = '\0'; 
            dataLength--; 
        } 

        /* Check if the user wants to quit */
        if (strcmp(g_input_number, "quit") == 0)
        {
            return -1; // Special return value to indicate "quit"
        }

        /*Check length of string*/
        if (dataLength != LENGTH_NUMBER)
        {
            printf(RED"Input number invalid\n"RESET);
            

            /*clear cache*/
            if(dataLength > LENGTH_NUMBER)
                while (getchar() != '\n');

            /*invalid input*/
            isValid = 0; 
            return isValid; 
        }
        else
        {
            /*Check input_number elements are numeric*/
            for (int i = 0; i < LENGTH_NUMBER; i++)
            {
                if (!isdigit(g_input_number[i]))
                {
                    printf(RED"Input number invalid\n"RESET);
                    isValid = 0; 
                    return isValid; 
                }
            }
            /*Valid input*/
            isValid = 1;
        }

    }
    else
    {}
    
    return isValid;
}

/**************************************************************************************
 *                              COMPARE_2_STRING
 **************************************************************************************/
int compare_2_string(User* user)
{
    int i;
    int isAllCorrect = 1; 
    int newCorrectGuess = 0; 
    int newIncorrectGuess = 0;

    /* Temporary array to track new correct guesses */
    static char s_newCommonChar[LENGTH_NUMBER+1]; 

    /*set new_common_char = "_ _ _ _ _ _ \0"*/
    memset(s_newCommonChar, '_', LENGTH_NUMBER); 
    s_newCommonChar[LENGTH_NUMBER] = '\0'; 

    /* Compare and store the new common char */
    for (i = 0; i < LENGTH_NUMBER; i++)
    {
        if (g_magic_number[i] == g_input_number[i])
        {
            s_newCommonChar[i] = g_magic_number[i]; 

            /*Check new correct*/
            if(s_newCommonChar[i] != g_common_char[i])
            {
               newCorrectGuess = 1;
            }
        }
        else
        {
            isAllCorrect = 0;

            /* Check if a previously correct guess is now incorrect */
            if (g_common_char[i] == g_magic_number[i])
            {
                newIncorrectGuess = 1; 
            }
        }
    }
 
    /* If there are new correct guesses, increment RightGuess */
    if (newCorrectGuess && !newIncorrectGuess)
    {
        user->rightGuess++;
    }

    /* If there are new incorrect guesses on previously correct chars, decrement RightGuess */
    if (newIncorrectGuess && !newCorrectGuess)
    {
        user->rightGuess--;

        /*Make sure that Rightguess >= 0*/
        if(user->rightGuess < 0)
        {
            user->rightGuess = 0; 
        }
    }

    /* Update the common_char with new correct guesses */
    strcpy(g_common_char, s_newCommonChar);

    /* Print check */
    printf("Result: %s\n", g_common_char);
    printf("Right Guesses: %d\n", user->rightGuess);

    return isAllCorrect; 
}

/**************************************************************************************
 *                          UPDATE 10 HIGHEST PLAYER
 **************************************************************************************/
void update_player_table(User* user, player_table* top_players)
{
    float userRatio = (float)user->rightGuess / user->totalGuess;
    int i, j;

    for (i = 0; i < 10; i++) {
        if (userRatio > top_players->luckyRatio[i] ||  (userRatio == top_players->luckyRatio[i] && user->timeRecord < top_players->timeRecord[i])) 
        {
            /* Shift lower ranking players down */
            for (j = 9; j > i; j--) 
            {
                strcpy(top_players->playerName[j], top_players->playerName[j-1]);
                top_players->luckyRatio[j] = top_players->luckyRatio[j-1];
                top_players->timeRecord[j] = top_players->timeRecord[j-1];
            }

            /* Insert the new player */
            strcpy(top_players->playerName[i], user->userName);
            top_players->luckyRatio[i] = userRatio;
            top_players->timeRecord[i] = user->timeRecord;
            break;
        }
    }
}

/**************************************************************************************
 *                             SAVE 10 TOP PLAYERS
 **************************************************************************************/
void save_player_table_to_file(const player_table* top_players)
{
    FILE* file = fopen("top_players.txt", "w");

    /*Check open file success*/
    if (file == NULL) 
    {
        perror("Error opening file");
        return;
    }

    /*Save top players*/
    fprintf(file, "TOP 10 PLAYERS TABLE\n");
    for (int i = 0; i < 10; i++) 
    {
        if (strlen(top_players->playerName[i]) > 0) 
        {
            fprintf(file, "%d. %s - %.2f - %.2fs\n", i + 1, top_players->playerName[i], top_players->luckyRatio[i], top_players->timeRecord[i]);
        }
    }

    fclose(file);
}

/**************************************************************************************
 *                             READ 10 TOP PLAYERS
 **************************************************************************************/
int read_player_table_from_file(player_table* top_players) 
{
    FILE* file = fopen("top_players.txt", "r");

    /*Check open file action */
    if (file == NULL) 
    {
        perror("Error opening file");
        return 0;
    }

    int i = 0;
    char line[1000];
    int  skipFirstLine = 1;

    // Initialize the player_table
    for (int j = 0; j < 10; j++) 
    {
        memset(top_players->playerName[j], '\0', LENGTH_STRING_MAX+1);
        top_players->luckyRatio[j] = 0.0f;
        top_players->timeRecord[j] = 0.0; 
    }

    while (fgets(line, sizeof(line), file) != NULL) 
    {
        /*Skip the first line*/
        if (skipFirstLine) 
        {
            skipFirstLine = 0;
            continue;
        }

        if (i >= 10) 
        {
            break;
        }

        /*Manually parse the line for player name, lucky ratio, and time record*/
        char name[LENGTH_STRING_MAX + 1] = {0};
        float ratio = 0.0f;
        double time = 0.0;

        /* Use sscanf to parse the line */
        if (sscanf(line, "%*d. %s - %f - %lfs", name, &ratio, &time) == 3) 
        {
            /* Store the parsed values*/
            strcpy(top_players->playerName[i], name);
            top_players->luckyRatio[i] = ratio;
            top_players->timeRecord[i] = time;
            i++;
        }
    }

    fclose(file);
    return 1;
}

/**************************************************************************************
 *                            PRINT 10 TOP PLAYERS
 **************************************************************************************/
void print_high_score(player_table *top_players)
{
    printf("Top 10 Players read from file:\n");
    for (int i = 0; i < 10; i++) 
    {
        if (strlen(top_players->playerName[i]) > 0) 
        {
            printf("%d. %s - %.2f - %.2fs\n", i + 1, top_players->playerName[i], top_players->luckyRatio[i],top_players->timeRecord[i]);
        }
    }
}

/**************************************************************************************
 *                            SAVE A LIST OF USER TO LOGFILE
 **************************************************************************************/
void save_user_list_to_file(User users[]) {
    FILE *file = fopen("log.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    for (int i = 0; i < 10; ++i) {
        fprintf(file, "Entry %d:\n", i + 1); // Print the sequence number
        fprintf(file, "Username: %s\n", users[i].userName);
        fprintf(file, "Total Guesses: %d\n", users[i].totalGuess);
        fprintf(file, "Right Guesses: %d\n", users[i].rightGuess);
        fprintf(file, "Time Record: %.2f\n", users[i].timeRecord);
        fprintf(file, "Magic Number: %s\n", g_magic_number_old[i]); // Save magic number as a string
        fprintf(file, "Common Char: %s\n", g_common_char_old[i]);   // Save common char sequence
        fprintf(file, "Magic numer guessed done: %d\n", g_is_all_correct_string[i]); 
        fprintf(file, "-------------------------\n");
    }

    fclose(file);
}

/**************************************************************************************
 *                            SAVE NEW USER TO LOGFILE
 **************************************************************************************/
void save_user_to_file(User users[],User user, int isAllCorrect) {
     
    /* Remove the oldest user to make room for the new one */
    for (int i = 9; i >= 0 ; i--) 
    {
        users[i+1] = users[i];
        strcpy(g_magic_number_old[i+1], g_magic_number_old[i]); 
        strcpy(g_common_char_old[i+1], g_common_char_old[i]); 
        g_is_all_correct_string[i+1] = g_is_all_correct_string[i];
    }


    /* Add the new user */ 
    users[0] = user;
    strcpy(g_common_char_old[0], g_common_char);  
    strcpy(g_magic_number_old[0], g_magic_number); 
    g_is_all_correct_string[0] = isAllCorrect; 

    /* Save the updated user list to the file */
    save_user_list_to_file(users);
}

/**************************************************************************************
 *                               LOAD LIST
 **************************************************************************************/
int load_user_list_from_file(User users[], User user)
{
    FILE *file = fopen("log.txt", "r");
    if (file == NULL) {
        return 0; // No file exists, so no users to load
    }

    int count = 0;
    int entryNumber;

    /*clear struct*/
    for (int i = 0; i < 10; i++) {
        memset(users[i].userName, '\0', sizeof(users[i].userName));
        users[i].rightGuess = 0;
        users[i].totalGuess = 0;
        users[i].timeRecord = 0.0f;
    }

    /*clear common_char_old + magic_number_old + isAllCorrect_string*/
    memset(g_common_char_old,'\0', sizeof(g_common_char_old)); 
    memset(g_magic_number_old,'\0',sizeof(g_magic_number_old));
    memset(g_is_all_correct_string, 0, sizeof(g_is_all_correct_string)); 

    /*Read data */
    while(fscanf(file, "Entry %d:\n", &entryNumber) == 1 &&
          fscanf(file, "Username: %20s\n", users[count].userName) == 1 &&
          fscanf(file, "Total Guesses: %d\n", &users[count].totalGuess) == 1 &&
          fscanf(file, "Right Guesses: %d\n", &users[count].rightGuess) == 1 &&
          fscanf(file, "Time Record: %f\n", &users[count].timeRecord) == 1 &&
          fscanf(file, "Magic Number: %s\n", g_magic_number_old[count]) == 1 && 
          fscanf(file, "Common Char: %s\n", g_common_char_old[count]) == 1 && 
          fscanf(file, "Magic numer guessed done: %d\n", &g_is_all_correct_string[count]) == 1) 
          {   
              fscanf(file, "-------------------------\n");
              count++;
              if (count >= 10) 
              {
                  break;
              }
          }

    fclose(file);

    /*Check incompleting game account*/
    for(int i = 0; i < 10; i++)
    {
        if(strcmp(users[i].userName, user.userName) == 0 && g_is_all_correct_string[i] == 0)
        {
            return i; 
        }
    }

    return -1; 
}

/**************************************************************************************
 *                        EXECUTION UNIT TEST FUNCTION
 **************************************************************************************/

/**************************************************************************************
 *                       USER'S REQUEST ENTER  FUNCTION
 **************************************************************************************/
void ut_enter_request(void)
{

    printf("Test enter user's request(enter request if print request is same with label symbolized, function runs correctly):\nResult:\n");
    static char ut_user_request[3];

    /* Clear request */
    memset(ut_user_request, '\0', 3);

    /*Input user request*/
    enter_request(ut_user_request); 

    /*Print check*/
    printf("Request that user type: %s\n", ut_user_request);
    printf("End test.\n");
}

/**************************************************************************************
 *                       INPUT USER NAME  FUNCTION
 **************************************************************************************/
void ut_input_user_name(void)
{
    User ut_user; 
    printf("Test user name input (check value of value s_isVald of funtion (1: valid; 0: invalid) and check string's length <= LENGTH_STRING_MAX (1: valid; 0: invalid)):\n"); 
    
    /*Clear user name*/
    memset(ut_user.userName,'\0',LENGTH_STRING_MAX + 2); 

    int ut_isValid = 0; 
    int ut_isLengthValid = 0; 

    /*Inut check*/
    ut_isValid = input_user_name(&ut_user); 
    ut_isLengthValid = (strlen(ut_user.userName) <= LENGTH_STRING_MAX ) ? 1 : 0; 

    printf("s_isValid: %d\nLength's string of user_name: %d\n", ut_isValid, ut_isLengthValid); 
    printf("End test.\n"); 
    ///*Clear user name*/
    //while(getchar() != '\n');
    //memset(ut_user.userName,'\0',sizeof(ut_user.userName));
}

/**************************************************************************************
 *                    RANDOM 6 DIGIT NUMBER  FUNCTION
 **************************************************************************************/
void ut_random_6_digits_number(void)
{
    char ut_magic_number[LENGTH_NUMBER+1]; 
    printf("Test random number 6 digit functions:\nTest : "); 

    /*Test */
    random_6_digits_number(); 
    strcpy(ut_magic_number,g_magic_number); 
    printf("%s\n",ut_magic_number);
    printf("End test\n"); 

    /*Clear magic number*/
    memset(g_magic_number,'\0',sizeof(g_magic_number)); 
}

/**************************************************************************************
 *                     INPUT 6 DIGIT NUMBER FUNCTION
 **************************************************************************************/
void ut_input_6_digits_number(void)
{
    int ut_isvalid; 
    printf("Test input 6 digits number function:\n"); 

    /*Test*/
    ut_isvalid = input_6_digits_number(); 

    /*Print result*/
    if (ut_isvalid == 1)
        printf("Input valid\n"); 
    else if(ut_isvalid == -1)
        printf("Game Stop\n"); 

    printf("End test.\n");

    /*Clear g_input_number*/
    memset(g_input_number,'\0',sizeof(g_input_number));
}

/**************************************************************************************
 *                     COMPARE 2 STRING FUNCTION
 **************************************************************************************/
void ut_compare_2_string(void)
{
    User ut_user; 
    int ut_isAllCorrect; 

    printf("Test compare string: \n"); 
    
    /*Test 1*/
    printf("Test 1: \n"); 
    strcpy(g_magic_number,"234561\0"); 
    strcpy(g_input_number,"234567\0"); 

    ut_isAllCorrect = compare_2_string(&ut_user); 

    printf("Guess all number: %d\nString common char: %s\n", ut_isAllCorrect, g_common_char); 

    /*Test 2*/
    printf("Test 2: \n"); 
    strcpy(g_input_number,"234551\0");

    ut_isAllCorrect = compare_2_string(&ut_user); 

    printf("Guess all number: %d\nString common char: %s\n", ut_isAllCorrect, g_common_char); 

    /*Test 3*/
    printf("Test 3: \n"); 
    strcpy(g_input_number,"234555\0");

    ut_isAllCorrect = compare_2_string(&ut_user); 

    printf("Guess all number: %d\nString common char: %s", ut_isAllCorrect, g_common_char);

    printf("\nEnd test.\n");
    /*Clear */
    memset(g_common_char,'\0', sizeof(g_common_char)); 
    memset(g_input_number,'\0', sizeof(g_input_number)); 
    memset(g_magic_number,'\0', sizeof(g_magic_number));
}

/**************************************************************************************
 *                     READ, PRINT UDATE AND SAVE FUNCTION
 **************************************************************************************/
void ut_load_read_save_print_top_file(void)
{
    player_table ut_top_player; 
    User ut_user; 

    strcpy(ut_user.userName, "tes10"); 

    /*Test read and print*/
    printf("Test funtion read, save, print and updata data with file top: \n"); 
    read_player_table_from_file(&ut_top_player); 
    print_high_score(&ut_top_player); 
    printf("End test read and print.\n"); 

    /*Test save and print*/
    update_player_table(&ut_user,&ut_top_player); 
    save_player_table_to_file(&ut_top_player); 
    print_high_score(&ut_top_player);    
    printf("End test save and print.\n");
}

/**************************************************************************************
 *                     READ, PRINT UDATE AND SAVE FUNCTION
 **************************************************************************************/
void ut_save_and_load_file(void)
{
    User ut_user; 
    User ut_users[10]; 
    int ut_isAllCorrect = 1; 

    printf("\nTest save and read data of file log:\n");

    /*Test*/
    load_user_list_from_file(ut_users,ut_user); 
    
    strcpy(ut_user.userName, "unit_test");
    ut_user.totalGuess = 12; 

    save_user_to_file(ut_users,ut_user,ut_isAllCorrect); 
    printf("End test open file log.txt for checking.\n");

    /*Clear */
    memset(g_common_char,'\0', sizeof(g_common_char)); 
    memset(g_input_number,'\0', sizeof(g_input_number)); 
    memset(g_magic_number,'\0', sizeof(g_magic_number));
    memset(g_is_all_correct_string,0,sizeof(g_is_all_correct_string));
}