/*
 *  This file is a part of the PARUS project.
 *  Copyright (C) 2008  Alexey N. Salnikov
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Alexey N. Salnikov (salnikov@cmc.msu.ru)
 *                     
 */

#include "px_const.h"

#include <string.h>
#include <stdio.h>

/****************************************************************************/
int px_val_scheduling_regime(char *regime_name)
{
 	if(regime_name==NULL) 
		return -1;
	if(!strcmp(regime_name,"combined")) 
		return PX_DEMON_REGIME_COMBINED;
	if(!strcmp(regime_name,"static"))
		return PX_DEMON_REGIME_STATIC;
	if(!strcmp(regime_name,"dynamic"))
		return PX_DEMON_REGIME_DYNAMIC;
	return -1;
}
/****************************************************************************/
int px_tag_to_string(int tag,char *string)
{
	switch(tag)
	{
	case PX_REGIME_TAG:
		strcpy(string,"REGIME");
	break;
	case PX_NODE_QUESTION_TAG:
		strcpy(string,"NODE_QUESTION");
	break;
	case PX_EDGES_PLACE_TAG:
		strcpy(string,"EDGES_PLACE");
	break;
	case PX_NODE_CLEAN_TAG:
		strcpy(string,"NODE_CLEAN");
	break;
	case PX_EDGE_QUESTION_TAG:
		strcpy(string,"EDGE_QUESTION");
	break;
	case PX_RECV_INFO_TAG:
		strcpy(string,"RECV_INFO");
	break;
	case PX_BLANK_TAG:
		strcpy(string,"BLANK");
	break;
	case PX_RECV_EDGE_FINISHED_TAG:
		strcpy(string,"RECV_EDGE_FINISHED");
	break;
	case PX_EDGES_SEND_TAG:
		strcpy(string,"EDGES_SEND");
	break;
	case PX_SEND_INFO_TAG:
		strcpy(string,"SEND_INFO");
	break;
	case PX_EDGES_NAME_TAG:
		strcpy(string,"EDGES_NAME");
	break;
	case PX_SEND_EDGES_NAMES_TAG:
		strcpy(string,"SEND_EDGES_NAMES");
	break;
	case PX_SEND_EDGES_DATA_SIZE_TAG:
		strcpy(string,"SEND_EDGES_DATA_SIZE");
	break;
	case PX_SEND_EDGES_SIZE_TAG:
		strcpy(string,"SEND_EDGES_SIZE");
	break;
	case PX_EDGES_SIZE_TAG:
		strcpy(string,"EDGES_SIZE");
	break;
	/*
	 *
	 * UNKNOWN TAG!!!
	 *
	 */
	default:
		sprintf(string,"unknown(tag=%d)",tag);
		return -1;
	break;
	}
	return 0;
}
/*****************************************************************************/
int px_proc_status_to_string(int status,char *string)
{
	switch(status)
	{
	case PX_PROC_FREE_STATUS:
		strcpy(string,"free");
	break;
	case PX_NODE_RUN_STATUS:
		strcpy(string,"node_run");
	break;
	case PX_NODE_WAIT_RECV_STATUS:
		strcpy(string,"waiting_recv");
	break;
	case PX_NODE_READY_DATA_STATUS:
		strcpy(string,"data_ready");
	break;
	case PX_NODE_INITIAL_STATUS:
		strcpy(string,"initial");
	break;
	case PX_NODE_RECV_DATA_STATUS:
		strcpy(string,"data_recieving");
	break;
	default:
		sprintf(string,"unknown(status=%d)",status);
		return -1;
	break;
	}
	return 0;
}
/****************************************************************/
int px_proc_regime_to_string(int regime,char *string)
{
	switch(regime)
	{
	case PX_WORK:
		strcpy(string,"work");
	break;
	case PX_STOP:
		strcpy(string,"stop");
	break;
	case PX_EDGE_REGIME:
		strcpy(string,"edges");
	break;
	case PX_PROCESSOR_CLEAN:
		strcpy(string,"proc_clean");
	break;
	default:
		sprintf(string,"unknown(regime=%d)",regime);
		return -1;
	break;
	}
	return 0;
}
/****************************************************************/

