#include "data/scripts/player.c"

/* * Biblioteca de Funções OpenBOR
 * Criada originalmente por Don Drago
 */

/* * Pula para uma ramificação (branch) se a pontuação combinada ultrapassar/ficar abaixo de um valor determinado
 * Você pode colocar isso em uma animação de morte de personagem para, por exemplo, ir para uma fase secreta
 */
void changeBranchByTotalScore(int score, int extraScorePerPlayer, int goNow, char branchHigh, char branchLow) {
	int players=openborvariant("count_players");
	int totalscore = 0;
	int scoreMultiplier = players-1;
	int x;
	
	for(x=0;x<players;x++) {
		totalscore+=getplayerproperty(x, "score",0);
	}
	scoreMultiplier*=extraScorePerPlayer;
	score+=scoreMultiplier;
	
	if(totalscore < score) {
		if(branchLow!= NULL()) { jumptobranch(branchLow, goNow); }
	}
	else {
		if(branchHigh!= NULL()) { jumptobranch(branchHigh, goNow); }		
	}
}

/* * Pula para uma ramificação se a pontuação de um jogador ultrapassar um valor determinado.
 * Se nenhum jogador cumprir o requisito de pontuação, você pode usá-lo para enviar os
 * jogadores para OUTRA ramificação
 * Você pode colocar isso em uma animação de morte de personagem para, por exemplo, ir para uma fase secreta
 */
void changeBranchByHighestScore(int score, int goNow, char branchHigh, char branchLow) {
	int players=openborvariant("count_players");
	int playerScore = 0;
	int maxScore = 0;
	int x;
	
	for(x=0;x<players;x++) {
		playerScore = getplayerproperty(x, "score",0);
		if(maxScore < playerScore) { maxScore =  playerScore; }
	}
	if(maxScore < score) {
		if(branchLow!= NULL()) { jumptobranch(branchLow, goNow);}
	}
	else {
		if(branchHigh!= NULL()) { jumptobranch(branchHigh, goNow);}		
	}
}

/* * Adiciona acertos ao seu contador de hits sem realmente "atingir" o oponente
 * Útil em agarrões de múltiplos acertos (multi hit throws)
 */
void addhits(int hits) {
	void self = getlocalvar("self");
	int previous = getentityproperty(self, "rush_count");
	changeentityproperty(self, "rush_count", previous+hits);
}


/* * Define o MP
 */
void setMp(int mp) {
	void self = getlocalvar("self");
	changeentityproperty(self, "mp", mp);
}

/* * Salto guiado para o oponente mais próximo, com o tempo desejado (reachAt) para alcançá-lo
 * e ajustes de posição (X, Y e Z). Se não houver oponente, usa valores de "whiff" (vazio)
 * para pular. Use flipMe = 1 para virar para o alvo, se desejar você pode usar isso
 * com ajustY=NULL() e whiffY=NULL() enquanto pula, para fazer um "salto de perseguição"
 * (veja o Super Final Fight Gold PLUS: Won Won tem um ataque de salto de perseguição)
 */
void guidedJump(int reachAt, int adjustX, int adjustY, int adjustZ, float whiffX, float whiffY, float whiffZ, int flipMe) {
	void self = getlocalvar("self");
	void opponent = findtarget(self);
	int facing = (getentityproperty(self,"direction")==0) ? (-1) : (1);

	float gravity = -(getlevelproperty("gravity")*(1.0-getentityproperty(self,"antigravity")));
	float pX = getentityproperty(self, "x");
	float pY = getentityproperty(self, "y");
	float pZ = getentityproperty(self, "z");
	float x, y, z;
	
	reachAt--;

	if(opponent!= NULL()) {
		float eX = getentityproperty(opponent, "x");
		float eY = getentityproperty(opponent, "y");
		float eZ = getentityproperty(opponent, "z");
		
		adjustX *= (eX < pX) ?(-1) : (1);
		
		if(adjustX !=NULL()) {
			x = (eX-pX+adjustX)*facing;
			x = (x/reachAt)*facing;			
		}
		else { x = NULL(); }
		
		if(adjustY !=NULL()) {
			y = eY-pY+adjustY;
			y = ((reachAt*gravity/2)+(y/reachAt));
		}
		else { y = NULL(); }
		
		if(adjustZ !=NULL()) {
			z = eZ-pZ+adjustZ;
			z = z/reachAt;
		}
		else { z = NULL(); }
		
		if(eX < pX) {
			if(flipMe != 0){ changeentityproperty(self, "direction", 0); }
		}
		else {
			if(flipMe != 0){ changeentityproperty(self, "direction", 1); }
		}			
	}
	else {
		x = whiffX*facing;
		y = whiffY;
		z = whiffZ;
	}
	
	 
 tossentity(self, y, x, z);
}

