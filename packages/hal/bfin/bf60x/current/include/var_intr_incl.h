#ifndef CYGONCE_HAL_VAR_INTR_INCL_H
#define CYGONCE_HAL_VAR_INTR_INCL_H

// define VSR numbers for interrupt vsrs
// GP interrupt 7
#define CYGNUM_HAL_VECTOR_INTR_7                        7
// GP interrupt 8
#define CYGNUM_HAL_VECTOR_INTR_8                        8
// GP interrupt 9
#define CYGNUM_HAL_VECTOR_INTR_9                        9
// GP interrupt 10
#define CYGNUM_HAL_VECTOR_INTR_10                       10
// GP interrupt 11
#define CYGNUM_HAL_VECTOR_INTR_11                       11
// GP interrupt 12
#define CYGNUM_HAL_VECTOR_INTR_12                       12
// GP interrupt 13
#define CYGNUM_HAL_VECTOR_INTR_13                       13


// define numbers for interrupt isrs
// GP7
#define CYGNUM_HAL_INTERRUPT_7                7
// GP8
#define CYGNUM_HAL_INTERRUPT_8                8
// GP9
#define CYGNUM_HAL_INTERRUPT_9                9
// GP10
#define CYGNUM_HAL_INTERRUPT_10               10
// GP11
#define CYGNUM_HAL_INTERRUPT_11               11
// GP12
#define CYGNUM_HAL_INTERRUPT_12               12
// GP13
#define CYGNUM_HAL_INTERRUPT_13               13

