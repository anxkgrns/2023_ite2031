/* Assembler code fragment for LC-2K */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXLINE 65536 /* 2^16*/
#define MAXLINELENGTH 1000
#define add 0
#define nor 1
#define lw 2
#define sw 3
#define beq 4
#define jalr 5
#define halt 6
#define noop 7


int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);
void outOfRegister(int, char*);
void outOfOffsetRange(int, char* );

struct labelmap{
	char label[MAXLINELENGTH];
	int address;
};

int main(int argc, char *argv[]) 
{
	char *inFileString, *outFileString;
	FILE *inFilePtr, *outFilePtr;
	char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH], 
			 arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

	if (argc != 3) {
		printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
				argv[0]);
		exit(1);
	}

	inFileString = argv[1];
	outFileString = argv[2];

	inFilePtr = fopen(inFileString, "r");
	if (inFilePtr == NULL) {
		printf("error in opening %s\n", inFileString);
		exit(1);
	}
	outFilePtr = fopen(outFileString, "w");
	if (outFilePtr == NULL) {
		printf("error in opening %s\n", outFileString);
		exit(1);
	}

	
	/* TODO: Phase-1 label calculation */

	struct labelmap labels[1000];
	int PC1 = 0;
	int label_num = 0; // label's number(개수)

	// make label map with label name & address
	for(;;PC1++){
		if (!readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
			break;
		}
		if(strcmp(label,"")){
			// check if label is longer than 6
			if(strlen(label)>6){
				printf("error: (Line %d) Illegal label - length\n",PC1+1);
				exit(1);
			}

			// check if label start with letter
			if(!isalpha(label[0])){
				printf("error: (Line %d) Illegal label - start char\n",PC1+1);
				exit(1);
			}

			for(int i=0;i<label_num;i++){
				// check for duplicate label
				if(!strcmp(labels[i].label,label)){
					printf("error: (Line %d) Duplicate definition of labels\n",PC1+1);
					exit(1);
				}
			}
			strcpy(labels[label_num].label,label);
			labels[label_num].address = PC1;
			//printf("%d %s %d\n",label_num,labels[label_num].label,labels[label_num].address);
			label_num++;

		}		
	}
	rewind(inFilePtr);

	
	/* TODO: Phase-2 generate machine codes to outfile */
	int PC = 0;
	for(;;PC++){
		int exist_opcode = -1;
		int LC2K_binary = 0;

		/* here is an example for how to use readAndParse to read a line from
			inFilePtr */
		if (!readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
			break;
		}


		/* after doing a readAndParse, you may want to do the following to test the
			opcode */
		if (!strcmp(opcode, "add") || !strcmp(opcode, "nor")) {

			if(!strcmp(opcode, "add"))
				exist_opcode = add;
			else if(!strcmp(opcode, "nor"))
				exist_opcode = nor;

			// check for register is number
			if(!isNumber(arg0)||!isNumber(arg1)||!isNumber(arg2)){
				printf("error: (Line %d) Non-integer register arguments\n",PC+1);
				exit(1);
			}

			// check for register is 0~7
			outOfRegister(PC,arg0);
			outOfRegister(PC,arg1);
			outOfRegister(PC,arg2);
			
			// generate machine code
			LC2K_binary = (exist_opcode<<22) + (atoi(arg0)<<19) + (atoi(arg1)<<16) + atoi(arg2);
		}

		if (!strcmp(opcode, "lw") || !strcmp(opcode, "sw") || !strcmp(opcode, "beq")) {
			int offset = 0;
			int flag = 0;
			if(!strcmp(opcode, "lw"))
				exist_opcode = lw;
			else if(!strcmp(opcode, "sw"))
				exist_opcode = sw;
			else if(!strcmp(opcode, "beq"))
				exist_opcode = beq;
				
			// check for register is number
			if(!isNumber(arg0)||!isNumber(arg1)){
				printf("error: (Line %d) Non-integer register arguments\n",PC+1);
				exit(1);
			}
			
			// check for register is 0~7
			outOfRegister(PC,arg0);
			outOfRegister(PC,arg1);

						// check if arg2 is offset or address
			if(isNumber(arg2)){ // is offset
				offset = atoi(arg2);
				flag = 1;
			}
			else { // is address
				for(int i = 0 ; i < label_num ; i++){
					if(!strcmp(labels[i].label,arg2)){
						offset = labels[i].address;
						flag = 1;
					}
				}
				if(!strcmp(opcode, "beq")){ // for beq opcode
					// branch addr(offset) = PC + 1 + offset
					offset = offset - PC - 1;
				}
			}
			// check offsetFields if it doesn’t fit in 16 bits
			outOfOffsetRange(PC,arg2);
			
			if(offset<0){
				//make offset 2의 보수
				offset += 65536; // 2^16
			}

			// check for label is not defined
			if(flag == 0){
				printf("error: (Line %d) Use of undefined label\n",PC+1);
				exit(1);
			}

			// generate machine code
			LC2K_binary = (exist_opcode<<22) + (atoi(arg0)<<19) + (atoi(arg1)<<16) + offset;
		}

		if (!strcmp(opcode, "jalr")) {
			exist_opcode = jalr;

			// check for register is number
			if(!isNumber(arg0)||!isNumber(arg1)){
				printf("error: (Line %d) Non-integer register arguments\n",PC+1);
				exit(1);
			}
			// check for register is 0~7
			outOfRegister(PC,arg0);
			outOfRegister(PC,arg1);
			
			// generate machine code
			LC2K_binary = (exist_opcode<<22) + (atoi(arg0)<<19) + (atoi(arg1)<<16);
		}

		if (!strcmp(opcode, "halt") || !strcmp(opcode, "noop")) {
			if(!strcmp(opcode, "halt"))
				exist_opcode = halt;
			else if(!strcmp(opcode, "noop"))
				exist_opcode = noop;

			// generate machine code
			LC2K_binary = (exist_opcode<<22);
		}
		
		if(!strcmp(opcode, ".fill")){
			exist_opcode = 0;
			int offset = 0;

			if(isNumber(arg0)){ // is offset
				offset = atoi(arg0);
			}
			else { // is address
				for(int i = 0 ; i < label_num ; i++){
					if(!strcmp(labels[i].label,arg0)){
						offset = labels[i].address;
					}
				}
				if(offset == 0){ // check for label is not defined
					printf("error: (Line %d) Use of undefined label\n",PC+1);
					exit(1);
				}
			}
			
			// generate machine code
			LC2K_binary = offset;
		}

		// check opcode is not recognized
		if(exist_opcode == -1){
			printf("error: (Line %d) Unrecognized opcodes\n",PC+1);
			exit(1);
		}
		fprintf(outFilePtr, "%d\n", LC2K_binary);
	}
	
	/* this is how to rewind the file ptr so that you start reading from the
		beginning of the file */
	rewind(inFilePtr);

	if (inFilePtr) {
		fclose(inFilePtr);
	}
	if (outFilePtr) {
		fclose(outFilePtr);
	}
	//return(0);
	exit(0);
}

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
		char *arg1, char *arg2)
{
	char line[MAXLINELENGTH];
	char *ptr = line;

	/* delete prior values */
	label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

	/* read the line from the assembly-language file */
	if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
		/* reached end of file */
		return(0);
	}

	/* check for line too long (by looking for a \n) */
	if (strchr(line, '\n') == NULL) {
		/* line too long */
		printf("error: line too long\n");
		exit(1);
	}

	/* is there a label? */
	ptr = line;
	if (sscanf(ptr, "%[^\t\n\r ]", label)) {
		/* successfully read label; advance pointer over the label */
		ptr += strlen(label);
		
	}

	/*
	 * Parse the rest of the line.  Would be nice to have real regular
	 * expressions, but scanf will suffice.
	 */
	sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%"
			"[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]", opcode, arg0, arg1, arg2);
	return(1);
}

int isNumber(char *string)
{
	/* return 1 if string is a number */
	int i;
	return( (sscanf(string, "%d", &i)) == 1);
}

void outOfRegister(int pc,char* arg){
	if(atoi(arg)<0||atoi(arg)>7){
		printf("error: (Line %d) Registers outside the range\n",pc+1);
		exit(1);
	}
}

void outOfOffsetRange(int PC,char* arg){
	if(atoi(arg)<-32768||atoi(arg)>32767){
		printf("error: (Line %d) OffsetFields don’t fit in 16 bits\n",PC+1);
		exit(1);
	}
}