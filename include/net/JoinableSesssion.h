#ifndef JOINABLE_SESSION
#define JOINABLE_SESSION

typedef struct JoinableSession
{
    char name[20];
    char hostname[20];
    int sessionID;
    int currentPlayers;
    int maxPlayers;
} JoinableSession;

#endif