// define VSR numbers for interrupt vsrs
#define CYGNUM_HAL_VECTOR_SEC_BASE                      16
// SEC 0
#define CYGNUM_HAL_VECTOR_INTR_16                        (CYGNUM_HAL_VECTOR_SEC_BASE + 0)
// SEC 1
#define CYGNUM_HAL_VECTOR_INTR_17                        (CYGNUM_HAL_VECTOR_SEC_BASE + 1)
// SEC 2
#define CYGNUM_HAL_VECTOR_INTR_18                        (CYGNUM_HAL_VECTOR_SEC_BASE + 2)
// SEC 3
#define CYGNUM_HAL_VECTOR_INTR_19                        (CYGNUM_HAL_VECTOR_SEC_BASE + 3)
// SEC 4
#define CYGNUM_HAL_VECTOR_INTR_20                        (CYGNUM_HAL_VECTOR_SEC_BASE + 4)
// SEC 5
#define CYGNUM_HAL_VECTOR_INTR_21                        (CYGNUM_HAL_VECTOR_SEC_BASE + 5)
// SEC 6
#define CYGNUM_HAL_VECTOR_INTR_22                        (CYGNUM_HAL_VECTOR_SEC_BASE + 6)
// SEC 7
#define CYGNUM_HAL_VECTOR_INTR_23                        (CYGNUM_HAL_VECTOR_SEC_BASE + 7)
// SEC 8
#define CYGNUM_HAL_VECTOR_INTR_24                        (CYGNUM_HAL_VECTOR_SEC_BASE + 8)
// SEC 9
#define CYGNUM_HAL_VECTOR_INTR_25                        (CYGNUM_HAL_VECTOR_SEC_BASE + 9)
// SEC 10
#define CYGNUM_HAL_VECTOR_INTR_26                        (CYGNUM_HAL_VECTOR_SEC_BASE + 10)
// SEC 11
#define CYGNUM_HAL_VECTOR_INTR_27                        (CYGNUM_HAL_VECTOR_SEC_BASE + 11)
// SEC 12
#define CYGNUM_HAL_VECTOR_INTR_28                        (CYGNUM_HAL_VECTOR_SEC_BASE + 12)
// SEC 13
#define CYGNUM_HAL_VECTOR_INTR_29                        (CYGNUM_HAL_VECTOR_SEC_BASE + 13)
// SEC 14
#define CYGNUM_HAL_VECTOR_INTR_30                        (CYGNUM_HAL_VECTOR_SEC_BASE + 14)
// SEC 15
#define CYGNUM_HAL_VECTOR_INTR_31                        (CYGNUM_HAL_VECTOR_SEC_BASE + 15)
// SEC 16
#define CYGNUM_HAL_VECTOR_INTR_32                        (CYGNUM_HAL_VECTOR_SEC_BASE + 16)
// SEC 17
#define CYGNUM_HAL_VECTOR_INTR_33                        (CYGNUM_HAL_VECTOR_SEC_BASE + 17)
// SEC 18
#define CYGNUM_HAL_VECTOR_INTR_34                        (CYGNUM_HAL_VECTOR_SEC_BASE + 18)
// SEC 19
#define CYGNUM_HAL_VECTOR_INTR_35                        (CYGNUM_HAL_VECTOR_SEC_BASE + 19)
// SEC 20
#define CYGNUM_HAL_VECTOR_INTR_36                        (CYGNUM_HAL_VECTOR_SEC_BASE + 20)
// SEC 21
#define CYGNUM_HAL_VECTOR_INTR_37                        (CYGNUM_HAL_VECTOR_SEC_BASE + 21)
// SEC 22
#define CYGNUM_HAL_VECTOR_INTR_38                        (CYGNUM_HAL_VECTOR_SEC_BASE + 22)
// SEC 23
#define CYGNUM_HAL_VECTOR_INTR_39                        (CYGNUM_HAL_VECTOR_SEC_BASE + 23)
// SEC 24
#define CYGNUM_HAL_VECTOR_INTR_40                        (CYGNUM_HAL_VECTOR_SEC_BASE + 24)
// SEC 25
#define CYGNUM_HAL_VECTOR_INTR_41                        (CYGNUM_HAL_VECTOR_SEC_BASE + 25)
// SEC 26
#define CYGNUM_HAL_VECTOR_INTR_42                        (CYGNUM_HAL_VECTOR_SEC_BASE + 26)
// SEC 27
#define CYGNUM_HAL_VECTOR_INTR_43                        (CYGNUM_HAL_VECTOR_SEC_BASE + 27)
// SEC 28
#define CYGNUM_HAL_VECTOR_INTR_44                        (CYGNUM_HAL_VECTOR_SEC_BASE + 28)
// SEC 29
#define CYGNUM_HAL_VECTOR_INTR_45                        (CYGNUM_HAL_VECTOR_SEC_BASE + 29)
// SEC 30
#define CYGNUM_HAL_VECTOR_INTR_46                        (CYGNUM_HAL_VECTOR_SEC_BASE + 30)
// SEC 31
#define CYGNUM_HAL_VECTOR_INTR_47                        (CYGNUM_HAL_VECTOR_SEC_BASE + 31)
// SEC 32
#define CYGNUM_HAL_VECTOR_INTR_48                        (CYGNUM_HAL_VECTOR_SEC_BASE + 32)
// SEC 33
#define CYGNUM_HAL_VECTOR_INTR_49                        (CYGNUM_HAL_VECTOR_SEC_BASE + 33)
// SEC 34
#define CYGNUM_HAL_VECTOR_INTR_50                        (CYGNUM_HAL_VECTOR_SEC_BASE + 34)
// SEC 35
#define CYGNUM_HAL_VECTOR_INTR_51                        (CYGNUM_HAL_VECTOR_SEC_BASE + 35)
// SEC 36
#define CYGNUM_HAL_VECTOR_INTR_52                        (CYGNUM_HAL_VECTOR_SEC_BASE + 36)
// SEC 37
#define CYGNUM_HAL_VECTOR_INTR_53                        (CYGNUM_HAL_VECTOR_SEC_BASE + 37)
// SEC 38
#define CYGNUM_HAL_VECTOR_INTR_54                        (CYGNUM_HAL_VECTOR_SEC_BASE + 38)
// SEC 39
#define CYGNUM_HAL_VECTOR_INTR_55                        (CYGNUM_HAL_VECTOR_SEC_BASE + 39)
// SEC 40
#define CYGNUM_HAL_VECTOR_INTR_56                        (CYGNUM_HAL_VECTOR_SEC_BASE + 40)
// SEC 41
#define CYGNUM_HAL_VECTOR_INTR_57                        (CYGNUM_HAL_VECTOR_SEC_BASE + 41)
// SEC 42
#define CYGNUM_HAL_VECTOR_INTR_58                        (CYGNUM_HAL_VECTOR_SEC_BASE + 42)
// SEC 43
#define CYGNUM_HAL_VECTOR_INTR_59                        (CYGNUM_HAL_VECTOR_SEC_BASE + 43)
// SEC 44
#define CYGNUM_HAL_VECTOR_INTR_60                        (CYGNUM_HAL_VECTOR_SEC_BASE + 44)
// SEC 45
#define CYGNUM_HAL_VECTOR_INTR_61                        (CYGNUM_HAL_VECTOR_SEC_BASE + 45)
// SEC 46
#define CYGNUM_HAL_VECTOR_INTR_62                        (CYGNUM_HAL_VECTOR_SEC_BASE + 46)
// SEC 47
#define CYGNUM_HAL_VECTOR_INTR_63                        (CYGNUM_HAL_VECTOR_SEC_BASE + 47)
// SEC 48
#define CYGNUM_HAL_VECTOR_INTR_64                        (CYGNUM_HAL_VECTOR_SEC_BASE + 48)
// SEC 49
#define CYGNUM_HAL_VECTOR_INTR_65                        (CYGNUM_HAL_VECTOR_SEC_BASE + 49)
// SEC 50
#define CYGNUM_HAL_VECTOR_INTR_66                        (CYGNUM_HAL_VECTOR_SEC_BASE + 50)
// SEC 51
#define CYGNUM_HAL_VECTOR_INTR_67                        (CYGNUM_HAL_VECTOR_SEC_BASE + 51)
// SEC 52
#define CYGNUM_HAL_VECTOR_INTR_68                        (CYGNUM_HAL_VECTOR_SEC_BASE + 52)
// SEC 53
#define CYGNUM_HAL_VECTOR_INTR_69                        (CYGNUM_HAL_VECTOR_SEC_BASE + 53)
// SEC 54
#define CYGNUM_HAL_VECTOR_INTR_70                        (CYGNUM_HAL_VECTOR_SEC_BASE + 54)
// SEC 55
#define CYGNUM_HAL_VECTOR_INTR_71                        (CYGNUM_HAL_VECTOR_SEC_BASE + 55)
// SEC 56
#define CYGNUM_HAL_VECTOR_INTR_72                        (CYGNUM_HAL_VECTOR_SEC_BASE + 56)
// SEC 57
#define CYGNUM_HAL_VECTOR_INTR_73                        (CYGNUM_HAL_VECTOR_SEC_BASE + 57)
// SEC 58
#define CYGNUM_HAL_VECTOR_INTR_74                        (CYGNUM_HAL_VECTOR_SEC_BASE + 58)
// SEC 59
#define CYGNUM_HAL_VECTOR_INTR_75                        (CYGNUM_HAL_VECTOR_SEC_BASE + 59)
// SEC 60
#define CYGNUM_HAL_VECTOR_INTR_76                        (CYGNUM_HAL_VECTOR_SEC_BASE + 60)
// SEC 61
#define CYGNUM_HAL_VECTOR_INTR_77                        (CYGNUM_HAL_VECTOR_SEC_BASE + 61)
// SEC 62
#define CYGNUM_HAL_VECTOR_INTR_78                        (CYGNUM_HAL_VECTOR_SEC_BASE + 62)
// SEC 63
#define CYGNUM_HAL_VECTOR_INTR_79                        (CYGNUM_HAL_VECTOR_SEC_BASE + 63)
// SEC 64
#define CYGNUM_HAL_VECTOR_INTR_80                        (CYGNUM_HAL_VECTOR_SEC_BASE + 64)
// SEC 65
#define CYGNUM_HAL_VECTOR_INTR_81                        (CYGNUM_HAL_VECTOR_SEC_BASE + 65)
// SEC 66
#define CYGNUM_HAL_VECTOR_INTR_82                        (CYGNUM_HAL_VECTOR_SEC_BASE + 66)
// SEC 67
#define CYGNUM_HAL_VECTOR_INTR_83                        (CYGNUM_HAL_VECTOR_SEC_BASE + 67)
// SEC 68
#define CYGNUM_HAL_INTERRUPT_84                        (CYGNUM_HAL_VECTOR_SEC_BASE + 68)
    #define CYGNUM_HAL_INTERRUPT_EMAC0                  CYGNUM_HAL_INTERRUPT_84
// SEC 69
#define CYGNUM_HAL_INTERRUPT_85                        (CYGNUM_HAL_VECTOR_SEC_BASE + 69)
// SEC 70
#define CYGNUM_HAL_INTERRUPT_86                        (CYGNUM_HAL_VECTOR_SEC_BASE + 70)
    #define CYGNUM_HAL_INTERRUPT_EMAC1                  CYGNUM_HAL_INTERRUPT_86
