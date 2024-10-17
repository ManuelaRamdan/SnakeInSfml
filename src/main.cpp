#include "sfml-utn-inspt.h"

const unsigned int ANCHO_VENT = 800;
const unsigned int ALTO_VENT = 600;
const unsigned int FRAMERATE = 60;

int main()
{
    RenderWindow ventana = RenderWindow({ANCHO_VENT, ALTO_VENT}, "Unidad 1 - Taller de Videojuegos");
    ventana.setFramerateLimit(FRAMERATE);
    CircleShape forma = CircleShape(10, 20);
    forma.setPosition((ANCHO_VENT / 2) + 20, (ALTO_VENT / 2));
    bool colisionConBordesForma(const CircleShape &forma, const RectangleShape &bordeIzquierdo, const RectangleShape &bordeDerecho, const RectangleShape &bordeInferior, const RectangleShape &bordeSuperior);

    bool colisionConBordesSnake(const vector<RectangleShape> &snake, const RectangleShape &bordeIzquierdo, const RectangleShape &bordeDerecho, const RectangleShape &bordeInferior, const RectangleShape &bordeSuperior);

    bool colisionConBordesRectangulo(const RectangleShape &bloque, const RectangleShape &bordeIzquierdo, const RectangleShape &bordeDerecho, const RectangleShape &bordeInferior, const RectangleShape &bordeSuperior);

    RectangleShape bloque(Vector2f(50, 20));
    bloque.setPosition(100, 100);
    bloque.setFillColor(Color::Magenta);

    /*Vector2f posMouse = {0, 0};
    int LADO = 5;

    RectangleShape movil = RectangleShape({LADO, LADO});
    movil.setFillColor(Color ::Green);
    movil.setOrigin(LADO / 2.f, LADO / 2.f);*/

    vector<RectangleShape> snake;
    int x = (ANCHO_VENT / 2) - 20;
    for (int i = 0; i <= 3; i++)
    {
        RectangleShape square(Vector2f(20, 20));
        square.setPosition(x, 180);
        x = x - 20;
        snake.push_back(square);
    }
    RectangleShape bordeIzquierdo(Vector2f(20, ALTO_VENT));
    bordeIzquierdo.setFillColor(Color::Red);
    bordeIzquierdo.setPosition(0, 0);

    RectangleShape bordeDerecho(Vector2f(20, ALTO_VENT));
    bordeDerecho.setFillColor(Color::Blue);
    bordeDerecho.setPosition(ANCHO_VENT - 20, 0);

    RectangleShape bordeInferior(Vector2f(ANCHO_VENT, 20));
    bordeInferior.setFillColor(Color::Green);
    bordeInferior.setPosition(0, ALTO_VENT - 20);

    // Crear borde superior
    RectangleShape bordeSuperior(Vector2f(ANCHO_VENT, 20)); // Cambia el tamaño del borde superior
    bordeSuperior.setFillColor(Color::Yellow);              // Asegúrate de usar bordeSuperior aquí
    bordeSuperior.setPosition(0, 0);                        // Posición en la parte superior

    // Agregar un mensaje de puntos
    /*Font fuente;
    fuente.loadFromFile("arial.ttf");
    Text textoPuntos;
    textoPuntos.setFont(fuente);
    textoPuntos.setCharacterSize(100);
    textoPuntos.setFillColor(Color::White);
    textoPuntos.setPosition(10, 10);
    textoPuntos.setString("Puntos: 0");*/

    int puntos = 0;
    while (ventana.isOpen())
    {
        Event evento = Event();
        while (ventana.pollEvent(evento))
        {
            if (evento.type == Event::Closed)
            {
                // Si el usuario toc´o la [X] para cerra la ventana:
                ventana.close();
            }
        }
        /*posMouse = Mouse ::getPosition(ventana);
        movil.setPosition(posMouse.x, posMouse.y);

        // Vector2f movil (posMouse.x, posMouse.y);

        if (movil.getGlobalBounds().intersects(forma.getGlobalBounds()))
        {
            forma.setPosition(rand() % ANCHO_VENT, rand() % ALTO_VENT);
        }*/

        // forma.setPosition(rand() % ANCHO_VENT+1,rand() % ALTO_VENT+1);
        // Agregar un borde a la pantalla
        // Crear bordes

        if (Keyboard::isKeyPressed(Keyboard::Right))
        {
            Vector2f position = snake[0].getPosition();
            for (int i = snake.size() - 1; i >= 0; --i)
            {
                if (i == 0)
                {
                    // Only move the head in the specified direction
                    snake[i].setPosition(position.x + 5, position.y);
                }
                else
                {

                    snake[i].setPosition(snake[i - 1].getPosition());
                }
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Left))
        {
            for (int i = snake.size() - 1; i >= 0; --i)
            {
                if (i == 0)
                {
                    // Only move the head in the specified direction
                    snake[i].setPosition(snake[i].getPosition().x - 5, snake[i].getPosition().y);
                }
                else
                {
                    // The rest of the body just follows the ahead bodypart's direction
                    snake[i].setPosition(snake[i - 1].getPosition());
                }
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Up))
        {
            for (int i = snake.size() - 1; i >= 0; --i)
            {
                if (i == 0)
                {
                    // Only move the head in the specified direction
                    snake[i].setPosition(snake[i].getPosition().x, snake[i].getPosition().y - 5);
                }
                else
                {
                    // The rest of the body just follows the ahead bodypart's direction
                    snake[i].setPosition(snake[i - 1].getPosition());
                }
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Down))
        {
            for (int i = snake.size() - 1; i >= 0; --i)
            {
                if (i == 0)
                {
                    // Only move the head in the specified direction
                    snake[i].setPosition(snake[i].getPosition().x, snake[i].getPosition().y + 5);
                }
                else
                {
                    // The rest of the body just follows the ahead bodypart's direction
                    snake[i].setPosition(snake[i - 1].getPosition());
                }
            }
        }

        if (snake[0].getGlobalBounds().intersects(forma.getGlobalBounds()))
        {
            do
            {
                forma.setPosition(rand() % ANCHO_VENT + 10, rand() % ALTO_VENT);

            } while (colisionConBordesForma(forma, bordeIzquierdo, bordeDerecho, bordeInferior, bordeSuperior));

            RectangleShape square(Vector2f(20, 20));
            square.setPosition(snake[snake.size() - 1].getPosition());
            snake.push_back(square);
            puntos++;
            // textoPuntos.setString("Puntos: " + to_string(puntos));
        }

        if (colisionConBordesSnake(snake, bordeIzquierdo, bordeDerecho, bordeInferior, bordeSuperior))
        {
            int x = (ANCHO_VENT / 2) - 20;
            for (int i = 0; i <= snake.size(); i++)
            {

                snake[i].setPosition(x, 180);
                x = x - 20;
            }
        }
        //bloque.setPosition(rand() % ANCHO_VENT + 10, rand() % ALTO_VENT);
        

        // Dentro del bucle principal
        if (puntos % 5 == 0 && puntos > 0)
        {
            do
            {
                bloque.setPosition(rand() % ANCHO_VENT + 10, rand() % ALTO_VENT);
            } while (colisionConBordesRectangulo(bloque, bordeIzquierdo, bordeDerecho, bordeInferior, bordeSuperior));
            
            
        }

        // Comprobar colisión entre la forma y el bloque
        if (forma.getGlobalBounds().intersects(bloque.getGlobalBounds()))
        {
            // Cambiar la posición de la forma a una nueva posición aleatoria
            do
            {
                forma.setPosition(rand() % ANCHO_VENT + 10, rand() % ALTO_VENT);
            } while (colisionConBordesForma(forma, bordeIzquierdo, bordeDerecho, bordeInferior, bordeSuperior));
        }

        cout << puntos << endl;

        ventana.clear();
        ventana.draw(bordeIzquierdo);
        ventana.draw(bordeDerecho);
        ventana.draw(bordeInferior);
        ventana.draw(bordeSuperior);
        ventana.draw(bloque);
        // ventana.draw(textoPuntos);
        for (const auto &square : snake)
        {

            ventana.draw(square);
        }
        ventana.draw(forma);
        ventana.display();
    }
}

