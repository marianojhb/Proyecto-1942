#include <iostream>
#include "Menu.h"


Menu::Menu(float width, float height) {
  // Carga de recursos y configuraciones fijas
  if (!font.loadFromFile("assets/fonts/fuente.ttf")) {
    std::cout << "No se pudo cargar la fuente" << std::endl;
  }

  if (!logoTexture.loadFromFile("assets/sprites/logo.png")) {
    std::cout << "No se pudo cargar el Logo del Juego" << std::endl;
  }

  if (!planeTexture.loadFromFile("assets/sprites/Personaje.png")) {
    std::cout << "No se pudo cargar la imagen del avion" << std::endl;
  }

  if (!capcomTexture.loadFromFile("assets/sprites/Capcom.jfif")) {
    std::cout << "No se pudo cargar la imagen del Logo Capcom" << std::endl;
  }

  // CONFIGURACIONES FÍSICAS DEL MENÚ (imagenes, posiciones)
  planeSprite.setTexture(planeTexture);
  planeSprite.setScale(0.70f, 0.70f);
  planeSprite.setPosition(100, 350);

  logoSprite.setTexture(logoTexture);
  logoSprite.setPosition(95, 195);
  logoSprite.setScale(2.0f, 2.0f);

  capcomSprite.setTexture(capcomTexture);
  capcomSprite.setPosition(230, 650);
  capcomSprite.setScale(0.40f, 0.40f);

  capcom.setFont(font);
  capcom.setFillColor(sf::Color::White);
  capcom.setString("@1984 CAPCOM");
  capcom.setCharacterSize(12);
  capcom.setPosition(223, 725);

  insertCoin.setFont(font);
  insertCoin.setFillColor(sf::Color::Yellow);
  insertCoin.setString("Insert Coin");
  insertCoin.setCharacterSize(32);
  insertCoin.setPosition(145, 400);



}

//-------------------------------------------------------------

// METODO QUE DIBUJA TODOS LOS ELEMENTOS DEL VISUALES DEL MENU
void Menu::start()
{
  // Lógica de inicialización dinámica
  showInsertCoin = true;
  showMenuOptions = false;

  // Opciones del menú
  menu[0].setFont(font);
  menu[0].setFillColor(sf::Color::Green);
  menu[0].setString("Jugar");
  menu[0].setCharacterSize(32);
  menu[0].setPosition(225, 350);

  menu[1].setFont(font);
  menu[1].setFillColor(sf::Color::White);
  menu[1].setString("Creditos");
  menu[1].setCharacterSize(32);
  menu[1].setPosition(190, 450);

  menu[2].setFont(font);
  menu[2].setFillColor(sf::Color::White);
  menu[2].setString("Salir");
  menu[2].setCharacterSize(32);
  menu[2].setPosition(225, 550);

  selectedItemIndex = 0;

  // Reinicia el reloj y el estado de parpadeo
  blinkClock.restart();
  blinkState = true;
}


void Menu::cmd()
{

}
//METODO QUE ACTUALIZA
//-------------------------------------------------------------------
void Menu::update() {

  if (blinkClock.getElapsedTime().asSeconds() > 0.5f) {   //registra el tiempo transcurrido desde la �ltima vez que se reinici� con blinkClock.restart()

    //Alterna el valor de blinkState entre true y false. Esto permite que el texto parpadee
    blinkState = !blinkState;

    //Reinicia el reloj blinkClock para que comience a contar nuevamente desde cero
    blinkClock.restart();
  }
}


