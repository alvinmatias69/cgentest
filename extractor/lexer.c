#include "lexer.h"
#include <stdlib.h>

struct token *new (enum token_type type, char *identifier);
void add_token(struct token **dst, struct token *src);
void ignore(FILE *stream, char start, char end);

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
    default:
      continue;
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
