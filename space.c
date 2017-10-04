/*
Este arquivo contem as implementações do arquivos space.h
*/

#include "space.h"

extern const unsigned short _acnave[];//sprite da nave
extern const unsigned short _alien[];//sprite dos inimigos
extern const unsigned short _shot[];//sprite do disparo


void setPos(struct Position *pos, uint16_t _x, uint16_t _y){
	pos->x = _x;
	pos->y = _y;
}

void setPlayer(struct Player *player, struct Position _pos, uint16_t _live){
	player->live = _live;
	player->pos = _pos;
}

void setEnemy(struct Enemy *enemy, struct Position _pos, uint16_t _live){
	enemy->live = _live;
	enemy->pos = _pos;
}

void moveHor(struct Position *pos, uint16_t value){
	pos->x += value;
}

void moveVer(struct Position *pos, uint16_t value){
	pos->y += value;
}

void drawPlayer(struct Player player){
	GLCD_DrawBitmap(player.pos.x, player.pos.y, PLAYER_DIMENSION_X, PLAYER_DIMENSION_Y, (uint8_t *) &_acnave[0]);
}

void drawEnemy(struct Enemy enemy){
	GLCD_DrawBitmap(enemy.pos.x, enemy.pos.y, ENEMY_DIMENSION, ENEMY_DIMENSION, (uint8_t *) &_alien[0]);
}

void movePlayer(struct Player *player, uint16_t _x){
	GLCD_DrawBargraph(player->pos.x, player->pos.y, PLAYER_DIMENSION_X, PLAYER_DIMENSION_Y, 50);
	player->pos.x += _x;
}

void setPositionPlayer(struct Player *player, uint8_t _x){
	//GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
	//GLCD_DrawBargraph(player->pos.x, player->pos.y, PLAYER_DIMENSION, PLAYER_DIMENSION, 100);
	player->pos.x = _x;
}

void moveEnemyX(struct Enemy *enemy, uint8_t directionMov){
	if(directionMov%2){
		enemy->pos.x += 5;
	}
	else{
		enemy->pos.x -= 5;
	}
}

void moveEnemyY(struct Enemy *enemy, uint8_t directionMov){
	enemy->pos.y += 5;
}

void setBullet(struct Bullet *bullet, struct Position pos, uint16_t _live){
	bullet->pos = pos;
	bullet->live = _live;
}

void moveBullet(struct Bullet *bullet){
	bullet->pos.y -= 5;
}

void drawBullet(struct Bullet bullet){
	GLCD_DrawBitmap(bullet.pos.x, bullet.pos.y, BULLET_DIMENSION, BULLET_DIMENSION, (uint8_t *) &_shot[0]);
}

double_t calcDist(struct Position p1, struct Position p2){
	double_t dist;
	dist = sqrt((pow((p2.x-p1.x), 2) + pow((p2.y-p1.y), 2)));
	return dist;
}