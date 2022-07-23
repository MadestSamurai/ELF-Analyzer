#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "elf.h"

#define INVALID                 (-1)

#define SECTION_NAME_SHSTRTAB   ".shstrtab"

#define SECTION_NAME_SYMTAB     ".symtab"
#define SECTION_NAME_STRTAB     ".strtab"

#define SECTION_NAME_DYNSYM     ".dynsym"
#define SECTION_NAME_DYNSTR     ".dynstr"

/* 字串和枚举转换结构体 */
typedef struct {
    int enum_value;
    char *enum_str;
} ENUM_S;

char *enum2str(ENUM_S *specs_enum, int enum_value) {
    int i = 0;

    if (NULL == specs_enum) {
        printf("param error! enum:%#x\n", (int) specs_enum);
        return NULL;
    }

    for (i = 0; specs_enum->enum_str != NULL; specs_enum++, i++) {
        if (specs_enum->enum_value == enum_value) {
            return specs_enum->enum_str;
        }
    }
    printf("enum_value not match:%d \n", enum_value);

    return NULL;
}

char *readFile(char *file, int *size) {
    int fsize = 0;
    char *mem = NULL;
    FILE *fp = NULL;

    fp = fopen(file, "rb");
    if (fp == NULL) {
        perror("fopen error!");
        goto RELEASE;
    }

    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);
    mem = malloc(fsize);
    if (mem == NULL) {
        perror("malloc error!");
        goto RELEASE;
    }
    printf("readFile size:%d\n", fsize);

    fseek(fp, 0, SEEK_SET);
    fread(mem, fsize, 1, fp);
    *size = fsize;

    RELEASE:
    fclose(fp);

    return mem;
}

/* magic num */
ENUM_S e_class[] = {{ELFCLASSNONE, "INVALID"},
                    {ELFCLASS32,   "ELF32"},
                    {ELFCLASS64,   "ELF64"},
                    {ELFCLASSNUM,  "ELFNUM"},
                    {INVALID, NULL}};
ENUM_S e_data[] = {{ELFDATANONE, "INVALID"},
                   {ELFDATA2LSB, "little endian"},
                   {ELFDATA2MSB, "big endian"},
                   {ELFDATANUM,  "data num"},
                   {INVALID, NULL}};

/* TODO add osabi */
ENUM_S e_osabi[] = {{ELFOSABI_NONE,      "UNIX - System V"},
                    {ELFOSABI_HPUX,      "HP-UX"},
                    {ELFOSABI_NETBSD,    "NetBSD"},
                    {ELFOSABI_ARM_AEABI, "ARM EABI"},
                    {ELFOSABI_ARM,       "ARM"},
                    {INVALID, NULL}};

/* elf header */
ENUM_S e_type[] = {{ET_REL,  "Relocatable file"},
                   {ET_EXEC, "Executable file"},
                   {ET_DYN,  "Shared object file"},
                   {ET_CORE, "Core file"},
                   {INVALID, NULL}};