void Menu::draw(sf::RenderTarget &target , sf::RenderStates states)const{

    target.draw(logoSprite,states); //DIBUJA EL LOGO 1942

    target.draw(capcom,states); //DIBUJA EL TEXTO @1984 CAPCOM

    target.draw(capcomSprite,states); //DIBUJA EL LOGO DE CAPCOM

    //blinkState controla el parpadeo del texto "Insert Coin". Si est� en true, dibuja insertCoin en la ventana, haciendo que el texto "Insert Coin" parpadee.
    if (showInsertCoin) {
        if (blinkState) {
            target.draw(insertCoin,states);
        }
    }
    //Verifica si showMenuOptions est� en true
    if (showMenuOptions) {
        for (int i = 0; i < 3; i++) {
            target.draw(menu[i],states);      //Si es as�, recorre el arreglo menu y dibuja cada elemento de las opciones del men�
        }
        target.draw(planeSprite,states); // dibuja el sprite planeSprite, que representa la imagen del avi�n
    }
}


void Menu::render(sf::RenderWindow &window)
{
  window.draw(*this);
}


//-------------------------------------------------------------------
//METODO PARA MOVER PARA ARRIBA EN EL MENU Y ACTUALIZAR EL COLOR Y EL AVION QUE INDICAN LA SELECCION
//--------------------------------------------------------------------------------------------------
void Menu::moveUp() {

    //Primero, verifica si showMenuOptions es true, es decir, si las opciones del men� est�n visibles.
    if (showMenuOptions) {

        //RESTABLECE EL COLOR DE LA OPCION ACTUAL ANTES DE CAMBIAR LA SELECCION
        menu[selectedItemIndex].setFillColor(sf::Color::White);

        //VERIFICA SI ESTA EN EL PRIMER ELEMENTO, SI ES ASI, VA AL ULTIMO
        if (selectedItemIndex - 1 < 0) {
            selectedItemIndex = 2; // CAMBIA EL INDICE AL ULTIMO ELEMENTO
        } else {
            selectedItemIndex--; //MUEVE LA SELECCION HACIA ARRIBA

        }
        //CAMBIA EL COLOR DE LA NUEVA SELECCION
        menu[selectedItemIndex].setFillColor(sf::Color::Green);

        //MUEVE EL AVION A LA NUEVA OPCION SELECCIONADA
        planeSprite.setPosition(100, menu[selectedItemIndex].getPosition().y);
    }
}



//------------------------------------------------------------------------------------------------
//METODO PARA MOVER PARA ABAJO EN EL MENU Y ACTUALIZAR EL COLOR Y EL AVION QUE INDICAN LA SELECCION
//--------------------------------------------------------------------------------------------------
void Menu::moveDown() {

    //Primero, verifica si showMenuOptions es true, es decir, si las opciones del men� est�n visibles.
    if (showMenuOptions) {

       //RESTABLECE EL COLOR DE LA OPCION ACTUAL ANTES DE CAMBIAR LA SELECCION
        menu[selectedItemIndex].setFillColor(sf::Color::White);

        //VERIFICA SI ESTA EN EL ULTIMO ELEMENTO, SI ES ASI, VA AL PRIMERO
        if (selectedItemIndex + 1 >= 3) {
            selectedItemIndex = 0;  // CAMBIA EL INDICE AL PRIMER ELEMENTO
        } else {
            selectedItemIndex++; //MUEVE LA SELECCION HACIA ABAJO

        }
        //CAMBIA EL COLOR DE LA NUEVA SELECCION
        menu[selectedItemIndex].setFillColor(sf::Color::Green);

        //MUEVE EL AVION A LA NUEVA OPCION SELECCIONADA
        planeSprite.setPosition(100, menu[selectedItemIndex].getPosition().y);
    }
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void Menu::handleEnterPress() {
    if (showInsertCoin) {
        showInsertCoin = false;
        showMenuOptions = true;
    } else if (showMenuOptions) {
        // Maneja la selecci�n del men�
        int x = getPressedItem();
        if (x == 0) {
            // L�gica para iniciar el juego
        } else if (x == 1) {
            // L�gica para mostrar cr�ditos
        } else if (x == 2) {
            // L�gica para salir
//          sceneManager->changeScene(changeScene(new Level1Scene());)
        }
    }
}

//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
int Menu::getPressedItem() {
  return selectedItemIndex;
}
//--------------------------------------------------------------------------------------------------

