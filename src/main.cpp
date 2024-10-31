#include "sfml-utn-inspt.h"
// constantes de la ventana
const unsigned int ANCHO_VENT = 800;
const unsigned int ALTO_VENT = 600;
const unsigned int FRAMERATE = 60;

//-------------------------------------------------------------------------
// creamos la variable direccion t cuanros cuadraditos se mueven
enum Direccion
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};
const float MOVIMIENTO = 5.0f;

const float VELOCIDAD = 4.5f;
const float LADO_CUADRADO = 25.f;

int main()
{
    // creamos la ventana
    RenderWindow ventana = RenderWindow({ANCHO_VENT, ALTO_VENT}, "Unidad 1 - Taller de Videojuegos");
    ventana.setFramerateLimit(FRAMERATE);
    //-------------------------------------------------------------------------
    // creamos la "manzana"
    CircleShape forma = CircleShape(10, 20);
    forma.setPosition((ANCHO_VENT / 2) + 20, (ALTO_VENT / 2));

    //-------------------------------------------------------------------------
    // cabeceras de las funciones
    bool colisionConBordesForma(const CircleShape &forma, const RectangleShape &bordeIzquierdo, const RectangleShape &bordeDerecho, const RectangleShape &bordeInferior, const RectangleShape &bordeSuperior);

    bool colisionConBordesSnake(const vector<RectangleShape> &snake, const RectangleShape &bordeIzquierdo, const RectangleShape &bordeDerecho, const RectangleShape &bordeInferior, const RectangleShape &bordeSuperior);

    bool colisionConBordesRectangulo(const RectangleShape &bloque, const RectangleShape &bordeIzquierdo, const RectangleShape &bordeDerecho, const RectangleShape &bordeInferior, const RectangleShape &bordeSuperior);

    void moverSnake(Direccion & direccion, vector<RectangleShape> & snake);

    bool colisionConBloque(const RectangleShape &bloque, const RectangleShape &cabeza);

    //-------------------------------------------------------------------------
    // creamos el bloque
    RectangleShape bloque(Vector2f(80, 20));
    bloque.setPosition(100, 100);
    bloque.setFillColor(Color::Magenta);

    //-------------------------------------------------------------------------
    // creamos  el "cuerpo" de la serpiente
    vector<RectangleShape> snake;
    int x = (ANCHO_VENT / 2);
    for (int i = 0; i < 3; i++)
    {
        RectangleShape square(Vector2f(20, 20));
        square.setPosition(x, 180);
        x = x - 5;
        snake.push_back(square);
    }

    //-------------------------------------------------------------------------
    // creamos los bordes
    RectangleShape bordeIzquierdo(Vector2f(20, ALTO_VENT));
    bordeIzquierdo.setFillColor(Color::Red);
    bordeIzquierdo.setPosition(0, 0);

    RectangleShape bordeDerecho(Vector2f(20, ALTO_VENT));
    bordeDerecho.setFillColor(Color::Green);
    bordeDerecho.setPosition(ANCHO_VENT - 20, 0);

    RectangleShape bordeInferior(Vector2f(ANCHO_VENT, 20));
    bordeInferior.setFillColor(Color::Green);
    bordeInferior.setPosition(0, ALTO_VENT - 20);

    RectangleShape bordeSuperior(Vector2f(ANCHO_VENT, 20)); // Cambia el tamaño del borde superior
    bordeSuperior.setFillColor(Color::Red);                 // Asegúrate de usar bordeSuperior aquí
    bordeSuperior.setPosition(0, 0);                        // Posición en la parte superior

    //-------------------------------------------------------------------------
    // mostrar los puntos
    // Agregar un mensaje de puntos
    Font fuente;
    fuente.loadFromFile("../recursos/TT Octosquares Trial Regular.ttf");
    Text textoPuntos;
    textoPuntos.setFont(fuente);
    textoPuntos.setCharacterSize(20);
    textoPuntos.setFillColor(Color::White);
    textoPuntos.setPosition(10, 0);
    textoPuntos.setString("Puntos: 0");

    Text textoInicio;
    textoInicio.setFont(fuente);
    textoInicio.setCharacterSize(30);
    textoInicio.setFillColor(Color::White);
    textoInicio.setString("Bienvenido a Snake!\nPresiona ESPACIO para jugar.");
    textoInicio.setPosition((ANCHO_VENT - textoInicio.getLocalBounds().width) / 2, ALTO_VENT / 2 - 50);

    Text textoPerdiste;
    textoPerdiste.setFont(fuente);
    textoPerdiste.setCharacterSize(30);
    textoPerdiste.setFillColor(Color::Red);
    textoPerdiste.setString("Perdiste!\n");
    textoInicio.setPosition((ANCHO_VENT - textoPerdiste.getLocalBounds().width) / 2, ALTO_VENT / 2 - 50);
    //-------------------------------------------------------------------------
    // declaramos las variables
    Direccion direccion = RIGHT;
    int puntos = 0;
    bool seMovio = true, haMovido = false;
    bool juegoIniciado = false;
    bool chocoConBorde = false;
    float xCuadrado = 1.0, yCuadrado = 1;
    bool derecha = true, abajo = true;
    bool mostrarCuadrado = false;
    //-------------------------------------------------------------------------
    // Se define un cuadrado de LADO_CUADRADO de lado
    RectangleShape cuadrado = RectangleShape({LADO_CUADRADO, LADO_CUADRADO});
    // Color RGB (Rojo, Verde, Azul) entre 0 y 255
    cuadrado.setFillColor(Color(255, 0, 128));

    // Los diferenciales de movimiento (cuanto avanza el cuadrado) en los ejes x,y
    Vector2 diff = {VELOCIDAD, VELOCIDAD};
    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    // se abre la ventana
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
        //-------------------------------------------------------------------------
        // cuando el juego empieza, muestre la pantalla de inicio
        if (!juegoIniciado)
        {
            ventana.clear();
            ventana.draw(textoInicio);
            ventana.display();

            if (Keyboard::isKeyPressed(Keyboard::Space))
            {
                juegoIniciado = true;
                puntos = 0;
                // Reiniciar variables del juego si es necesario
            }
            continue;
        }

        //-------------------------------------------------------------------------
        // movimiento de la serpiente
        Direccion nuevaDireccion = direccion;

        if (Keyboard::isKeyPressed(Keyboard::Right) && direccion != LEFT)
        {
            nuevaDireccion = RIGHT;
            haMovido = true;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Down) && direccion != UP)
        {
            nuevaDireccion = DOWN;
            haMovido = true;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Left) && direccion != RIGHT)
        {
            nuevaDireccion = LEFT;
            haMovido = true;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Up) && direccion != DOWN)
        {
            nuevaDireccion = UP;
            haMovido = true;
        }

        if (nuevaDireccion != direccion)
        {
            direccion = nuevaDireccion;
        }

        if (haMovido)
        {
            moverSnake(direccion, snake);
            // haMovido = false; // Resetear la bandera
        }
        //-------------------------------------------------------------------------
        // movimiento aleatorio de la "manzana"
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
            textoPuntos.setString("Puntos: " + to_string(puntos));
        }
        //-------------------------------------------------------------------------
        // que pasa si el snake choca con el borde
        if (colisionConBordesSnake(snake, bordeIzquierdo, bordeDerecho, bordeInferior, bordeSuperior))
        {
            chocoConBorde = true;
            puntos = 0;
        }

        if (chocoConBorde)
        {
            ventana.clear();
            ventana.draw(textoPerdiste); // Dibuja el mensaje de pérdida
            ventana.display();
            continue; // Salir del bucle para evitar dibujar el juego
        }
        //-------------------------------------------------------------------------
        // si la snake choca con el bloque
        if (colisionConBloque(bloque, snake[0]))
        {
            puntos = 0;
            textoPuntos.setString("Puntos: " + to_string(puntos));

            for (int i = 2; i < snake.size(); i++)
            {
                snake.pop_back();
            }

            int x = (ANCHO_VENT / 2) - 20;
            for (int i = 0; i <= snake.size(); i++)
            {
                snake[i].setPosition(x, 180);
                x = x - 5;
            }
        }

        //-------------------------------------------------------------------------
        // movimiento del bloque
        if (puntos % 5 == 0 && puntos > 0 && seMovio)
        {
            do
            {
                bloque.setPosition(rand() % ANCHO_VENT, rand() % ALTO_VENT);
            } while (colisionConBordesRectangulo(bloque, bordeIzquierdo, bordeDerecho, bordeInferior, bordeSuperior));
            seMovio = false;
        }

        if (puntos % 5 != 0)
        {
            seMovio = true;
        }
        //-------------------------------------------------------------------------
        // que se agregue otro bloque que se mueva
        if (puntos >= 15 && puntos <= 30)
        {
            cuadrado.move(xCuadrado, yCuadrado);

            if (cuadrado.getPosition().y == (ALTO_VENT / 2))
            {
                if (abajo)
                {
                    xCuadrado *= -1;
                    derecha = false;
                }
                else
                {
                    xCuadrado *= 1;
                    derecha = false;
                }
            }

            if (cuadrado.getPosition().x == (ANCHO_VENT / 2))
            {
                if (derecha)
                {
                    yCuadrado *= 1;
                    abajo = false;
                }
                else
                {
                    yCuadrado *= -1;
                    abajo = true;
                }
            }

            mostrarCuadrado =  true;
        }
        if (puntos< 15 || puntos>30)
        {
            mostrarCuadrado = false;
        }
        
        //-------------------------------------------------------------------------
        // que la "manzana" no este donde esta el bloque
        if (forma.getGlobalBounds().intersects(bloque.getGlobalBounds()))
        {
            // Cambiar la posición de la forma a una nueva posición aleatoria
            do
            {
                forma.setPosition(rand() % ANCHO_VENT + 10, rand() % ALTO_VENT);
            } while (colisionConBordesForma(forma, bordeIzquierdo, bordeDerecho, bordeInferior, bordeSuperior));
        }

        //-------------------------------------------------------------------------
        // dibuja todos los objetos necesarios
        ventana.clear();
        ventana.draw(bordeIzquierdo);
        ventana.draw(bordeDerecho);
        ventana.draw(bordeInferior);
        ventana.draw(bordeSuperior);
        ventana.draw(bloque);
        ventana.draw(textoPuntos);

        if (mostrarCuadrado)
        {
            ventana.draw(cuadrado);
        }
        

        for (int i = 0; i < snake.size(); i++)
        {
            ventana.draw(snake[i]);
        }
        ventana.draw(forma);
        ventana.display();
    }
}