ENUM_S e_machine[] = {{EM_NONE,          "No machine"},
                      {EM_M32,           "AT&T WE 32100"},
                      {EM_SPARC,         "SUN SPARC"},
                      {EM_386,           "Intel 80386"},
                      {EM_68K,           "Motorola m68k family"},
                      {EM_88K,           "Motorola m88k family"},
                      {EM_IAMCU,         "Intel MCU"},
                      {EM_860,           "Intel 80860"},
                      {EM_MIPS,          "MIPS R3000 big-endian"},
                      {EM_S370,          "IBM System/370"},
                      {EM_MIPS_RS3_LE,   "MIPS R3000 little-endian"},
                      {EM_PARISC,        "HPPA"},
                      {EM_VPP500,        "Fujitsu VPP500"},
                      {EM_SPARC32PLUS,   "Sun's v8plus"},
                      {EM_960,           "Intel 80960"},
                      {EM_PPC,           "PowerPC"},
                      {EM_PPC64,         "PowerPC 64-bit"},
                      {EM_S390,          "IBM S390"},
                      {EM_SPU,           "IBM SPU/SPC"},
                      {EM_V800,          "NEC V800 series"},
                      {EM_FR20,          "Fujitsu FR20"},
                      {EM_RH32,          "TRW RH-32"},
                      {EM_RCE,           "Motorola RCE"},
                      {EM_ARM,           "ARM"},
                      {EM_FAKE_ALPHA,    "Digital Alpha"},
                      {EM_SH,            "Hitachi SH"},
                      {EM_SPARCV9,       "SPARC v9 64-bit"},
                      {EM_TRICORE,       "Siemens Tricore"},
                      {EM_ARC,           "Argonaut RISC Core"},
                      {EM_H8_300,        "Hitachi H8/300"},
                      {EM_H8_300H,       "Hitachi H8/300H"},
                      {EM_H8S,           "Hitachi H8S"},
                      {EM_H8_500,        "Hitachi H8/500"},
                      {EM_IA_64,         "Intel Merced"},
                      {EM_MIPS_X,        "Stanford MIPS-X"},
                      {EM_COLDFIRE,      "Motorola Coldfire"},
                      {EM_68HC12,        "Motorola M68HC12"},
                      {EM_MMA,           "Fujitsu MMA Multimedia Accelerator"},
                      {EM_PCP,           "Siemens PCP"},
                      {EM_NCPU,          "Sony nCPU embeeded RISC"},
                      {EM_NDR1,          "Denso NDR1 microprocessor"},
                      {EM_STARCORE,      "Motorola Start*Core processor"},
                      {EM_ME16,          "Toyota ME16 processor"},
                      {EM_ST100,         "STMicroelectronic ST100 processor"},
                      {EM_TINYJ,         "Advanced Logic Corp. Tinyj emb.fam"},
                      {EM_X86_64,        "AMD x86-64 architecture"},
                      {EM_PDSP,          "Sony DSP Processor"},
                      {EM_PDP10,         "Digital PDP-10"},
                      {EM_PDP11,         "Digital PDP-11"},
                      {EM_FX66,          "Siemens FX66 microcontroller"},
                      {EM_ST9PLUS,       "STMicroelectronics ST9+ 8/16 mc"},
                      {EM_ST7,           "STmicroelectronics ST7 8 bit mc"},
                      {EM_68HC16,        "Motorola MC68HC16 microcontroller"},
                      {EM_68HC11,        "Motorola MC68HC11 microcontroller"},
                      {EM_68HC08,        "Motorola MC68HC08 microcontroller"},
                      {EM_68HC05,        "Motorola MC68HC05 microcontroller"},
                      {EM_SVX,           "Silicon Graphics SVx"},
                      {EM_ST19,          "STMicroelectronics ST19 8 bit mc"},
                      {EM_VAX,           "Digital VAX"},
                      {EM_CRIS,          "Axis Communications 32-bit emb.proc"},
                      {EM_JAVELIN,       "Infineon Technologies 32-bit emb.proc"},
                      {EM_FIREPATH,      "Element 14 64-bit DSP Processor"},
                      {EM_ZSP,           "LSI Logic 16-bit DSP Processor"},
                      {EM_MMIX,          "Donald Knuth's educational 64-bit proc"},
                      {EM_HUANY,         "Harvard University machine-independent object files"},
                      {EM_PRISM,         "SiTera Prism"},
                      {EM_AVR,           "Atmel AVR 8-bit microcontroller"},
                      {EM_FR30,          "Fujitsu FR30"},
                      {EM_D10V,          "Mitsubishi D10V"},
                      {EM_D30V,          "Mitsubishi D30V"},
                      {EM_V850,          "NEC v850"},
                      {EM_M32R,          "Mitsubishi M32R"},
                      {EM_MN10300,       "Matsushita MN10300"},
                      {EM_MN10200,       "Matsushita MN10200"},
                      {EM_PJ,            "picoJava"},
                      {EM_OPENRISC,      "OpenRISC 32-bit embedded processor"},
                      {EM_ARC_COMPACT,   "ARC International ARCompact"},
                      {EM_XTENSA,        "Tensilica Xtensa Architecture"},
                      {EM_VIDEOCORE,     "Alphamosaic VideoCore"},
                      {EM_TMM_GPP,       "Thompson Multimedia General Purpose Proc"},
                      {EM_NS32K,         "National Semi. 32000"},
                      {EM_TPC,           "Tenor Network TPC"},
                      {EM_SNP1K,         "Trebia SNP 1000"},
                      {EM_ST200,         "STMicroelectronics ST200"},
                      {EM_IP2K,          "Ubicom IP2xxx"},
                      {EM_MAX,           "MAX processor"},
                      {EM_CR,            "National Semi. CompactRISC"},
                      {EM_F2MC16,        "Fujitsu F2MC16"},
                      {EM_MSP430,        "Texas Instruments msp430"},
                      {EM_BLACKFIN,      "Analog Devices Blackfin DSP"},
                      {EM_SE_C33,        "Seiko Epson S1C33 family"},
                      {EM_SEP,           "Sharp embedded microprocessor"},
                      {EM_ARCA,          "Arca RISC"},
                      {EM_UNICORE,       "PKU-Unity & MPRC Peking Uni. mc series"},
                      {EM_EXCESS,        "eXcess configurable cpu"},
                      {EM_DXP,           "Icera Semi. Deep Execution Processor"},
                      {EM_ALTERA_NIOS2,  "Altera Nios II"},
                      {EM_CRX,           "National Semi. CompactRISC CRX"},
                      {EM_XGATE,         "Motorola XGATE"},
                      {EM_C166,          "Infineon C16x/XC16x"},
                      {EM_M16C,          "Renesas M16C"},
                      {EM_DSPIC30F,      "Microchip Technology dsPIC30F"},
                      {EM_CE,            "Freescale Communication Engine RISC"},
                      {EM_M32C,          "Renesas M32C"},
                      {EM_TSK3000,       "Altium TSK3000"},
                      {EM_RS08,          "Freescale RS08"},
                      {EM_SHARC,         "Analog Devices SHARC family"},
                      {EM_ECOG2,         "Cyan Technology eCOG2"},
                      {EM_SCORE7,        "Sunplus S+core7 RISC"},
                      {EM_DSP24,         "New Japan Radio (NJR) 24-bit DSP"},
                      {EM_VIDEOCORE3,    "Broadcom VideoCore III"},
                      {EM_LATTICEMICO32, "RISC for Lattice FPGA"},
                      {EM_SE_C17,        "Seiko Epson C17"},
                      {EM_TI_C6000,      "Texas Instruments TMS320C6000 DSP"},
                      {EM_TI_C2000,      "Texas Instruments TMS320C2000 DSP"},
                      {EM_TI_C5500,      "Texas Instruments TMS320C55x DSP"},
                      {EM_TI_ARP32,      "Texas Instruments App. Specific RISC"},
                      {EM_TI_PRU,        "Texas Instruments Prog. Realtime Unit"},
                      {EM_MMDSP_PLUS,    "STMicroelectronics 64bit VLIW DSP"},
                      {EM_CYPRESS_M8C,   "Cypress M8C"},
                      {EM_R32C,          "Renesas R32C"},
                      {EM_TRIMEDIA,      "NXP Semi. TriMedia"},
                      {EM_QDSP6,         "QUALCOMM DSP6"},
                      {EM_8051,          "Intel 8051 and variants"},
                      {EM_STXP7X,        "STMicroelectronics STxP7x"},
                      {EM_NDS32,         "Andes Tech. compact code emb. RISC"},
                      {EM_ECOG1X,        "Cyan Technology eCOG1X"},
                      {EM_MAXQ30,        "Dallas Semi. MAXQ30 mc"},
                      {EM_XIMO16,        "New Japan Radio (NJR) 16-bit DSP"},
                      {EM_MANIK,         "M2000 Reconfigurable RISC"},
                      {EM_CRAYNV2,       "Cray NV2 vector architecture"},
                      {EM_RX,            "Renesas RX"},
                      {EM_METAG,         "Imagination Tech. META"},
                      {EM_MCST_ELBRUS,   "MCST Elbrus"},
                      {EM_ECOG16,        "Cyan Technology eCOG16"},
                      {EM_CR16,          "National Semi. CompactRISC CR16"},
                      {EM_ETPU,          "Freescale Extended Time Processing Unit"},
                      {EM_SLE9X,         "Infineon Tech. SLE9X"},
                      {EM_L10M,          "Intel L10M"},
                      {EM_K10M,          "Intel K10M"},
                      {EM_AARCH64,       "ARM AARCH64"},
                      {EM_AVR32,         "Amtel 32-bit microprocessor"},
                      {EM_STM8,          "STMicroelectronics STM8"},
                      {EM_TILE64,        "Tileta TILE64"},
                      {EM_TILEPRO,       "Tilera TILEPro"},
                      {EM_MICROBLAZE,    "Xilinx MicroBlaze"},
                      {EM_CUDA,          "NVIDIA CUDA"},
                      {EM_TILEGX,        "Tilera TILE-Gx"},
                      {EM_CLOUDSHIELD,   "CloudShield"},
                      {EM_COREA_1ST,     "KIPO-KAIST Core-A 1st gen."},
                      {EM_COREA_2ND,     "KIPO-KAIST Core-A 2nd gen."},
                      {EM_ARC_COMPACT2,  "Synopsys ARCompact V2"},
                      {EM_OPEN8,         "Open8 RISC"},
                      {EM_RL78,          "Renesas RL78"},
                      {EM_VIDEOCORE5,    "Broadcom VideoCore V"},
                      {EM_78KOR,         "Renesas 78KOR"},
                      {EM_56800EX,       "Freescale 56800EX DSC"},
                      {EM_BA1,           "Beyond BA1"},
                      {EM_BA2,           "Beyond BA2"},
                      {EM_XCORE,         "XMOS xCORE"},
                      {EM_MCHP_PIC,      "Microchip 8-bit PIC(r)"},
                      {EM_KM32,          "KM211 KM32"},
                      {EM_KMX32,         "KM211 KMX32"},
                      {EM_EMX16,         "KM211 KMX16"},
                      {EM_EMX8,          "KM211 KMX8"},
                      {EM_KVARC,         "KM211 KVARC"},
                      {EM_CDP,           "Paneve CDP"},
                      {EM_COGE,          "Cognitive Smart Memory Processor"},
                      {EM_COOL,          "Bluechip CoolEngine"},
                      {EM_NORC,          "Nanoradio Optimized RISC"},
                      {EM_CSR_KALIMBA,   "CSR Kalimba"},
                      {EM_Z80,           "Zilog Z80"},
                      {EM_VISIUM,        "Controls and Data Services VISIUMcore"},
                      {EM_FT32,          "FTDI Chip FT32"},
                      {EM_MOXIE,         "Moxie processor"},
                      {EM_AMDGPU,        "AMD GPU"},
                      {EM_RISCV,         "RISC-V"},
                      {EM_BPF,           "Linux BPF -- in-kernel virtual machine"},
                      {EM_CSKY,          "C-SKY"},
                      {INVALID, NULL}};

