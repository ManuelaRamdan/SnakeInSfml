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
const float MOVIMIENTO = 20.0f;
Texture texCuerpoHorizontal, texCuerpoVertical;

const float VELOCIDAD = 4.5f;
const float LADO_CUADRADO = 25.f;

const string PATH = "../recursos/Graphics/";
#define CANT_TEXT 4
Texture textCola[CANT_TEXT];
Texture textDoblar[CANT_TEXT];

Texture texturaCabeza, texturaCuerpo, texturaCola;

// cabeceras de las funciones
bool colisionConBordesForma(const CircleShape &forma, const RectangleShape &bordeIzquierdo, const RectangleShape &bordeDerecho, const RectangleShape &bordeInferior, const RectangleShape &bordeSuperior);

bool colisionConBordesSnake(const vector<RectangleShape> &snake, const RectangleShape &bordeIzquierdo, const RectangleShape &bordeDerecho, const RectangleShape &bordeInferior, const RectangleShape &bordeSuperior);

bool colisionConBordesRectangulo(const RectangleShape &bloque, const RectangleShape &bordeIzquierdo, const RectangleShape &bordeDerecho, const RectangleShape &bordeInferior, const RectangleShape &bordeSuperior);

void moverSnake(Direccion &direccion, vector<RectangleShape> &snake);

bool colisionConBloque(const RectangleShape &bloque, const RectangleShape &cabeza);

int posCola(Vector2f actual, Vector2f anterior);
int posCurva(Vector2f anterior, Vector2f actual, Vector2f siguiente);

