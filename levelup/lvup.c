void lvup(int plIndex)
{
	void score=getplayerproperty(plIndex,"score");
	void ent=getplayerproperty(plIndex,"entity");
	int oldLevel=getglobalvar("level."+plIndex);
	int lv=getLevel(score);
	if (oldLevel==NULL()){oldLevel=1;}
	if (lv>oldLevel){
		setAbi2(plIndex,ent, lv-oldLevel);
		setglobalvar("level."+plIndex,lv);
		spawnLvup(ent);
	}
	setglobalvar("score"+plIndex,score);
}
void spawnLvup(void ent)
{
	clearspawnentry();
	setspawnentry("name", "levelup");
	void vSpawn=spawn();
	float fX =getentityproperty(ent, "x");
	float fY = getentityproperty(ent, "a");
	float fZ = getentityproperty(ent, "z");
	changeentityproperty(vSpawn, "position", fX, fZ, fY + 1);
}
void setLevel()
{
	void pindex	=	getentityproperty(getlocalvar("self"),"playerindex");
	void score	=	getplayerproperty(pindex,"score");
	int lv		=	getLevel(score);
	setAbi(pindex,getlocalvar("self"));
}

void setAbi2(void plIndex, void ent, int delta)
{
	changeentityproperty(ent,"maxhealth",getentityproperty(ent,"maxhealth")+5*delta);
	changeentityproperty(ent,"maxmp",getentityproperty(ent,"maxmp")+5*delta);
	changeentityproperty(ent,"health",getentityproperty(ent,"maxhealth"));
	changeentityproperty(ent,"mp",getentityproperty(ent,"maxmp"));
//	changeplayerproperty(plIndex, "spawnhealth", getentityproperty(ent,"maxhealth"));
//	changeplayerproperty(plIndex, "spawnmp", getentityproperty(ent,"maxmp"));
}
void setAbi(void plIndex,void ent)
{
	void score=getplayerproperty(plIndex,"score");
	void lv=getLevel(score);
	setAbi2(plIndex, ent, lv-1);
	setglobalvar("level."+plIndex,lv);
}
int getLevel(void score)
{
	int lv;
	if(score>9999999){
		lv=36;
	}else if(score>9999998){
		lv=35;
	}else if(score>9999997){
		lv=34;
	}else if(score>9999996){
		lv=33;
	}else if(score>9999995){
		lv=32;
	}else if(score>9999994){
		lv=31;
	}else if(score>9999993){
		lv=30;
	}else if(score>9999992){
		lv=29;
	}else if(score>9999991){
		lv=28;
	}else if(score>9999990){
		lv=27;
	}else if(score>9999989){
		lv=26;
	}else if(score>9999988){
		lv=25;
	}else if(score>9999987){
		lv=24;
	}else if(score>9999986){
		lv=23;
	}else if(score>9999985){
		lv=22;
	}else if(score>9999984){
		lv=21;
	}else if(score>9999983){
		lv=20;
	}else if(score>9999982){
		lv=19;
	}else if(score>9999981){
		lv=18;
	}else if(score>9999980){
		lv=17;
	}else if(score>9999979){
		lv=16;
	}else if(score>9999978){
		lv=15;
	}else if(score>9999977){
		lv=14;
	}else if(score>9999976){
		lv=13;
	}else if(score>9999975){
		lv=12;
	}else if(score>9999974){
		lv=11;
	}else if(score>9999973){
		lv=10;
	}else if(score>9999972){
		lv=9;
	}else if(score>9999971){
		lv=8;
	}else if(score>9999970){
		lv=7;
	}else if(score>9999969){
		lv=6;
	}else if(score>9999968){
		lv=5;
	}else if(score>9999967){
		lv=4;
	}else if(score>9999966){
		lv=3;
	}else if(score>9999965){
		lv=2;
	}else{
		lv=1;
	}
	return lv;
}
void getPlLevel(int plIndex)
{
	void ent=getplayerproperty(plIndex,"entity");
	if (!ent){return;}else{}
	void score=getplayerproperty(plIndex,"score");
	setglobalvar("level."+plIndex,getLevel(score));
}
void changeLv(void pindex)
{
	lvup(pindex);
	void lv	=	getglobalvar("level."+pindex);
	if(lv==NULL())lv=1;
}

