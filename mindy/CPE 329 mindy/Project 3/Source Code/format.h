#ifndef FORMAT_H_
#define FORMAT_H_

/*locations for items when updating*/
#define DCBAR 7
#define RMSBAR 67
#define OFFSET 164
#define PEAK 152
#define TRUERMS 128
#define CALCRMS 176
#define DC 187
#define FREQUENCY 140

/*initial frame setup*/
char frame[] = {0x1B, '[', '?', '2', '5', 'l', 0x1B, '[', 'H', '|','-','-',
                '-','-','-','-','-','-','-','-','-','-','-','-','-','-','-',
                '-','-','-','-','-','-','-','-','-','-','-','-','-','-','-',
                '-','-','-','-','-','-','-','-','-','-','-','-','-','-','-',
                '-','-','-','-','-','-','-','-','-','-','-','-','-','-','-',
                '-','-','-','-','-','|','\r', '\n','|',' ','V','o','l','t',
                'a','g','e',' ','(','V',')',' ','|','0','.','0',' ',' ',' ',
                '0','.','5',' ',' ',' ','1','.','0',' ',' ',' ','1','.','5',
                ' ',' ',' ','2','.','0',' ',' ',' ','2','.','5',' ',' ',' ',
                '3','.','0',' ',' ',' ','3','.','5',' ',' ',' ','4','.','0',
                ' ',' ','|','\r', '\n','|','-','-','-','-','-','-','-','-',
                '-','-','-','-','-','|','-','-','-','-','-','-','-','-','-',
                '-','-','-','-','-','-','-','-','-','-','-','-','-','-','-',
                '-','-','-','-','-','-','-','-','-','-','-','-','-','-','-',
                '-','-','-','-','-','-','-','-','-','-','-','-','-','-','|',
                '\r', '\n','|',' ','D','C',' ',' ',' ',' ',' ',' ',' ',' ',
                ' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|','\r', '\n',
                '|',' ','C','a','l','c','R','M','S',' ',' ',' ',' ',' ','|',
                ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                ' ',' ',' ',' ',' ',' ',' ',' ','|','\r', '\n','|','-','-',
                '-','-','-','-','-','-','-','-','-','-','-','-','-','-','-',
                '-','-','-','-','-','-','-','-','-','-','-','-','-','-','-',
                '-','-','-','-','-','-','-','-','-','-','-','-','-','-','-',
                '-','-','-','-','-','-','-','-','-','-','-','-','-','-','-',
                '-','-','-','-','-','|','\r', '\n','|',' ','D','C',' ','V',
                'a','l','u','e',' ',' ',':',' ','0','.','0','0',' ','V',' ',
                ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                ' ',' ','|','\r', '\n','|','-','-','-','-','-','-','-','-',
                '-','-','-','-','-','-','-','-','-','-','-','-','-','-','-',
                '-','-','-','-','-','-','-','-','-','-','-','-','-','-','-',
                '-','-','-','-','-','-','-','-','-','-','-','-','-','-','-',
                '-','-','-','-','-','-','-','-','-','-','-','-','-','-','|',
                '\r', '\n','|',' ','A','C',' ','V','a','l','u','e','s',' ',
                ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|','\r', '\n',
                '|','-','-','-','-','-','-','-','-','-','-','-','-','-','-',
                '-','-','-','-','-','-','-','-','-','-','-','-','-','-','-',
                '-','-','-','-','-','-','-','-','-','-','-','-','-','-','-',
                '-','-','-','-','-','-','-','-','-','-','-','-','-','-','-',
                '-','-','-','-','-','-','-','-','|','\r', '\n','|',' ','T',
                'r','u','e',' ','R','M','S',' ',' ',':',' ','0','.','0','0',
                ' ','V',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                ' ',' ',' ',' ',' ','|','\r', '\n','|',' ','F','r','e','q',
                'u','e','n','c','y',' ',':',' ','0','0','0','0',' ','H','z',
                ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                ' ',' ','|','\r', '\n','|',' ','P','e','a','k','-','P','e',
                'a','k',' ',':',' ','0','.','0','0',' ','V',' ',' ',' ',' ',
                ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',
                '\r', '\n','|',' ','D','C',' ','O','f','f','s','e','t',' ',
                ':',' ','0','.','0','0',' ','V',' ',' ',' ',' ',' ',' ',' ',
                ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|','\r', '\n',
                '|',' ','C','a','l','c','R','M','S',' ',' ',' ',':',' ','0',
                '.','0','0',' ','V',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                ' ',' ',' ',' ',' ',' ',' ',' ','|','\r', '\n','|','-','-',
                '-','-','-','-','-','-','-','-','-','-','-','-','-','-','-',
                '-','-','-','-','-','-','-','-','-','-','-','-','-','-','-',
                '-','-','-','-','-','-','-','-','-','-','-','-','-','-','-',
                '-','-','-','-','-','-','-','-','-','-','-','-','-','-','-',
                '-','-','-','-','-','|', '\0'};

/*update setup*/
char update[] = {0x1B,'[','4',';','1','6','H',' ',' ',' ',' ',' ',' ',' ',
                 ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                 ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                 ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                 ' ',0x1B,'[','5',';','1','6','H',' ',' ',' ',' ',' ',' ',
                 ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                 ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                 ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                 ' ',' ',0x1B,'[','1','1',';','1','5','H','0','.','0','0',
                 0x1B,'[','1','2',';','1','5','H','0','0','0','0',0x1B,'[',
                 '1','3',';','1','5','H','0','.','0','0',0x1B,'[','1','4',
                 ';','1','5','H','0','.','0','0',0x1B,'[','1','5',';','1',
                 '5','H','0','.','0','0',0x1B,'[','7',';','1','5','H','0',
                 '.','0','0', '\0'};
#endif /* FORMAT_H_ */
