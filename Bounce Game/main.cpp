#include <iostream>
#include <random>
#include <unistd.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

using namespace sf;

int main (){
    RenderWindow window (VideoMode(640,400), "Bounce...");
    window.setFramerateLimit(60);
    

    // Seed for the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-10, 10); // Range from -10 to 10
    
    // Generate a random number between -10 and 10
    int randomNumber = dis(gen);

    // bouncing box
    RectangleShape box(Vector2f{25.f,25.f});
    box.setOutlineThickness(5.f);
    box.setFillColor(Color(255,255,0,255));
    box.setOutlineColor(Color::Black);
    box.setPosition(Vector2f{50.f,50.f});


    // bar which will slide;
    RectangleShape bar(Vector2f{70.f,15.f});
    bar.setFillColor(Color(204,0,0,255));
    bar.setPosition(Vector2f{50.f,static_cast<float>(window.getSize().y - 50)});
    
    //gravity
    float gravity = 9.8;

    //  direction Variables
    //  Y Axis  : (0) is for going down
    //          : (1) is for goint up
    int dir_Y = 0;

    //  X Axis  : (1) is for going Right
    //          : (-1) is for goint Left
    int dir_X = 1;


    // Newton laws of motion;
    float vi = 0;
    float vf = 0;
    float time = 0;

    // for Velocity in X-axis
    int Vx = 0;

    // for positions of the mouse
    float x1 = 0;
    float x2 = 0;


    while (window.isOpen())
    {   
        Event event;
        while(window.pollEvent(event)){
            if (event.type == Event::Closed){
                window.close();
            }
        }
        // If the obj is to move down
        if (dir_Y == 0){
            // calculate speed for each individual frame
            vf = (gravity * time);
            // move the obj
            box.move(0.f,vf);

            // if the obj intersects with the bar
            if ( box.getGlobalBounds().intersects(bar.getGlobalBounds())){
                // set initial velocity for upward motion to the final velocity of the downward motion
                vi = (gravity * time);

                // set time 
                time = 0;

                // change direction variable
                dir_Y = 1;

                // set random velocity for x-axis
                Vx = dis(gen);
                continue;
            } 
        }


        // if the obj collides with the bottom of the screen
        if(box.getPosition().y + box.getSize().y >= window.getSize().y){
            box.setPosition(Vector2f{25.f,25.f});
            time = 0;
        }


        // if the obj is to go upward
        if (dir_Y == 1){

            // calculate final velocity
            vf = vi - (gravity * time);
            box.move(0.f,-vf);

            // if the box reaches the top then change its direction
            // or if the boxes final velocity reaches zero then change its direction
            if (box.getPosition().y <= 0 || vf <= 0){
                // set time to zero
                time = 0;
                // change its direction
                dir_Y = 0;
                // set initial velocity for downward motion to be zero
                vi = 0;
            } 
        }

            // if the box touches the right side of the screen
            if(box.getPosition().x + box.getSize().x >= window.getSize().x ){
                // invert its velocity direction
                Vx = Vx * (-1);
            }
        

            // If the box touches the left side of the screne 
            if(box.getPosition().x <= 0 ){
                // invert its velocity direction
                Vx = Vx * (-1);
            }

        // make obj move in the x-axis
        box.move(Vx,0.f);


        // If the Left mouse button is pressed then move the bar to the position of the mouse
        if (Mouse::isButtonPressed(Mouse::Left)){
            bar.setPosition(Vector2f{static_cast<float> (Mouse::getPosition(window).x) ,static_cast<float>(window.getSize().y - 50) });
            // bar.setPosition(Vector2f{static_cast<float> (Mouse::getPosition(window).x) ,static_cast<float> (Mouse::getPosition(window).y) });
        }

        
        // I want to be updated in seconds
        // I also know that the framerate is 60 FPS
        // so after each frame the time is incremented by 0.0166666667
        // so that after 60 such incrementation time will be increased to one
        // there can be other better ways to do this but i chose this one 
        // as time helps me to calculate the velocity at each individual frame
        time += 0.0166667;

        window.clear(Color::White);
        window.draw(box);
        window.draw(bar);
        window.display();
    }
    

}