/* * Vira para o lado oposto
 */
void turn() {
	void self = getlocalvar("self");
	void facing = getentityproperty(self, "direction");
		
	if(facing) { changeentityproperty(self, "direction", 0); }
	else { changeentityproperty(self, "direction", 1); }
}

/* * Vira automaticamente para o alvo mais próximo
 */
void autoTurn() {
	void self = getlocalvar("self");
	void opponent = findtarget(self);

	float pX = getentityproperty(self, "x");

	if(opponent!= NULL()) {
		float eX = getentityproperty(opponent, "x");
		
		if(eX < pX) { changeentityproperty(self, "direction", 0); }
		else { changeentityproperty(self, "direction", 1); }
	}
}


/* * Para no ar.
 */
void airstop() {
    void self = getlocalvar("self");
    changeentityproperty(self, "velocity", 0, 0, 0); //Para o movimento!
	changeentityproperty(self,"antigravity",1);	
}


/* * Recupera a gravidade
 */
void airfollow() {
    void self = getlocalvar("self");
	changeentityproperty(self,"antigravity",0.1);	
}



/* * velAdd no estilo MUGEN
 * dirFree: se definido, aplica movimento para esquerda/direita
 * em vez de trás/frente
 */
void velAdd (float x, float y, float z, void dirFree) {
    void self = getlocalvar("self");
	int side = 1;
	int facing = getentityproperty(self, "direction");
	if(dirFree == NULL()) {
		side = (facing == 0) ? (-1) : (1);		
	}
	
	x = getentityproperty(self,"xdir")+(x*side);
	y = getentityproperty(self,"tossv")+y;
	z = getentityproperty(self,"zdir")+z;
	
	changeentityproperty(self, "velocity", x, z, y);
}

/* * velMul no estilo MUGEN
 */
void velMul (float x, float y, float z) {
    void self = getlocalvar("self");
	
	if(x != NULL()) { x *= getentityproperty(self,"xdir"); }
	if(y != NULL()) { y *= getentityproperty(self,"tossv"); }
	if(z != NULL()) { z *= getentityproperty(self,"zdir"); }
	
	changeentityproperty(self, "velocity", x, z, y);
}

/* * velSet no estilo MUGEN
 * dirFree: se definido, aplica movimento para esquerda/direita
 * em vez de trás/frente
 */
void velSet (float x, float y, float z, void dirFree) {
    void self = getlocalvar("self");	
	int side = 1;
	
	if(dirFree == NULL()) {
		side = (getentityproperty(self, "direction")== 0) ? (-1) : (1);		
	}
	
	x = x*side;
	
	changeentityproperty(self, "velocity", x, z, y);
}


/* * Lê o pressionamento de tecla
 * retorna 1 se a tecla selecionada foi pressionada, 0 caso contrário
 * em vez de trás/frente
 */