// SEC 71
#define CYGNUM_HAL_VECTOR_INTR_87                        (CYGNUM_HAL_VECTOR_SEC_BASE + 71)
// SEC 72
#define CYGNUM_HAL_VECTOR_INTR_88                        (CYGNUM_HAL_VECTOR_SEC_BASE + 72)
// SEC 73
#define CYGNUM_HAL_VECTOR_INTR_89                        (CYGNUM_HAL_VECTOR_SEC_BASE + 73)
// SEC 74
#define CYGNUM_HAL_VECTOR_INTR_90                        (CYGNUM_HAL_VECTOR_SEC_BASE + 74)
// SEC 75
#define CYGNUM_HAL_VECTOR_INTR_91                        (CYGNUM_HAL_VECTOR_SEC_BASE + 75)
// SEC 76
#define CYGNUM_HAL_VECTOR_INTR_92                        (CYGNUM_HAL_VECTOR_SEC_BASE + 76)
// SEC 77
#define CYGNUM_HAL_VECTOR_INTR_93                        (CYGNUM_HAL_VECTOR_SEC_BASE + 77)
// SEC 78
#define CYGNUM_HAL_VECTOR_INTR_94                        (CYGNUM_HAL_VECTOR_SEC_BASE + 78)
// SEC 79
#define CYGNUM_HAL_VECTOR_INTR_95                        (CYGNUM_HAL_VECTOR_SEC_BASE + 79)
// SEC 80
#define CYGNUM_HAL_VECTOR_INTR_96                        (CYGNUM_HAL_VECTOR_SEC_BASE + 80)
// SEC 81
#define CYGNUM_HAL_VECTOR_INTR_97                        (CYGNUM_HAL_VECTOR_SEC_BASE + 81)
// SEC 82
#define CYGNUM_HAL_VECTOR_INTR_98                        (CYGNUM_HAL_VECTOR_SEC_BASE + 82)
// SEC 83
#define CYGNUM_HAL_VECTOR_INTR_99                        (CYGNUM_HAL_VECTOR_SEC_BASE + 83)
// SEC 84
#define CYGNUM_HAL_VECTOR_INTR_100                        (CYGNUM_HAL_VECTOR_SEC_BASE + 84)
// SEC 85
#define CYGNUM_HAL_VECTOR_INTR_101                        (CYGNUM_HAL_VECTOR_SEC_BASE + 85)
// SEC 86
#define CYGNUM_HAL_VECTOR_INTR_102                        (CYGNUM_HAL_VECTOR_SEC_BASE + 86)
// SEC 87
#define CYGNUM_HAL_VECTOR_INTR_103                        (CYGNUM_HAL_VECTOR_SEC_BASE + 87)
// SEC 88
#define CYGNUM_HAL_VECTOR_INTR_104                        (CYGNUM_HAL_VECTOR_SEC_BASE + 88)
// SEC 89
#define CYGNUM_HAL_VECTOR_INTR_105                        (CYGNUM_HAL_VECTOR_SEC_BASE + 89)
// SEC 90
#define CYGNUM_HAL_VECTOR_INTR_106                        (CYGNUM_HAL_VECTOR_SEC_BASE + 90)
// SEC 91
#define CYGNUM_HAL_VECTOR_INTR_107                        (CYGNUM_HAL_VECTOR_SEC_BASE + 91)
// SEC 92
#define CYGNUM_HAL_VECTOR_INTR_108                        (CYGNUM_HAL_VECTOR_SEC_BASE + 92)
// SEC 93
#define CYGNUM_HAL_VECTOR_INTR_109                        (CYGNUM_HAL_VECTOR_SEC_BASE + 93)
// SEC 94
#define CYGNUM_HAL_VECTOR_INTR_110                        (CYGNUM_HAL_VECTOR_SEC_BASE + 94)
// SEC 95
#define CYGNUM_HAL_VECTOR_INTR_111                        (CYGNUM_HAL_VECTOR_SEC_BASE + 95)
// SEC 96
#define CYGNUM_HAL_VECTOR_INTR_112                        (CYGNUM_HAL_VECTOR_SEC_BASE + 96)
// SEC 97
#define CYGNUM_HAL_VECTOR_INTR_113                        (CYGNUM_HAL_VECTOR_SEC_BASE + 97)
// SEC 98
#define CYGNUM_HAL_VECTOR_INTR_114                        (CYGNUM_HAL_VECTOR_SEC_BASE + 98)
// SEC 99
#define CYGNUM_HAL_VECTOR_INTR_115                        (CYGNUM_HAL_VECTOR_SEC_BASE + 99)
// SEC 100
#define CYGNUM_HAL_VECTOR_INTR_116                        (CYGNUM_HAL_VECTOR_SEC_BASE + 100)
// SEC 101
#define CYGNUM_HAL_VECTOR_INTR_117                        (CYGNUM_HAL_VECTOR_SEC_BASE + 101)
// SEC 102
#define CYGNUM_HAL_VECTOR_INTR_118                        (CYGNUM_HAL_VECTOR_SEC_BASE + 102)
// SEC 103
#define CYGNUM_HAL_VECTOR_INTR_119                        (CYGNUM_HAL_VECTOR_SEC_BASE + 103)
// SEC 104
#define CYGNUM_HAL_VECTOR_INTR_120                        (CYGNUM_HAL_VECTOR_SEC_BASE + 104)
// SEC 105
#define CYGNUM_HAL_VECTOR_INTR_121                        (CYGNUM_HAL_VECTOR_SEC_BASE + 105)
// SEC 106
#define CYGNUM_HAL_VECTOR_INTR_122                        (CYGNUM_HAL_VECTOR_SEC_BASE + 106)
// SEC 107
#define CYGNUM_HAL_VECTOR_INTR_123                        (CYGNUM_HAL_VECTOR_SEC_BASE + 107)
// SEC 108
#define CYGNUM_HAL_VECTOR_INTR_124                        (CYGNUM_HAL_VECTOR_SEC_BASE + 108)
// SEC 109
#define CYGNUM_HAL_VECTOR_INTR_125                        (CYGNUM_HAL_VECTOR_SEC_BASE + 109)
// SEC 110
#define CYGNUM_HAL_VECTOR_INTR_126                        (CYGNUM_HAL_VECTOR_SEC_BASE + 110)
// SEC 111
#define CYGNUM_HAL_VECTOR_INTR_127                        (CYGNUM_HAL_VECTOR_SEC_BASE + 111)
// SEC 112
#define CYGNUM_HAL_VECTOR_INTR_128                        (CYGNUM_HAL_VECTOR_SEC_BASE + 112)
// SEC 113
#define CYGNUM_HAL_VECTOR_INTR_129                        (CYGNUM_HAL_VECTOR_SEC_BASE + 113)
// SEC 114
#define CYGNUM_HAL_VECTOR_INTR_130                        (CYGNUM_HAL_VECTOR_SEC_BASE + 114)
// SEC 115
#define CYGNUM_HAL_VECTOR_INTR_131                        (CYGNUM_HAL_VECTOR_SEC_BASE + 115)
// SEC 116
#define CYGNUM_HAL_VECTOR_INTR_132                        (CYGNUM_HAL_VECTOR_SEC_BASE + 116)
// SEC 117
#define CYGNUM_HAL_VECTOR_INTR_133                        (CYGNUM_HAL_VECTOR_SEC_BASE + 117)
// SEC 118
#define CYGNUM_HAL_VECTOR_INTR_134                        (CYGNUM_HAL_VECTOR_SEC_BASE + 118)
// SEC 119
#define CYGNUM_HAL_VECTOR_INTR_135                        (CYGNUM_HAL_VECTOR_SEC_BASE + 119)
// SEC 120
#define CYGNUM_HAL_VECTOR_INTR_136                        (CYGNUM_HAL_VECTOR_SEC_BASE + 120)
// SEC 121
#define CYGNUM_HAL_VECTOR_INTR_137                        (CYGNUM_HAL_VECTOR_SEC_BASE + 121)
// SEC 122
#define CYGNUM_HAL_VECTOR_INTR_138                        (CYGNUM_HAL_VECTOR_SEC_BASE + 122)
// SEC 123
#define CYGNUM_HAL_VECTOR_INTR_139                        (CYGNUM_HAL_VECTOR_SEC_BASE + 123)
// SEC 124
#define CYGNUM_HAL_VECTOR_INTR_140                        (CYGNUM_HAL_VECTOR_SEC_BASE + 124)
// SEC 125
#define CYGNUM_HAL_VECTOR_INTR_141                        (CYGNUM_HAL_VECTOR_SEC_BASE + 125)
// SEC 126
#define CYGNUM_HAL_VECTOR_INTR_142                        (CYGNUM_HAL_VECTOR_SEC_BASE + 126)
// SEC 127
#define CYGNUM_HAL_VECTOR_INTR_143                        (CYGNUM_HAL_VECTOR_SEC_BASE + 127)
// SEC 128
#define CYGNUM_HAL_VECTOR_INTR_144                        (CYGNUM_HAL_VECTOR_SEC_BASE + 128)
// SEC 129
#define CYGNUM_HAL_VECTOR_INTR_145                        (CYGNUM_HAL_VECTOR_SEC_BASE + 129)
// SEC 130
#define CYGNUM_HAL_VECTOR_INTR_146                        (CYGNUM_HAL_VECTOR_SEC_BASE + 130)
// SEC 131
#define CYGNUM_HAL_VECTOR_INTR_147                        (CYGNUM_HAL_VECTOR_SEC_BASE + 131)
// SEC 132
#define CYGNUM_HAL_VECTOR_INTR_148                        (CYGNUM_HAL_VECTOR_SEC_BASE + 132)
// SEC 133
#define CYGNUM_HAL_VECTOR_INTR_149                        (CYGNUM_HAL_VECTOR_SEC_BASE + 133)
// SEC 134
#define CYGNUM_HAL_VECTOR_INTR_150                        (CYGNUM_HAL_VECTOR_SEC_BASE + 134)
// SEC 135
#define CYGNUM_HAL_VECTOR_INTR_151                        (CYGNUM_HAL_VECTOR_SEC_BASE + 135)
// SEC 136
#define CYGNUM_HAL_VECTOR_INTR_152                        (CYGNUM_HAL_VECTOR_SEC_BASE + 136)
// SEC 137
#define CYGNUM_HAL_VECTOR_INTR_153                        (CYGNUM_HAL_VECTOR_SEC_BASE + 137)
// SEC 138
#define CYGNUM_HAL_VECTOR_INTR_154                        (CYGNUM_HAL_VECTOR_SEC_BASE + 138)
// SEC 139
#define CYGNUM_HAL_VECTOR_INTR_155                        (CYGNUM_HAL_VECTOR_SEC_BASE + 139)


