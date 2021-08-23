#include <terminal.h>
#include <stdio.h>
#include <gdt.h>
#include <idt.h>
#include <pci.h>
#include <threads.h>
#include <irq.h>
#include <pic.h>
#include <stdlib.h>
#include <ps2.h>
#include <string.h>

extern uint8_t key_state[];

uint32_t count = 0;

extern struct ThreadInfo* first_ready_to_run;
extern struct ThreadInfo* last_ready_to_run;

extern struct KeypressBuffer keypress_buffer;

#include <ports.h>

void klik()
{
    uint8_t dupa = inb(0x61);
    if(dupa & 2)
    {
        outb(0x61, dupa & ~(0x3));
    }
    else
        outb(0x61, (dupa | (0x2)) & ~(1));
}

void timer()
{
    count++;
    pic_master_eoi();
    
    //klik();
    scheduler_lock();
    schedule();
    scheduler_unlock();
}

void play_divisor(int divisor)
{
    outb(0x61, 0x3f);
    outb(0x42, (uint8_t)divisor);
    outb(0x42, (uint8_t)(divisor >> 8));
}

void stop_playing()
{
    outb(0x61, 0x3e);
}

extern uint8_t nie_buforuj;

// void play(char note)
// {

// }

// void play_sharp(char note)
// {

// }

extern uint8_t terminal_row, terminal_column;
void draw(const char* frame)
{
    terminal_row = terminal_column = 0;
    for(uint32_t i = 0; i < strlen(frame); i++)
        if(frame[i] == ' ')
            terminal_column++;
        else
            putc(frame[i]);
}

const char* ramka = 
"\n"
"\n"
"\n"
"\n"
"\n"
"            \xDA\xC4\xC4\xC4\xDC\xDC\xDC\xC4\xC4\xC4\xDC\xDC\xDC\xC4\xC4\xC4\xC2\xC4\xC4\xC4\xDC\xDC\xDC\xC4\xC4\xC4\xDC\xDC\xDC\xC4\xC4\xC4\xDC\xDC\xDC\xC4\xC4\xC4\xC2\xC4\xC4\xC4\xDC\xDC\xDC\xC4\xC4\xC4\xDC\xDC\xDC\xC4\xC4\xC4\xBF\n"
"            \xB3   \xDB\xDB\xDB   \xDB\xDB\xDB   \xB3   \xDB\xDB\xDB   \xDB\xDB\xDB   \xDB\xDB\xDB   \xB3   \xDB\xDB\xDB   \xDB\xDB\xDB   \xB3\n"
"            \xB3   \xDB\xDB\xDB   \xDB\xDB\xDB   \xB3   \xDB\xDB\xDB   \xDB\xDB\xDB   \xDB\xDB\xDB   \xB3   \xDB\xDB\xDB   \xDB\xDB\xDB   \xB3\n"
"            \xB3   \xDB\xDB\xDB   \xDB\xDB\xDB   \xB3   \xDB\xDB\xDB   \xDB\xDB\xDB   \xDB\xDB\xDB   \xB3   \xDB\xDB\xDB   \xDB\xDB\xDB   \xB3\n"
"            \xB3   \xDB\xDB\xDB   \xDB\xDB\xDB   \xB3   \xDB\xDB\xDB   \xDB\xDB\xDB   \xDB\xDB\xDB   \xB3   \xDB\xDB\xDB   \xDB\xDB\xDB   \xB3\n"
"            \xB3    \xB3     \xB3    \xB3    \xB3     \xB3     \xB3    \xB3    \xB3     \xB3    \xB3\n"
"            \xB3    \xB3     \xB3    \xB3    \xB3     \xB3     \xB3    \xB3    \xB3     \xB3    \xB3\n"
"            \xB3    \xB3     \xB3    \xB3    \xB3     \xB3     \xB3    \xB3    \xB3     \xB3    \xB3\n"
"            \xB3    \xB3     \xB3    \xB3    \xB3     \xB3     \xB3    \xB3    \xB3     \xB3    \xB3\n"
"            \xC0\xC4\xC4\xC4\xC4\xC1\xC4\xC4\xC4\xC4\xC4\xC1\xC4\xC4\xC4\xC4\xC1\xC4\xC4\xC4\xC4\xC1\xC4\xC4\xC4\xC4\xC4\xC1\xC4\xC4\xC4\xC4\xC4\xC1\xC4\xC4\xC4\xC4\xC1\xC4\xC4\xC4\xC4\xC1\xC4\xC4\xC4\xC4\xC4\xC1\xC4\xC4\xC4\xC4\xD9";