void readKey(void key) {
	void self = getlocalvar("self");
	int pindex = getentityproperty(self,"playerindex");
	int facing = getentityproperty(self, "direction");
	
	if(key=="left" || key=="l") { return playerkeys(pindex, 0, "moveleft"); }
	else if(key=="right" || key=="r") { return playerkeys(pindex, 0, "moveright"); }
	else if(key=="up" || key=="u") { return playerkeys(pindex, 0, "moveup"); }
	else if(key=="down" || key=="d") { return playerkeys(pindex, 0, "movedown"); }
	else if(key=="jump" || key=="j") { return playerkeys(pindex, 0, "jump"); }
	else if(key=="attack" || key=="atk" || key=="a") { return playerkeys(pindex, 0, "attack"); }
	else if(key=="attack2" || key=="atk2" || key=="a2") { return playerkeys(pindex, 0, "attack2"); }
	else if(key=="attack3" || key=="atk3" || key=="a3") { return playerkeys(pindex, 0, "attack3"); }
	else if(key=="attack4" || key=="atk4" || key=="a4") { return playerkeys(pindex, 0, "attack4"); }
	else if(key=="special" || key=="sp" || key=="s") { return playerkeys(pindex, 0, "special"); }
	else if(key=="backward" || key=="back" || key=="b") {
		if(facing) { return playerkeys(pindex, 0, "moveleft"); }
		else { return playerkeys(pindex, 0, "moveright"); }
	}
	else if(key=="forward" || key=="fwd" || key=="f") {
		if(facing) { return playerkeys(pindex, 0, "moveright"); }
		else { return playerkeys(pindex, 0, "moveleft"); }
	}
	else { return 0; }
}

/* * obtém a velocidade X
 */
float getXvel() {
	void self = getlocalvar("self");
	int side = (getentityproperty(self, "direction")== 0) ? (-1) : (1);

	return getentityproperty(self,"xdir")*side;
}

/* * limita a velocidade X entre o mínimo e o máximo
 */
void limitXvel(float min, float max) {
	void self = getlocalvar("self");
	float x = getentityproperty(self,"xdir");
	
	if(x < min) { x = min; }
	else if(x > max) { x = max; }

	changeentityproperty(self, "velocity", x, NULL(), NULL());
}




/* * armazena a velocidade X em uma variável local,
 * para ser recuperada por retrieveXvel()
 */
void storeXvel() {
	void self = getlocalvar("self");
	int side = (getentityproperty(self, "direction")== 0) ? (-1) : (1);

	setlocalvar("xVel" + self, getentityproperty(self,"xdir")*side);
}

/* * recupera a velocidade X em uma variável local,
 * armazenada por storeXvel()
 */
float retrieveXvel() {
	void self = getlocalvar("self");
	return getlocalvar("xVel" + self);
}

/* * changeState no estilo MUGEN
 * você deve usar os nomes de animação do openBOR, sem o ANI_
 * como argumento, por exemplo "FREESPECIAL2"
 */
void changeState(void anim) {
	void self = getlocalvar("self");
	changeentityproperty(self, "animation", openborconstant("ANI_"+anim));
}

/* * Calcula o dano baseado na vida máxima do oponente
 * útil para ataques baseados em % de vida
 */
int calcRelativeDamage(float percent) { 
	void self = getlocalvar("self");
	void target = target = getentityproperty(self, "opponent");
	int damage = percent;
	float percentRecalc = percent*0.01;

	if(target!=NULL()) {
		percentRecalc *= getentityproperty(target,"maxhealth");
		damage = percentRecalc;
	}
	
	return damage;
}

void adjustGrabDistance(float dx)
{ //ajusta a distância normal de agarrão
   void self = getlocalvar("self");
   void target = getentityproperty(self, "grabbing");
   
   if(target!=NULL())
   {
     bindentity(target, self, dx, 0, 0, 0, 0);
     bindentity(target, NULL());
   }
}

void adjustGrabAnim()
{ //ajusta a animação normal de agarrão
   void self = getlocalvar("self");
   void target = getentityproperty(self, "grabbing");
   
   if(target!=NULL())
   {
       damageentity(target, self, 0, 0, openborconstant("ATK_NORMAL2")); // Causa dano ao alvo com o dano desejado
       updateframe(target, 0);
	}
}

int isPlatform(int x, int z, int a) {
	void getPlatform = checkplatformbelow(x,z,a);
	if(getPlatform) {
		if(getentityproperty(getPlatform,"type") == openborconstant("TYPE_NONE") ) { return 1; }
		else { return 0; }
	}
	else { return 0; }
}

