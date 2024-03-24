#pragma once
#include <SFML/Graphics.hpp>
class keyEvents {
    public:
       static void W();
       static void A();
       static void S();
       static void D();

       static void J();
       static void K();

       static void F();

        static  void Num0();
    static void Space();
    static void Shift(sf::Event ev);
    static void Ctrl(sf::Event ev);

    static void Up();
    static void Down();
    static void Left();
    static void Right();
};