ENUM_S sh_type_list[] = {{SHT_NULL,           "NULL"},
                         {SHT_PROGBITS,       "PROGBITS"},
                         {SHT_SYMTAB,         "SYMTAB"},
                         {SHT_STRTAB,         "STRTAB"},
                         {SHT_RELA,           "RELA"},
                         {SHT_HASH,           "HASH"},
                         {SHT_DYNAMIC,        "DYNAMIC"},
                         {SHT_NOTE,           "NOTE"},
                         {SHT_NOBITS,         "NOBITS"},
                         {SHT_REL,            "REL"},
                         {SHT_SHLIB,          "SHLIB"},
                         {SHT_DYNSYM,         "DYNSYM"},
                         {SHT_INIT_ARRAY,     "INIT_ARRAY"},
                         {SHT_FINI_ARRAY,     "FINI_ARRAY"},
                         {SHT_PREINIT_ARRAY,  "PREINIT_ARRAY"},
                         {SHT_GROUP,          "GROUP"},
                         {SHT_SYMTAB_SHNDX,   "SYMTAB_SHNDX"},
                         {SHT_NUM,            "NUM"},
                         {SHT_LOOS,           "LOOS"},
                         {SHT_GNU_ATTRIBUTES, "GNU_ATTRIBUTES"},
                         {SHT_GNU_HASH,       "GNU_HASH"},
                         {SHT_GNU_LIBLIST,    "GNU_LIBLIST"},
                         {SHT_CHECKSUM,       "CHECKSUM"},
                         {SHT_LOSUNW,         "LOSUNW"},
                         {SHT_SUNW_move,      "SUNW_move"},
                         {SHT_SUNW_COMDAT,    "SUNW_COMDAT"},
                         {SHT_SUNW_syminfo,   "SUNW_syminfo"},
                         {SHT_GNU_verdef,     "GNU_verdef"},
                         {SHT_GNU_verneed,    "GNU_verneed"},
                         {SHT_GNU_versym,     "GNU_versym"},
                         {SHT_HISUNW,         "HISUNW"},
                         {SHT_HIOS,           "HIOS"},
                         {SHT_LOPROC,         "LOPROC"},
                         {SHT_HIPROC,         "HIPROC"},
                         {SHT_LOUSER,         "LOUSER"},
                         {SHT_HIUSER,         "HIUSER"},
                         {INVALID, NULL}};

