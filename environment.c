/*
Este arquivo contem as fun��es que gerenciam todo andamento do jogo.

*/

#include "environment.h"
#define N_ENEMIES 9//Numero m�ximo de inimigos
#define N_BULLETS 3//Valor m�ximo de disparos
#define N_HC 32//Valor m�ximo de pontua��es

extern const unsigned short _logo[];//Sprite da Logo do jogo
extern const unsigned short _acmenuPointer[];//Sprite do cursor do menu principal

uint16_t n_enemies; //n�mero de inimigos
uint16_t i, j, k; //contadores auxiliares
uint32_t valueConv, memConv; //variaveis para armazenar valor do ADC
uint16_t nBullets, nEnemies; //quantidade de disparos e inimigos 
uint8_t controlMov, directionMov; //variaveis de controle da movimenta��o inimiga
uint8_t gameState;

//flags de controle geral
uint8_t flagMovEnemies;
uint8_t flagShot, flagMenu;

uint32_t highScores[32]; //vetor para armazenar as pontua��es
uint32_t score; //pontua��o atual
uint16_t nPlays;//numero de vezes que o jogador jogou
uint16_t stage;//nivel que o jogador est�
uint16_t limEnemies;//limite que os inimigos podem chegar
uint16_t dificult;//nivel de dificuldade
uint16_t valueTIM;//valor do TIM
uint16_t contTIM;//contador do TIM

char textAux[64];//String auxiliar

struct Enemy enemies[N_ENEMIES]; //vetor de inimigos
struct Bullet bullets[N_BULLETS];//vetor de disparos
struct Enemy auxE; //variavel auxiliar
struct Position auxP;//variavel de posisi�ao auxiliar
struct Player player;//jogador
struct Bullet bullet;//variavel para cria��o de disparos
struct Position menuPointer;//cursor do menu

void updateScreen(){//Fun��o respons�vel por atualizar a tela do jogo a cada frame
	for(i=0; i<nEnemies; ++i){
		drawEnemy(enemies[i]);
	}
	
	for(i=0; i<nBullets; ++i){
		drawBullet(bullets[i]);
	}
	drawPlayer(player);
}

void printStatusMenu(uint8_t opt){//Atualiza o menu lateral do jogo 
	GLCD_SetForegroundColor(GLCD_COLOR_BLACK);
	if(opt == 0){
		GLCD_DrawVLine(225, 0, 240);
		GLCD_DrawString(226, 10, "STAGE");
		sprintf(textAux, "%d", stage);
		GLCD_DrawString(226, 30, textAux);
		
		GLCD_DrawString(226, 60, "SCORE");
		sprintf(textAux, "%d", score);
		GLCD_DrawString(226, 80, textAux);
		
	}
	else if(opt == 1){
		GLCD_DrawString(226, 60, "SCORE");
		sprintf(textAux, "%d", score);
		GLCD_DrawString(226, 80, textAux);
	}
	GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
}

void restartSpace(){//Reinicia todas as variaveis quando o jogador passa de nivel
	uint16_t _posEH, _posEV;
	controlMov= directionMov = 0;
	flagMovEnemies = flagShot = 0;
	nBullets = 0;
	nEnemies = N_ENEMIES;
	i = j = k = 0;
	_posEH = 30;
	_posEV = 0;
	setPos(&menuPointer, 70, 140);
	for(j=0; j<3; ++j){
		for(i=0; i<3; ++i){
			setPos(&auxP, _posEH, _posEV);
			setEnemy(&auxE, auxP, 1);
			enemies[k] = auxE;
			_posEH += 80;
			k++;
		}
		_posEH = 30;
		_posEV += 30;
	}
	setPos(&auxP, 160, 200);
	setPlayer(&player, auxP, 1);
}

void startSpace(){//Inicia todas as variaveis como inimigos, joagdor, posi��es
	uint16_t _posEH, _posEV;
	controlMov= directionMov=gameState=0;
	flagMovEnemies = flagShot = flagMenu = 0;
	nBullets = score = 0;
	valueTIM = 100;
	contTIM = 5;
	limEnemies = 160;
	nEnemies = N_ENEMIES;
	stage = 1;
	i = j = k = 0;
	_posEH = 30;
	_posEV = 0;
	setPos(&menuPointer, 70, 140);
	for(j=0; j<3; ++j){
		for(i=0; i<3; ++i){
			setPos(&auxP, _posEH, _posEV);
			setEnemy(&auxE, auxP, 1);
			enemies[k] = auxE;
			_posEH += 80;
			k++;
		}
		_posEH = 30;
		_posEV += 30;
	}
	setPos(&auxP, 160, 200);
	setPlayer(&player, auxP, 1);
}

