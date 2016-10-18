//////////////////////////////////////////////////////////
// $Author: pkaplan $
//
// $Revision: 1.4 $
//
//////////////////
//
// A register based packet injection driver
// 
//////////////////////////////////////////////////////////
#ifndef RED_HW_FIN_DRIVER
#define RED_HW_FIN_DRIVER

int red_hw_frame_input_dhs_handler    
                                ( int inst, const unsigned int THE_DHS_ADDR
                                , const unsigned char* PktPtr
                                , const unsigned int PktSize
                                , const unsigned int BPF //bytes per flit
                                , const int is_big_endian
                                , const int inject_error
                                ) ;

int red_hw_frame_output_dhs_handler (int inst, const unsigned int THE_DHS_ADDR, unsigned char* buffer, const unsigned int BPF, const unsigned int is_big_endian) ;

int red_hw_ss_output_packet_available(int inst, const unsigned int which_ss, const unsigned int which_port, int* packet_available) ;
#endif