/* Elf_Sym st_info_binding */
ENUM_S st_info_bind[] = {{STB_LOCAL,  "LOCAL"},
                         {STB_GLOBAL, "GLOBAL"},
                         {STB_WEAK,   "WEAK"},
                         {STB_NUM,    "NUM"},
                         {STB_HIOS,   "HIOS"},
                         {STB_LOPROC, "LOPROC"},
                         {STB_HIPROC, "HIPROC"},
                         {INVALID, NULL}};
/* Elf_Sym st_info_type */
ENUM_S st_info_type[] = {{STT_NOTYPE,  "NOTYPE"},
                         {STT_OBJECT,  "OBJECT"},
                         {STT_FUNC,    "FUNC"},
                         {STT_SECTION, "SECTION"},
                         {STT_FILE,    "FILE"},
                         {STT_COMMON,  "COMMON"},
                         {STT_TLS,     "TLS"},
                         {STT_NUM,     "NUM"},
                         {INVALID, NULL}};
/*Elf_Sym visibility */
ENUM_S st_other_visibility[] = {{STV_DEFAULT,   "DEFAULT"},
                                {STV_INTERNAL,  "INTERNAL"},
                                {STV_HIDDEN,    "HIDDEN"},
                                {STV_PROTECTED, "PROTECTED"},
                                {INVALID, NULL}};

ENUM_S p_type[] = {{PT_NULL,         "NULL"},
                   {PT_LOAD,         "LOAD"},
                   {PT_DYNAMIC,      "DYNAMIC"},
                   {PT_INTERP,       "INTERP"},
                   {PT_NOTE,         "NOTE"},
                   {PT_SHLIB,        "SHLIB"},
                   {PT_PHDR,         "PHDR"},
                   {PT_TLS,          "TLS"},
                   {PT_GNU_EH_FRAME, "GNU_EH_FRAME"},
                   {PT_GNU_STACK,    "GNU_STACK"},
                   {PT_GNU_RELRO,    "GNU_RELRO"},
                   {INVALID, NULL},};
ENUM_S p_flags[] = {{PF_X, "X"},
                    {PF_W, "W"},
                    {PF_R, "R"},
                    {PF_X | PF_W,        "WE"},
                    {PF_X | PF_R,        "RE"},
                    {PF_W | PF_R,        "RW"},
                    {PF_X | PF_W | PF_R, "RWE"},
                    {INVALID,            NULL},};

/* 32位分析 */

int parseHeader32(Elf32_Ehdr *header) {
    int i = 0;

    unsigned char *e_ident = header->e_ident;

    if ((e_ident[EI_MAG0] != ELFMAG0) || (e_ident[EI_MAG1] != ELFMAG1) ||
        (e_ident[EI_MAG2] != ELFMAG2) || (e_ident[EI_MAG3] != ELFMAG3)) {
        printf("Not elf format!\n");
        return -1;
    }
    printf("ELF Header:\n\tMagic:");
    for (i = 0; i < EI_NIDENT; i++) {
        printf(" %02x", e_ident[i]);
    }
    printf("\n");
    printf("\tClass:                             %s\n", enum2str(e_class, e_ident[EI_CLASS]));
    printf("\tData:                              2's complement, %s\n", enum2str(e_data, e_ident[EI_DATA]));
    printf("\tVersion:                           %d (current)\n", e_ident[EI_VERSION]);
    printf("\tOS/ABI:                            %s\n", enum2str(e_osabi, e_ident[EI_OSABI]));
    printf("\tABI Version:                       %d\n", e_ident[EI_ABIVERSION]);
    printf("\tType:                              %s\n", enum2str(e_type, header->e_type));
    printf("\tMachine:                           %s\n", enum2str(e_machine, header->e_machine));
    printf("\tVersion:                           0x%x\n", header->e_version);
    printf("\tEntry point address:               0x%x\n", header->e_entry);
    printf("\tStart of program headers:          %d (bytes into file)\n", header->e_phoff);
    printf("\tStart of section headers:          %d (bytes into file)\n", header->e_shoff);
    printf("\tFlags:                             0x%x\n", header->e_flags);
    printf("\tSize of this header:               %d (bytes)\n", header->e_ehsize);
    printf("\tSize of program headers:           %d (bytes)\n", header->e_phentsize);
    printf("\tNumber of program headers:         %d\n", header->e_phnum);
    printf("\tSize of section headers:           %d (bytes)\n", header->e_shentsize);
    printf("\tNumber of section headers:         %d\n", header->e_shnum);
    printf("\tSection header string table index: %d\n", header->e_shstrndx);

    return 0;
}