bool colisionConBordesForma(const CircleShape &forma, const RectangleShape &bordeIzquierdo, const RectangleShape &bordeDerecho, const RectangleShape &bordeInferior, const RectangleShape &bordeSuperior)
{
    return forma.getGlobalBounds().intersects(bordeIzquierdo.getGlobalBounds()) ||
           forma.getGlobalBounds().intersects(bordeInferior.getGlobalBounds()) ||
           forma.getGlobalBounds().intersects(bordeDerecho.getGlobalBounds()) ||
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

void moverSnake(Direccion &direccion, vector<RectangleShape> &snake)
{

    Vector2f cabezaPos = snake[0].getPosition();

    switch (direccion)
    {
    case UP:
        cabezaPos.y -= MOVIMIENTO;
        break;
    case DOWN:
        cabezaPos.y += MOVIMIENTO;
        break;
    case LEFT:
        cabezaPos.x -= MOVIMIENTO;
        break;
    case RIGHT:
        cabezaPos.x += MOVIMIENTO;
        break;
    }

    for (int i = snake.size() - 1; i > 0; --i)
    {
        snake[i].setPosition(snake[i - 1].getPosition());
    }
    snake[0].setPosition(cabezaPos);
}

bool colisionConBloque(const RectangleShape &bloque, const RectangleShape &cabeza)
{
    return cabeza.getGlobalBounds().intersects(bloque.getGlobalBounds());
}
