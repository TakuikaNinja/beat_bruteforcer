#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MINLEN 4
#define MAXERRORS 7
#define DBG 0

// dictionary & length from dict.c
extern char *dict[];
extern int dictlen;

void fatal(char *s) {
	fprintf(stderr,"%s\n",s);
	exit(1);
}

void getword(char *word) {
	// pick a random word
	int pos = rand() % dictlen;
	char *wbuf;
	// skip words which are too short or contain hyphens/apostrophes
	do {
		wbuf = dict[pos];
		pos = (pos + 1) % dictlen;
	} while(strlen(wbuf) < MINLEN || strchr(wbuf,'-') != NULL || strchr(wbuf,'\'') != NULL);
	
	// copy word over to allocated string
	strcpy(word,wbuf);
	
	// print word for debugging purposes
	if (DBG) printf("%s\n", word);
}

struct player {
	char guess[26];
	char input;
	char alpha[26];
	int errors;
	int score;
};

void init(struct player *p, char *word) {
	memset(p->guess, 0, 26);
	p->input = '\0';
	memset(p->alpha, 0, 26);
	p->errors = 0;
	p->score = 0;
	for (int i=0; i<strlen(word); i++) {
		p->guess[i] = '-';
	}
}

void scanletter(struct player *p) {
	p->input = tolower(fgetc(stdin)); // get single char & convert to lower case
	while ((getchar()) != '\n'); // flush rest of stdin
}

int finished(struct player *p) {
	// if player lost
	if (p->errors == MAXERRORS) {
		return -1;
	}
	
	// if player completed the word
	for (int i=0; i<strlen(p->guess); i++) {
		if (p->guess[i] == '-') {
			return 0;
		}
	}
	return 1;
}

int checkguess(struct player *p, char *word) {
	int ok = 0;
	// fill all guessed letters
	for (int i=0; i<strlen(word); i++) {
		if (p->input == word[i]) {
			p->guess[i] = word[i];
			ok = 1;
		}
	}
	// save input in used letters
	p->alpha[p->input - 'a'] = p->input;
	
	// if no letters were found
	if (ok == 0) {
		p->errors++;
	}
	return ok;
}

void getinput(struct player *p, char *word) {
	// get valid input not seen yet
	do {
		printf("%s\nYour guess:\n", p->guess);
		scanletter(p);
		if (!isalpha(p->input)) {
			printf("Not a letter\n");
		} else if (p->alpha[p->input - 'a'] != 0) {
			printf("Already guessed\n");
			p->input = '\0';
		}
	} while(!isalpha(p->input));
	
	// check the input
	if(checkguess(p, word)) {
		printf("Correct!\n");
	} else {
		printf("Incorrect\n");
	}
}

void bruteforce(struct player *p, char *word) {
	// guess the letters in order of most to least common
	// order sourced from Robert Lewand's "Cryptological Mathematics"
	char *common = "etaoinshrdlcumwfgypbvkjxqz";
	for (int i=0; i<strlen(common); i++) {
		p->input = common[i];
		if (p->alpha[p->input - 'a'] == 0) {
			break;
		}
	}
	
	// for debugging purposes
	printf("\n");
	if (DBG) {
		printf("Bruteforcer's guess: %c\n", p->input);
	}
	
	// check the input
	if(checkguess(p, word)) {
		printf("Bruteforcer guessed a letter\n");
	} else {
		printf("Bruteforcer made an error\n");
	}
}

void printerrors(struct player *p, struct player *o) {
	printf("\nYour errors: %d\n", p->errors);
	printf("Bruteforcer's errors: %d\n", o->errors);
}

int main() {
	// seed RNG
	srand(time(0));
	
	printf("Welcome to beat_bruteforcer!\n");
	
	char word[26];
	int cont;
	do {
		cont = 0;
		
		// pick random word from dictionary
		memset(word, 0, 26);
		getword(word);
		
		// init players
		struct player player;
		init(&player, word);
		struct player cpu;
		init(&cpu, word);
		
		// get inputs from players until someone either guesses the word or makes too many errors
		while (finished(&player) == 0 && finished(&cpu) == 0) {
			printerrors(&player, &cpu);
			getinput(&player, word);
			bruteforce(&cpu, word);
		}
		printerrors(&player, &cpu);
		
		// reveal word and check who won
		printf("\nThe word was: %s\n", word);
		if (finished(&player) == finished(&cpu)) {
			printf("You tied the Bruteforcer\n");
		} else if (finished(&player) > finished(&cpu)) {
			printf("You won against the Bruteforcer!\n");
			player.score++;
		} else {
			printf("You lost to the Bruteforcer...\n");
			cpu.score++;
		}
		
		printf("Do you want to play again? (y/n):\n");
		scanletter(&player);
		if (player.input == 'y') {
			cont = 1;
		}
		printf("\nScores:\nYou %d:%d Bruteforcer\n", player.score, cpu.score);
	} while (cont);
	
	return 0;
}