char *getSectionTypeName32(Elf32_Word sh_type) {


    return enum2str(sh_type_list, (int) sh_type);
}

char *getStrTabStr32(Elf32_Ehdr *ehdr, Elf32_Shdr *strtabhdr, int idx) {
    if (idx < strtabhdr->sh_size) {
        return ((char *) ehdr + strtabhdr->sh_offset + idx);
    }

    return NULL;
}

Elf32_Shdr *getSHStrTab32(Elf32_Ehdr *ehdr) {
    int i = 0, shnum = ehdr->e_shnum;
    char *nameStr = NULL;
    Elf32_Shdr *shdr = (Elf32_Shdr *) ((char *) ehdr + ehdr->e_shoff);

    for (i = 0; i < shnum; i++, shdr++) {
        if (shdr->sh_type == SHT_STRTAB) {
            if (shdr->sh_name >= shdr->sh_size) {
                continue;
            }

            nameStr = (char *) ehdr + shdr->sh_offset + shdr->sh_name;
            if (strcmp(nameStr, SECTION_NAME_SHSTRTAB) == 0) {
                return shdr;
            }
        }
    }

    return NULL;
}

Elf32_Shdr *getSHdrByName32(Elf32_Ehdr *ehdr, const char *name, Elf32_Word sh_type) {
    int i = 0, shnum = ehdr->e_shnum;
    char *nameStr = NULL;
    Elf32_Shdr *shstrtabhdr = NULL;
    Elf32_Shdr *shdr = (Elf32_Shdr *) ((char *) ehdr + ehdr->e_shoff);

    shstrtabhdr = getSHStrTab32(ehdr);
    if (shstrtabhdr == NULL) {
        printf("getSHdrByName32 Get shstrtabhdr failed name:%s sh_type:%d\n", name, sh_type);
        return NULL;
    }

    shdr = (Elf32_Shdr *) ((char *) ehdr + ehdr->e_shoff);
    for (i = 0; i < shnum; i++, shdr++) {
        if (shdr->sh_type == sh_type) {
            nameStr = getStrTabStr32(ehdr, shstrtabhdr, shdr->sh_name);
            if (nameStr && strcmp(nameStr, name) == 0) {
                return shdr;
            }
        }
    }

    printf("getSHdrByName32 Get section header failed name:%s sh_type:%d\n", name, sh_type);
    return NULL;
}

int parseSectionHeader32(Elf32_Ehdr *ehdr) {
    int i = 0, shnum = ehdr->e_shnum;
    Elf32_Shdr *shdr = (Elf32_Shdr *) ((char *) ehdr + ehdr->e_shoff);
    Elf32_Shdr *shstrtabhdr = NULL;

    shstrtabhdr = getSHStrTab32(ehdr);
    if (shstrtabhdr == NULL) {
        printf("parseSectionHeader32 Get shstrtabhdr failed\n");
        return -1;
    }

    printf("\nThere are %d section headers, starting at offset 0x%d:\n", shnum, ehdr->e_shoff);
    printf("Section Headers:\n");
    printf("  [Nr] Name                     Type            Addr     Off    Size   ES Flg Lk Inf Al\n");
    for (i = 0; i < shnum; i++, shdr++) {
        printf("  [%2d] ", i);
        printf("%-24s ", getStrTabStr32(ehdr, shstrtabhdr, shdr->sh_name));
        printf("%-15s ", getSectionTypeName32(shdr->sh_type));
        printf("%08x ", shdr->sh_addr);
        printf("%06x ", shdr->sh_offset);
        printf("%06x ", shdr->sh_size);
        printf("%02x ", shdr->sh_entsize);
        printf("%03x ", shdr->sh_flags);
        printf("%02x ", shdr->sh_link);
        printf("%03x ", shdr->sh_info);
        printf("%02x ", shdr->sh_addralign);
        printf("\n");
    }

    return 0;
}

/* 解析动态和静态符号表时，须告知符号节名、类型及符号表用到字串节名称 */
int parseSymTab32(Elf32_Ehdr *ehdr, char *symtab, char *strtab, Elf32_Word sh_type) {
    int i = 0, sym_cnt = 0;
    Elf32_Shdr *shdrStrTab = NULL;
    Elf32_Shdr *shdrSymTab = NULL;
    Elf32_Sym *sym = NULL;

    shdrSymTab = getSHdrByName32(ehdr, symtab, sh_type);
    if (shdrSymTab == NULL) {
        printf("parseSymTab32 getSymTab faild\n");
        return -1;
    }
    shdrStrTab = getSHdrByName32(ehdr, strtab, SHT_STRTAB);
    if (shdrStrTab == NULL) {
        printf("parseSymTab32 getStrTab faild\n");
        return -1;
    }
    sym_cnt = shdrSymTab->sh_size / sizeof(Elf32_Sym);
    printf("\nSymbol table '%s' contains %d entries:\n", symtab, sym_cnt);
    printf("   Num:    Value  Size Type    Bind   Vis      Ndx   Name\n");

    sym = (Elf32_Sym *) ((char *) ehdr + shdrSymTab->sh_offset);
    for (i = 0; i < sym_cnt; i++, sym++) {
        printf("%6d: ", i);
        printf("%08x ", sym->st_value);
        printf("%5d ", sym->st_size);
        printf("%-7s ", enum2str(st_info_type, ELF32_ST_TYPE(sym->st_info)));
        printf("%-6s ", enum2str(st_info_bind, ELF32_ST_BIND(sym->st_info)));
        printf("%-8s ", enum2str(st_other_visibility, ELF32_ST_VISIBILITY(sym->st_other)));
        printf("%5d ", sym->st_shndx);
        printf("%-32s", getStrTabStr32(ehdr, shdrStrTab, sym->st_name));
        printf("\n");
    }

    return 0;
}

