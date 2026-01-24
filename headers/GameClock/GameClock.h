#pragma once
#include "./../default_includes.h"


class GameClock
{
    public:
        static GameClock* getInstance();

        GameClock(GameClock& other) = delete;
        void operator=(const GameClock&) = delete;
        

    private:
        static GameClock* gameClock;

        //Clocks will be kept in a handle - clock map
        long long curr_time = 0;

        GameClock();
        virtual ~GameClock();
};

