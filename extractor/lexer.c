#include "lexer.h"
#include <ctype.h>
#include <stdlib.h>

struct token *new_token(enum token_type type, char *identifier);
void add_token(struct token **dst, struct token *src);
void ignore(FILE *stream, char start, char end);
struct token *extract_ident(FILE *stream, struct trie_node *trie);

struct token *tokenize(FILE *stream, struct trie_node *trie) {
  // set result to dummy token
  struct token *result = new_token(STAR, ""), *current = result;
  char ch;

  while ((ch = fgetc(stream)) != EOF) {
    switch (ch) {
    case '{':
      add_token(&current, new_token(LEFT_BRACE, NULL));
      ignore(stream, '{', '}');
      break;
    case '}':
      add_token(&current, new_token(RIGHT_BRACE, NULL));
      break;
    case '(':
      add_token(&current, new_token(LEFT_PAREN, NULL));
      break;
    case ')':
      add_token(&current, new_token(RIGHT_PAREN, NULL));
      break;
    case ',':
      add_token(&current, new_token(COMMA, NULL));
      break;
    case '*':
      add_token(&current, new_token(STAR, NULL));
      break;
    case ';':
      add_token(&current, new_token(SEMICOLON, NULL));
      break;
    case ' ':
    case '\n':
    case '\0':
      continue;
    default:
      ungetc(ch, stream);
      struct token *token = extract_ident(stream, trie);
      if (token == NULL)
        continue;
      add_token(&current, token);
    }
  }

  // remove dummy token
  current = result;
  result = result->next;
  free(current);

  return result;
}

struct token *new_token(enum token_type type, char *identifier) {
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

int extract_word(FILE *stream, char **target) {
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

  (*target)[count++] = '\0';
  *target = realloc(*target, count);
  return count;
}

struct token *extract_ident(FILE *stream, struct trie_node *trie) {
  char *identifier;
  if (extract_word(stream, &identifier) == 0)
    return NULL;

  enum token_type type;
  if (!search(trie, identifier, &type))
    return new_token(IDENTIFIER, identifier);

  return new_token(type, identifier);
}
