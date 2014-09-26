
#include "pollQuit.h"

#if defined(_WIN32)
    // For windows
    #include <windows.h>
    #include <conio.h>
#else
    // For linux
    #include <stdio.h>
    #include <unistd.h>
    #include <iostream>
#endif

void PollQuit::DoWork()
{
#if defined(_WIN32)
    const HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);

    if (WaitForSingleObject( hStdin, 0 ) == WAIT_OBJECT_0 )
    {
        if( _kbhit() ) // _kbhit() always returns immediately
        {
            int i = _getch();
            if (i == 'q')
            {
                emit QuitRequested();
            }
        }
    }

#else

    char c = 0;

    // Below cin operation should be executed within stipulated period of time
    fd_set readSet;
    FD_ZERO(&readSet);
    FD_SET(STDIN_FILENO, &readSet);
    struct timeval tv = {0,500000};  // seconds, microseconds
    int retval = select(STDIN_FILENO+1, &readSet, NULL, NULL, &tv);

    if (retval == -1)
    {
        perror("select()");
    }
    else if (retval)
    {
        // Data is available
        std::cin >> c;
        if (c == 'q')
        {
            emit QuitRequested();
        }
    }
    else
    {
        // No data
    }

#endif

}