const char* cleaner = 
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"             \xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\n"
"             \xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\n"
"             \xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\n"
"             \xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\n";


const char* key_a =
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"             \xDB\xDB\xDB\xDB\n"
"             \xDB\xDB\xDB\xDB\n"
"             \xDB\xDB\xDB\xDB\n"
"             \xDB\xDB\xDB\xDB\n";

const char* key_w =
"\n"
"\n"
"\n"
"\n"
"\n"
"                \xDC\xDC\xDC\n"
"                \xDB\xDB\xDB\n"
"                \xDB\xDB\xDB\n"
"                \xDB\xDB\xDB\n"
"                \xDB\xDB\xDB\n"
"";

const char* key_s =
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"                  \xDB\xDB\xDB\xDB\xDB\n"
"                  \xDB\xDB\xDB\xDB\xDB\n"
"                  \xDB\xDB\xDB\xDB\xDB\n"
"                  \xDB\xDB\xDB\xDB\xDB\n";

const char* key_e =
"\n"
"\n"
"\n"
"\n"
"\n"
"                      \xDC\xDC\xDC\n"
"                      \xDB\xDB\xDB\n"
"                      \xDB\xDB\xDB\n"
"                      \xDB\xDB\xDB\n"
"                      \xDB\xDB\xDB\n"
"";

const char* key_d =
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"                        \xDB\xDB\xDB\xDB\n"
"                        \xDB\xDB\xDB\xDB\n"
"                        \xDB\xDB\xDB\xDB\n"
"                        \xDB\xDB\xDB\xDB\n";

const char* key_f =
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"                             \xDB\xDB\xDB\xDB\n"
"                             \xDB\xDB\xDB\xDB\n"
"                             \xDB\xDB\xDB\xDB\n"
"                             \xDB\xDB\xDB\xDB\n";

const char* key_t =
"\n"
"\n"
"\n"
"\n"
"\n"
"                                \xDC\xDC\xDC\n"
"                                \xDB\xDB\xDB\n"
"                                \xDB\xDB\xDB\n"
"                                \xDB\xDB\xDB\n"
"                                \xDB\xDB\xDB\n"
"";

const char* key_g =
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"                                  \xDB\xDB\xDB\xDB\xDB\n"
"                                  \xDB\xDB\xDB\xDB\xDB\n"
"                                  \xDB\xDB\xDB\xDB\xDB\n"
"                                  \xDB\xDB\xDB\xDB\xDB\n";

const char* key_y =
"\n"
"\n"
"\n"
"\n"
"\n"
"                                      \xDC\xDC\xDC\n"
"                                      \xDB\xDB\xDB\n"
"                                      \xDB\xDB\xDB\n"
"                                      \xDB\xDB\xDB\n"
"                                      \xDB\xDB\xDB\n"
"";

const char* key_h =
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"                                        \xDB\xDB\xDB\xDB\xDB\n"
"                                        \xDB\xDB\xDB\xDB\xDB\n"
"                                        \xDB\xDB\xDB\xDB\xDB\n"
"                                        \xDB\xDB\xDB\xDB\xDB\n";

const char* key_u =
"\n"
"\n"
"\n"
"\n"
"\n"
"                                            \xDC\xDC\xDC\n"
"                                            \xDB\xDB\xDB\n"
"                                            \xDB\xDB\xDB\n"
"                                            \xDB\xDB\xDB\n"
"                                            \xDB\xDB\xDB\n"
"";

const char* key_j =
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"                                              \xDB\xDB\xDB\xDB\n"
"                                              \xDB\xDB\xDB\xDB\n"
"                                              \xDB\xDB\xDB\xDB\n"
"                                              \xDB\xDB\xDB\xDB\n";

const char* key_k =
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"                                                   \xDB\xDB\xDB\xDB\n"
"                                                   \xDB\xDB\xDB\xDB\n"
"                                                   \xDB\xDB\xDB\xDB\n"
"                                                   \xDB\xDB\xDB\xDB\n";

const char* key_o =
"\n"
"\n"
"\n"
"\n"
"\n"
"                                                      \xDC\xDC\xDC\n"
"                                                      \xDB\xDB\xDB\n"
"                                                      \xDB\xDB\xDB\n"
"                                                      \xDB\xDB\xDB\n"
"                                                      \xDB\xDB\xDB\n"
"";

