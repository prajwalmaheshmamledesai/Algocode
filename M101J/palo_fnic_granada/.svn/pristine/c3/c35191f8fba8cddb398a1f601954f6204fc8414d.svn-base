/********************************************************************
 *
 *      File:   mv88e6095_reg_desc.h 
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       Marvell switch register description 
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/

#define MV88E6095_PORT_REG_DESC		\
   REG_DEF("swport", 0x0000, "PORTSTAT",	"Port Status Register", 	0xffff, 0x0, "sta")	\
   REG_DEF_FLD(0, "PauseEn",   15, 15, "", "")\
   REG_DEF_FLD(0, "MyPause",   14, 14, "", "")\
   REG_DEF_FLD(0, "HDXFlow",   13, 13, "", "")\
   REG_DEF_FLD(0, "PhyDetect", 12, 12, "", "")\
   REG_DEF_FLD(0, "Link",      11, 11, "", "")\
   REG_DEF_FLD(0, "Duplex",    10, 10, "", "")\
   REG_DEF_FLD(0, "Speed",     9, 8, "", "")\
   REG_DEF_FLD(0, "MgmiiSerdes",   6, 6, "", "")\
   REG_DEF_FLD(0, "TxPaused",      5, 5, "", "")\
   REG_DEF_FLD(0, "FlowCtrl",      4, 4, "", "")\
   REG_DEF_FLD(0, "Config_Duplex", 3, 3, "", "")\
   REG_DEF_FLD(0, "ConfigMode",    2, 0, "", "")\
   \
   REG_DEF("swport", 0x0001, "PCS_CONTROL",	"PCS Control Register", 	0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(1, "PcsLinkStatus", 15, 15, "", "")\
   REG_DEF_FLD(1, "SyncOK",        14, 14, "", "")\
   REG_DEF_FLD(1, "SyncFail",      13, 13, "", "")\
   REG_DEF_FLD(1, "AnBypassed",    12, 12, "", "")\
   REG_DEF_FLD(1, "AnBypass",      11, 11, "", "")\
   REG_DEF_FLD(1, "PCSAnEn",       10, 10, "", "")\
   REG_DEF_FLD(1, "RestartPCSAn",  9,  9,  "", "")\
   REG_DEF_FLD(1, "PCSAnDone",     8,  8,  "", "")\
   REG_DEF_FLD(1, "FCValue",       7,  7,  "", "")\
   REG_DEF_FLD(1, "ForcedFC",      6,  6,  "", "")\
   REG_DEF_FLD(1, "LinkValue",     5,  5,  "", "")\
   REG_DEF_FLD(1, "ForcedLink",    4,  4,  "", "")\
   REG_DEF_FLD(1, "DpxValue",      3,  3,  "", "")\
   REG_DEF_FLD(1, "ForcedDpx",     2,  2,  "", "")\
   REG_DEF_FLD(1, "ForceSpd",      1,  0,  "", "")\
   \
   REG_DEF("swport", 0x0003, "SWITCHID",	"Product Identifier",   	0xffff, 0x0, "sta")	\
   REG_DEF_FLD(3, "ProductId",     15, 4,  "", "")\
   REG_DEF_FLD(3, "RevisionId",    3,  0,  "", "")\
   \
   REG_DEF("swport", 0x0004, "PORTCTRL",	"Port Control", 		0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(4, "UseCoreTag",    15, 15, "", "")\
   REG_DEF_FLD(4, "DropOnLock",    14, 14, "", "")\
   REG_DEF_FLD(4, "EgressMode",    12, 12, "", "")\
   REG_DEF_FLD(4, "Header",        11, 11, "", "")\
   REG_DEF_FLD(4, "IGMP-MLD_snoop",10, 10, "", "")\
   REG_DEF_FLD(4, "DoubleTag",      9,  9, "", "")\
   REG_DEF_FLD(4, "DSATag",         8,  8,  "", "")\
   REG_DEF_FLD(4, "VlanTunnel",     7,  7,  "", "")\
   REG_DEF_FLD(4, "TagIfBoth",      6,  6,  "", "")\
   REG_DEF_FLD(4, "UseIP",          5,  5,  "", "")\
   REG_DEF_FLD(4, "UseTag",         4,  4,  "", "")\
   REG_DEF_FLD(4, "ForwardUnknown", 2,  2,  "", "")\
   REG_DEF_FLD(4, "PortState",      1,  0,  "", "")\
   \
   REG_DEF("swport", 0x0005, "PORTCTRL1",	"Port Control 1", 		0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(5, "MessagePort",  15, 15, "", "")\
   REG_DEF_FLD(5, "TrunkPort",    14, 14, "", "")\
   REG_DEF_FLD(5, "TrunkID",       7, 4, "", "")\
   REG_DEF_FLD(5, "DBNum",         2, 0, "", "")\
   \
   REG_DEF("swport", 0x0006, "PORTVLANMAP",	"Port Based VLAN Map", 		0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(6, "DBNum",        15, 12, "", "")\
   REG_DEF_FLD(6, "LearnDisable", 11, 11, "", "")\
   REG_DEF_FLD(6, "VLANTable",    10,  0, "", "")\
   \
   REG_DEF("swport", 0x0007, "PORTVLANID",	"Default Port VLAN ID & Pri", 	0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(7, "DefPri",          15, 13, "", "")\
   REG_DEF_FLD(7, "ForceDefaultVID", 12, 12, "", "")\
   REG_DEF_FLD(7, "DefaultVID",      11,  0, "", "")\
   \
   REG_DEF("swport", 0x0008, "PORTCTRL2",	"Port Control 2", 		0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(8, "IgnoreFCS",       15, 15, "", "")\
   REG_DEF_FLD(8, "VTUPriOverride",  14, 14, "", "")\
   REG_DEF_FLD(8, "SAPriOverride",   13, 13, "", "")\
   REG_DEF_FLD(8, "DAPriOverride",   12, 12, "", "")\
   REG_DEF_FLD(8, "802_1qMode",      10, 10, "", "")\
   REG_DEF_FLD(8, "DiscardTagged",    9,  9, "", "")\
   REG_DEF_FLD(8, "DiscardUntagged",  8,  8,  "", "")\
   REG_DEF_FLD(8, "MapDA",            7,  7,  "", "")\
   REG_DEF_FLD(8, "DefaultForward",   6,  6,  "", "")\
   REG_DEF_FLD(8, "EgressMonitorSource",  5,  5,  "", "")\
   REG_DEF_FLD(8, "IngressMonitorSource", 4,  4,  "", "")\
   REG_DEF_FLD(8, "CPUPort", 3,  0,  "", "")\
   \
   REG_DEF("swport", 0x0009, "RATECTRL",	"Rate Control", 		0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(9, "LimitMGMT",  15, 15, "", "")\
   REG_DEF_FLD(9, "Pri3Rate",   14, 14, "", "")\
   REG_DEF_FLD(9, "Pri2Rate",   13, 13, "", "")\
   REG_DEF_FLD(9, "Pri1Rate",   12, 12, "", "")\
   REG_DEF_FLD(9, "Pri0Rate",   11,  0, "", "")\
   \
   REG_DEF("swport", 0x000A, "RATECTRL2",	"Rate Control 2", 		0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(0xA, "LimitMode",   15, 14, "", "")\
   REG_DEF_FLD(0xA, "CountIFG",   13, 13, "", "")\
   REG_DEF_FLD(0xA, "CountPre",   12, 12, "", "")\
   REG_DEF_FLD(0xA, "EgressRate",   11,  0, "", "")\
   \
   REG_DEF("swport", 0x000B, "PORTASSVECT",	"Port Association Vector", 	0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(0xB, "LockedPort",   13, 13, "", "")\
   REG_DEF_FLD(0xB, "IgnoreWrongData",   12, 12, "", "")\
   REG_DEF_FLD(0xB, "PortAssociationVector", 10,  0, "", "")\
   \
   REG_DEF("swport", 0x0010, "INDISCCNT_LO",	"InDiscardsLo Frame Counter", 	0xffff, 0x0, "cnt")	\
   \
   REG_DEF("swport", 0x0011, "INDISCCNT_HI",	"InDiscardsHi Frame Counter", 	0xffff, 0x0, "cnt")	\
   \
   REG_DEF("swport", 0x0012, "INFILTCNT",	"InFiltered Frame Counter", 	0xffff, 0x0, "cnt")	\
   \
   REG_DEF("swport", 0x0013, "OUTFILTCNT",	"OutFiltered Frame Counter", 	0xffff, 0x0, "cnt")	\
   \
   REG_DEF("swport", 0x0017, "TAGREMAPLO",	"Tag Remap 3:0", 		0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(0x17, "TagRemap3", 13,  12, "", "")\
   REG_DEF_FLD(0x17, "TagRemap2",  9,   8, "", "")\
   REG_DEF_FLD(0x17, "TagRemap1",  5,   4, "", "")\
   REG_DEF_FLD(0x17, "TagRemap0",  1,   0, "", "")\
   \
   REG_DEF("swport", 0x0018, "TAGREMAPHI",	"Tag Remap 7:4",		0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(0x18, "TagRemap7", 13,  12, "", "")\
   REG_DEF_FLD(0x18, "TagRemap6",  9,   8, "", "")\
   REG_DEF_FLD(0x18, "TagRemap5",  5,   4, "", "")\
   REG_DEF_FLD(0x18, "TagRemap4",  1,   0, "", "")\

#define MV88E6095_GLOBAL1_REG_DESC		\
   REG_DEF("switch", 0x0000, "G1_GSR",		"Global Status",		0xffff, 0x0, "sta")	\
   REG_DEF_FLD(0, "PhyPollingUnitState",   15, 14, "", "")\
   REG_DEF_FLD(0, "SwMode",                13, 12, "", "")\
   REG_DEF_FLD(0, "InitReady",             11, 11, "", "")\
   REG_DEF_FLD(0, "StatsDone",             6, 6, "", "")\
   REG_DEF_FLD(0, "VTUProb",               5, 5, "", "")\
   REG_DEF_FLD(0, "VTUDone",               4, 4, "", "")\
   REG_DEF_FLD(0, "ATUProb",               3, 3, "", "")\
   REG_DEF_FLD(0, "ATUDone",               2, 2, "", "")\
   REG_DEF_FLD(0, "PHYInt",                1, 1, "", "")\
   REG_DEF_FLD(0, "EEInt",                 0, 0, "", "")\
   \
   REG_DEF("switch", 0x0001, "G1_MAC01",	"Mac Addr 0, 1",		0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(1, "MacByte0",              15, 9, "", "")\
   REG_DEF_FLD(1, "DiffAddr",               8, 8, "", "")\
   REG_DEF_FLD(1, "MacByte1",               7, 0, "", "")\
   \
   REG_DEF("switch", 0x0002, "G1_MAC23",	"Mac Addr 2, 3",		0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(2, "MacByte2",              15, 8, "", "")\
   REG_DEF_FLD(2, "MacByte3",               7, 0, "", "")\
   \
   REG_DEF("switch", 0x0003, "G1_MAC45",	"Mac Addr 4, 5",		0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(3, "MacByte4",              15, 8, "", "")\
   REG_DEF_FLD(3, "MacByte5",               7, 0, "", "")\
   \
   REG_DEF("switch", 0x0004, "G1_GCR",		"Global Control",		0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(4, "SwReset",              15, 15, "", "")\
   REG_DEF_FLD(4, "PPU_EN",               14, 14, "", "")\
   REG_DEF_FLD(4, "Discard_Excessive",    13, 13, "", "")\
   REG_DEF_FLD(4, "Scheduling",           11, 11, "", "")\
   REG_DEF_FLD(4, "MaxFrameSize",         10, 10, "", "")\
   REG_DEF_FLD(4, "Reload",                9,  9, "", "")\
   REG_DEF_FLD(4, "StatsDoneIntEn",        6,  6, "", "")\
   REG_DEF_FLD(4, "VTUProbIntEn",          5,  5, "", "")\
   REG_DEF_FLD(4, "VTUDoneIntEn",          4,  4, "", "")\
   REG_DEF_FLD(4, "ATUProbIntEn",          3,  3, "", "")\
   REG_DEF_FLD(4, "ATUDoneIntEn",          2,  2, "", "")\
   REG_DEF_FLD(4, "PHYIntEn",              1,  1, "", "")\
   REG_DEF_FLD(4, "EEIntEn",               0,  0, "", "")\
   \
   REG_DEF("switch", 0x0005, "G1_VTU_OPE",	"Vlan Table Op",		0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(5, "VTUBusy",              15, 15, "", "")\
   REG_DEF_FLD(5, "VTUOp",                14, 12, "", "")\
   REG_DEF_FLD(5, "DBNum[7:4]",           11,  8, "", "")\
   REG_DEF_FLD(5, "MemberViolation",       6,  6, "", "")\
   REG_DEF_FLD(5, "MissViolation",         5,  5, "", "")\
   REG_DEF_FLD(5, "DBNum-SPID",            3,  0, "", "")\
   \
   REG_DEF("switch", 0x0006, "G1_VTU_VID",	"Vlan Table VID",		0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(6, "VLAN Valid",           12, 12, "", "")\
   REG_DEF_FLD(6, "VLAN id",              11,  0, "", "")\
   \
   REG_DEF("switch", 0x0007, "G1_VTU_DATAL",	"Data Reg Ports 0-3",		0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(7, "PortStateP3",           15, 14, "", "")\
   REG_DEF_FLD(7, "MemberTagP3",           13, 12, "", "")\
   REG_DEF_FLD(7, "PortStateP2",           11, 10, "", "")\
   REG_DEF_FLD(7, "MemberTagP2",            9,  8, "", "")\
   REG_DEF_FLD(7, "PortStateP1",            7,  6, "", "")\
   REG_DEF_FLD(7, "MemberTagP1",            5,  4, "", "")\
   REG_DEF_FLD(7, "PortStateP0",            3,  2, "", "")\
   REG_DEF_FLD(7, "MemberTagP0",            1,  0, "", "")\
   \
   REG_DEF("switch", 0x0008, "G1_VTU_DATAM",	"Data Reg Ports 4-7",		0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(8, "PortStateP7",           15, 14, "", "")\
   REG_DEF_FLD(8, "MemberTagP7",           13, 12, "", "")\
   REG_DEF_FLD(8, "PortStateP6",           11, 10, "", "")\
   REG_DEF_FLD(8, "MemberTagP6",            9,  8, "", "")\
   REG_DEF_FLD(8, "PortStateP5",            7,  6, "", "")\
   REG_DEF_FLD(8, "MemberTagP5",            5,  4, "", "")\
   REG_DEF_FLD(8, "PortStateP4",            3,  2, "", "")\
   REG_DEF_FLD(8, "MemberTagP4",            1,  0, "", "")\
   \
   REG_DEF("switch", 0x0009, "G1_VTU_DATAH",	"Data Reg Ports 8-10",		0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(9, "VidPriOverride",        15, 15, "", "")\
   REG_DEF_FLD(9, "VidPri",                14, 12, "", "")\
   REG_DEF_FLD(9, "PortStateP10",          11, 10, "", "")\
   REG_DEF_FLD(9, "MemberTagP10",           9,  8, "", "")\
   REG_DEF_FLD(9, "PortStateP9",            7,  6, "", "")\
   REG_DEF_FLD(9, "MemberTagP9",            5,  4, "", "")\
   REG_DEF_FLD(9, "PortStateP8",            3,  2, "", "")\
   REG_DEF_FLD(9, "MemberTagP8",            1,  0, "", "")\
   \
   REG_DEF("switch", 0x000A, "G1_ATU_CTRL",	"ATU Contrl",			0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(0x0A, "DBNum",            15,  12, "", "")\
   REG_DEF_FLD(0x0A, "AgeTime",          11,   4, "", "")\
   REG_DEF_FLD(0x0A, "Learn2All",         3,   3, "", "")\
   \
   REG_DEF("switch", 0x000B, "G1_ATU_OP",	"ATU Operation",		0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(0x0B, "ATUBusy",         15,   15, "", "")\
   REG_DEF_FLD(0x0B, "ATUOp",           14,   12, "", "")\
   REG_DEF_FLD(0x0B, "MacPri",          11,    8, "", "")\
   REG_DEF_FLD(0x0B, "MemberViolation",  6,    6, "", "")\
   REG_DEF_FLD(0x0B, "MissViolation",    5,    5, "", "")\
   REG_DEF_FLD(0x0B, "AtuFullViolation", 4,    4, "", "")\
   REG_DEF_FLD(0x0B, "DBNum",            3,    0, "", "")\
   \
   REG_DEF("switch", 0x000C, "G1_ATU_DATA",	"ATU Data0",			0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(0x0C, "Trunk",           15,   15, "", "")\
   REG_DEF_FLD(0x0C, "PortVec-TrunkId", 14,    4, "", "")\
   REG_DEF_FLD(0x0C, "EntryState-SPID",  3,    0, "", "")\
   \
   REG_DEF("switch", 0x000D, "G1_ATU_MAC01",	"ATU Data1",			0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(0x0D, "ATU Mac0",  15,    8, "", "")\
   REG_DEF_FLD(0x0D, "ATU Mac1",   7,    0, "", "")\
   \
   REG_DEF("switch", 0x000E, "G1_ATU_MAC23",	"ATU Data2",			0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(0x0E, "ATU Mac2",  15,    8, "", "")\
   REG_DEF_FLD(0x0E, "ATU Mac3",   7,    0, "", "")\
   \
   REG_DEF("switch", 0x000F, "G1_ATU_MAC45",	"ATU Data3",			0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(0x0F, "ATU Mac4",  15,    8, "", "")\
   REG_DEF_FLD(0x0F, "ATU Mac5",   7,    0, "", "")\
   \
   REG_DEF("switch", 0x0010, "IP_PRI_MAP0",	"IP Pri Mapping",		0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(0x10, "IP_0x1C",   15, 14, "", "")\
   REG_DEF_FLD(0x10, "IP_0x18",   13, 12, "", "")\
   REG_DEF_FLD(0x10, "IP_0x14",   11, 10, "", "")\
   REG_DEF_FLD(0x10, "IP_0x10",    9,  8, "", "")\
   REG_DEF_FLD(0x10, "IP_0x0C",    7,  6, "", "")\
   REG_DEF_FLD(0x10, "IP_0x08",    5,  4, "", "")\
   REG_DEF_FLD(0x10, "IP_0x04",    3,  2, "", "")\
   REG_DEF_FLD(0x10, "IP_0x00",    1,  0, "", "")\
   \
   REG_DEF("switch", 0x0011, "IP_PRI_MAP1",	"IP Pri Mapping",		0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(0x11, "IP_0x3C",   15, 14, "", "")\
   REG_DEF_FLD(0x11, "IP_0x38",   13, 12, "", "")\
   REG_DEF_FLD(0x11, "IP_0x34",   11, 10, "", "")\
   REG_DEF_FLD(0x11, "IP_0x30",    9,  8, "", "")\
   REG_DEF_FLD(0x11, "IP_0x2C",    7,  6, "", "")\
   REG_DEF_FLD(0x11, "IP_0x28",    5,  4, "", "")\
   REG_DEF_FLD(0x11, "IP_0x24",    3,  2, "", "")\
   REG_DEF_FLD(0x11, "IP_0x20",    1,  0, "", "")\
   \
   REG_DEF("switch", 0x0012, "IP_PRI_MAP2",	"IP Pri Mapping",		0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(0x12, "IP_0x5C",   15, 14, "", "")\
   REG_DEF_FLD(0x12, "IP_0x58",   13, 12, "", "")\
   REG_DEF_FLD(0x12, "IP_0x54",   11, 10, "", "")\
   REG_DEF_FLD(0x12, "IP_0x50",    9,  8, "", "")\
   REG_DEF_FLD(0x12, "IP_0x4C",    7,  6, "", "")\
   REG_DEF_FLD(0x12, "IP_0x48",    5,  4, "", "")\
   REG_DEF_FLD(0x12, "IP_0x44",    3,  2, "", "")\
   REG_DEF_FLD(0x12, "IP_0x40",    1,  0, "", "")\
   \
   REG_DEF("switch", 0x0013, "IP_PRI_MAP3",	"IP Pri Mapping",		0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(0x13, "IP_0x7C",   15, 14, "", "")\
   REG_DEF_FLD(0x13, "IP_0x78",   13, 12, "", "")\
   REG_DEF_FLD(0x13, "IP_0x74",   11, 10, "", "")\
   REG_DEF_FLD(0x13, "IP_0x70",    9,  8, "", "")\
   REG_DEF_FLD(0x13, "IP_0x6C",    7,  6, "", "")\
   REG_DEF_FLD(0x13, "IP_0x68",    5,  4, "", "")\
   REG_DEF_FLD(0x13, "IP_0x64",    3,  2, "", "")\
   REG_DEF_FLD(0x13, "IP_0x60",    1,  0, "", "")\
   \
   REG_DEF("switch", 0x0014, "IP_PRI_MAP4",	"IP Pri Mapping",		0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(0x14, "IP_0x9C",   15, 14, "", "")\
   REG_DEF_FLD(0x14, "IP_0x98",   13, 12, "", "")\
   REG_DEF_FLD(0x14, "IP_0x94",   11, 10, "", "")\
   REG_DEF_FLD(0x14, "IP_0x90",    9,  8, "", "")\
   REG_DEF_FLD(0x14, "IP_0x8C",    7,  6, "", "")\
   REG_DEF_FLD(0x14, "IP_0x88",    5,  4, "", "")\
   REG_DEF_FLD(0x14, "IP_0x84",    3,  2, "", "")\
   REG_DEF_FLD(0x14, "IP_0x80",    1,  0, "", "")\
   \
   REG_DEF("switch", 0x0015, "IP_PRI_MAP5",	"IP Pri Mapping",		0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(0x15, "IP_0xBC",   15, 14, "", "")\
   REG_DEF_FLD(0x15, "IP_0xB8",   13, 12, "", "")\
   REG_DEF_FLD(0x15, "IP_0xB4",   11, 10, "", "")\
   REG_DEF_FLD(0x15, "IP_0xB0",    9,  8, "", "")\
   REG_DEF_FLD(0x15, "IP_0xAC",    7,  6, "", "")\
   REG_DEF_FLD(0x15, "IP_0xA8",    5,  4, "", "")\
   REG_DEF_FLD(0x15, "IP_0xA4",    3,  2, "", "")\
   REG_DEF_FLD(0x15, "IP_0xA0",    1,  0, "", "")\
   \
   REG_DEF("switch", 0x0016, "IP_PRI_MAP6",	"IP Pri Mapping",		0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(0x16, "IP_0xDC",   15, 14, "", "")\
   REG_DEF_FLD(0x16, "IP_0xD8",   13, 12, "", "")\
   REG_DEF_FLD(0x16, "IP_0xD4",   11, 10, "", "")\
   REG_DEF_FLD(0x16, "IP_0xD0",    9,  8, "", "")\
   REG_DEF_FLD(0x16, "IP_0xCC",    7,  6, "", "")\
   REG_DEF_FLD(0x16, "IP_0xC8",    5,  4, "", "")\
   REG_DEF_FLD(0x16, "IP_0xC4",    3,  2, "", "")\
   REG_DEF_FLD(0x16, "IP_0xC0",    1,  0, "", "")\
   \
   REG_DEF("switch", 0x0017, "IP_PRI_MAP7",	"IP Pri Mapping",		0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(0x17, "IP_0xFC",   15, 14, "", "")\
   REG_DEF_FLD(0x17, "IP_0xF8",   13, 12, "", "")\
   REG_DEF_FLD(0x17, "IP_0xF4",   11, 10, "", "")\
   REG_DEF_FLD(0x17, "IP_0xF0",    9,  8, "", "")\
   REG_DEF_FLD(0x17, "IP_0xEC",    7,  6, "", "")\
   REG_DEF_FLD(0x17, "IP_0xE8",    5,  4, "", "")\
   REG_DEF_FLD(0x17, "IP_0xE4",    3,  2, "", "")\
   REG_DEF_FLD(0x17, "IP_0xE0",    1,  0, "", "")\
   \
   REG_DEF("switch", 0x0018, "IEEE-PRI",	"IEEE-PRI Pri Mapping",		0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(0x18, "Tag_0x7",   15, 14, "", "")\
   REG_DEF_FLD(0x18, "Tag_0x6",   13, 12, "", "")\
   REG_DEF_FLD(0x18, "Tag_0x5",   11, 10, "", "")\
   REG_DEF_FLD(0x18, "Tag_0x4",    9,  8, "", "")\
   REG_DEF_FLD(0x18, "Tag_0x3",    7,  6, "", "")\
   REG_DEF_FLD(0x18, "Tag_0x2",    5,  4, "", "")\
   REG_DEF_FLD(0x18, "Tag_0x1",    3,  2, "", "")\
   REG_DEF_FLD(0x18, "Tag_0x0",    1,  0, "", "")\
   \
   REG_DEF("switch", 0x0019, "CORE_TAG_TYPE",	"Core Tag Type",		0xffff, 0x0, "cfg")	\
   \
   REG_DEF("switch", 0x001A, "MONITOR_CTRL",	"Monitor Control",		0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(0x1A, "IngressMonitorDestPort",    15,  12, "", "")\
   REG_DEF_FLD(0x1A, "EgressMonitorDestPort",     11,  8, "", "")\
   REG_DEF_FLD(0x1A, "ARPDestPort",                7,  4, "", "")\
   \
   REG_DEF("switch", 0x001C, "GLOBAL_CTRL2",	"Global Control2",		0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(0x1C, "CascadePort",                15,  12, "", "")\
   REG_DEF_FLD(0x1C, "DeviceNumber",                3,   0, "", "")\
   \
   REG_DEF("switch", 0x001D, "STATS_OP",	"Stats Operation",		0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(0x1D, "StatsBsy",     15,   15, "", "")\
   REG_DEF_FLD(0x1D, "StatsOp",      14,   12, "", "")\
   REG_DEF_FLD(0x1D, "HistogramMode",11,   10, "", "")\
   REG_DEF_FLD(0x1D, "StatsPtr",      5,   0, "", "")\
   \
   REG_DEF("switch", 0x001E, "STATS_COUNTER0",	"Stats Counter 0",		0xffff, 0x0, "cfg")	\
   REG_DEF("switch", 0x001F, "STATS_COUNTER1",	"Stats Counter 1",		0xffff, 0x0, "cfg")	\


#define MV88E6095_GLOBAL2_REG_DESC		\
   REG_DEF("switch", 0x0003, "CPU_EN_VEC",	"Rsvd2CPU Enable Vector",	0xffff, 0x0, "cfg")	\
   \
   REG_DEF("switch", 0x0004, "FLOW_CTRL_DELAY",	"Flow Control Delays",		0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(0x4, "Update",     15,   15, "", "")\
   REG_DEF_FLD(0x4, "Speed",      14,   13, "", "")\
   REG_DEF_FLD(0x4, "FcDelayTime",12,    0, "", "")\
   \
   REG_DEF("switch", 0x0005, "MANAGEMENT",	"Management",			0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(0x5, "UseDoubleTag", 15,   15, "", "")\
   REG_DEF_FLD(0x5, "PreventLoops", 14,   14, "", "")\
   REG_DEF_FLD(0x5, "FlowControlMessage", 13,   13, "", "")\
   REG_DEF_FLD(0x5, "ForceFlowControlPri",  7,    7, "", "")\
   REG_DEF_FLD(0x5, "FCPri",  6,    4, "", "")\
   REG_DEF_FLD(0x5, "Rsvd2CPU",  3,    3, "", "")\
   REG_DEF_FLD(0x5, "MgmtPri",  2,    0, "", "")\
   \
   REG_DEF("switch", 0x0006, "DEV_MAP_TBL",	"Device Mapping Table",		0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(0x6, "Update",        15,   15, "", "")\
   REG_DEF_FLD(0x6, "Trg_DevValue",  12,   8, "", "")\
   REG_DEF_FLD(0x6, "Trg_DevPort",    3,   0, "", "")\
   \
   REG_DEF("switch", 0x0007, "TRUNK_MASK",	"Trunk Mask",			0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(0x7, "Update",        15,   15, "", "")\
   REG_DEF_FLD(0x7, "MaskNum",       14,   12, "", "")\
   REG_DEF_FLD(0x7, "HashTrunk",     11,   11, "", "")\
   REG_DEF_FLD(0x7, "TrunkMask",     10,    0, "", "")\
   \
   REG_DEF("switch", 0x0008, "TRUNK_MEMBERS",	"Trunk Members",		0xffff, 0x0, "cfg")	\
   REG_DEF_FLD(0x8, "Update",        15,   15, "", "")\
   REG_DEF_FLD(0x8, "TrunkId",       14,   12, "", "")\
   REG_DEF_FLD(0x8, "TrunkMembers",  10,    0, "", "")\


#define MV88E6095_PHY_REG_DESC\
  REG_DEF("phy", 0, "control", "", 0xffc0, 0x00, "cfg")\
  REG_DEF_FLD(0, "RESET",         15, 15, "", "")\
  REG_DEF_FLD(0, "LOOPBACK_EN",   14, 14, "", "")\
  REG_DEF_FLD(0, "SPEED_LSB",     13, 13, "", "")\
  REG_DEF_FLD(0, "AUTONEG_EN",    12, 12, "", "")\
  REG_DEF_FLD(0, "POWERDOWN",     11, 11, "", "")\
  REG_DEF_FLD(0, "ISOLATE",       10, 10, "", "")\
  REG_DEF_FLD(0, "AUTONEG_RESTART",9, 9, "", "")\
  REG_DEF_FLD(0, "FULL_DUPLEX",    8, 8, "", "")\
  REG_DEF_FLD(0, "COLLISION_TEST", 7, 7, "", "")\
  REG_DEF_FLD(0, "SPEED_MSB",      6, 6, "", "")\
  \
  REG_DEF("phy", 1, "status", "", 0xff7F, 0x00, "sta")\
  REG_DEF_FLD(1, "100BASE_T4",            15,15, "", "")\
  REG_DEF_FLD(1, "100BASE_X_FULL_DUP",    14,14, "", "")\
  REG_DEF_FLD(1, "100BASE_X_HALF_DUP",    13,13, "", "")\
  REG_DEF_FLD(1, "10BASE_X_FULL_DUP",     12,12, "", "")\
  REG_DEF_FLD(1, "10BASE_X_HALF_DUP",     11,11, "", "")\
  REG_DEF_FLD(1, "100BASE_T2_FULL_DUP",   10,10, "", "")\
  REG_DEF_FLD(1, "100BASE_T2_HALF_DUP",   9 ,9, "", "")\
  REG_DEF_FLD(1, "EXT_STATUS",            8 ,8, "", "")\
  REG_DEF_FLD(1, "MF_PREAMBLE_SUPP",      6 ,6, "", "")\
  REG_DEF_FLD(1, "AUTO_NEG_COMPLETE",     5 ,5, "", "")\
  REG_DEF_FLD(1, "REMOTE_FAULT",          4 ,4, "", "")\
  REG_DEF_FLD(1, "AUTO_NEG_ABILITY",      3 ,3, "", "")\
  REG_DEF_FLD(1, "INK_STATUS",            2 ,2, "", "")\
  REG_DEF_FLD(1, "JABBER_DETECT",         1 ,1, "", "")\
  REG_DEF_FLD(1, "EXT_CAPABILITY",        0 ,0, "", "")\
  \
  REG_DEF("phy", 2, "phy_id_msb", "", 0xffff, 0x00, "sta")\
  \
  REG_DEF("phy", 3, "phy_id_lsb", "", 0xffff, 0x00, "sta")\
  REG_DEF_FLD(3, "OUI Lsb",               15,10, "", "")\
  REG_DEF_FLD(3, "Model Number",          9,4, "", "")\
  REG_DEF_FLD(3, "Revision Number",       3,0, "", "")\
  \
  REG_DEF("phy", 4, "auto_neg_advertisement", "", 0xffff, 0x00, "cfg")\
  REG_DEF_FLD(4, "NEXT_PAGE",             15,15, "", "")\
  REG_DEF_FLD(4, "ACK",                   14,14, "", "")\
  REG_DEF_FLD(4, "REMOTE_FAULT",          13,13, "", "")\
  REG_DEF_FLD(4, "PAUSE",                 10,10, "", "")\
  REG_DEF_FLD(4, "100BASE_T4",            9 ,9, "", "")\
  REG_DEF_FLD(4, "100BASE_TX_FULL_DUP",   8 ,8, "", "")\
  REG_DEF_FLD(4, "100BASE_TX_HALF_DUP",   7 ,7, "", "")\
  REG_DEF_FLD(4, "100BASE_T_FULL_DUP",    6 ,6, "", "")\
  REG_DEF_FLD(4, "100BASE_T_HALF_DUP",    5 ,5, "", "")\
  REG_DEF_FLD(4, "SELECTOR_FIELD",        4, 0, "", "")\
  \
  REG_DEF("phy", 5, "link_partner_ability", "", 0xffff, 0x00, "sta")\
  REG_DEF_FLD(5, "NEXT_PAGE",             15,15, "", "")\
  REG_DEF_FLD(5, "ACK",                   14,14, "", "")\
  REG_DEF_FLD(5, "REMOTE_FAULT",          13,13, "", "")\
  REG_DEF_FLD(5, "LP_TECH_ABILITY",       12,5, "", "")\
  REG_DEF_FLD(5, "LP_SECTOR_FIELD",       4, 0, "", "")\
  \
  REG_DEF("phy", 6, "autoneg expansion", "", 0xffff, 0x00, "cfg")\
  REG_DEF_FLD(6, "DETECT_FAULT",          4 ,4, "", "")\
  REG_DEF_FLD(6, "LP_NEXT_PAGE_ABLE",     3 ,3, "", "")\
  REG_DEF_FLD(6, "LOCAL_NEXT_PAGE_ABLE",  2 ,2, "", "")\
  REG_DEF_FLD(6, "PAGE_RECEIVED",         1 ,1, "", "")\
  REG_DEF_FLD(6, "LP_AUTO_NEG_ABLE",      0 ,0, "", "")\
  \
  REG_DEF("phy", 7, "next_page_tx", "", 0xBfff, 0x00, "cfg")\
  REG_DEF_FLD(7, "NEXT_PAGE",             15,15, "", "")\
  REG_DEF_FLD(7, "MSG_PAGE_MODE",         13,13, "", "")\
  REG_DEF_FLD(7, "ACK2",                  12,12, "", "")\
  REG_DEF_FLD(7, "TOGGLE",                11,11, "", "")\
  REG_DEF_FLD(7, "MSG_UNFORMAT_FLD",      10, 0, "", "")\
  \
  REG_DEF("phy", 8, "lnk_partner_nxt_tx", "", 0xBfff, 0x00, "sta")\
  REG_DEF_FLD(8, "NEXT_PAGE",             15, 15, "", "")\
  REG_DEF_FLD(8, "RxACK",                 14, 14, "", "")\
  REG_DEF_FLD(8, "RxMSG_PAGE_MODE",       13, 13, "", "")\
  REG_DEF_FLD(8, "RxACK2",                12, 12, "", "")\
  REG_DEF_FLD(8, "RxTOGGLE",              11, 11, "", "")\
  REG_DEF_FLD(8, "RxMSG_UNFORMAT_FLD",    10, 0, "", "")\
  \
  REG_DEF("phy", 9, "1000BaseT_Control", "", 0xffff, 0x00, "cfg") \
  REG_DEF_FLD(9, "TEST_MODE",             15, 13, "", "")\
  REG_DEF_FLD(9, "MASTER_SLAVE_EN",       12,12, "", "")\
  REG_DEF_FLD(9, "MASTER_CFG",            11,11, "", "")\
  REG_DEF_FLD(9, "PORT_TYPE",             10,10, "", "")\
  REG_DEF_FLD(9, "FULL_DUP",              9 ,9, "", "")\
  REG_DEF_FLD(9, "HALF_DUP",              8 ,8, "", "")\
  \
  REG_DEF("phy", 0x10, "Specific_control", "", 0xffff, 0x00, "cfg")\
  REG_DEF_FLD(0x10, "ENABLE DTE DETECT",  15, 15, "", "")\
  REG_DEF_FLD(0x10, "ENERGY DETECT",      14, 14, "", "")\
  REG_DEF_FLD(0x10, "DIS_LINK_PULSE_CHK", 13, 13, "", "")\
  REG_DEF_FLD(0x10, "ENA_LP_NEXT_REG",    12, 12, "", "")\
  REG_DEF_FLD(0x10, "DIS_LNK_PULSE_GEN",  11, 11, "", "")\
  REG_DEF_FLD(0x10, "FORCE_LINK_GOOD",    10, 10, "", "")\
  REG_DEF_FLD(0x10, "DIS_SCRAMBLER",      9, 9, "", "")\
  REG_DEF_FLD(0x10, "DIS_FEFI",           8, 8, "", "")\
  REG_DEF_FLD(0x10, "EXT_DIST_EN",        7, 7, "", "")\
  REG_DEF_FLD(0x10, "TWISTED_PAIR_SEL",   6, 6, "", "")\
  REG_DEF_FLD(0x10, "MDI/MDIX CROSSOVER", 5, 4, "", "")\
  REG_DEF_FLD(0x10, "RX_FIFO_DEPTH",      3, 2, "", "")\
  REG_DEF_FLD(0x10, "POLARITY_REVERSAL",  1, 1, "", "")\
  REG_DEF_FLD(0x10, "JABBER_DISABLE",     0, 0, "", "")\
  \
  REG_DEF("phy", 0x11, "Specific_Status", "", 0xffff, 0x00, "sta")\
  REG_DEF_FLD(0x11, "DTE DETECT",         15, 15, "", "")\
  REG_DEF_FLD(0x11, "SPEED",              14, 14, "", "")\
  REG_DEF_FLD(0x11, "DUPLEX",             13 ,13, "", "")\
  REG_DEF_FLD(0x11, "PAGE_RECEIVED",      12 ,12, "", "")\
  REG_DEF_FLD(0x11, "SPD_DUP_RESOLVED",   11 ,11, "", "")\
  REG_DEF_FLD(0x11, "LINK",               10 ,10, "", "")\
  REG_DEF_FLD(0x11, "MDI_CROSS_OVER",     6 ,6, "", "")\
  REG_DEF_FLD(0x11, "SLEEP",              4 ,4, "", "")\
  REG_DEF_FLD(0x11, "POLARITY",           1 ,1, "", "")\
  REG_DEF_FLD(0x11, "JABBER",             0 ,0, "", "")\
  \
  REG_DEF("phy", 0x12, "irq_enable", "", 0xfff7, 0x00, "cfg")\
  REG_DEF_FLD(0x12, "DTE_DETECT_STATE_CHG",       15,15, "", "")\
  REG_DEF_FLD(0x12, "SPEED_CHANGED",              14,14, "", "")\
  REG_DEF_FLD(0x12, "DUPLEX_CHANGED",             13,13, "", "")\
  REG_DEF_FLD(0x12, "PAGE_RECIEVED",              12,12, "", "")\
  REG_DEF_FLD(0x12, "AUTO_NEG_COMPLETE",          11,11, "", "")\
  REG_DEF_FLD(0x12, "LINK_STATUS_CHANGED",        10,10, "", "")\
  REG_DEF_FLD(0x12, "SYMBOL_ERROR",               9 ,9, "", "")\
  REG_DEF_FLD(0x12, "FALSE_CARRIER",              8 ,8, "", "")\
  REG_DEF_FLD(0x12, "FIFO_OVER_UNDER_FLOW",       7 ,7, "", "")\
  REG_DEF_FLD(0x12, "MDI_CROSS_OVER",             6 ,6, "", "")\
  REG_DEF_FLD(0x12, "ENERGY_DETECT",              4 ,4, "", "")\
  REG_DEF_FLD(0x12, "POLARITY_CHANGED",           1 ,1, "", "")\
  REG_DEF_FLD(0x12, "JABBER",                     0 ,0, "", "")\
  \
  REG_DEF("phy", 0x13, "irq_status", "", 0xfff7, 0x00, "int")\
  REG_DEF_FLD(0x13, "DTE_DETECT_STATE",   15,15, "", "")\
  REG_DEF_FLD(0x13, "SPEED_CHANGED",      14,14, "", "")\
  REG_DEF_FLD(0x13, "DUPLEX_CHANGED",     13,13, "", "")\
  REG_DEF_FLD(0x13, "PAGE_RECIEVED",      12,12, "", "")\
  REG_DEF_FLD(0x13, "AUTO_NEG_COMPLETE",  11,11, "", "")\
  REG_DEF_FLD(0x13, "AUTO_LINK_STATUS",   10,10, "", "")\
  REG_DEF_FLD(0x13, "SYMBOL_ERROR",       9 ,9, "", "")\
  REG_DEF_FLD(0x13, "FALSE_CARRIER",      8 ,8, "", "")\
  REG_DEF_FLD(0x13, "FIFO_OVER_FLOW",     7 ,7, "", "")\
  REG_DEF_FLD(0x13, "MDI_CROSS_OVER",     6 ,6, "", "")\
  REG_DEF_FLD(0x13, "ENERGY_DETECT",      4 ,4, "", "")\
  REG_DEF_FLD(0x13, "POLARITY_CHANGED",   1 ,1, "", "")\
  REG_DEF_FLD(0x13, "JABBER",             0 ,0, "", "")\
  \
  REG_DEF("phy", 0x14, "phy_int_port_summary", "", 0xffff, 0x00, "sta")\
  REG_DEF_FLD(0x14, "PORT7_INT_ACTIVE",   7 ,7, "", "")\
  REG_DEF_FLD(0x14, "PORT6_INT_ACTIVE",   6 ,6, "", "")\
  REG_DEF_FLD(0x14, "PORT5_INT_ACTIVE",   5 ,5, "", "")\
  REG_DEF_FLD(0x14, "PORT4_INT_ACTIVE",   4 ,4, "", "")\
  REG_DEF_FLD(0x14, "PORT3_INT_ACTIVE",   3 ,3, "", "")\
  REG_DEF_FLD(0x14, "PORT2_INT_ACTIVE",   2 ,2, "", "")\
  REG_DEF_FLD(0x14, "PORT1_INT_ACTIVE",   1 ,1, "", "")\
  REG_DEF_FLD(0x14, "PORT0_INT_ACTIVE",   0 ,0, "", "")\
  \
  REG_DEF("phy", 0x15, "rx_err_counter", "", 0xffff, 0x00, "cnt")\
  \
  REG_DEF("phy", 0x16, "led_parallel_sel", "", 0xffff, 0x00, "cfg")\
  REG_DEF_FLD(0x16, "DTE_DETECT_STA_DRP", 15, 12, "", "")\
  REG_DEF_FLD(0x16, "LED2",               11, 8, "", "")\
  REG_DEF_FLD(0x16, "LED1",                7, 4, "", "")\
  REG_DEF_FLD(0x16, "LED0",                3, 0, "", "")\
  \
  REG_DEF("phy", 0x17, "led_stream_sel", "", 0xffff, 0x00, "sta")\
  REG_DEF_FLD(0x17, "LED_LINK_ACTIVITY",  15, 14, "", "")\
  REG_DEF_FLD(0x17, "LED_RX_LINK",        13, 12, "", "")\
  REG_DEF_FLD(0x17, "LED_ACTIVITY",       11, 10, "", "")\
  REG_DEF_FLD(0x17, "LED_RECEIVE",         9,  8, "", "")\
  REG_DEF_FLD(0x17, "LED_TRANSMIT",        7,  6, "", "")\
  REG_DEF_FLD(0x17, "LED_LINK",            5,  4, "", "")\
  REG_DEF_FLD(0x17, "LED_SPEED",           3,  2, "", "")\
  REG_DEF_FLD(0x17, "LED_DUP_COLX",        1,  0, "", "")\
  \
  REG_DEF("phy", 0x18, "led_control", "", 0xffff, 0x00, "cfg")\
  REG_DEF_FLD(0x18, "PULSE_STRETCH",      14, 12, "", "")\
  REG_DEF_FLD(0x18, "BLINK_RATE",         11, 9, "", "")\
  REG_DEF_FLD(0x18, "SER_STREAM_UPDATE",   8, 6, "", "")\
  REG_DEF_FLD(0x18, "DUPLEX",              5, 4, "", "")\
  REG_DEF_FLD(0x18, "ERROR",               3, 2, "", "")\
  REG_DEF_FLD(0x18, "COLX",                1 ,0, "", "")\
  \
  REG_DEF("phy", 0x19, "manual_led_override", "", 0xCfff, 0x00, "cfg")\
  REG_DEF_FLD(0x19, "INV_LED2",     14,14, "", "")\
  REG_DEF_FLD(0x19, "INV_LED1",     13,13, "", "")\
  REG_DEF_FLD(0x19, "INV_LED0",     12,12, "", "")\
  REG_DEF_FLD(0x19, "SPD_LED2",     11,10, "", "")\
  REG_DEF_FLD(0x19, "SPD_LED1",      9, 8, "", "")\
  REG_DEF_FLD(0x19, "SPD_LED0",      7, 6, "", "")\
  REG_DEF_FLD(0x19, "FORCE_LED2",    5, 4, "", "")\
  REG_DEF_FLD(0x19, "FORCE_LED1",    3, 2, "", "")\
  REG_DEF_FLD(0x19, "FORCE_LED0",    1, 0, "", "")\
  \
  REG_DEF("phy", 0x1A, "vct_reg_txpn_pins", "", 0xffff, 0x00, "cfg")\
  REG_DEF_FLD(0x1A, "VCT_EN",         15,15, "", "")\
  REG_DEF_FLD(0x1A, "VCT_TST",        14,13, "", "")\
  REG_DEF_FLD(0x1A, "REFLECTION_AMP", 12,8, "", "")\
  REG_DEF_FLD(0x1A, "REFLECTION_LEN",  7,0, "", "")\
  \
  REG_DEF("phy", 0x1B, "vct_txpn_stat_pins", "", 0xffff, 0x00, "sta")\
  REG_DEF_FLD(0x1B, "VCT_TST",        14,13, "", "")\
  REG_DEF_FLD(0x1B, "REFLECTION_AMP", 12,8, "", "")\
  REG_DEF_FLD(0x1B, "REFLECTION_LEN",  7,0, "", "")\
  \
  REG_DEF("phy", 0x1C, "specific_ctrl2", "", 0xffff, 0x00, "cfg")\
  REG_DEF_FLD(0x1C, "LINE_LPBK_EN",        4,4, "", "")\
  REG_DEF_FLD(0x1C, "MEDIA_SELECT",        3,3, "", "")\
  REG_DEF_FLD(0x1C, "TDR_WAIT_TIME",       2,2, "", "")\
  REG_DEF_FLD(0x1C, "RX_CLK_ENA",          1,1, "", "")\
  REG_DEF_FLD(0x1C, "CLASS_A_SELECT",      0,0, "", "")
