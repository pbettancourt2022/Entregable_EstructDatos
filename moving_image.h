#ifndef MOVING_IMG_H
#define MOVING_IMG_H
#include <stack>
#include <functional>
#include "basics.h"

// Clase que representa una imagen como una colección de 3 matrices siguiendo el
// esquema de colores RGB
struct Move {
    enum class Type { Up, Down, Left, Right, Rotate };
    Type type;
    int distance;
};

class moving_image {
private:
  unsigned char **red_layer; // Capa de tonalidades rojas
  unsigned char **green_layer; // Capa de tonalidades verdes
  unsigned char **blue_layer; // Capa de tonalidades azules
  std::stack<Move> moves_history;
  std::stack<Move> undone_moves;

public:
    bool opcion=true;
  // Constructor de la imagen. Se crea una imagen por defecto
  moving_image() {
    // Reserva de memoria para las 3 matrices RGB
    red_layer = new unsigned char*[H_IMG];
    green_layer = new unsigned char*[H_IMG];
    blue_layer = new unsigned char*[H_IMG];
    
    for(int i=0; i < H_IMG; i++) {
      red_layer[i] = new unsigned char[W_IMG];
      green_layer[i] = new unsigned char[W_IMG];
      blue_layer[i] = new unsigned char[W_IMG];
    }

    // Llenamos la imagen con su color de fondo
    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++) {
	red_layer[i][j] = DEFAULT_R;
	green_layer[i][j] = DEFAULT_G;
	blue_layer[i][j] = DEFAULT_B;
      }

    // Dibujamos el objeto en su posición inicial
    for(int i=0; i < 322; i++)
      for(int j=0; j < 256; j++) {
	if(!s_R[i][j] && !s_G[i][j] && !s_B[i][j]) {
	  red_layer[INIT_Y+i][INIT_X+j] = DEFAULT_R;
	  green_layer[INIT_Y+i][INIT_X+j] = DEFAULT_G;
	  blue_layer[INIT_Y+i][INIT_X+j] = DEFAULT_B;
	} else {
	  red_layer[INIT_Y+i][INIT_X+j] = s_R[i][j];
	  green_layer[INIT_Y+i][INIT_X+j] = s_G[i][j];
	  blue_layer[INIT_Y+i][INIT_X+j] = s_B[i][j];
	}
      }   
  }

  // Destructor de la clase
  ~moving_image() {
    for(int i=0; i < H_IMG; i++) {
      delete red_layer[i];
      delete green_layer[i];
      delete blue_layer[i];
    }

    delete red_layer;
    delete green_layer;
    delete blue_layer;
  }

  // Función utilizada para guardar la imagen en formato .png
  void draw(const char* nb) {
    _draw(nb);
  }
  void undo() {
    if (!moves_history.empty()) {
      Move last_move = moves_history.top();
      moves_history.pop();
      undone_moves.push(last_move);
      opcion=false;
      switch (last_move.type) {
            case Move::Type::Up:
                move_down(last_move.distance);
                break;
            case Move::Type::Down:
                move_up(last_move.distance);
                break;
            case Move::Type::Left:
                move_right(last_move.distance);
                break;
            case Move::Type::Right:
                move_left(last_move.distance);
                break;
            case Move::Type::Rotate:
                rotate();
                rotate();
                rotate();
                break;
        }
    }
  }
 void redo() {
     if (!undone_moves.empty()) {
         Move redo_move = undone_moves.top();
         undone_moves.pop();
         opcion= false;
         switch (redo_move.type) {
            case Move::Type::Up:
                move_up(redo_move.distance);
                break;
            case Move::Type::Down:
                move_down(redo_move.distance);
                break;
            case Move::Type::Left:
                move_left(redo_move.distance);
                break;
            case Move::Type::Right:
                move_right(redo_move.distance);
                break;
            case Move::Type::Rotate:
                rotate();
                break;
        }
     }
 }
  // Función que similar desplazar la imagen, de manera circular, d pixeles a la izquierda
  void move_left(int d) {
    unsigned char tmp_layer[H_IMG][W_IMG];

    // Mover la capa roja
    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG-d; j++)
	    tmp_layer[i][j] = red_layer[i][j+d];      
    
    for(int i=0; i < H_IMG; i++)
      for(int j=W_IMG-d, k=0; j < W_IMG; j++, k++)
    	tmp_layer[i][j] = red_layer[i][k];      

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
	    red_layer[i][j] = tmp_layer[i][j];

    // Mover la capa verde
    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG-d; j++)
	    tmp_layer[i][j] = green_layer[i][j+d];      
    
    for(int i=0; i < H_IMG; i++)
      for(int j=W_IMG-d, k=0; j < W_IMG; j++, k++)
    	tmp_layer[i][j] = green_layer[i][k];      

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
	    green_layer[i][j] = tmp_layer[i][j];

    // Mover la capa azul
    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG-d; j++)
	    tmp_layer[i][j] = blue_layer[i][j+d];      
    
    for(int i=0; i < H_IMG; i++)
      for(int j=W_IMG-d, k=0; j < W_IMG; j++, k++)
    	tmp_layer[i][j] = blue_layer[i][k];      

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
	    blue_layer[i][j] = tmp_layer[i][j];
    if (opcion){
    
    Move move_left;
    move_left.type = Move::Type::Left; // El movimiento contrario a move_up es move_down
    move_left.distance = d;
    moves_history.push(move_left);
    }
    opcion=true;
  }

 // Función que desplaza la imagen d pixeles a la derecha