// define ISR table entry pointers
// SEC 0
#define CYGNUM_HAL_INTERRUPT_16                        (CYGNUM_HAL_VECTOR_SEC_BASE + 0)
// SEC 1
#define CYGNUM_HAL_INTERRUPT_17                        (CYGNUM_HAL_VECTOR_SEC_BASE + 1)
// SEC 2
#define CYGNUM_HAL_INTERRUPT_18                        (CYGNUM_HAL_VECTOR_SEC_BASE + 2)
// SEC 3
#define CYGNUM_HAL_INTERRUPT_19                        (CYGNUM_HAL_VECTOR_SEC_BASE + 3)
// SEC 4
#define CYGNUM_HAL_INTERRUPT_20                        (CYGNUM_HAL_VECTOR_SEC_BASE + 4)
// SEC 5
#define CYGNUM_HAL_INTERRUPT_21                        (CYGNUM_HAL_VECTOR_SEC_BASE + 5)
// SEC 6
#define CYGNUM_HAL_INTERRUPT_22                        (CYGNUM_HAL_VECTOR_SEC_BASE + 6)
// SEC 7
#define CYGNUM_HAL_INTERRUPT_23                        (CYGNUM_HAL_VECTOR_SEC_BASE + 7)
// SEC 8
#define CYGNUM_HAL_INTERRUPT_24                        (CYGNUM_HAL_VECTOR_SEC_BASE + 8)
// SEC 9
#define CYGNUM_HAL_INTERRUPT_25                        (CYGNUM_HAL_VECTOR_SEC_BASE + 9)
// SEC 10
#define CYGNUM_HAL_INTERRUPT_26                        (CYGNUM_HAL_VECTOR_SEC_BASE + 10)
// SEC 11
#define CYGNUM_HAL_INTERRUPT_27                        (CYGNUM_HAL_VECTOR_SEC_BASE + 11)
// SEC 12
#define CYGNUM_HAL_INTERRUPT_28                        (CYGNUM_HAL_VECTOR_SEC_BASE + 12)
// SEC 13
#define CYGNUM_HAL_INTERRUPT_29                        (CYGNUM_HAL_VECTOR_SEC_BASE + 13)
// SEC 14
#define CYGNUM_HAL_INTERRUPT_30                        (CYGNUM_HAL_VECTOR_SEC_BASE + 14)
// SEC 15
#define CYGNUM_HAL_INTERRUPT_31                        (CYGNUM_HAL_VECTOR_SEC_BASE + 15)
// SEC 16
#define CYGNUM_HAL_INTERRUPT_32                        (CYGNUM_HAL_VECTOR_SEC_BASE + 16)
// SEC 17
#define CYGNUM_HAL_INTERRUPT_33                        (CYGNUM_HAL_VECTOR_SEC_BASE + 17)
// SEC 18
#define CYGNUM_HAL_INTERRUPT_34                        (CYGNUM_HAL_VECTOR_SEC_BASE + 18)
// SEC 19
#define CYGNUM_HAL_INTERRUPT_35                        (CYGNUM_HAL_VECTOR_SEC_BASE + 19)
// SEC 20
#define CYGNUM_HAL_INTERRUPT_36                        (CYGNUM_HAL_VECTOR_SEC_BASE + 20)
// SEC 21
#define CYGNUM_HAL_INTERRUPT_37                        (CYGNUM_HAL_VECTOR_SEC_BASE + 21)
#define INTR_PINT0_BLOCK                      			CYGNUM_HAL_INTERRUPT_37           /* Pin Interrupt Block */
// SEC 22
#define CYGNUM_HAL_INTERRUPT_38                        (CYGNUM_HAL_VECTOR_SEC_BASE + 22)
#define INTR_PINT1_BLOCK                      			CYGNUM_HAL_INTERRUPT_38           /* Pin Interrupt Block */
// SEC 23
#define CYGNUM_HAL_INTERRUPT_39                        (CYGNUM_HAL_VECTOR_SEC_BASE + 23)
#define INTR_PINT2_BLOCK                      			CYGNUM_HAL_INTERRUPT_39           /* Pin Interrupt Block */
// SEC 24
#define CYGNUM_HAL_INTERRUPT_40                        (CYGNUM_HAL_VECTOR_SEC_BASE + 24)
#define INTR_PINT3_BLOCK                      			CYGNUM_HAL_INTERRUPT_40           /* Pin Interrupt Block */
// SEC 25
#define CYGNUM_HAL_INTERRUPT_41                        (CYGNUM_HAL_VECTOR_SEC_BASE + 25)
#define INTR_PINT4_BLOCK                      			CYGNUM_HAL_INTERRUPT_41           /* Pin Interrupt Block */
// SEC 26
#define CYGNUM_HAL_INTERRUPT_42                        (CYGNUM_HAL_VECTOR_SEC_BASE + 26)
#define INTR_PINT5_BLOCK                      			CYGNUM_HAL_INTERRUPT_42           /* Pin Interrupt Block */
// SEC 27
#define CYGNUM_HAL_INTERRUPT_43                        (CYGNUM_HAL_VECTOR_SEC_BASE + 27)
// SEC 28
#define CYGNUM_HAL_INTERRUPT_44                        (CYGNUM_HAL_VECTOR_SEC_BASE + 28)
// SEC 29
#define CYGNUM_HAL_INTERRUPT_45                        (CYGNUM_HAL_VECTOR_SEC_BASE + 29)
// SEC 30
#define CYGNUM_HAL_INTERRUPT_46                        (CYGNUM_HAL_VECTOR_SEC_BASE + 30)
// SEC 31
#define CYGNUM_HAL_INTERRUPT_47                        (CYGNUM_HAL_VECTOR_SEC_BASE + 31)
// SEC 32
#define CYGNUM_HAL_INTERRUPT_48                        (CYGNUM_HAL_VECTOR_SEC_BASE + 32)
#define CYGNUM_HAL_INTERRUPT_TWI0                      CYGNUM_HAL_INTERRUPT_48
// SEC 33
#define CYGNUM_HAL_INTERRUPT_49                        (CYGNUM_HAL_VECTOR_SEC_BASE + 33)
#define CYGNUM_HAL_INTERRUPT_TWI1                      CYGNUM_HAL_INTERRUPT_49
// SEC 34
#define CYGNUM_HAL_INTERRUPT_50                        (CYGNUM_HAL_VECTOR_SEC_BASE + 34)
// SEC 35
#define CYGNUM_HAL_INTERRUPT_51                        (CYGNUM_HAL_VECTOR_SEC_BASE + 35)
// SEC 36
#define CYGNUM_HAL_INTERRUPT_52                        (CYGNUM_HAL_VECTOR_SEC_BASE + 36)
// SEC 37
#define CYGNUM_HAL_INTERRUPT_53                        (CYGNUM_HAL_VECTOR_SEC_BASE + 37)
// SEC 38
#define CYGNUM_HAL_INTERRUPT_54                        (CYGNUM_HAL_VECTOR_SEC_BASE + 38)
// SEC 39
#define CYGNUM_HAL_INTERRUPT_55                        (CYGNUM_HAL_VECTOR_SEC_BASE + 39)
// SEC 40
#define CYGNUM_HAL_INTERRUPT_56                        (CYGNUM_HAL_VECTOR_SEC_BASE + 40)
// SEC 41
#define CYGNUM_HAL_INTERRUPT_57                        (CYGNUM_HAL_VECTOR_SEC_BASE + 41)
// SEC 42
#define CYGNUM_HAL_INTERRUPT_58                        (CYGNUM_HAL_VECTOR_SEC_BASE + 42)
// SEC 43
#define CYGNUM_HAL_INTERRUPT_59                        (CYGNUM_HAL_VECTOR_SEC_BASE + 43)
	#define INTR_SPORT0_A_DMA								CYGNUM_HAL_INTERRUPT_59
