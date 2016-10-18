/********************************************************************
 *
 * File: redwood_ss_stats.h
 * Name: Bruce McLoughlin
 *
 * Description:
 *   SS block stats definitions for Redwood.
 *
 * Copyright (c) 2006-2009 by Nuova Systems, Inc.
 **********************************************************************/
#ifndef SS_STATS_INCLUDED
#define SS_STATS_INCLUDED

/*********************************************************************
    Following is the definition from HW:


[bmcloughlin@t1 ~]$ cat ~pkaplan/tmp/ss_stats.txt
DHS offset              STAT
----------              ----
0                       unicast   class 0 write port 0
1                       unicast   class 1 write port 0
2                       unicast   class 2 write port 0
3                       unicast   class 3 write port 0
4                       multicast class 0 write port 0
5                       multicast class 1 write port 0
6                       multicast class 2 write port 0
7                       multicast class 3 write port 0
8                       discard no credit write port 0
9                       discard multicast write port 0
10                      discard error     write port 0
11                      pause sent        write port 0
12                      reserved
13                      reserved
14                      reserved
15                      reserved

16                      unicast   class 0 write port 1
17                      unicast   class 1 write port 1
18                      unicast   class 2 write port 1
19                      unicast   class 3 write port 1
20                      multicast class 0 write port 1
21                      multicast class 1 write port 1
22                      multicast class 2 write port 1
23                      multicast class 3 write port 1
24                      discard no credit write port 1
25                      discard multicast write port 1
26                      discard error     write port 1
27                      pause sent        write port 1
28                      reserved
29                      reserved
30                      reserved
31                      reserved

32                      unicast   class 0 write port 2
33                      unicast   class 1 write port 2
34                      unicast   class 2 write port 2
35                      unicast   class 3 write port 2
36                      multicast class 0 write port 2
37                      multicast class 1 write port 2
38                      multicast class 2 write port 2
39                      multicast class 3 write port 2
40                      discard no credit write port 2
41                      discard multicast write port 2
42                      discard error     write port 2
43                      pause sent        write port 2
44                      reserved
45                      reserved
46                      reserved
47                      reserved

48                      unicast   class 0 write port 3
49                      unicast   class 1 write port 3
50                      unicast   class 2 write port 3
51                      unicast   class 3 write port 3
52                      multicast class 0 write port 3
53                      multicast class 1 write port 3
54                      multicast class 2 write port 3
55                      multicast class 3 write port 3
56                      discard no credit write port 3
57                      discard multicast write port 3
58                      discard error     write port 3
59                      pause sent        write port 3
60                      reserved
61                      reserved
62                      reserved
63                      reserved

64                      unicast   class 0 write port 4
65                      unicast   class 1 write port 4
66                      unicast   class 2 write port 4
67                      unicast   class 3 write port 4
68                      multicast class 0 write port 4
69                      multicast class 1 write port 4
70                      multicast class 2 write port 4
71                      multicast class 3 write port 4
72                      discard no credit write port 4
73                      discard multicast write port 4
74                      discard error     write port 4
75                      pause sent        write port 4
76                      reserved
77                      reserved
78                      reserved
79                      reserved

80                      unicast   class 0 read port 0
81                      unicast   class 1 read port 0
82                      unicast   class 2 read port 0
83                      unicast   class 3 read port 0
84                      multicast class 0 read port 0
85                      multicast class 1 read port 0
86                      multicast class 2 read port 0
87                      multicast class 3 read port 0
88                      discard           read port 0
89                      flush             read port 0
90                      pause seen        read port 0
91                      reserved
92                      reserved
93                      reserved
94                      reserved
95                      reserved

96                      unicast   class 0 read port 1
97                      unicast   class 1 read port 1
98                      unicast   class 2 read port 1
99                      unicast   class 3 read port 1
100                     multicast class 0 read port 1
101                     multicast class 1 read port 1
102                     multicast class 2 read port 1
103                     multicast class 3 read port 1
104                     discard           read port 1
105                     flush             read port 1
106                     pause seen        read port 1
107                     reserved
108                     reserved
109                     reserved
110                     reserved
111                     reserved

112                     unicast   class 0 read port 2
113                     unicast   class 1 read port 2
114                     unicast   class 2 read port 2
115                     unicast   class 3 read port 2
116                     multicast class 0 read port 2
117                     multicast class 1 read port 2
118                     multicast class 2 read port 2
119                     multicast class 3 read port 2
120                     discard           read port 2
121                     flush             read port 2
122                     pause seen        read port 2
123                     reserved
124                     reserved
125                     reserved
126                     reserved
127                     reserved

128                     unicast   class 0 read port 3
129                     unicast   class 1 read port 3
130                     unicast   class 2 read port 3
131                     unicast   class 3 read port 3
132                     multicast class 0 read port 3
133                     multicast class 1 read port 3
134                     multicast class 2 read port 3
135                     multicast class 3 read port 3
136                     discard           read port 3
137                     flush             read port 3
138                     pause seen        read port 3
139                     reserved
140                     reserved
141                     reserved
142                     reserved
143                     reserved

144                     unicast   class 0 read port 4
145                     unicast   class 1 read port 4
146                     unicast   class 2 read port 4
147                     unicast   class 3 read port 4
148                     multicast class 0 read port 4
149                     multicast class 1 read port 4
150                     multicast class 2 read port 4
151                     multicast class 3 read port 4
152                     discard           read port 4
153                     flush             read port 4
154                     pause seen        read port 4
155                     reserved
156                     reserved
157                     reserved
158                     reserved
159                     reserved

160..255                configurable stats. Use RED_SS0_CFG_STATS_CONF_SEL: (IN_PORT,OUT_PORT,OPCODE)
                        OPCODE: 0=port to port packet count
                                1=write xoff count
                                2=read xoff count
                                3=read !drdy count

For opcode=0 (port to port packet count)

160       class=0 in_port=RED_SS0_CFG_STATS_CONF_SEL_FLD_IN_PORT out_port=RED_SS0_CFG_STATS_CONF_SEL_FLD_OUT_PORT
161       class=1 in_port=RED_SS0_CFG_STATS_CONF_SEL_FLD_IN_PORT out_port=RED_SS0_CFG_STATS_CONF_SEL_FLD_OUT_PORT
162       class=2 in_port=RED_SS0_CFG_STATS_CONF_SEL_FLD_IN_PORT out_port=RED_SS0_CFG_STATS_CONF_SEL_FLD_OUT_PORT
163       class=3 in_port=RED_SS0_CFG_STATS_CONF_SEL_FLD_IN_PORT out_port=RED_SS0_CFG_STATS_CONF_SEL_FLD_OUT_PORT

For opcode=1 (write xoff 8 cycle count)
160                     class=0 in_port=RED_SS0_CFG_STATS_CONF_SEL_FLD_IN_PORT
161                     class=1 in_port=RED_SS0_CFG_STATS_CONF_SEL_FLD_IN_PORT
162                     class=2 in_port=RED_SS0_CFG_STATS_CONF_SEL_FLD_IN_PORT
163                     class=3 in_port=RED_SS0_CFG_STATS_CONF_SEL_FLD_IN_PORT

For opcode=2 (read xoff 8 cycle count)
160                     class=0 out_port=RED_SS0_CFG_STATS_CONF_SEL_FLD_OUT_PORT
161                     class=1 out_port=RED_SS0_CFG_STATS_CONF_SEL_FLD_OUT_PORT
162                     class=2 out_port=RED_SS0_CFG_STATS_CONF_SEL_FLD_OUT_PORT
163                     class=3 out_port=RED_SS0_CFG_STATS_CONF_SEL_FLD_OUT_PORT

For opcode=3 (read !drdy 8 cycle count)
160                     out_port=RED_SS0_CFG_STATS_CONF_SEL_FLD_OUT_PORT
161                     reserved
162                     reserved
163                     reserved
******************************************************************************/

