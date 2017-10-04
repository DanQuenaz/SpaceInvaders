#ifndef SPACE_H				
#define SPACE_H

/*
Este arquivo contem a definição de cada entidade do jogo e as funções relacionadas as ações de cada entidade
*/

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PLAYER_DIMENSION_X 25//Dimensao do jogador em X
#define PLAYER_DIMENSION_Y 24//Dimensão do jogador em Y
#define ENEMY_DIMENSION 32//Dimensão do inimigo 32x32
#define BULLET_DIMENSION 10//Dimensao do disparo 10x10


//Posição (x,y)
struct Position{
	uint16_t x;
	uint16_t y;
	
};

//Jogador
struct Player{
	struct Position pos;
	uint16_t live;

};

//Inimigo
struct Enemy{
	struct Position pos;
	uint16_t live;
	
};

//Disparos
struct Bullet{
	struct Position pos;
	uint16_t live;
};

void setPos(struct Position *pos, uint16_t _x, uint16_t _y);//Seta posição de um entidade qualquer

void setPlayer(struct Player *player, struct Position _pos, uint16_t _live);//Aloca um jogador

void setEnemy(struct Enemy *enemy, struct Position _pos, uint16_t _live);//Aloca um inimigo

void moveHor(struct Position *pos, uint16_t value);//Realiza movimento horizontal de uma entidade

void moveVer(struct Position *pos, uint16_t value);//Realiza movimento horizontal de uma entidade

void drawPlayer(struct Player player);//Desenha a nave do jogador

void drawEnemy(struct Enemy enemy);//Desenha um inimigo

void movePlayer(struct Player *player, uint16_t _x);//Movimenta o jogador

void setPositionPlayer(struct Player *player, uint8_t _x);//Seta uma posição especifica para o jogador

void moveEnemyX(struct Enemy *enemy, uint8_t directionMov);//Movimenta inimigo no eixo X

void moveEnemyY(struct Enemy *enemy, uint8_t directionMov);//Movimenta inimigo no eixo Y

void setBullet(struct Bullet *bullet, struct Position pos, uint16_t _live);//Aloca um disparo

void moveBullet(struct Bullet *bullet);//Movimenta um disparo

void drawBullet(struct Bullet bullet);//Desenha um disparo

double_t calcDist(struct Position p1, struct Position p2);//Retorna a distancia entre dois pontos

#endif

