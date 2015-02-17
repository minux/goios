package arm64

var Anames = []string{
	"XXX",
	"ADC",
	"ADCS",
	"ADCSW",
	"ADCW",
	"ADD",
	"ADDS",
	"ADDSW",
	"ADDW",
	"ADR",
	"ADRP",
	"AND",
	"ANDS",
	"ANDSW",
	"ANDW",
	"ASR",
	"ASRW",
	"AT",
	"B",
	"BFI",
	"BFIW",
	"BFM",
	"BFMW",
	"BFXIL",
	"BFXILW",
	"BIC",
	"BICS",
	"BICSW",
	"BICW",
	"BL",
	"BRK",
	"CBNZ",
	"CBNZW",
	"CBZ",
	"CBZW",
	"CCMN",
	"CCMNW",
	"CCMP",
	"CCMPW",
	"CINC",
	"CINCW",
	"CINV",
	"CINVW",
	"CLREX",
	"CLS",
	"CLSW",
	"CLZ",
	"CLZW",
	"CMN",
	"CMNW",
	"CMP",
	"CMPW",
	"CNEG",
	"CNEGW",
	"CRC32B",
	"CRC32CB",
	"CRC32CH",
	"CRC32CW",
	"CRC32CX",
	"CRC32H",
	"CRC32W",
	"CRC32X",
	"CSEL",
	"CSELW",
	"CSET",
	"CSETM",
	"CSETMW",
	"CSETW",
	"CSINC",
	"CSINCW",
	"CSINV",
	"CSINVW",
	"CSNEG",
	"CSNEGW",
	"DC",
	"DCPS1",
	"DCPS2",
	"DCPS3",
	"DMB",
	"DRPS",
	"DSB",
	"EON",
	"EONW",
	"EOR",
	"EORW",
	"ERET",
	"EXTR",
	"EXTRW",
	"HINT",
	"HLT",
	"HVC",
	"IC",
	"ISB",
	"LDAR",
	"LDARB",
	"LDARH",
	"LDARW",
	"LDAXP",
	"LDAXPW",
	"LDAXR",
	"LDAXRB",
	"LDAXRH",
	"LDAXRW",
	"LDP",
	"LDXR",
	"LDXRB",
	"LDXRH",
	"LDXRW",
	"LDXP",
	"LDXPW",
	"LSL",
	"LSLW",
	"LSR",
	"LSRW",
	"MADD",
	"MADDW",
	"MNEG",
	"MNEGW",
	"MOVK",
	"MOVKW",
	"MOVN",
	"MOVNW",
	"MOVZ",
	"MOVZW",
	"MRS",
	"MSR",
	"MSUB",
	"MSUBW",
	"MUL",
	"MULW",
	"MVN",
	"MVNW",
	"NEG",
	"NEGS",
	"NEGSW",
	"NEGW",
	"NGC",
	"NGCS",
	"NGCSW",
	"NGCW",
	"NOP",
	"ORN",
	"ORNW",
	"ORR",
	"ORRW",
	"PRFM",
	"PRFUM",
	"RBIT",
	"RBITW",
	"REM",
	"REMW",
	"RET",
	"REV",
	"REV16",
	"REV16W",
	"REV32",
	"REVW",
	"ROR",
	"RORW",
	"SBC",
	"SBCS",
	"SBCSW",
	"SBCW",
	"SBFIZ",
	"SBFIZW",
	"SBFM",
	"SBFMW",
	"SBFX",
	"SBFXW",
	"SDIV",
	"SDIVW",
	"SEV",
	"SEVL",
	"SMADDL",
	"SMC",
	"SMNEGL",
	"SMSUBL",
	"SMULH",
	"SMULL",
	"STXR",
	"STXRB",
	"STXRH",
	"STXP",
	"STXPW",
	"STXRW",
	"STLP",
	"STLPW",
	"STLR",
	"STLRB",
	"STLRH",
	"STLRW",
	"STLXP",
	"STLXPW",
	"STLXR",
	"STLXRB",
	"STLXRH",
	"STLXRW",
	"STP",
	"SUB",
	"SUBS",
	"SUBSW",
	"SUBW",
	"SVC",
	"SXTB",
	"SXTBW",
	"SXTH",
	"SXTHW",
	"SXTW",
	"SYS",
	"SYSL",
	"TBNZ",
	"TBZ",
	"TLBI",
	"TST",
	"TSTW",
	"UBFIZ",
	"UBFIZW",
	"UBFM",
	"UBFMW",
	"UBFX",
	"UBFXW",
	"UDIV",
	"UDIVW",
	"UMADDL",
	"UMNEGL",
	"UMSUBL",
	"UMULH",
	"UMULL",
	"UREM",
	"UREMW",
	"UXTB",
	"UXTH",
	"UXTW",
	"UXTBW",
	"UXTHW",
	"WFE",
	"WFI",
	"YIELD",
	"MOVB",
	"MOVBU",
	"MOVH",
	"MOVHU",
	"MOVW",
	"MOVWU",
	"MOV",
	"MOVNP",
	"MOVNPW",
	"MOVP",
	"MOVPD",
	"MOVPQ",
	"MOVPS",
	"MOVPSW",
	"MOVPW",
	"BEQ",
	"BNE",
	"BCS",
	"BHS",
	"BCC",
	"BLO",
	"BMI",
	"BPL",
	"BVS",
	"BVC",
	"BHI",
	"BLS",
	"BGE",
	"BLT",
	"BGT",
	"BLE",
	"FABSD",
	"FABSS",
	"FADDD",
	"FADDS",
	"FCCMPD",
	"FCCMPED",
	"FCCMPS",
	"FCCMPES",
	"FCMPD",
	"FCMPED",
	"FCMPES",
	"FCMPS",
	"FCVTSD",
	"FCVTDS",
	"FCVTZSD",
	"FCVTZSDW",
	"FCVTZSS",
	"FCVTZSSW",
	"FCVTZUD",
	"FCVTZUDW",
	"FCVTZUS",
	"FCVTZUSW",
	"FDIVD",
	"FDIVS",
	"FMOVD",
	"FMOVS",
	"FMULD",
	"FMULS",
	"FNEGD",
	"FNEGS",
	"FSQRTD",
	"FSQRTS",
	"FSUBD",
	"FSUBS",
	"SCVTFD",
	"SCVTFS",
	"SCVTFWD",
	"SCVTFWS",
	"UCVTFD",
	"UCVTFS",
	"UCVTFWD",
	"UCVTFWS",
	"TEXT",
	"DATA",
	"GLOBL",
	"HISTORY",
	"NAME",
	"WORD",
	"DYNT",
	"INIT",
	"BCASE",
	"CASE",
	"DWORD",
	"SIGNAME",
	"GOK",
	"RETURN",
	"END",
	"FCSELS",
	"FCSELD",
	"FMAXS",
	"FMINS",
	"FMAXD",
	"FMIND",
	"FMAXNMS",
	"FMAXNMD",
	"FNMULS",
	"FNMULD",
	"FRINTNS",
	"FRINTND",
	"FRINTPS",
	"FRINTPD",
	"FRINTMS",
	"FRINTMD",
	"FRINTZS",
	"FRINTZD",
	"FRINTAS",
	"FRINTAD",
	"FRINTXS",
	"FRINTXD",
	"FRINTIS",
	"FRINTID",
	"FMADDS",
	"FMADDD",
	"FMSUBS",
	"FMSUBD",
	"FNMADDS",
	"FNMADDD",
	"FNMSUBS",
	"FNMSUBD",
	"FMINNMS",
	"FMINNMD",
	"FCVTDH",
	"FCVTHS",
	"FCVTHD",
	"FCVTSH",
	"AESD",
	"AESE",
	"AESIMC",
	"AESMC",
	"SHA1C",
	"SHA1H",
	"SHA1M",
	"SHA1P",
	"SHA1SU0",
	"SHA1SU1",
	"SHA256H",
	"SHA256H2",
	"SHA256SU0",
	"SHA256SU1",
	"UNDEF",
	"USEFIELD",
	"TYPE",
	"FUNCDATA",
	"PCDATA",
	"CHECKNIL",
	"VARDEF",
	"VARKILL",
	"DUFFCOPY",
	"DUFFZERO",
	"LAST",
}

