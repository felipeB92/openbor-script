#include "data/scripts/levelup/lvup.c"
void changeLv(void pindex)
{
	lvup(pindex);
	void lv	=	getglobalvar("level."+pindex);
	if(lv==NULL())lv=1;
	settextobj(pindex,999+999*pindex,999,999,999,"Level"+lv);
}