// SEC 44
#define CYGNUM_HAL_INTERRUPT_60                        (CYGNUM_HAL_VECTOR_SEC_BASE + 44)
// SEC 45
#define CYGNUM_HAL_INTERRUPT_61                        (CYGNUM_HAL_VECTOR_SEC_BASE + 45)
	#define INTR_SPORT0_B_DMA                           	CYGNUM_HAL_INTERRUPT_61
// SEC 46
#define CYGNUM_HAL_INTERRUPT_62                        (CYGNUM_HAL_VECTOR_SEC_BASE + 46)
// SEC 47
#define CYGNUM_HAL_INTERRUPT_63                        (CYGNUM_HAL_VECTOR_SEC_BASE + 47)
	#define INTR_SPORT1_A_DMA                        		CYGNUM_HAL_INTERRUPT_63
// SEC 48
#define CYGNUM_HAL_INTERRUPT_64                        (CYGNUM_HAL_VECTOR_SEC_BASE + 48)
// SEC 49
#define CYGNUM_HAL_INTERRUPT_65                        (CYGNUM_HAL_VECTOR_SEC_BASE + 49)
	#define INTR_SPORT1_B_DMA                        		CYGNUM_HAL_INTERRUPT_65
// SEC 50
#define CYGNUM_HAL_INTERRUPT_66                        (CYGNUM_HAL_VECTOR_SEC_BASE + 50)
// SEC 51
#define CYGNUM_HAL_INTERRUPT_67                        (CYGNUM_HAL_VECTOR_SEC_BASE + 51)
	#define INTR_SPORT2_A_DMA                        		CYGNUM_HAL_INTERRUPT_67