var cnames7 = []string{
	"NONE",
	"REG",
	"PAIR",
	"RSP",
	"SHIFT",
	"EXTREG",
	"FREG",
	"SPR",
	"COND",
	"ZCON",
	"ADDCON0",
	"ADDCON",
	"MOVCON",
	"BITCON",
	"ABCON",
	"MBCON",
	"LCON",
	"FCON",
	"VCON",
	"VCONADDR",
	"AACON",
	"LACON",
	"AECON",
	"SBRA",
	"LBRA",
	"NPAUTO",
	"NSAUTO",
	"PSAUTO",
	"PPAUTO",
	"UAUTO4K",
	"UAUTO8K",
	"UAUTO16K",
	"UAUTO32K",
	"UAUTO64K",
	"LAUTO",
	"SEXT1",
	"SEXT2",
	"SEXT4",
	"SEXT8",
	"SEXT16",
	"LEXT",
	"NPOREG",
	"NSOREG",
	"ZOREG",
	"PSOREG",
	"PPOREG",
	"UOREG4K",
	"UOREG8K",
	"UOREG16K",
	"UOREG32K",
	"UOREG64K",
	"LOREG",
	"ADDR",
	"ROFF",
	"XPOST",
	"XPRE",
	"VREG",
	"GOK",
	"NCLASS",
}

var dnames7 = [D_LAST]string{
	D_GOK:    "GOK/R0",
	D_NONE:   "NONE",
	D_EXTERN: "EXTERN",
	D_STATIC: "STATIC",
	D_AUTO:   "AUTO",
	D_PARAM:  "PARAM",
	D_BRANCH: "BRANCH",
	D_OREG:   "OREG",
	D_XPRE:   "XPRE",
	D_XPOST:  "XPOST",
	D_CONST:  "CONST",
	D_DCONST: "DCONST",
	D_FCONST: "FCONST",
	D_SCONST: "SCONST",
	D_REG:    "REG",
	D_SP:     "SP",
	D_FREG:   "FREG",
	D_VREG:   "VREG",
	D_SPR:    "SPR",
	D_FILE:   "FILE",
	D_OCONST: "OCONST",
	D_FILE1:  "FILE1",
	D_SHIFT:  "SHIFT",
	D_PAIR:   "PAIR",
	D_ADDR:   "ADDR",
	D_ADRP:   "ADRP",
	D_ADRLO:  "ADRLO",
	D_EXTREG: "EXTREG",
	D_ROFF:   "ROFF",
	D_COND:   "COND",
	D_VLANE:  "VLANE",
	D_VSET:   "VSET",
}