int parseProgramHeader32(Elf32_Ehdr *ehdr) {
    int i = 0, phnum = ehdr->e_phnum;
    Elf32_Phdr *phdr = (Elf32_Phdr *) ((char *) ehdr + ehdr->e_phoff);

    printf("\nProgram Headers:\n");
    printf("  Type           Offset   VirtAddr   PhysAddr   FileSiz    MemSiz  Flg Align\n");
    for (i = 0; i < phnum; i++, phdr++) {
        printf("  %-12s ", enum2str(p_type, phdr->p_type));
        printf("0x%08x ", phdr->p_offset);
        printf("0x%08x ", phdr->p_vaddr);
        printf("0x%08x ", phdr->p_paddr);
        printf("0x%08x ", phdr->p_filesz);
        printf("0x%08x ", phdr->p_memsz);
        printf("%-3s ", enum2str(p_flags, phdr->p_flags));
        printf("%#04x ", phdr->p_align);
        printf("\n");

        /* 解释器类型,则打印出解析器名称 */
        if (phdr->p_type == PT_INTERP) {
            printf("    [Requesting program interpreter:%s]\n", (char *) ehdr + phdr->p_offset);
        }
    }

    return 0;
}

/* 解析段包含的section信息 */
int parseSegment32(Elf32_Ehdr *ehdr) {
    int i = 0, j = 0;
    Elf32_Shdr *shdr = NULL;
    Elf32_Phdr *phdr = (Elf32_Phdr *) ((char *) ehdr + ehdr->e_phoff);
    Elf32_Shdr *shstrtabhdr = NULL;

    shstrtabhdr = getSHStrTab32(ehdr);
    if (shstrtabhdr == NULL) {
        printf("parseSectionHeader32 Get shstrtabhdr failed\n");
        return -1;
    }

    printf("\nSection to Segment mapping:\n");
    printf("  Segment Sections...\n");

    for (i = 0; i < ehdr->e_phnum; i++, phdr++) {
        printf("   %2.2d     ", i);

        shdr = (Elf32_Shdr *) ((char *) ehdr + ehdr->e_shoff);
        for (j = 1; j < ehdr->e_shnum; j++, shdr++) {
            if (shdr->sh_size <= 0) {
                continue;
            }

            /* 地址或者偏移在segment地址范围内的section都归到该segment */
            if (shdr->sh_flags & SHF_ALLOC) {
                if ((shdr->sh_addr < phdr->p_vaddr) ||
                    (shdr->sh_addr + shdr->sh_size > phdr->p_vaddr + phdr->p_memsz)) {
                    continue;
                }
            } else {
                if ((shdr->sh_offset < phdr->p_offset) ||
                    (shdr->sh_offset + shdr->sh_size > phdr->p_offset + phdr->p_filesz)) {
                    continue;
                }
            }
            printf("%s ", getStrTabStr32(ehdr, shstrtabhdr, shdr->sh_name));
        }
        printf("\n");
    }

    return 0;
}


/* 64位分析 */
int parseHeader64(Elf64_Ehdr *header) {
    int i = 0;

    unsigned char *e_ident = header->e_ident;

    if ((e_ident[EI_MAG0] != ELFMAG0) || (e_ident[EI_MAG1] != ELFMAG1) ||
        (e_ident[EI_MAG2] != ELFMAG2) || (e_ident[EI_MAG3] != ELFMAG3)) {
        printf("Not elf format!\n");
        return -1;
    }
    printf("ELF Header:\n\tMagic:");
    for (i = 0; i < EI_NIDENT; i++) {
        printf(" %02x", e_ident[i]);
    }
    printf("\n");
    printf("\tClass:                             %s\n", enum2str(e_class, e_ident[EI_CLASS]));
    printf("\tData:                              2's complement, %s\n", enum2str(e_data, e_ident[EI_DATA]));
    printf("\tVersion:                           %d (current)\n", e_ident[EI_VERSION]);
    printf("\tOS/ABI:                            %s\n", enum2str(e_osabi, e_ident[EI_OSABI]));
    printf("\tABI Version:                       %d\n", e_ident[EI_ABIVERSION]);
    printf("\tType:                              %s\n", enum2str(e_type, header->e_type));
    printf("\tMachine:                           %s\n", enum2str(e_machine, header->e_machine));
    printf("\tVersion:                           0x%x\n", header->e_version);
    printf("\tEntry point address:               0x%x\n", header->e_entry);
    printf("\tStart of program headers:          %d (bytes into file)\n", header->e_phoff);
    printf("\tStart of section headers:          %d (bytes into file)\n", header->e_shoff);
    printf("\tFlags:                             0x%x\n", header->e_flags);
    printf("\tSize of this header:               %d (bytes)\n", header->e_ehsize);
    printf("\tSize of program headers:           %d (bytes)\n", header->e_phentsize);
    printf("\tNumber of program headers:         %d\n", header->e_phnum);
    printf("\tSize of section headers:           %d (bytes)\n", header->e_shentsize);
    printf("\tNumber of section headers:         %d\n", header->e_shnum);
    printf("\tSection header string table index: %d\n", header->e_shstrndx);

    return 0;
}

char *getSectionTypeName64(Elf64_Word sh_type) {
    return enum2str(sh_type_list, (int) sh_type);
}