void EXTI9_5_IRQHandler(void){//Tratamento de interrup�ao para botao USER
	EXTI_ClearITPendingBit(EXTI_Line7);
	flagMenu = 1;
}

void EXTI15_10_IRQHandler(void){//Tratamento de interrup��o para bot�o TAMPER
	EXTI_ClearITPendingBit(EXTI_Line13);
	flagShot = 1;
}

void TIM4_IRQHandler(){//Tratamento de interrup��o para o TIM
	uint16_t t1;
	if(TIM_GetITStatus(TIM4, TIM_IT_CC1) == SET){
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
		t1 = TIM_GetCapture1(TIM4);
		TIM_SetCompare1(TIM4, t1+valueTIM);
		flagMovEnemies++;
	}
	
}
void createBullet(){//Fun��o que aloca novos disparos sempre que necess�rio
	setPos(&auxP, player.pos.x+(PLAYER_DIMENSION_X/2), player.pos.y);
	setBullet(&bullet, auxP, 1);
	bullets[nBullets] = bullet;
	nBullets++;
}

void moveAllEnemies(){//Fun��o que movimenta todos os inimigos quando necess�rio
	for(i=0; i<nEnemies; ++i){
		moveEnemyX(&enemies[i], directionMov);
	}
	controlMov++;
	if(controlMov>=5){
		for(i=0; i<nEnemies; ++i){
			moveEnemyY(&enemies[i], directionMov);
			if(enemies[i].pos.y >= limEnemies){
				gameState = 2;
			}
		}
		controlMov = 0;
		directionMov++;
	}
}

void removeBullet(uint16_t index){//Fun��o que remove um disparo quando o mesmo atinge o topo da tela ou um inimigo
	for(k=index; k<(nBullets-1); ++k){
		bullets[k] = bullets[k+1];
	}
	nBullets--;
}

void removeEnemy(uint16_t index){//Fun��o que remove um inimigo quando o mesmo � atingido por um disparo
	for(k=index; k<(nEnemies-1); ++k){
		enemies[k] = enemies[k+1];
	}
	nEnemies--;
}

void checkColision(){//Fun��o que verifica colis�es 
	struct Position aux1, aux2;
	uint16_t _x, _y;
	GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
	for(i=0; i<nBullets; ++i){
		moveBullet(&(bullets[i]));
		if(bullets[i].pos.y<=2){
			_x = bullets[i].pos.x;
			_y = bullets[i].pos.y;
			removeBullet(i);
			GLCD_DrawBargraph(_x, _y, 30, 30, 100);
		}
		for(j=0; j<nEnemies; ++j){
			setPos(&aux1, bullets[i].pos.x+(BULLET_DIMENSION/2), bullets[i].pos.y+(BULLET_DIMENSION/2));
			setPos(&aux2, enemies[j].pos.x+(ENEMY_DIMENSION/2), enemies[j].pos.y+(ENEMY_DIMENSION/2));
			if(calcDist(aux1, aux2) <= (ENEMY_DIMENSION/2)){
				_x = enemies[j].pos.x;
				_y = enemies[j].pos.y;
				removeBullet(i);
				removeEnemy(j);
				score += 5;
				GLCD_DrawBargraph(_x, _y, 40, 40, 100);
				printStatusMenu(1);
			}
		}
	}
}

void printHighScores(){//Fun��o que exibe as maiores pontua��es
	GLCD_ClearScreen();
	GLCD_SetForegroundColor(GLCD_COLOR_BLACK);
	GLCD_DrawString(3*16, 0*24, "HIGH SCORES");
	for(i=0; i<nPlays; ++i){
		for(k=i; k<nPlays; ++k){
			if(highScores[i] < highScores[k]){
				highScores[i] = highScores[i] + highScores[k];
				highScores[k] = highScores[i] - highScores[k];
				highScores[i] = highScores[i] - highScores[k];
			}
		}
	}
	for(i=0; i<5; ++i){
		if(nPlays > i){
			sprintf(textAux, "%d", (i+1));
			GLCD_DrawString(1*16, 24*(i+3), textAux);
			GLCD_DrawString(2*16, 24*(i+3), " - ");
			sprintf(textAux, "%d", highScores[i]);
			GLCD_DrawString(5*16, 24*(i+3), textAux);
		}
		else{
			sprintf(textAux, "%d", (i+1));
			GLCD_DrawString(1*16, 24*(i+3), textAux);
			GLCD_DrawString(2*16, 24*(i+3), " - ");
			GLCD_DrawString(5*16, 24*(i+3), "xxx");
		}
	}
	
	while(!flagShot);
	flagShot = 0;
	GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
	GLCD_ClearScreen();
}

uint8_t findScore(){//Funa��o que verifica se um pontua��o j� foi registrada
	for(k=0; k<nPlays; ++k){
		if(highScores[k] == score){
			return 0;
		}
	}
	return 1;
}