// SEC 52
#define CYGNUM_HAL_INTERRUPT_68                        (CYGNUM_HAL_VECTOR_SEC_BASE + 52)
// SEC 53
#define CYGNUM_HAL_INTERRUPT_69                        (CYGNUM_HAL_VECTOR_SEC_BASE + 53)
	#define INTR_SPORT2_B_DMA                        		CYGNUM_HAL_INTERRUPT_69
// SEC 54
#define CYGNUM_HAL_INTERRUPT_70                        (CYGNUM_HAL_VECTOR_SEC_BASE + 54)
// SEC 55
#define CYGNUM_HAL_INTERRUPT_71                        (CYGNUM_HAL_VECTOR_SEC_BASE + 55)
	#define INTR_SPI0_TXDMA                        			CYGNUM_HAL_INTERRUPT_71
// SEC 56
#define CYGNUM_HAL_INTERRUPT_72                        (CYGNUM_HAL_VECTOR_SEC_BASE + 56)
	#define INTR_SPI0_RXDMA                        			CYGNUM_HAL_INTERRUPT_72
// SEC 57
#define CYGNUM_HAL_INTERRUPT_73                        (CYGNUM_HAL_VECTOR_SEC_BASE + 57)
	#define CYGNUM_HAL_INTERRUPT_SPI0				    CYGNUM_HAL_INTERRUPT_73
// SEC 58
#define CYGNUM_HAL_INTERRUPT_74                        (CYGNUM_HAL_VECTOR_SEC_BASE + 58)
	#define INTR_SPI1_TXDMA                        			CYGNUM_HAL_INTERRUPT_74
// SEC 59
#define CYGNUM_HAL_INTERRUPT_75                        (CYGNUM_HAL_VECTOR_SEC_BASE + 59)
	#define INTR_SPI1_RXDMA                        			CYGNUM_HAL_INTERRUPT_75
// SEC 60
#define CYGNUM_HAL_INTERRUPT_76                        (CYGNUM_HAL_VECTOR_SEC_BASE + 60)
	#define CYGNUM_HAL_INTERRUPT_SPI1				    CYGNUM_HAL_INTERRUPT_76
// SEC 61
#define CYGNUM_HAL_INTERRUPT_77                        (CYGNUM_HAL_VECTOR_SEC_BASE + 61)
	#define INTR_RSI0_DMA                        			CYGNUM_HAL_INTERRUPT_77
// SEC 62
#define CYGNUM_HAL_INTERRUPT_78                        (CYGNUM_HAL_VECTOR_SEC_BASE + 62)
// SEC 63
#define CYGNUM_HAL_INTERRUPT_79                        (CYGNUM_HAL_VECTOR_SEC_BASE + 63)
// SEC 64
#define CYGNUM_HAL_INTERRUPT_80                        (CYGNUM_HAL_VECTOR_SEC_BASE + 64)
	#define INTR_SDU0_DMA                        			CYGNUM_HAL_INTERRUPT_80
// SEC 65
#define CYGNUM_HAL_INTERRUPT_81                        (CYGNUM_HAL_VECTOR_SEC_BASE + 65)
// SEC 66
#define CYGNUM_HAL_INTERRUPT_82                        (CYGNUM_HAL_VECTOR_SEC_BASE + 66)
// SEC 67
#define CYGNUM_HAL_INTERRUPT_83                        (CYGNUM_HAL_VECTOR_SEC_BASE + 67)
// SEC 68
#define CYGNUM_HAL_INTERRUPT_84                        (CYGNUM_HAL_VECTOR_SEC_BASE + 68)
// SEC 69
#define CYGNUM_HAL_INTERRUPT_85                        (CYGNUM_HAL_VECTOR_SEC_BASE + 69)
// SEC 70
#define CYGNUM_HAL_INTERRUPT_86                        (CYGNUM_HAL_VECTOR_SEC_BASE + 70)
// SEC 71
#define CYGNUM_HAL_INTERRUPT_87                        (CYGNUM_HAL_VECTOR_SEC_BASE + 71)
// SEC 72
#define CYGNUM_HAL_INTERRUPT_88                        (CYGNUM_HAL_VECTOR_SEC_BASE + 72)
	#define INTR_LP0_DMA                        			CYGNUM_HAL_INTERRUPT_88
// SEC 73
#define CYGNUM_HAL_INTERRUPT_89                        (CYGNUM_HAL_VECTOR_SEC_BASE + 73)
// SEC 74
#define CYGNUM_HAL_INTERRUPT_90                        (CYGNUM_HAL_VECTOR_SEC_BASE + 74)
	#define INTR_LP1_DMA                        			CYGNUM_HAL_INTERRUPT_90
// SEC 75
#define CYGNUM_HAL_INTERRUPT_91                        (CYGNUM_HAL_VECTOR_SEC_BASE + 75)
// SEC 76
#define CYGNUM_HAL_INTERRUPT_92                        (CYGNUM_HAL_VECTOR_SEC_BASE + 76)
	#define INTR_LP2_DMA                        			CYGNUM_HAL_INTERRUPT_92
// SEC 77
#define CYGNUM_HAL_INTERRUPT_93                        (CYGNUM_HAL_VECTOR_SEC_BASE + 77)
// SEC 78
#define CYGNUM_HAL_INTERRUPT_94                        (CYGNUM_HAL_VECTOR_SEC_BASE + 78)
	#define INTR_LP3_DMA                        			CYGNUM_HAL_INTERRUPT_94
// SEC 79
#define CYGNUM_HAL_INTERRUPT_95                        (CYGNUM_HAL_VECTOR_SEC_BASE + 79)
// SEC 80
#define CYGNUM_HAL_INTERRUPT_96                        (CYGNUM_HAL_VECTOR_SEC_BASE + 80)
	#define INTR_UART0_TXDMA                        		CYGNUM_HAL_INTERRUPT_96
