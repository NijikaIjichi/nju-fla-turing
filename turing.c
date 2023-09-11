#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define END(str) (&str[strlen(str) - 1])
#define SPLIT(s, str) (char *s = strtok((str), ",}"); s; s = strtok(NULL, ",}"))
#define Z2_(c) ((c) ? : '_')
#define _2Z(c) ((c) == '_' ? 0 : (c))
#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) < (b) ? (b) : (a))
#define DIGIT(n) snprintf(NULL, 0, "%d", n)
#define MINLEFT(tp) MIN((tp)->left, (tp)->curr)
#define MAXRIGHT(tp) MAX((tp)->right, (tp)->curr)
#define RANGE(l, r, tp) (char *l = MINLEFT(tp), *r = MAXRIGHT(tp); l <= r; ++l)
#define ABSPOS(p, tp) abs((p) - (tp)->zero)

#define check(cond) \
  do {if (!(cond)) {fprintf(stderr, "syntax error %s:%d\n", __FILE__, __LINE__); exit(1);}} \
  while (0)

int verbose;
int tape_num;
char tape_buf[1<<10][1<<20];
struct tape {
  char *curr, *zero, *left, *right;
} tapes[1<<10];
int state_num, curr_state;
char *states[1<<14];
int input_sym_lut[128], tape_sym_lut[128];
struct rule {
  int from, to;
  char *old, *new, *move;
} rules[1<<14];
int rule_num;
char file_buf[1<<14];

int find_state(char *state) {
  for (int i = 0; i < state_num; ++i) {
    if (strcmp(states[i], state) == 0) return i;
  }
  check(0);
}

void read_file(const char* filename) {
  FILE *fp = fopen(filename, "r");
  check(fp);
  while (fscanf(fp, " %[^\n]", file_buf) != EOF) {
    char *comment = strchr(file_buf, ';'), dummy;
    if (comment) *comment = 0;
    for (char *s = END(file_buf); s >= file_buf && isspace(*s); --s) *s = 0;
    if (file_buf[0] == 0) continue;
    if (file_buf[0] == '#') {
      check(strlen(file_buf) >= 6);
      switch (file_buf[1]) {
      case 'Q': check(strncmp(file_buf, "#Q = {", 6) == 0 && *END(file_buf) == '}');
        for SPLIT(s, &file_buf[6]) {
          for (char *i = s; *i; ++i) check(isalnum(*i) || *i == '_');
          states[state_num++] = strdup(s);
        } break;
      case 'S': check(strncmp(file_buf, "#S = {", 6) == 0 && *END(file_buf) == '}');
        for SPLIT(s, &file_buf[6]) {
          check(isprint(s[0]) && s[1] == 0 && strchr(" ,;{}*_", s[0]) == NULL);
          input_sym_lut[s[0]] = 1;
        } break;
      case 'G': check(strncmp(file_buf, "#G = {", 6) == 0 && *END(file_buf) == '}');
        for SPLIT(s, &file_buf[6]) {
          check(isprint(s[0]) && s[1] == 0 && strchr(" ,;{}*", s[0]) == NULL);
          tape_sym_lut[s[0]] = 1;
        } break;
      case 'q': check(strncmp(file_buf, "#q0 = ", 6) == 0);
        curr_state = find_state(&file_buf[6]); break;
      case 'B': check(strcmp(file_buf, "#B = _") == 0); break;
      case 'F': check(strncmp(file_buf, "#F = {", 6) == 0 && *END(file_buf) == '}');
        for SPLIT(s, &file_buf[6]) {find_state(s);} break;
      case 'N': check(strncmp(file_buf, "#N = ", 5) == 0 
        && sscanf(&file_buf[5], "%d%c", &tape_num, &dummy) == 1 && tape_num > 0); break;
      default: check(0);
      }
    } else {
      char *s = strtok(file_buf, " "); check(s); rules[rule_num].from = find_state(s);
      s = strtok(NULL, " "); check(s && strlen(s) == tape_num);
      for (char *i = s; *i; ++i) check(*i == '*' || tape_sym_lut[*i]);
      rules[rule_num].old = strdup(s);
      s = strtok(NULL, " "); check(s && strlen(s) == tape_num);
      for (char *i = s, *j = rules[rule_num].old; *i; ++i, ++j) 
        check((*i == '*' && *j == '*') || tape_sym_lut[*i]);
      rules[rule_num].new = strdup(s);
      s = strtok(NULL, " "); check(s && strlen(s) == tape_num);
      for (char *i = s; *i; ++i) check(strchr("rl*", *i) != NULL);
      rules[rule_num].move = strdup(s);
      s = strtok(NULL, " "); check(s); rules[rule_num].to = find_state(s);
      check(strtok(NULL, " ") == NULL); rule_num++;
    }
  }
  check(tape_sym_lut['_']);
  for (int i = 0; i < 128; ++i) check(!input_sym_lut[i] || tape_sym_lut[i]);
  for (int i = 0; i < tape_num; ++i) {
    struct tape *tp = &tapes[i];
    tp->curr = tp->left = tp->right = tp->zero = &tape_buf[i][1<<19];
  }
}