#define NUM_CLASSES 4
typedef struct wr_port_stats_t {
    unsigned int unicast[NUM_CLASSES];
    unsigned int multicast[NUM_CLASSES];
    unsigned int discard_no_credit;
    unsigned int discard_multicast;
    unsigned int discard_error;
    unsigned int pause_sent;
    unsigned int reserved[4];
} wr_port_stats_t;

typedef struct rd_port_stats_t {
    unsigned int unicast[NUM_CLASSES];
    unsigned int multicast[NUM_CLASSES];
    unsigned int discard;
    unsigned int flush;
    unsigned int pause_seen;
    unsigned int reserved[5];
} rd_port_stats_t;

#define NUM_PORTS 5
typedef struct ss_stats_fixed_t {
    wr_port_stats_t wr_port[ NUM_PORTS ];
    rd_port_stats_t rd_port[ NUM_PORTS ];
} ss_stats_fixed_t;

char *wr_port_stat_names[] = {
"unicast   class 0 write port ",
"unicast   class 1 write port ",
"unicast   class 2 write port ",
"unicast   class 3 write port ",
"multicast class 0 write port ",
"multicast class 1 write port ",
"multicast class 2 write port ",
"multicast class 3 write port ",
"discard no credit write port ",
"discard multicast write port ",
"discard error     write port ",
"pause sent        write port "
};

char *rd_port_stat_names[] = {
"unicast   class 0 read port ",
"unicast   class 1 read port ",
"unicast   class 2 read port ",
"unicast   class 3 read port ",
"multicast class 0 read port ",
"multicast class 1 read port ",
"multicast class 2 read port ",
"multicast class 3 read port ",
"discard           read port ",
"flush             read port ",
"pause seen        read port "
};

#endif /*SS_STATS_INCLUDED*/