int isBlockedFront(int edge) {
	
	void self = getlocalvar("self");
	int facing = getentityproperty(self, "direction");
	int x = getentityproperty(self, "x");
	int z = getentityproperty(self, "z");
	float Hf = 0;
	int minX = openborvariant("xpos");
	int maxX = openborvariant("hResolution")+minX;


	// verifica paredes em X
	Hf = (facing==1) ? (checkwall(x+edge,z)) : (checkwall(x-edge,z));
	
	// verifica plataformas em X
	if(Hf <= 0) {Hf = (facing==1) ? (isPlatform(x+edge,z,1001)) : (isPlatform(x-edge,z,1001));}
	
	// verifica cantos em X
	if(Hf <= 0) { Hf = (facing==1) ? (maxX-edge < x) :  (minX+edge > x); }
	  
	return (Hf > 0);
	
}

int isBlockedBack(int edge) {
	
	void self = getlocalvar("self");
	int facing = getentityproperty(self, "direction");
	int x = getentityproperty(self, "x");
	int z = getentityproperty(self, "z");
	float Hb = 0;
	int minX = openborvariant("xpos");
	int maxX = openborvariant("hResolution")+minX;

	// verifica paredes em X
	Hb = (facing==1) ? (checkwall(x-edge,z)) : (checkwall(x+edge,z));
	
	// verifica plataformas em X
	if(Hb <= 0) {Hb = (facing==1) ? (isPlatform(x-edge,z,1001)) : (isPlatform(x+edge,z,1001));}
	
	// verifica cantos em X
	if(Hb <= 0) { Hb = (facing==1) ? (minX+edge > x) :  (maxX-edge < x); }
	
	return (Hb > 0);
}

int isBlockedUp(int edge) {
	
	void self = getlocalvar("self");
	int x = getentityproperty(self, "x");
	int z = getentityproperty(self, "z");
	float Hu = 0;
	int minZ = openborvariant("PLAYER_MIN_Z");


	// verifica paredes em Z
	Hu = checkwall(x,z+edge);
	
	// verifica plataformas em Z
	if(Hu <= 0) { Hu = isPlatform(x,z+edge,1001); }

	// verifica cantos em Z
	if(Hu <= 0) { Hu = (minZ+edge > z); }

	return (Hu > 0);
}

int isBlockedDown(int edge) {
	
	void self = getlocalvar("self");
	int x = getentityproperty(self, "x");
	int z = getentityproperty(self, "z");
	float Hd = 0;
	int maxZ = openborvariant("PLAYER_MAX_Z");


	// verifica paredes em Z
	Hd = checkwall(x,z-edge);
	
	// verifica plataformas em Z
	if(Hd <= 0) { Hd = isPlatform(x,z-edge,1001); }

	// verifica cantos em Z
	if(Hd <= 0) { Hd = (maxZ-edge < z); }

	return (Hd > 0);
}


void screenBound(int frontEdge, int backEdge, int upEdge, int downEdge, int divisor) {
	
	void self = getlocalvar("self");
	int facing = getentityproperty(self, "direction");
	int x = getentityproperty(self, "x");
	int z = getentityproperty(self, "z");
	float Hf = 0;
	float Hb = 0;
	float Hu = 0;
	float Hd = 0;
	float newX = 0;

	if(divisor == NULL()|| divisor == 0) {divisor = 1;}

	// verifica obstáculos superiores e inferiores
	Hu = isBlockedUp(upEdge);
	Hd = isBlockedDown(downEdge);

	// aplica correção em Z
	if(Hu > 0) { changeentityproperty(self, "position", x, z+(upEdge/divisor)); }
	if(Hd > 0) { changeentityproperty(self, "position", x, z-(downEdge/divisor)); }

	// verifica obstáculos frontais e traseiros
	Hf = isBlockedFront(frontEdge);
	Hb = isBlockedBack(backEdge);
	  
	// aplica correção em X (frente)
	if(Hf > 0) {
		newX = (facing==1) ? (x-(frontEdge/divisor)) :  (x+(frontEdge/divisor)); 
		changeentityproperty(self, "position", newX); 
	}
	
	// aplica correção em X (trás)
	if(Hb > 0) {
		newX = (facing==1) ? (x+(backEdge/divisor)) :  (x-(backEdge/divisor)); 
		changeentityproperty(self, "position", newX);
	}
}