char *getStrTabStr64(Elf64_Ehdr *ehdr, Elf64_Shdr *strtabhdr, int idx) {
    if (idx < strtabhdr->sh_size) {
        return ((char *) ehdr + strtabhdr->sh_offset + idx);
    }

    return NULL;
}

Elf64_Shdr *getSHStrTab64(Elf64_Ehdr *ehdr) {
    int i = 0, shnum = ehdr->e_shnum;
    char *nameStr = NULL;
    Elf64_Shdr *shdr = (Elf64_Shdr *) ((char *) ehdr + ehdr->e_shoff);

    for (i = 0; i < shnum; i++, shdr++) {
        if (shdr->sh_type == SHT_STRTAB) {
            if (shdr->sh_name >= shdr->sh_size) {
                continue;
            }

            nameStr = (char *) ehdr + shdr->sh_offset + shdr->sh_name;
            if (strcmp(nameStr, SECTION_NAME_SHSTRTAB) == 0) {
                return shdr;
            }
        }
    }

    return NULL;
}

Elf64_Shdr *getSHdrByName64(Elf64_Ehdr *ehdr, const char *name, Elf64_Word sh_type) {
    int i = 0, shnum = ehdr->e_shnum;
    char *nameStr = NULL;
    Elf64_Shdr *shstrtabhdr = NULL;
    Elf64_Shdr *shdr = (Elf64_Shdr *) ((char *) ehdr + ehdr->e_shoff);

    shstrtabhdr = getSHStrTab64(ehdr);
    if (shstrtabhdr == NULL) {
        printf("getSHdrByName64 Get shstrtabhdr failed name:%s sh_type:%d\n", name, sh_type);
        return NULL;
    }

    shdr = (Elf64_Shdr *) ((char *) ehdr + ehdr->e_shoff);
    for (i = 0; i < shnum; i++, shdr++) {
        if (shdr->sh_type == sh_type) {
            nameStr = getStrTabStr64(ehdr, shstrtabhdr, shdr->sh_name);
            if (nameStr && strcmp(nameStr, name) == 0) {
                return shdr;
            }
        }
    }

    printf("getSHdrByName64 Get section header failed name:%s sh_type:%d\n", name, sh_type);
    return NULL;
}

int parseSectionHeader64(Elf64_Ehdr *ehdr) {
    int i = 0, shnum = ehdr->e_shnum;
    Elf64_Shdr *shdr = (Elf64_Shdr *) ((char *) ehdr + ehdr->e_shoff);
    Elf64_Shdr *shstrtabhdr = NULL;

    shstrtabhdr = getSHStrTab64(ehdr);
    if (shstrtabhdr == NULL) {
        printf("parseSectionHeader64 Get shstrtabhdr failed\n");
        return -1;
    }

    printf("\nThere are %d section headers, starting at offset 0x%d:\n", shnum, ehdr->e_shoff);
    printf("Section Headers:\n");
    printf("  [Nr] Name                     Type            Address          Off    Size   ES Flg Lk Inf Al\n");
    for (i = 0; i < shnum; i++, shdr++) {
        printf("  [%2d] ", i);
        printf("%-24s ", getStrTabStr64(ehdr, shstrtabhdr, shdr->sh_name));
        printf("%-15s ", getSectionTypeName64(shdr->sh_type));
        printf("%016x ", shdr->sh_addr);
        printf("%06x ", shdr->sh_offset);
        printf("%06x ", shdr->sh_size);
        printf("%02x ", shdr->sh_entsize);
        printf("%03x ", shdr->sh_flags);
        printf("%02x ", shdr->sh_link);
        printf("%03x ", shdr->sh_info);
        printf("%02x ", shdr->sh_addralign);
        printf("\n");
    }

    return 0;
}

/* 解析动态和静态符号表时，须告知符号节名、类型及符号表用到字串节名称 */
int parseSymTab64(Elf64_Ehdr *ehdr, char *symtab, char *strtab, Elf64_Word sh_type) {
    int i = 0, sym_cnt = 0;
    Elf64_Shdr *shdrStrTab = NULL;
    Elf64_Shdr *shdrSymTab = NULL;
    Elf64_Sym *sym = NULL;

    shdrSymTab = getSHdrByName64(ehdr, symtab, sh_type);
    if (shdrSymTab == NULL) {
        printf("parseSymTab64 getSymTab faild\n");
        return -1;
    }
    shdrStrTab = getSHdrByName64(ehdr, strtab, SHT_STRTAB);
    if (shdrStrTab == NULL) {
        printf("parseSymTab64 getStrTab faild\n");
        return -1;
    }
    sym_cnt = shdrSymTab->sh_size / sizeof(Elf64_Sym);
    printf("\nSymbol table '%s' contains %d entries:\n", symtab, sym_cnt);
    printf("   Num:            Value  Size Type    Bind   Vis      Ndx   Name\n");

    sym = (Elf64_Sym *) ((char *) ehdr + shdrSymTab->sh_offset);
    for (i = 0; i < sym_cnt; i++, sym++) {
        printf("%6d: ", i);
        printf("%016x ", sym->st_value);
        printf("%5d ", sym->st_size);
        printf("%-7s ", enum2str(st_info_type, ELF64_ST_TYPE(sym->st_info)));
        printf("%-6s ", enum2str(st_info_bind, ELF64_ST_BIND(sym->st_info)));
        printf("%-8s ", enum2str(st_other_visibility, ELF64_ST_VISIBILITY(sym->st_other)));
        printf("%5d ", sym->st_shndx);
        printf("%-64s", getStrTabStr64(ehdr, shdrStrTab, sym->st_name));
        printf("\n");
    }

    return 0;
}

