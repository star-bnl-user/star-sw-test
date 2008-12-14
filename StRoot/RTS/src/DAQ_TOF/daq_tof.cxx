#include <assert.h>
#include <sys/types.h>
#include <errno.h>

#include <rtsLog.h>
#include <rtsSystems.h>

#include <SFS/sfs_index.h>

#include <DAQ_READER/daqReader.h>
#include <DAQ_READER/daq_dta.h>

#include "daq_tof.h"

extern int tof_reader(char *mem, struct tof_t *tof, u_int driver) ;




const char *daq_tof::help_string = "\
\n\
TOF Help: \n\
Supported Banks: \n\
	raw	returns=ptr of start of DDL data; c1=sector[1..1]; c2=rdo[1..4]; \n\
	legacy	returns=ptr to struct tof_t; \n\
\n\
\n\
" ;

class daq_det_tof_factory : public daq_det_factory
{
public:
        daq_det_tof_factory() {
                daq_det_factory::det_factories[TOF_ID] = this ;
        }

        daq_det *create() {
                return new daq_tof ;
        }
} ;

static daq_det_tof_factory tof_factory ;



daq_tof::daq_tof(daqReader *rts_caller) 
{
	rts_id = TOF_ID ;
	name = rts2name(rts_id) ;
	sfs_name = "tof" ;
	caller = rts_caller ;
	if(caller) caller->insert(this, rts_id) ;

	raw = new daq_dta ;
	legacy = new daq_dta ;


	
	LOG(DBG,"%s: constructor: caller %p",name,rts_caller) ;
	return ;
}

daq_tof::~daq_tof() 
{
	LOG(DBG,"%s: DEstructor",name) ;

	delete raw ;
	delete legacy ;

	return ;
}



daq_dta *daq_tof::get(const char *bank, int sec, int row, int pad, void *p1, void *p2) 
{
	Make() ;

	if(present==0) return 0 ;

	LOG(DBG,"%s: looking for bank %s",name,bank) ;

	if(strcmp(bank,"*")==0) bank = "legacy" ;
		


	if(strcasecmp(bank,"raw")==0) {
		if((present & DET_PRESENT_SFS)==0) return 0 ;		// no DDL
		return handle_raw(sec,row) ;		// actually sec, rdo; r1 is the number of bytes
	}
	else if(strcasecmp(bank,"legacy")==0) {
//		if((present & DET_PRESENT_DATAP)==0) return 0 ;	// no legacy
		return handle_legacy() ;
	}
	else {
		LOG(ERR,"%s: unknown bank type \"%s\"",name,bank) ;
	}

	return 0 ;
}


daq_dta *daq_tof::handle_legacy()
{
	assert(caller) ;

	legacy->create(1,"tof_t",rts_id,DAQ_DTA_STRUCT(tof_t)) ;
	

	tof_t *tof_p = (tof_t *) legacy->request(1) ;	// need ONE tof_t object

	
	memset(tof_p->ddl_words,0,sizeof(tof_p->ddl_words)) ;	// zap it!
	
	if(present & DET_PRESENT_DATAP) {	// datap 	
		tof_reader(caller->mem, tof_p, m_Debug) ;
	}
	else {
		tof_p->mode = 1 ;	// old...
		tof_p->channels = 0 ;	// signal that we have NO old data...
		tof_p->max_channels = 48+48+32+12 ;	// stale but compatible with old tofReadaer...

		
		for(int r=1;r<=4;r++) {
			daq_dta *dd = handle_raw(0,r) ;
			if(dd && dd->iterate()) {
				u_int *tmp = (u_int *)dd->Void ;

				u_int words = dd->ncontent/4 ;	// tof wants words...

				tof_p->ddl_words[r-1] = words ;

				
				for(u_int i=0;i<words;i++) {	//words!
					tof_p->ddl[r-1][i] = l2h32(*tmp) ;
					tmp++ ;
				}

			}

		}

	}
	legacy->finalize(1,1,0,0) ;	// 1 entry; sector 1, row 0, pad 0
	legacy->rewind() ;

	return legacy ;
}