void move_right(int d) {
    unsigned char tmp_layer[H_IMG][W_IMG];

    // Mover la capa roja
    for(int i = 0; i < H_IMG; i++)
        for(int j = W_IMG - 1; j >= d; j--)
            tmp_layer[i][j] = red_layer[i][j - d];
    
    for(int i = 0; i < H_IMG; i++)
        for(int j = 0, k = W_IMG - d; j < d; j++, k++)
            tmp_layer[i][j] = red_layer[i][k];

    for(int i = 0; i < H_IMG; i++)
        for(int j = 0; j < W_IMG; j++)
            red_layer[i][j] = tmp_layer[i][j];

    // Mover la capa verde
    for(int i = 0; i < H_IMG; i++)
        for(int j = W_IMG - 1; j >= d; j--)
            tmp_layer[i][j] = green_layer[i][j - d];
    
    for(int i = 0; i < H_IMG; i++)
        for(int j = 0, k = W_IMG - d; j < d; j++, k++)
            tmp_layer[i][j] = green_layer[i][k];

    for(int i = 0; i < H_IMG; i++)
        for(int j = 0; j < W_IMG; j++)
            green_layer[i][j] = tmp_layer[i][j];

    // Mover la capa azul
    for(int i = 0; i < H_IMG; i++)
        for(int j = W_IMG - 1; j >= d; j--)
            tmp_layer[i][j] = blue_layer[i][j - d];
    
    for(int i = 0; i < H_IMG; i++)
        for(int j = 0, k = W_IMG - d; j < d; j++, k++)
            tmp_layer[i][j] = blue_layer[i][k];

    for(int i = 0; i < H_IMG; i++)
        for(int j = 0; j < W_IMG; j++)
            blue_layer[i][j] = tmp_layer[i][j];
    if (opcion){
    Move move_right;
    move_right.type = Move::Type::Right; // El movimiento contrario a move_up es move_down
    move_right.distance = d;
    moves_history.push(move_right);
    }
    opcion=true;
}
// Función que desplaza la imagen d pixeles hacia arriba
void move_up(int d) {
    unsigned char tmp_layer[H_IMG][W_IMG];

    // Mover la capa roja
    for(int i = 0; i < H_IMG - d; i++)
        for(int j = 0; j < W_IMG; j++)
            tmp_layer[i][j] = red_layer[i + d][j];
    
    for(int i = H_IMG - d, k = 0; i < H_IMG; i++, k++)
        for(int j = 0; j < W_IMG; j++)
            tmp_layer[i][j] = red_layer[k][j];

    for(int i = 0; i < H_IMG; i++)
        for(int j = 0; j < W_IMG; j++)
            red_layer[i][j] = tmp_layer[i][j];

    // Mover la capa verde
    for(int i = 0; i < H_IMG - d; i++)
        for(int j = 0; j < W_IMG; j++)
            tmp_layer[i][j] = green_layer[i + d][j];
    
    for(int i = H_IMG - d, k = 0; i < H_IMG; i++, k++)
        for(int j = 0; j < W_IMG; j++)
            tmp_layer[i][j] = green_layer[k][j];

    for(int i = 0; i < H_IMG; i++)
        for(int j = 0; j < W_IMG; j++)
            green_layer[i][j] = tmp_layer[i][j];

    // Mover la capa azul
    for(int i = 0; i < H_IMG - d; i++)
        for(int j = 0; j < W_IMG; j++)
            tmp_layer[i][j] = blue_layer[i + d][j];
    
    for(int i = H_IMG - d, k = 0; i < H_IMG; i++, k++)
        for(int j = 0; j < W_IMG; j++)
            tmp_layer[i][j] = blue_layer[k][j];

    for(int i = 0; i < H_IMG; i++)
        for(int j = 0; j < W_IMG; j++)
            blue_layer[i][j] = tmp_layer[i][j];
    
    if (opcion){
    Move move_up;
    move_up.type = Move::Type::Up; // El movimiento contrario a move_up es move_down
    move_up.distance = d;
    moves_history.push(move_up);
    }
    opcion=true;
}
// Función que desplaza la imagen d pixeles hacia abajo
void move_down(int d) {
    unsigned char tmp_layer[H_IMG][W_IMG];

    // Mover la capa roja
    for(int i = H_IMG - 1; i >= d; i--)
        for(int j = 0; j < W_IMG; j++)
            tmp_layer[i][j] = red_layer[i - d][j];
    
    for(int i = 0; i < d; i++)
        for(int j = 0; j < W_IMG; j++)
            tmp_layer[i][j] = red_layer[H_IMG - d + i][j];

    for(int i = 0; i < H_IMG; i++)
        for(int j = 0; j < W_IMG; j++)
            red_layer[i][j] = tmp_layer[i][j];

    // Mover la capa verde
    for(int i = H_IMG - 1; i >= d; i--)
        for(int j = 0; j < W_IMG; j++)
            tmp_layer[i][j] = green_layer[i - d][j];
    
    for(int i = 0; i < d; i++)
        for(int j = 0; j < W_IMG; j++)
            tmp_layer[i][j] = green_layer[H_IMG - d + i][j];

    for(int i = 0; i < H_IMG; i++)
        for(int j = 0; j < W_IMG; j++)
            green_layer[i][j] = tmp_layer[i][j];

    // Mover la capa azul
    for(int i = H_IMG - 1; i >= d; i--)
        for(int j = 0; j < W_IMG; j++)
            tmp_layer[i][j] = blue_layer[i - d][j];
    
    for(int i = 0; i < d; i++)
        for(int j = 0; j < W_IMG; j++)
            tmp_layer[i][j] = blue_layer[H_IMG - d + i][j];

    for(int i = 0; i < H_IMG; i++)
        for(int j = 0; j < W_IMG; j++)
            blue_layer[i][j] = tmp_layer[i][j];
    if (opcion){
    Move move_down;
    move_down.type = Move::Type::Down; // El movimiento contrario a move_up es move_down
    move_down.distance = d;
    moves_history.push(move_down);
    }
    opcion=true;

}
// Función que rota la imagen 90 grados en sentido contrario a las agujas del reloj
void rotate() {
    unsigned char tmp_layer[H_IMG][W_IMG];

    // Rotar la capa roja
    for(int i = 0; i < H_IMG; i++)
        for(int j = 0; j < W_IMG; j++)
            tmp_layer[H_IMG - j - 1][i] = red_layer[i][j];

    for(int i = 0; i < H_IMG; i++)
        for(int j = 0; j < W_IMG; j++)
            red_layer[i][j] = tmp_layer[i][j];

    // Rotar la capa verde
    for(int i = 0; i < H_IMG; i++)
        for(int j = 0; j < W_IMG; j++)
            tmp_layer[H_IMG - j - 1][i] = green_layer[i][j];

    for(int i = 0; i < H_IMG; i++)
        for(int j = 0; j < W_IMG; j++)
            green_layer[i][j] = tmp_layer[i][j];

    // Rotar la capa azul
    for(int i = 0; i < H_IMG; i++)
        for(int j = 0; j < W_IMG; j++)
            tmp_layer[H_IMG - j - 1][i] = blue_layer[i][j];

    for(int i = 0; i < H_IMG; i++)
        for(int j = 0; j < W_IMG; j++)
            blue_layer[i][j] = tmp_layer[i][j];
    if (opcion){
    Move rotate;
    rotate.type = Move::Type::Rotate; // El movimiento contrario a move_up es move_down
    rotate.distance = 0;
    moves_history.push(rotate);
    }
    opcion=true;
}

private:
  // Función privada que guarda la imagen en formato .png
  void _draw(const char* nb) {
    unsigned char *rgb = new unsigned char [H_IMG * W_IMG * 3], *p = rgb;
    unsigned x, y;

    // La imagen resultante tendrá el nombre dado por la variable 'nb'
    FILE *fp = fopen(nb, "wb");

    // Transformamos las 3 matrices en una arreglo unidimensional
    for (y = 0; y < H_IMG; y++)
        for (x = 0; x < W_IMG; x++) {
            *p++ = red_layer[y][x];    /* R */
            *p++ = green_layer[y][x];    /* G */
            *p++ = blue_layer[y][x];    /* B */
        }
    // La función svpng() transforma las 3 matrices RGB en una imagen PNG 
    svpng(fp, W_IMG, H_IMG, rgb, 0);
    fclose(fp);
}

  
};

#endif