bool colisionConBordesForma(const CircleShape &forma, const RectangleShape &bordeIzquierdo, const RectangleShape &bordeDerecho, const RectangleShape &bordeInferior, const RectangleShape &bordeSuperior)
{
    return forma.getGlobalBounds().intersects(bordeIzquierdo.getGlobalBounds()) ||
           forma.getGlobalBounds().intersects(bordeDerecho.getGlobalBounds()) ||
           forma.getGlobalBounds().intersects(bordeInferior.getGlobalBounds()) ||
           forma.getGlobalBounds().intersects(bordeSuperior.getGlobalBounds());
}

bool colisionConBordesSnake(const vector<RectangleShape> &snake, const RectangleShape &bordeIzquierdo, const RectangleShape &bordeDerecho, const RectangleShape &bordeInferior, const RectangleShape &bordeSuperior)
{
    return snake[0].getGlobalBounds().intersects(bordeIzquierdo.getGlobalBounds()) ||
           snake[0].getGlobalBounds().intersects(bordeDerecho.getGlobalBounds()) ||
           snake[0].getGlobalBounds().intersects(bordeInferior.getGlobalBounds()) ||
           snake[0].getGlobalBounds().intersects(bordeSuperior.getGlobalBounds());
}

bool colisionConBordesRectangulo(const RectangleShape &bloque, const RectangleShape &bordeIzquierdo, const RectangleShape &bordeDerecho, const RectangleShape &bordeInferior, const RectangleShape &bordeSuperior)
{
    return bloque.getGlobalBounds().intersects(bordeIzquierdo.getGlobalBounds()) ||
           bloque.getGlobalBounds().intersects(bordeDerecho.getGlobalBounds()) ||
           bloque.getGlobalBounds().intersects(bordeInferior.getGlobalBounds()) ||
           bloque.getGlobalBounds().intersects(bordeSuperior.getGlobalBounds());
}