void print_tape(int i, int v) {
  struct tape *tp = &tapes[i];
  for (; tp->left[0] == 0 && tp->left <= MAXRIGHT(tp); tp->left++) ;
  for (; tp->right[0] == 0 && tp->right >= MINLEFT(tp); tp->right--) ;
  if (v) {
    printf("Index%-*d : ", DIGIT(tape_num), i);
    for RANGE(l, r, tp) printf("%d ", ABSPOS(l, tp));
    printf("\nTape%-*d  : ", DIGIT(tape_num), i);
    for RANGE(l, r, tp) printf("%-*c ", DIGIT(ABSPOS(l, tp)), Z2_(*l));
    printf("\nHead%-*d  : ", DIGIT(tape_num), i);
    for RANGE(l, r, tp) printf("%-*c ", DIGIT(ABSPOS(l, tp)), (l == tp->curr ? '^' : ' '));
  } else for (char *l = tp->left, *r = tp->right; l <= r; ++l) putchar(Z2_(*l));
  putchar('\n');
}

void print_turing(int step) {
  if (!verbose) return;
  printf("%-*s: %d\n", DIGIT(tape_num) + 6, "Step", step);
  printf("%-*s: %s\n", DIGIT(tape_num) + 6, "State", states[curr_state]);
  for (int i = 0; i < tape_num; ++i) {
    print_tape(i, 1);
  }
  printf("---------------------------------------------\n");
}

void do_turing() {
  for (int step = 0; ; ++step) {
    print_turing(step);
    for (int i = 0; i < rule_num; ++i) {
      struct rule *rl = &rules[i];
      if (rl->from == curr_state) {
        for (int j = 0; j < tape_num; ++j) {
          char curr = tapes[j].curr[0], old = rl->old[j];
          if (!((old == '*' && curr != 0) || old == Z2_(curr))) goto lose;
        }
        for (int j = 0; j < tape_num; ++j) {
          char new = rl->new[j], mv = rl->move[j];
          if (new != '*') tapes[j].curr[0] = _2Z(new);
          if (tapes[j].curr[0]) {
            tapes[j].left = MINLEFT(&tapes[j]);
            tapes[j].right = MAXRIGHT(&tapes[j]);
          }
          if (mv != '*') tapes[j].curr += mv == 'l' ? -1 : 1;
        }
        curr_state = rl->to;
        goto next;
      }
  lose:;
    }
    goto fin;
  next:;
  }
  fin:;
}

int main(int argc, char *argv[]) {
  char *filename = NULL, *input = NULL;
  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      printf("usage: turing [-v|--verbose] [-h|--help] <tm> <input>\n");
      return 0;
    } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) verbose = 1;
      else if (filename == NULL) filename = argv[i];
      else if (input == NULL) input = argv[i];
      else goto err;
  }
  if (filename == NULL) {
  err:
    fprintf(stderr, "usage: turing [-v|--verbose] [-h|--help] <tm> <input>\n");
    return 1;
  }
  if (input == NULL) input = "";
  read_file(filename);
  for (char *s = input; *s; ++s) {
    if (!input_sym_lut[*s]) {
      if (verbose) {
        fprintf(stderr, "Input: %s\n==================== ERR ====================\n", input);
        fprintf(stderr, "error: %c was not declared in the set of input symbols\n", *s);
        fprintf(stderr, "Input: %s\n", input);
        fprintf(stderr, "%*c\n", 8 + (int)(s - input), '^');
        fprintf(stderr, "==================== END ====================\n");
      } else fprintf(stderr, "illegal input\n");
      return 1;
    }
  }
  strcpy(tapes[0].curr, input);
  tapes[0].right += strlen(input) - 1;
  if (verbose) printf("Input: %s\n==================== RUN ====================\n", input);
  do_turing();
  if (verbose) {
    printf("Result: "); print_tape(0, 0);
    printf("==================== END ====================\n");
  } else print_tape(0, 0);
  return 0;
}