const char* key_l =
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"                                                        \xDB\xDB\xDB\xDB\xDB\n"
"                                                        \xDB\xDB\xDB\xDB\xDB\n"
"                                                        \xDB\xDB\xDB\xDB\xDB\n"
"                                                        \xDB\xDB\xDB\xDB\xDB\n";

const char* key_p =
"\n"
"\n"
"\n"
"\n"
"\n"
"                                                            \xDC\xDC\xDC\n"
"                                                            \xDB\xDB\xDB\n"
"                                                            \xDB\xDB\xDB\n"
"                                                            \xDB\xDB\xDB\n"
"                                                            \xDB\xDB\xDB\n"
"";

const char* key_semi =
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"                                                              \xDB\xDB\xDB\xDB\n"
"                                                              \xDB\xDB\xDB\xDB\n"
"                                                              \xDB\xDB\xDB\xDB\n"
"                                                              \xDB\xDB\xDB\xDB\n";

extern uint8_t echo;


#define   VGA_AC_INDEX      0x3C0
#define   VGA_AC_WRITE      0x3C0
#define   VGA_AC_READ      0x3C1
#define   VGA_MISC_WRITE      0x3C2
#define VGA_SEQ_INDEX      0x3C4
#define VGA_SEQ_DATA      0x3C5
#define   VGA_DAC_READ_INDEX   0x3C7
#define   VGA_DAC_WRITE_INDEX   0x3C8
#define   VGA_DAC_DATA      0x3C9
#define   VGA_MISC_READ      0x3CC
#define VGA_GC_INDEX       0x3CE
#define VGA_GC_DATA       0x3CF
/*         COLOR emulation      MONO emulation */
#define VGA_CRTC_INDEX      0x3D4      /* 0x3B4 */
#define VGA_CRTC_DATA      0x3D5      /* 0x3B5 */
#define   VGA_INSTAT_READ      0x3DA

#define   VGA_NUM_SEQ_REGS   5
#define   VGA_NUM_CRTC_REGS   25
#define   VGA_NUM_GC_REGS      9
#define   VGA_NUM_AC_REGS      21
#define   VGA_NUM_REGS      (1 + VGA_NUM_SEQ_REGS + VGA_NUM_CRTC_REGS + \
            VGA_NUM_GC_REGS + VGA_NUM_AC_REGS)

unsigned char g_320x200x256[] =
{
/* MISC */
   0x63,
/* SEQ */
   0x03, 0x01, 0x0F, 0x00, 0x0E,
/* CRTC */
   0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
   0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x9C, 0x0E, 0x8F, 0x28,   0x40, 0x96, 0xB9, 0xA3,
   0xFF,
/* GC */
   0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
   0xFF,
/* AC */
   0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
   0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
   0x41, 0x00, 0x0F, 0x00,   0x00
};

void write_regs(unsigned char *regs)
{
   unsigned i;

/* write MISCELLANEOUS reg */
   outb(VGA_MISC_WRITE, *regs);
   regs++;
/* write SEQUENCER regs */
   for(i = 0; i < VGA_NUM_SEQ_REGS; i++)
   {
      outb(VGA_SEQ_INDEX, i);
      outb(VGA_SEQ_DATA, *regs);
      regs++;
   }
/* unlock CRTC registers */
   outb(VGA_CRTC_INDEX, 0x03);
   outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) | 0x80);
   outb(VGA_CRTC_INDEX, 0x11);
   outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) & ~0x80);
/* make sure they remain unlocked */
   regs[0x03] |= 0x80;
   regs[0x11] &= ~0x80;
/* write CRTC regs */
   for(i = 0; i < VGA_NUM_CRTC_REGS; i++)
   {
      outb(VGA_CRTC_INDEX, i);
      outb(VGA_CRTC_DATA, *regs);
      regs++;
   }
/* write GRAPHICS CONTROLLER regs */
   for(i = 0; i < VGA_NUM_GC_REGS; i++)
   {
      outb(VGA_GC_INDEX, i);
      outb(VGA_GC_DATA, *regs);
      regs++;
   }
/* write ATTRIBUTE CONTROLLER regs */
   for(i = 0; i < VGA_NUM_AC_REGS; i++)
   {
      (void)inb(VGA_INSTAT_READ);
      outb(VGA_AC_INDEX, i);
      outb(VGA_AC_WRITE, *regs);
      regs++;
   }
/* lock 16-color palette and unblank display */
   (void)inb(VGA_INSTAT_READ);
   outb(VGA_AC_INDEX, 0x20);
}