int parseProgramHeader64(Elf64_Ehdr *ehdr) {
    int i = 0, phnum = ehdr->e_phnum;
    Elf64_Phdr *phdr = (Elf64_Phdr *) ((char *) ehdr + ehdr->e_phoff);

    printf("\nProgram Headers:\n");
    printf("  Type         Offset             VirtAddress        PhysicAddress      FileSize           MemorySize         Flg Align\n");
    for (i = 0; i < phnum; i++, phdr++) {
        printf("  %-12s ", enum2str(p_type, phdr->p_type));
        printf("0x%016x ", phdr->p_offset);
        printf("0x%016x ", phdr->p_vaddr);
        printf("0x%016x ", phdr->p_paddr);
        printf("0x%016x ", phdr->p_filesz);
        printf("0x%016x ", phdr->p_memsz);
        printf("%-3s ", enum2str(p_flags, phdr->p_flags));
        printf("%#04x ", phdr->p_align);
        printf("\n");

        /* 解释器类型,则打印出解析器名称 */
        if (phdr->p_type == PT_INTERP) {
            printf("    [Requesting program interpreter:%s]\n", (char *) ehdr + phdr->p_offset);
        }
    }

    return 0;
}

/* 解析段包含的section信息 */
int parseSegment64(Elf64_Ehdr *ehdr) {
    int i = 0, j = 0;
    Elf64_Shdr *shdr = NULL;
    Elf64_Phdr *phdr = (Elf64_Phdr *) ((char *) ehdr + ehdr->e_phoff);
    Elf64_Shdr *shstrtabhdr = NULL;

    shstrtabhdr = getSHStrTab64(ehdr);
    if (shstrtabhdr == NULL) {
        printf("parseSectionHeader64 Get shstrtabhdr failed\n");
        return -1;
    }

    printf("\nSection to Segment mapping:\n");
    printf("  Segment Sections...\n");

    for (i = 0; i < ehdr->e_phnum; i++, phdr++) {
        printf("   %2.2d     ", i);

        shdr = (Elf64_Shdr *) ((char *) ehdr + ehdr->e_shoff);
        for (j = 1; j < ehdr->e_shnum; j++, shdr++) {
            if (shdr->sh_size <= 0) {
                continue;
            }

            /* 地址或者偏移在segment地址范围内的section都归到该segment */
            if (shdr->sh_flags & SHF_ALLOC) {
                if ((shdr->sh_addr < phdr->p_vaddr) ||
                    (shdr->sh_addr + shdr->sh_size > phdr->p_vaddr + phdr->p_memsz)) {
                    continue;
                }
            } else {
                if ((shdr->sh_offset < phdr->p_offset) ||
                    (shdr->sh_offset + shdr->sh_size > phdr->p_offset + phdr->p_filesz)) {
                    continue;
                }
            }
            printf("%s ", getStrTabStr64(ehdr, shstrtabhdr, shdr->sh_name));
        }
        printf("\n");
    }

    return 0;
}


/* 32位文件分析 */
int parseFile32(char *mem, int size) {
    int ret = 0;
    Elf32_Ehdr *ehdr = (Elf32_Ehdr *) mem;

    /* 解析elf总头 */
    parseHeader32(ehdr);

    /* 解析section总头 */
    parseSectionHeader32(ehdr);

    /* 解析.symtab与.dynsym */
    parseSymTab32(ehdr, SECTION_NAME_SYMTAB, SECTION_NAME_STRTAB, SHT_SYMTAB);
    parseSymTab32(ehdr, SECTION_NAME_DYNSYM, SECTION_NAME_DYNSTR, SHT_DYNSYM);

    /* 解析程序头与segment信息 */
    parseProgramHeader32(ehdr);
    parseSegment32(ehdr);

    return 0;
}

/* 64位文件分析 */
int parseFile64(char *mem, int size) {
    int ret = 0;
    Elf64_Ehdr *ehdr = (Elf64_Ehdr *) mem;

    /* 解析elf总头 */
    parseHeader64(ehdr);

    /* 解析section总头 */
    parseSectionHeader64(ehdr);

    /* 解析.symtab与.dynsym */
    parseSymTab64(ehdr, SECTION_NAME_SYMTAB, SECTION_NAME_STRTAB, SHT_SYMTAB);
    parseSymTab64(ehdr, SECTION_NAME_DYNSYM, SECTION_NAME_DYNSTR, SHT_DYNSYM);

    /* 解析程序头与segment信息 */
    parseProgramHeader64(ehdr);
    parseSegment64(ehdr);

    return 0;
}


int main(int argc, char *argv[]) {
    int size = 0;
    char *mem = NULL;

    if (argc < 2) {
        printf("./readelf (elf file)\n");
        return -1;
    }
    mem = readFile(argv[1], &size);

    freopen("result.txt", "w", stdout); //切换输出流到文件
    printf("");    //清空文件内容

    if (mem[4] == 2) {
        parseFile64(mem, size);
    } else {
        parseFile32(mem, size);
    }

    if (mem) {
        free(mem);
    }

    freopen("CON", "w", stdout); //切换输出流到控制台
    FILE *fp;
    char ch;
    fp = fopen("result.txt", "r");
    fscanf(fp, "%c", &ch);
    while (!feof(fp)) {
        putchar(ch);
        fscanf(fp, "%c", &ch);
    }
    fclose(fp);
    printf("\n");
}