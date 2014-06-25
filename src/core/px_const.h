/*
 *  This file is a part of the PARUS project.
 *  Copyright (C) 2006  Alexey N. Salnikov (salnikov@cmc.msu.ru)
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
 */

#ifndef __PX_CONST_H_
#define __PX_CONST_H_

#define PX_ROOT_NODE -10
#define PX_TAIL_NODE -20

#define PX_MPI_DATA_TAG                        100

#define PX_REGIME_TAG                          1
#define PX_NODE_QUESTION_TAG                   2
#define PX_EDGES_PLACE_TAG                     3
#define PX_NODE_CLEAN_TAG                      5
#define PX_EDGE_QUESTION_TAG                   6
#define PX_RECV_INFO_TAG                       8
#define PX_BLANK_TAG                           9
#define PX_RECV_EDGE_FINISHED_TAG              11
#define PX_EDGES_SEND_TAG                      12
#define PX_SEND_INFO_TAG                       14
#define PX_EDGES_NAME_TAG                      15
#define PX_SEND_EDGES_NAMES_TAG                16
#define PX_SEND_EDGES_DATA_SIZE_TAG            17
#define PX_SEND_EDGES_SIZE_TAG                 18
#define PX_EDGES_SIZE_TAG                      19

#define PX_WORK            1
#define PX_STOP            2
#define PX_EDGE_REGIME     3
#define PX_PROCESSOR_CLEAN 4

#define PX_NUM_EXAMPLES_SENDS     100
#define PX_NUM_EXAMPLES_OPERATION 100


#define PX_DEMON_REGIME_COMBINED 0
#define PX_DEMON_REGIME_STATIC   1
#define PX_DEMON_REGIME_DYNAMIC  2

#define PX_PROC_FREE_STATUS         0
#define PX_NODE_RUN_STATUS          1
#define PX_NODE_WAIT_RECV_STATUS    2
#define PX_NODE_READY_DATA_STATUS   3
#define PX_NODE_INITIAL_STATUS      4
#define PX_NODE_RECV_DATA_STATUS    5


extern int px_val_scheduling_regime(char *regime_name);
extern int px_tag_to_string(int tag,char *string);
extern int px_proc_status_to_string(int status,char *string);
extern int px_proc_regime_to_string(int regime,char *string);
#endif /*__PX_CONST_H_ */