// SEC 81
#define CYGNUM_HAL_INTERRUPT_97                        (CYGNUM_HAL_VECTOR_SEC_BASE + 81)
	#define INTR_UART0_RXDMA                        		CYGNUM_HAL_INTERRUPT_97
// SEC 82
#define CYGNUM_HAL_INTERRUPT_98                        (CYGNUM_HAL_VECTOR_SEC_BASE + 82)
// SEC 83
#define CYGNUM_HAL_INTERRUPT_99                        (CYGNUM_HAL_VECTOR_SEC_BASE + 83)
	#define INTR_UART1_TXDMA                        		CYGNUM_HAL_INTERRUPT_99
// SEC 84
#define CYGNUM_HAL_INTERRUPT_100                        (CYGNUM_HAL_VECTOR_SEC_BASE + 84)
	#define INTR_UART1_RXDMA                        		CYGNUM_HAL_INTERRUPT_100
// SEC 85
#define CYGNUM_HAL_INTERRUPT_101                        (CYGNUM_HAL_VECTOR_SEC_BASE + 85)
// SEC 86
#define CYGNUM_HAL_INTERRUPT_102                        (CYGNUM_HAL_VECTOR_SEC_BASE + 86)
	#define INTR_MDMA0_SRC                        			CYGNUM_HAL_INTERRUPT_102
// SEC 87
#define CYGNUM_HAL_INTERRUPT_103                        (CYGNUM_HAL_VECTOR_SEC_BASE + 87)
	#define INTR_MDMA0_DST                        			CYGNUM_HAL_INTERRUPT_103
// SEC 88
#define CYGNUM_HAL_INTERRUPT_104                        (CYGNUM_HAL_VECTOR_SEC_BASE + 88)
// SEC 89
#define CYGNUM_HAL_INTERRUPT_105                        (CYGNUM_HAL_VECTOR_SEC_BASE + 89)
// SEC 90
#define CYGNUM_HAL_INTERRUPT_106                        (CYGNUM_HAL_VECTOR_SEC_BASE + 90)
	#define INTR_MDMA1_SRC                        			CYGNUM_HAL_INTERRUPT_106
// SEC 91
#define CYGNUM_HAL_INTERRUPT_107                        (CYGNUM_HAL_VECTOR_SEC_BASE + 91)
	#define INTR_MDMA1_DST                        			CYGNUM_HAL_INTERRUPT_107
// SEC 92
#define CYGNUM_HAL_INTERRUPT_108                        (CYGNUM_HAL_VECTOR_SEC_BASE + 92)
// SEC 93
#define CYGNUM_HAL_INTERRUPT_109                        (CYGNUM_HAL_VECTOR_SEC_BASE + 93)
// SEC 94
#define CYGNUM_HAL_INTERRUPT_110                        (CYGNUM_HAL_VECTOR_SEC_BASE + 94)
	#define INTR_MDMA2_SRC                        			CYGNUM_HAL_INTERRUPT_110
// SEC 95
#define CYGNUM_HAL_INTERRUPT_111                        (CYGNUM_HAL_VECTOR_SEC_BASE + 95)
	#define INTR_MDMA2_DST                        			CYGNUM_HAL_INTERRUPT_111
// SEC 96
#define CYGNUM_HAL_INTERRUPT_112                        (CYGNUM_HAL_VECTOR_SEC_BASE + 96)
	#define INTR_MDMA3_SRC                        			CYGNUM_HAL_INTERRUPT_112
// SEC 97
#define CYGNUM_HAL_INTERRUPT_113                        (CYGNUM_HAL_VECTOR_SEC_BASE + 97)
	#define INTR_MDMA3_DST                        			CYGNUM_HAL_INTERRUPT_113
// SEC 98
#define CYGNUM_HAL_INTERRUPT_114                        (CYGNUM_HAL_VECTOR_SEC_BASE + 98)
	#define INTR_EPPI0_CH0_DMA								CYGNUM_HAL_INTERRUPT_114
// SEC 99
#define CYGNUM_HAL_INTERRUPT_115                        (CYGNUM_HAL_VECTOR_SEC_BASE + 99)
	#define INTR_EPPI0_CH1_DMA								CYGNUM_HAL_INTERRUPT_115
// SEC 100
#define CYGNUM_HAL_INTERRUPT_116                        (CYGNUM_HAL_VECTOR_SEC_BASE + 100)
	#define CYGNUM_HAL_INTERRUPT_EPPI0						CYGNUM_HAL_INTERRUPT_116
// SEC 101
#define CYGNUM_HAL_INTERRUPT_117                        (CYGNUM_HAL_VECTOR_SEC_BASE + 101)
	#define INTR_EPPI2_CH0_DMA								CYGNUM_HAL_INTERRUPT_117
// SEC 102
#define CYGNUM_HAL_INTERRUPT_118                        (CYGNUM_HAL_VECTOR_SEC_BASE + 102)
	#define INTR_EPPI2_CH1_DMA								CYGNUM_HAL_INTERRUPT_118
// SEC 103
#define CYGNUM_HAL_INTERRUPT_119                        (CYGNUM_HAL_VECTOR_SEC_BASE + 103)
	#define CYGNUM_HAL_INTERRUPT_EPPI2						CYGNUM_HAL_INTERRUPT_119
// SEC 104
#define CYGNUM_HAL_INTERRUPT_120                        (CYGNUM_HAL_VECTOR_SEC_BASE + 104)
	#define INTR_EPPI1_CH0_DMA								CYGNUM_HAL_INTERRUPT_120
// SEC 105
#define CYGNUM_HAL_INTERRUPT_121                        (CYGNUM_HAL_VECTOR_SEC_BASE + 105)
	#define INTR_EPPI1_CH1_DMA								CYGNUM_HAL_INTERRUPT_121
// SEC 106
#define CYGNUM_HAL_INTERRUPT_122                        (CYGNUM_HAL_VECTOR_SEC_BASE + 106)
	#define CYGNUM_HAL_INTERRUPT_EPPI1						CYGNUM_HAL_INTERRUPT_122
// SEC 107
#define CYGNUM_HAL_INTERRUPT_123                        (CYGNUM_HAL_VECTOR_SEC_BASE + 107)
	#define INTR_PIXC0_CH0_DMA                        		CYGNUM_HAL_INTERRUPT_123
// SEC 108
#define CYGNUM_HAL_INTERRUPT_124                        (CYGNUM_HAL_VECTOR_SEC_BASE + 108)
	#define INTR_PIXC0_CH1_DMA                        		CYGNUM_HAL_INTERRUPT_124