int main()
{
    texCuerpoHorizontal.loadFromFile(PATH + "cuerpoHorizontal.png");
    texCuerpoVertical.loadFromFile(PATH + "cuerpoVertical.png");

    textCola[0].loadFromFile(PATH + "colaArriba.png");
    textCola[1].loadFromFile(PATH + "colaDerecha.png");
    textCola[2].loadFromFile(PATH + "colaAbajo.png");
    textCola[3].loadFromFile(PATH + "colaIzq.png");

    // creamos la ventana
    RenderWindow ventana = RenderWindow({ANCHO_VENT, ALTO_VENT}, "Unidad 1 - Taller de Videojuegos");
    ventana.setFramerateLimit(FRAMERATE);
    //-------------------------------------------------------------------------
    // creamos la "manzana"
    Texture manzana;
    manzana.loadFromFile("../recursos/Graphics/manzana.png");
    CircleShape forma = CircleShape(15, 20);
    forma.setPosition((ANCHO_VENT / 2) + 20, (ALTO_VENT / 2));
    forma.setTexture(&manzana);

    //-------------------------------------------------------------------------
    // cabeceras de las funciones
    // Mover esta línea al inicio del archivo

    //-------------------------------------------------------------------------
    // creamos el bloque
    Texture bloqueText;
    bloqueText.loadFromFile("../recursos/Graphics/arbustro.png");
    RectangleShape bloque(Vector2f(100, 100));
    bloque.setPosition(100, 100);
    bloque.setTexture(&bloqueText);
    //-------------------------------------------------------------------------
    // cargar textura de la serpiente

    if (!texturaCabeza.loadFromFile(PATH + "cabezaDerecha.png") ||
        !texturaCuerpo.loadFromFile(PATH + "cuerpoHorizontal.png") ||
        !texturaCola.loadFromFile(PATH + "colaDerecha.png"))
    {
        return -1; // Termina si no se pueden cargar las texturas
    }

    //-------------------------------------------------------------------------
    // creamos  el "cuerpo" de la serpiente
    vector<RectangleShape> snake;
    int x = (ANCHO_VENT / 2);
    for (int i = 0; i < 3; i++)
    {
        RectangleShape square(Vector2f(20, 20));
        square.setPosition(x - (i * 20), 180);

        if (i == 0)
        {
            square.setTexture(&texturaCabeza); // Textura de la cabeza
        }
        else if (i == 2)
        {
            square.setTexture(&texturaCola); // Textura de la cola
        }
        else
        {
            square.setTexture(&texturaCuerpo); // Textura del cuerpo
        }

        snake.push_back(square);
    }

    //-------------------------------------------------------------------------
    // creamos los bordes
    RectangleShape bordeIzquierdo(Vector2f(20, ALTO_VENT));
    bordeIzquierdo.setFillColor(Color::Cyan);
    bordeIzquierdo.setPosition(0, 0);

    RectangleShape bordeDerecho(Vector2f(20, ALTO_VENT));
    bordeDerecho.setFillColor(Color::Blue);
    bordeDerecho.setPosition(ANCHO_VENT - 20, 0);

    RectangleShape bordeInferior(Vector2f(ANCHO_VENT, 20));
    bordeInferior.setFillColor(Color::Blue);
    bordeInferior.setPosition(0, ALTO_VENT - 20);

    RectangleShape bordeSuperior(Vector2f(ANCHO_VENT, 20)); // Cambia el tamaño del borde superior
    bordeSuperior.setFillColor(Color::Blue);                 // Asegúrate de usar bordeSuperior aquí
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
    bool juego = true;
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
    int contFrame;
    //-------------------------------------------------------------------------
    Texture texture;
    !texture.loadFromFile(PATH + "f3.png") && cout << "error en carga de imagen desde disco";
    // Crea un sprite y le asigna la textura cargada
    Sprite sprite(texture);
    sprite.setTextureRect(IntRect(0, 0, ANCHO_VENT, ALTO_VENT));

    //-------------------------------------------------------------------------
    int cantFrame =10;

    //-------------------------------------------------------------------------
    // se abre la ventana
    while (ventana.isOpen())
    {
        if (contFrame == 60)
        {
            contFrame = 0;
        }
        else
        {
            contFrame++;
        }

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


        if (haMovido && contFrame % cantFrame == 0)
        {
            moverSnake(direccion, snake);
            // haMovido = false; // Resetear la bandera
        }

        if (cantFrame>2 && cantFrame< 10 && ((puntos == 20 ) || (puntos == 40)))
        {
            cantFrame-=2;
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
            int posVecTextCola;
            posVecTextCola = posCola(snake[snake.size() - 1].getPosition(), snake[snake.size() - 2].getPosition());
            square.setTexture(&textCola[posVecTextCola]);
            snake[snake.size() - 1].setTexture(snake[snake.size() - 2].getTexture());
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

            for (int i = 3; i < snake.size(); i++)
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
        /*if (puntos >= 15 && puntos <= 30)
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

            mostrarCuadrado = true;
        }
        if (puntos < 15 || puntos > 30)
        {
            mostrarCuadrado = false;
        }*/

        //-------------------------------------------------------------------------
        // que la "manzana" no este donde esta el bloque
        while (forma.getGlobalBounds().intersects(bloque.getGlobalBounds()))
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
        ventana.draw(sprite);
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
    // Guarda la posición actual de la cabeza de la serpiente
    Vector2f cabezaPos = snake[0].getPosition();

    // Mueve la cabeza en la dirección deseada
    switch (direccion)
    {
    case UP:
        cabezaPos.y -= MOVIMIENTO;
        texturaCabeza.loadFromFile(PATH + "cabezaArriba.png");
        snake[0].setTexture(&texturaCabeza);

        break;
    case DOWN:
        cabezaPos.y += MOVIMIENTO;
        texturaCabeza.loadFromFile(PATH + "cabezaAbajo.png");
        snake[0].setTexture(&texturaCabeza);

        break;
    case LEFT:
        cabezaPos.x -= MOVIMIENTO;
        texturaCabeza.loadFromFile(PATH + "cabezaIzq.png");
        snake[0].setTexture(&texturaCabeza);

        break;
    case RIGHT:
        cabezaPos.x += MOVIMIENTO;
        texturaCabeza.loadFromFile(PATH + "cabezaDerecha.png");
        snake[0].setTexture(&texturaCabeza);

        break;
    }

    textDoblar[0].loadFromFile(PATH + "cuerpo_upRight.png");
    textDoblar[1].loadFromFile(PATH + "cuerpo_upLeft.png");
    textDoblar[2].loadFromFile(PATH + "cuerpo_downRight.png");
    textDoblar[3].loadFromFile(PATH + "cuerpo_downLeft.png");

    for (int i = snake.size() - 1; i > 0; --i)
    {

        Vector2f posAnterior = snake[i - 1].getPosition();
        if ((posAnterior.x != snake[i].getPosition().x))
        {
            snake[i].setTexture(&texCuerpoHorizontal);
        }
        else if ((posAnterior.y != snake[i].getPosition().y))
        {
            snake[i].setTexture(&texCuerpoVertical);
        }

        snake[i].setPosition(posAnterior);
    }

    for (int i = snake.size() - 2; i > 0; --i)
    {

        Vector2f posAnterior = snake[i + 1].getPosition();
        Vector2f posActual = snake[i].getPosition();
        Vector2f posSiguiente = snake[i - 1].getPosition();
        if (posAnterior.x != posSiguiente.x && posAnterior.y != posSiguiente.y)
        {
            int curvaPos = posCurva(posAnterior, posActual, posSiguiente);
            if (curvaPos != -1)
            {
                snake[i].setTexture(&textDoblar[curvaPos]);
            }
        }
    }

    int posVecTextCola;
    posVecTextCola = posCola(snake[snake.size() - 1].getPosition(), snake[snake.size() - 2].getPosition());
    snake[snake.size() - 1].setTexture(&textCola[posVecTextCola]);

    snake[0].setPosition(cabezaPos);
}

bool colisionConBloque(const RectangleShape &bloque, const RectangleShape &cabeza)
{
    return cabeza.getGlobalBounds().intersects(bloque.getGlobalBounds());
}

int posCola(Vector2f actual, Vector2f anterior)
{
    int pos = -1;
    if (actual.x < anterior.x)
    {
        pos = 1;
    }
    else if (actual.x > anterior.x)
    {
        pos = 3;
    }
    else if (actual.y > anterior.y)
    {
        pos = 0;
    }
    else if (actual.y < anterior.y)
    {
        pos = 2;
    }

    return pos;
}

int posCurva(Vector2f anterior, Vector2f actual, Vector2f siguiente)
{
    int pos = -1;
    if (anterior.x < actual.x)
    { // derecha
        if (actual.y < siguiente.y)
        {
            // derecha abajo
            pos = 1;
        }
        else
        {
            // derecha arriba
            pos = 3;
        }
    }
    else if (anterior.x > actual.x) // iqz
    {
        if (actual.y < siguiente.y)
        {
            // izq abajo
            pos = 0;
        }
        else
        {
            // izq arriba
            pos = 2;
        }
    }
    else if (anterior.y < actual.y)
    { // abajo
        if (actual.x < siguiente.x)
        {
            // abajo derecha
            pos = 2;
        }
        else
        {
            // abajo izquierda
            pos = 3;
        }
    }
    else if (anterior.y > actual.y)
    {
        if (actual.x < siguiente.x)
        {
            // arriba derecha
            pos = 0;
        }
        else
        {
            // arriba izquierda
            pos = 1;
        }
    }

    return pos;
}