/*
Esta � a principal fun��o, nela contem o loop infinito que mantem o jogo rodando.
A mesma tamb�m gerencia atrav�s do estado das variaveis qual tela � exibida ao jogador
Ela tamb�m realiza as chamadas de todas as demais fun��es anteriores
*/
void environment(){
	startSpace();
	nPlays = 0;
	valueConv = directionMov = 0;
	memConv = 0;
	dificult = 0;
	
	while(!0){
		if(gameState == 1){//Estado de jogo iniciado
			updateScreen();
			ADC_SoftwareStartConvCmd(ADC1, ENABLE);
			while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
			
			if(ADC_GetConversionValue(ADC1)>=memConv+30 || ADC_GetConversionValue(ADC1)<=memConv-30){
				memConv = ADC_GetConversionValue(ADC1);
				valueConv = ((memConv*190)/4096)-player.pos.x;
				movePlayer(&player, valueConv);		
				//setPositionPlayer(&player, (valueConv*200)/4096);
			}
			if(flagMovEnemies>= contTIM){
				moveAllEnemies();
				flagMovEnemies = 0;
			}
			if(flagShot && nBullets <= N_BULLETS){
				createBullet();
				flagShot = 0;
			}
			checkColision();
			GLCD_DrawHLine(0, limEnemies+ENEMY_DIMENSION, 225);
			if(!nEnemies){
				stage++;
				sprintf(textAux, "%d", stage);
				GLCD_ClearScreen();
				GLCD_SetForegroundColor(GLCD_COLOR_BLACK);
				GLCD_DrawString(30, 130, "STAGE: ");
				GLCD_DrawString(150, 130, textAux);
				while(!flagShot);
				flagShot = 0;
				flagMenu =0;
				if(contTIM - 1>= 0) contTIM --;
				GLCD_ClearScreen();
				restartSpace();
				printStatusMenu(0);
				
			}
		}
		else if(gameState == 0){//Estado de menu inicial
			GLCD_SetForegroundColor(GLCD_COLOR_BLACK);
			GLCD_DrawBitmap(menuPointer.x, menuPointer.y, 20, 14, (uint8_t *) &_acmenuPointer[0]);
			GLCD_DrawBitmap(50, 10, 240, 100, (uint8_t *)&_logo[0]);
			GLCD_DrawString(90, 140, "START GAME");
			GLCD_DrawString(90, 170, "HIGH SCORES");
			
			if(dificult == 0){
				GLCD_DrawString(90, 200, "EASY");
				limEnemies = 160;
			}
			else if(dificult == 1){
				GLCD_DrawString(90, 200, "NORMAL");
				limEnemies = 140;
			}
			else if(dificult == 2){
				GLCD_DrawString(90, 200, "HARD");
				limEnemies = 120;
			}
			else{
				GLCD_DrawString(90, 200, "IMPOSSIBLE");
				limEnemies = 100;
			}
			
			if(flagMenu){
				GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
				if(menuPointer.y == 140){
					GLCD_DrawBargraph(menuPointer.x, menuPointer.y, 20, 14, 50);
					setPos(&menuPointer, 70, 170);
				}
				else if(menuPointer.y == 170){
					GLCD_DrawBargraph(menuPointer.x, menuPointer.y, 20, 14, 50);
					setPos(&menuPointer, 70, 200);
				}
				else{
					GLCD_DrawBargraph(menuPointer.x, menuPointer.y, 20, 14, 50);
					setPos(&menuPointer, 70, 140);
				}
				flagMenu = 0;
			}
			if(flagShot){
				if(menuPointer.y == 140){
					sprintf(textAux, "%d", stage);
					GLCD_ClearScreen();
					GLCD_DrawString(30, 130, "STAGE: ");
					GLCD_DrawString(150, 130, textAux);
					while(!flagShot);
					flagMenu =0;
					flagShot =0;
					GLCD_ClearScreen();
					gameState = 1;
					printStatusMenu(0);
				}
				else if(menuPointer.y == 170){
					
					flagShot =0;
					printHighScores();
					flagMenu =0;
				}
				else{
					GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
					GLCD_DrawBargraph(90, 200, 200, 24, 100);
					dificult++;
					if(dificult >= 4) dificult=0;
				}
				flagShot = 0;
			}
			
		}
		
		else{//Estado de game over
			if(findScore() && score){
				highScores[nPlays] = score;
				nPlays++;
			}
			GLCD_ClearScreen();
			GLCD_SetForegroundColor(GLCD_COLOR_BLACK);
			GLCD_DrawString(100, 140, "GAME OVER");
			while(!flagShot);
			flagShot = 0;
			flagMenu =0;
			GLCD_ClearScreen();
			gameState = 0;
			startSpace();
			
		}
	}
}