void set_vga_300x220(void) {
  write_regs(g_320x200x256);
}

uint8_t* vga_graphics = (uint8_t*)0xa0000;

void kernel_main()
{
    set_color(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GREY);
    terminal_initialize();

    printf("MDOS\n");
    printf("----\n");

    echo = 1;

    gdt_init();
    idt_init();
    pci_enum();

    threads_init();
    irq_install_handler(0, timer);

    ps2_controller_init();

    outb(0x43, 0xB6);

    

    char line[50];
    while(1)
    {
        printf(">");
        scanf("%s", line);
        char* command = strtok(line, ' ');
        if(!strcmp(command, "inb"))
        {
            char* argument = strtok(NULL, '\n');
            printf("%1", inb(strtol(argument, NULL, 16)));
        }
        else if(!strcmp(command, "outb"))
        {
            char* argument = strtok(NULL, ' ');
            char* argument2 = strtok(NULL, '\n');
            outb(strtol(argument, NULL, 16), strtol(argument2, NULL, 16));
        }
        if(!strcmp(command, "clr"))
        {
            terminal_initialize();
        }
        if(!strcmp(command, "magic"))
        {
            set_vga_300x220();
            for(int i = 0; i < 64000; i++)
        vga_graphics[i] = i % 9;
            while(1);
        }
        if(!strcmp(command, "piano"))
        {
            disable_cursor();
            echo = 0;
            set_color(VGA_COLOR_BLUE, VGA_COLOR_WHITE);
            terminal_initialize();
            draw(ramka);

            while(1)
            {
                char klawisz;
                scanf("%c", &klawisz);
                nie_buforuj = 1;

                set_color(VGA_COLOR_BLUE, VGA_COLOR_BLACK);
                switch(klawisz)
                {
                case 'a':
                    
                    draw(key_a);
                    play_divisor(2280);
                    while(key_state[0x61])
                        ;
                    break;

                case 'w':
                    draw(key_w);
                    play_divisor(2152);
                    while(key_state[0x42])
                        ;
                    break;


                 case 's':
                    draw(key_s);
                    play_divisor(2031);
                    while(key_state[0x62])
                        ;
                    break;

                case 'e':
                    set_color(VGA_COLOR_BLUE, VGA_COLOR_BLACK);
                    draw(key_e);
                    play_divisor(1917);
                    while(key_state[0x43])
                        ;
                    break;

                 case 'd':
                    draw(key_d);
                    play_divisor(1809);
                    while(key_state[0x63])
                        ;
                    break;

                case 'f':
                    draw(key_f);
                    play_divisor(1715);
                    while(key_state[0x64])
                        ;
                    break;

                case 't':
                    draw(key_t);
                    play_divisor(1612);
                    while(key_state[0x45])
                        ;
                    break;

                case 'g':
                    draw(key_g);
                    play_divisor(1521);
                    while(key_state[0x65])
                        ;
                    break;

                case 'y':
                    draw(key_y);
                    play_divisor(1436);
                    while(key_state[0x46])
                        ;
                    break;

                case 'h':
                    draw(key_h);
                    play_divisor(1355);
                    while(key_state[0x66])
                        ;
                    break;

                case 'u':
                    draw(key_u);
                    play_divisor(1292);
                    while(key_state[0x47])
                        ;
                    break;

                case 'j':
                    draw(key_j);
                    play_divisor(1207);
                    while(key_state[0x67])
                        ;
                    break;

                case 'k':
                    draw(key_k);
                    play_divisor(1140);
                    while(key_state[0x68])
                        ;
                    break;

                case 'o':
                    draw(key_o);
                    play_divisor(1076);
                    while(key_state[0x49])
                        ;
                    break;

                case 'l':
                    draw(key_l);
                    play_divisor(1016);
                    while(key_state[0x69])
                        ;
                    break;

                case 'p':
                    draw(key_p);
                    play_divisor(959);
                    while(key_state[0x4a])
                        ;
                    break;

                case ';':
                    draw(key_semi);
                    play_divisor(904);
                    while(key_state[0x6a])
                        ;
                    break;

                }
                set_color(VGA_COLOR_BLUE, VGA_COLOR_BLUE);
                draw(cleaner);
                set_color(VGA_COLOR_BLUE, VGA_COLOR_WHITE);
                //terminal_initialize();
                draw(ramka);
                stop_playing();
                nie_buforuj = 0;
            }
        }
        
    }
}