#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>


struct transition {
    sf::Vector2f from;
    sf::Vector2f to;
    string symbol;
};

class draw {
public:
	sf::CircleShape icons[100];
    sf::Vector2f locations[100];
    sf::Text labels[100];
    struct transition tran[100];
    int n;
    int t;
    sf::Font font;


    draw(int n) {
        this->n = n;
    }
    void setn(int a) {
        n = a;
        setdraw(n, ::nfa.states);
    }
    void setdraw(int n, vector<string>& states) {
        this->n = n;
        t = 0;
        sf::Vector2f center(600.f, 350.f);
        float radius = 12.f * n; 

        for (int i = 0; i < n; i++) {
            float angle = 2 * 3.14159 * i / n; 
            float x = center.x + radius * cos(angle);
            float y = center.y + radius * sin(angle);

            icons[i].setRadius(20.f);
            //cout << "s = " << states[i] << endl;
            if(i == 0)
                icons[i].setFillColor(sf::Color::Cyan);
            else if(find(::nfa.final_state.begin(), ::nfa.final_state.end(),states[i]) == ::nfa.final_state.end())
                icons[i].setFillColor(sf::Color::Green);
            else
                icons[i].setFillColor(sf::Color::Magenta);

            icons[i].setPosition(x, y);
            if (!font.loadFromFile("ModernWarfare-OV7KP.ttf")) {
                cout << " FONT NOT LOADED " << endl;
            }
            labels[i].setFont(font);
            labels[i].setCharacterSize(15); 
            labels[i].setString(states[i]); 
            labels[i].setFillColor(sf::Color::White);
            labels[i].setPosition(x + 10, y + 10);
        }

        setTransitions(::nfa.transition_function);
    }

    void setTransitions(vector<vector<string>>& t) {
        for (auto a : t) {
            string fro = a[0];
            string too = a[2];
            struct transition temp;
            for (int i = 0; i < n; i++) {
                if (labels[i].getString() == fro) {
                    //cout << fro << " " << too << endl;
                    temp.from.x = icons[i].getPosition().x + 20;
                    temp.from.y = icons[i].getPosition().y + 20;
                    temp.symbol = a[1];
                }
                if (labels[i].getString() == too) {

                    temp.to.y = icons[i].getPosition().y + 20;
                    temp.to.x = icons[i].getPosition().x + 20;
                }
            }

            tran[this->t++] = temp;

        }
    }

    void drawArrow(sf::RenderWindow& w, sf::Vector2f from, sf::Vector2f to, const std::string& label) {
        sf::Vertex line[] = {
            sf::Vertex(from, sf::Color::Red),
            sf::Vertex(to, sf::Color::Red)
        };
        w.draw(line, 2, sf::Lines);

        sf::Vector2f direction = to - from;
        float length = hypot(direction.x, direction.y);
        direction /= length;

        const float arrowSize = 10.f;
        sf::Vector2f unitDirection = direction;
        sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

        sf::Vector2f arrowTip = to;
        sf::Vector2f arrowLeft = to - arrowSize * (unitDirection + unitPerpendicular);
        sf::Vector2f arrowRight = to - arrowSize * (unitDirection - unitPerpendicular);

        sf::ConvexShape arrow;
        arrow.setPointCount(3);
        arrow.setPoint(0, arrowTip);
        arrow.setPoint(1, arrowLeft);
        arrow.setPoint(2, arrowRight);
        arrow.setFillColor(sf::Color::Red);
        w.draw(arrow);

        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(15);
        text.setString(label);
        text.setFillColor(sf::Color::Blue);
        text.setPosition((from + to) / 2.f);
        w.draw(text);
    }


    void drawIcons(sf::RenderWindow& w) {
        for (int i = 0; i < 25; i++) {
            w.draw(icons[i]);
            w.draw(labels[i]);
        }
        
        for(int i = 0; i < t; i++)
            drawArrow(w, tran[i].from, tran[i].to, tran[i].symbol);
    }

	void simulate() {
        sf::RenderWindow window(sf::VideoMode(1200, 700), "SFML works!");
        
        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear();
            drawIcons(window);
            window.display();
        }

	}
};


draw d(25);