daq_dta *daq_tof::handle_raw(int sec, int rdo)
{
	char str[128] ;
	int tot_bytes ;
	int min_rdo, max_rdo ;
	int min_sec, max_sec ;
	struct {
		int sec ;
		int rb ;
		u_int bytes ;
	} obj[MAX_SEC*MAX_RDO] ;

	// sanity
	sec = 1 ;	// nothing else for now...
	min_sec = max_sec = sec ;

	if(rdo==-1) {
		min_rdo = 1 ;
		max_rdo = MAX_RDO ;
	}
	else if((rdo<1) || (rdo>MAX_RDO)) return 0 ;
	else {
		min_rdo = max_rdo = rdo ;
	}

	// bring in the bacon from the SFS file....
	assert(caller) ;

	// calc total bytes
	tot_bytes = 0 ;
	int o_cou = 0 ;
	for(int s=min_sec;s<=max_sec;s++) {
	for(int r=min_rdo;r<=max_rdo;r++) {
	
		sprintf(str,"%s/sec%02d/rb%02d/raw",sfs_name, s, r) ;
		char *full_name = caller->get_sfs_name(str) ;
	
		LOG(DBG,"%s: trying sfs on \"%s\"",name,str) ;
		if(full_name == 0) continue ;

		int size = caller->sfs->fileSize(full_name) ;	// this is bytes

		LOG(DBG,"Got %d",size) ;

		if(size <= 0) {
			LOG(DBG,"%s: %s: not found in this event",name,str) ;
			continue ;
		}
		else {
			obj[o_cou].sec = s ;
			obj[o_cou].rb = r ;
			obj[o_cou].bytes = size ;

			o_cou++ ;
	
			tot_bytes += size ;
			LOG(DBG,"%s: %s: reading in \"%s\": bytes %d",name,str,"raw", size) ;
		}
	}
	}

	raw->create(tot_bytes,"tof_raw",rts_id,DAQ_DTA_STRUCT(u_char)) ;

	for(int i=0;i<o_cou;i++) {

		sprintf(str,"%s/sec%02d/rb%02d/raw",sfs_name, obj[i].sec, obj[i].rb) ;
		char *full_name = caller->get_sfs_name(str) ;
		if(full_name == 0) continue ;

		char *st = (char *) raw->request(obj[i].bytes) ;

		int ret = caller->sfs->read(full_name, st, obj[i].bytes) ;

                if(ret != (int)obj[i].bytes) {
                        LOG(ERR,"%s: %s: read failed, expect %d, got %d [%s]",name,str,
                                obj[i].bytes,ret,strerror(errno)) ;
                }

                else {
                        LOG(NOTE,"%s: %s read %d bytes",name,str,ret) ;

                }

                raw->finalize(obj[i].bytes,obj[i].sec,obj[i].rb,0) ;

	}

	raw->rewind() ;
	return raw ;
}


// knows how to get the token out of an event...
int daq_tof::get_token(char *addr, int words)
{
	int cou ;
	struct daq_trg_word trg[128] ;

	cou = get_l2(addr,words,trg,1) ;

	if(cou==0) return -1000 ;	// special marker...
	if(trg[0].t==0) return -ENOSYS ;

	return trg[0].t ;
}

// knows how to get a/the L2 command out of the event...
int daq_tof::get_l2(char *addr, int words, struct daq_trg_word *trg, int do_log)
{
	u_int *w ;
	int cou = 0 ;
	int t_cou = 0 ;


	w = (u_int *)addr ;
	words-- ;	// point to last datum now...

	// this will be DBG...
	LOG(NOTE,"First words 0x%08X 0x%08X 0x%08X, last words 0x%08X 0x%08X 0x%08X [+0x%08X], %u",
	    w[0],w[1],w[2],w[words-2],w[words-1],w[words],w[words+1],words+1) ;


	// prompt token is in word 0!
	// unless it just reads 0xA0000000, in which case this is purely trigger data -- no
	//    content!

	trg[t_cou].t = w[0] & 0xFFF ;
	trg[t_cou].daq = (w[0]>>12) & 0xF ;
	trg[t_cou].trg = (w[0]>>16) & 0xF ;
	trg[t_cou].rhic = 0 ;
	trg[t_cou].rhic_delta = 0 ;
	t_cou++ ;


	if(do_log) LOG(NOTE,"prompt: T %4d, trg %d, daq %d [0x%08X]: words %d",trg[0].t,trg[0].trg,trg[0].daq,w[0],words+1) ;

	if(w[0] == 0xA0000000) {
		trg[0].t = 4097;	// trigger only contrib...
	}
	else if(trg[0].t == 0) {
		LOG(ERR,"prompt: T %4d, trg %d, daq %d",trg[0].t,trg[0].trg,trg[0].daq) ;
		trg[0].t = 4097 ;
	}

	if((w[0] >> 28) != 0xA) {
		LOG(ERR,"First word in data has an incorrect signature 0x%08X",w[0]) ;
		trg[0].t = 4097 ;
	}

	// move backwards to the start of the trigger block
	while(words) {
		if((w[words] >> 28)==0xA) {	// trigger stuff
			// fish trigger command
			int daq = (w[words]>>12) & 0xF ; 
			int cmd = (w[words]>>16) & 0xF ;
			int t = (w[words]) & 0xFFF ;
			
			if(do_log) LOG(NOTE,"   FIFO %d: T %4d, trg %d, daq %d: word %d",cou,t,cmd,daq,words);

			words-- ;
			cou++ ;
		}
		else {
			break ;
		}
	}



	words++ ;	// move forward to start of trigger

	// words not points to the first trigger of the FIFO block

	for(int i=0;i<cou;i++) {
		
		trg[t_cou].t = w[words+i] & 0xFFF ;
		trg[t_cou].daq = (w[words+i]>>12) & 0xF ;
		trg[t_cou].trg = (w[words+i]>>16) & 0xF ;
		trg[t_cou].rhic = i+1 ;
		trg[t_cou].rhic_delta = i+1 ;

		if(trg[t_cou].t == 0) {
			LOG(ERR,"  FIFO %d: T %4d, trg %d, daq %d",i,trg[t_cou].t,trg[t_cou].trg,trg[t_cou].daq);
			continue ;
		}

		// we will take all the non-L2 components here...
		switch(trg[t_cou].trg) {
		case 13 :
		case 15 :
			break ;
		default :		// take out ALL L0 commands!
			continue ;
		}

		t_cou++ ;
		if(t_cou >=128) {
			LOG(ERR,"Too many trigger contributions!") ;
			break ;
		}
	}


	return t_cou ;
}