// SEC 109
#define CYGNUM_HAL_INTERRUPT_125                        (CYGNUM_HAL_VECTOR_SEC_BASE + 109)
	#define INTR_PIXC0_CH2_DMA                        		CYGNUM_HAL_INTERRUPT_125
// SEC 110
#define CYGNUM_HAL_INTERRUPT_126                        (CYGNUM_HAL_VECTOR_SEC_BASE + 110)
// SEC 111
#define CYGNUM_HAL_INTERRUPT_127                        (CYGNUM_HAL_VECTOR_SEC_BASE + 111)
	#define INTR_PVP0_CPDOB_DMA                        		CYGNUM_HAL_INTERRUPT_127
// SEC 112
#define CYGNUM_HAL_INTERRUPT_128                        (CYGNUM_HAL_VECTOR_SEC_BASE + 112)
	#define INTR_PVP0_CPDOC_DMA                        		CYGNUM_HAL_INTERRUPT_128
// SEC 113
#define CYGNUM_HAL_INTERRUPT_129                        (CYGNUM_HAL_VECTOR_SEC_BASE + 113)
	#define INTR_PVP0_CPSTAT_DMA                        	CYGNUM_HAL_INTERRUPT_129
// SEC 114
#define CYGNUM_HAL_INTERRUPT_130                        (CYGNUM_HAL_VECTOR_SEC_BASE + 114)
	#define INTR_PVP0_CPCI_DMA                        		CYGNUM_HAL_INTERRUPT_130
// SEC 115
#define CYGNUM_HAL_INTERRUPT_131                        (CYGNUM_HAL_VECTOR_SEC_BASE + 115)
// SEC 116
#define CYGNUM_HAL_INTERRUPT_132                        (CYGNUM_HAL_VECTOR_SEC_BASE + 116)
	#define INTR_PVP0_MPDO_DMA                        		CYGNUM_HAL_INTERRUPT_132
// SEC 117
#define CYGNUM_HAL_INTERRUPT_133                        (CYGNUM_HAL_VECTOR_SEC_BASE + 117)
	#define INTR_PVP0_MPDI_DMA                        		CYGNUM_HAL_INTERRUPT_133
// SEC 118
#define CYGNUM_HAL_INTERRUPT_134                        (CYGNUM_HAL_VECTOR_SEC_BASE + 118)
	#define INTR_PVP0_MPSTAT_DMA                        	CYGNUM_HAL_INTERRUPT_134
// SEC 119
#define CYGNUM_HAL_INTERRUPT_135                        (CYGNUM_HAL_VECTOR_SEC_BASE + 119)
	#define INTR_PVP0_MPCI_DMA                        		CYGNUM_HAL_INTERRUPT_135
// SEC 120
#define CYGNUM_HAL_INTERRUPT_136                        (CYGNUM_HAL_VECTOR_SEC_BASE + 120)
	#define INTR_PVP0_CPDOA_DMA                        		CYGNUM_HAL_INTERRUPT_136
// SEC 121
#define CYGNUM_HAL_INTERRUPT_137                        (CYGNUM_HAL_VECTOR_SEC_BASE + 121)
// SEC 122
#define CYGNUM_HAL_INTERRUPT_138                        (CYGNUM_HAL_VECTOR_SEC_BASE + 122)
// SEC 123
#define CYGNUM_HAL_INTERRUPT_139                        (CYGNUM_HAL_VECTOR_SEC_BASE + 123)
// SEC 124
#define CYGNUM_HAL_INTERRUPT_140                        (CYGNUM_HAL_VECTOR_SEC_BASE + 124)
// SEC 125
#define CYGNUM_HAL_INTERRUPT_141                        (CYGNUM_HAL_VECTOR_SEC_BASE + 125)
// SEC 126
#define CYGNUM_HAL_INTERRUPT_142                        (CYGNUM_HAL_VECTOR_SEC_BASE + 126)
// SEC 127
#define CYGNUM_HAL_INTERRUPT_143                        (CYGNUM_HAL_VECTOR_SEC_BASE + 127)
// SEC 128
#define CYGNUM_HAL_INTERRUPT_144                        (CYGNUM_HAL_VECTOR_SEC_BASE + 128)
	#define INTR_DMAC_ERR                        		 	CYGNUM_HAL_VECTOR_INTR_144           /* DMA Controller Error */
// SEC 129
#define CYGNUM_HAL_INTERRUPT_145                        (CYGNUM_HAL_VECTOR_SEC_BASE + 129)
// SEC 130
#define CYGNUM_HAL_INTERRUPT_146                        (CYGNUM_HAL_VECTOR_SEC_BASE + 130)
// SEC 131
#define CYGNUM_HAL_INTERRUPT_147                        (CYGNUM_HAL_VECTOR_SEC_BASE + 131)
// SEC 132
#define CYGNUM_HAL_INTERRUPT_148                        (CYGNUM_HAL_VECTOR_SEC_BASE + 132)
// SEC 133
#define CYGNUM_HAL_INTERRUPT_149                        (CYGNUM_HAL_VECTOR_SEC_BASE + 133)
// SEC 134
#define CYGNUM_HAL_INTERRUPT_150                        (CYGNUM_HAL_VECTOR_SEC_BASE + 134)
// SEC 135
#define CYGNUM_HAL_INTERRUPT_151                        (CYGNUM_HAL_VECTOR_SEC_BASE + 135)
// SEC 136
#define CYGNUM_HAL_INTERRUPT_152                        (CYGNUM_HAL_VECTOR_SEC_BASE + 136)
// SEC 137
#define CYGNUM_HAL_INTERRUPT_153                        (CYGNUM_HAL_VECTOR_SEC_BASE + 137)
// SEC 138
#define CYGNUM_HAL_INTERRUPT_154                        (CYGNUM_HAL_VECTOR_SEC_BASE + 138)
// SEC 139
#define CYGNUM_HAL_INTERRUPT_155                        (CYGNUM_HAL_VECTOR_SEC_BASE + 139)

#define CYGNUM_HAL_BFIN_EXCEPTIONS_BASE                 CYGNUM_HAL_INTERRUPT_155 + 1

// Min/Max ISR numbers and how many there are
#define CYGNUM_HAL_ISR_MIN                     CYGNUM_HAL_INTERRUPT_5
#define CYGNUM_HAL_ISR_MAX                     CYGNUM_HAL_INTERRUPT_155
#define CYGNUM_HAL_ISR_COUNT                   CYGNUM_HAL_ISR_MAX + 1

#endif // CYGONCE_HAL_VAR_INTR_INCL_H