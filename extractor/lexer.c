#include "lexer.h"
#include <ctype.h>
#include <stdlib.h>

struct token *new (enum token_type type, char *identifier);
void add_token(struct token **dst, struct token *src);
void ignore(FILE *stream, char start, char end);
int extract_ident(FILE *stream, char **target);

struct token *tokenize(FILE *stream) {
  // set result to dummy token
  struct token *result = new (STAR, ""), *current = result;
  char ch;

  while ((ch = fgetc(stream)) != EOF) {
    switch (ch) {
    case '{':
      add_token(&current, new (LEFT_BRACE, NULL));
      ignore(stream, '{', '}');
      break;
    case '}':
      add_token(&current, new (RIGHT_BRACE, NULL));
      break;
    case '(':
      add_token(&current, new (LEFT_PAREN, NULL));
      break;
    case ')':
      add_token(&current, new (RIGHT_PAREN, NULL));
      break;
    case ',':
      add_token(&current, new (COMMA, NULL));
      break;
    case '*':
      add_token(&current, new (STAR, NULL));
      break;
    case ';':
      add_token(&current, new (SEMICOLON, NULL));
      break;
    case ' ':
    case '\n':
    case '\0':
      continue;
    // TODO: clean this up and add additional token classification
    default: {
      ungetc(ch, stream);
      char *identifier;
      extract_ident(stream, &identifier);
      add_token(&current, new (IDENTIFIER, identifier));
    }
    }
  }

  // remove dummy token
  current = result;
  result = result->next;
  free(current);

  return result;
}

struct token *new (enum token_type type, char *identifier) {
  struct token *token = malloc(sizeof(struct token));
  token->type = type;
  token->identifier = identifier;
  token->next = NULL;
  return token;
}

void add_token(struct token **dst, struct token *src) {
  (*dst)->next = src;
  (*dst) = src;
}

void ignore(FILE *stream, char start, char end) {
  int stack_count = 1;
  char ch;

  while (stack_count > 0) {
    ch = fgetc(stream);
    if (ch == EOF)
      break;

    if (ch == start) {
      stack_count++;
    } else if (ch == end) {
      stack_count--;
    }
  }

  ungetc(ch, stream);
}

int extract_ident(FILE *stream, char **target) {
  int count = 0;
  *target = malloc(sizeof(char) * MAXIMUM_IDENT_LENGTH);
  char ch;

  while ((ch = fgetc(stream)) != EOF) {
    if (isalnum(ch) == 0 && ch != '_') {
      ungetc(ch, stream);
      break;
    }

    (*target)[count++] = ch;
  }

  *target = realloc(*target, count);
  return count;
}
