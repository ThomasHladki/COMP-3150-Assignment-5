#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>

static int callback(void* data, int argc, char** argv, char** ColName){

 for(int i = 0; i<argc; i++){
 printf("%s = %s\n", ColName[i], argv[i]);
 }

 return 0;
}

int main(){

    sqlite3* db;
    int c;
    c = sqlite3_open_v2("SongStat.db", &db, SQLITE_OPEN_READWRITE, NULL);
    if(c != SQLITE_OK) {
        printf("Unable to open database %s\n", sqlite3_errmsg(db));
        return(0);
    }
    
    printf("Connected to the database successfully.\n");
    printf("Welcome to SongStat. What would you like to do?\n");
    int userInput;
    int r;
    char* errorMsg;
    while(1){
        
        printf("0) Exit program\n1) Find any songs that haven't been listend to\n2) Find any albums that haven't been listened to yet\n");
        printf("3) Add death date to an artist who has passed away\n4) List all songs in the database\n5) Find the most listened to song\n");
        printf("6) Find the most listened to album\n7) Display the average number of times songs have been listened to\n");
        printf("8) Display the total number of songs listened to\n9) Display how many songs an artist features on\n");
        printf("10) Get the total number of songs listened to on a particular album\nWhat would you like to do?\n");
        scanf("%d",&userInput);
        
        fflush(stdin);
        switch (userInput){

            case 0:
                return 0;
                break;
            
            default:
                puts("Invalid input");
                break;

            case 1:
                errorMsg = 0;
                char* sql1 = "SELECT SongName FROM Song WHERE NumberOfTimesListenedTo = 0;";    
                int r = sqlite3_exec(db, sql1, callback, 0, &errorMsg);

                if( r != SQLITE_OK ){
                    printf("SQL error: %s\n", errorMsg);
                    sqlite3_free(errorMsg);
                }
                break;

            case 2:
                errorMsg = 0;
                char* sql2 = "SELECT AlbumName FROM Album WHERE TimesListenedTo = 0";    
                r = sqlite3_exec(db, sql2, callback, 0, &errorMsg);

                if( r != SQLITE_OK ){
                    printf("SQL error: %s\n", errorMsg);
                    sqlite3_free(errorMsg);
                }
                break;
            
            case 3:
                char artistName[40];
                char date[25];
                printf("Enter the name of the artist who died\n");
                scanf("%[^\n]",artistName);
                fflush(stdin);
                printf("Enter the date that the artist died\n");
                scanf("%[^\n]",date);
                strcat(artistName, "'");
                fflush(stdin);
                strcat(date, "'");
                char str1[100] = "Update MusicalArtist SET DateOfDeath ='";
                strcat(str1, date);
                char str2[50]="WHERE StageName = '";
                strcat(str2, artistName);
                strcat(str1, str2);
                
                errorMsg=0;
                r = sqlite3_exec(db, str1, NULL, 0, &errorMsg);
                if( r != SQLITE_OK ){
                    printf("SQL error: %s\n", errorMsg);
                    sqlite3_free(errorMsg);
                }
                puts("New entry for artist");
                char sql3[100] = "SELECT DateOfDeath FROM MusicalArtist WHERE StageName = '";
                strcat(sql3, artistName);
                r = sqlite3_exec(db, sql3, callback, 0, &errorMsg);
                if( r != SQLITE_OK ){
                    printf("SQL error: %s\n", errorMsg);
                    sqlite3_free(errorMsg);
                }
                break;

            case 4:
                char* sql4 = "SELECT SongName FROM Song";
                r = sqlite3_exec(db, sql4, callback, 0, &errorMsg);
                if( r != SQLITE_OK ){
                    printf("SQL error: %s\n", errorMsg);
                    sqlite3_free(errorMsg);
                }
                break;

            case 5:
                char* sql5 = "SELECT SongName FROM Song WHERE NumberOfTimesListenedTo = (SELECT MAX(NumberOfTimesListenedTo) FROM Song);";
                errorMsg=0;
                puts("Most listened to song");
                r = sqlite3_exec(db, sql5, callback, 0, &errorMsg);
                if( r != SQLITE_OK ){
                    printf("SQL error: %s\n", errorMsg);
                    sqlite3_free(errorMsg);
                }
                break;

            case 6:
                char* sql6 = "SELECT AlbumName FROM Album WHERE TimesListenedTo = (SELECT MAX(TimesListenedTo) FROM Album);";
                errorMsg=0;
                puts("Most listened to album");
                r = sqlite3_exec(db, sql6, callback, 0, &errorMsg);
                if( r != SQLITE_OK ){
                    printf("SQL error: %s\n", errorMsg);
                    sqlite3_free(errorMsg);
                }
                break;
            
            case 7:
                char* sql7 = "SELECT AVG(NumberOfTimesListenedTo) FROM Song;";
                errorMsg=0;
                puts("Average number of times songs have been listened to:");
                r = sqlite3_exec(db, sql7, callback, 0, &errorMsg);
                if( r != SQLITE_OK ){
                    printf("SQL error: %s\n", errorMsg);
                    sqlite3_free(errorMsg);
                }
                break;

            case 8:
                char* sql8 = "SELECT SUM(NumberOfTimesListenedTo) FROM Song;";
                errorMsg=0;
                puts("Total number of times songs have been listened to:");
                r = sqlite3_exec(db, sql8, callback, 0, &errorMsg);
                if( r != SQLITE_OK ){
                    printf("SQL error: %s\n", errorMsg);
                    sqlite3_free(errorMsg);
                }
                break;
             
            case 9:
                char artistStageName[35];
                char sql9[60] = "SELECT COUNT(*) FROM FeaturesRelationship WHERE ArtistID =";
                puts("Enter the artist ID of the artist in question");
                scanf("%[^\n]",artistStageName);
                strcat(sql9,artistStageName);
                r = sqlite3_exec(db, sql9, callback, 0, &errorMsg);
                if( r != SQLITE_OK ){
                    printf("SQL error: %s\n", errorMsg);
                    sqlite3_free(errorMsg);
                }
                break;

            case 10:
                char albumID[3];
                char sql10[150] = "SELECT SUM(NumberOfTimesListenedTo) FROM Song WHERE SongID IN (SELECT SongID FROM AppearsOnRelationship WHERE AlbumID = ";
                puts("Enter the album ID of the album in question");
                scanf("%[^\n]",albumID);
                strcat(sql10,albumID);
                strcat(sql10, ");");
                r = sqlite3_exec(db, sql10, callback, 0, &errorMsg);
                if( r != SQLITE_OK ){
                    printf("SQL error: %s\n", errorMsg);
                    sqlite3_free(errorMsg);
                }
                break;
                

        
        }   
        


    }
    return 0;
    
}