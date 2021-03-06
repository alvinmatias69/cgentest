#include "lexer.h"
#include <ctype.h>
#include <stdlib.h>

struct token *new_token(enum token_type type, char *identifier);
void add_token(struct token **dst, struct token *src);
void ignore(FILE *stream, char start, char end);
struct token *extract_ident(FILE *stream, struct trie_node *trie);

// tokenize stream input into list of token. Each token type is categorized by
// token_type. Each list items are dynamically allocated.
struct token *tokenize(FILE *stream, struct trie_node *trie) {
  // set result to dummy token
  struct token *result = new_token(STAR, ""), *current = result;
  char ch;

  while ((ch = fgetc(stream)) != EOF) {
    switch (ch) {
    case '{':
      add_token(&current, new_token(LEFT_BRACE, NULL));
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
    case '=':
      add_token(&current, new_token(EQUAL, NULL));
      break;
    case '!':
      add_token(&current, new_token(EXCLAMATION, NULL));
      break;
    case '+':
      add_token(&current, new_token(PLUS, NULL));
      break;
    case '-':
      add_token(&current, new_token(MINUS, NULL));
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

// Create new token based on given parameters. Created token memory is
// dynamically created, need to be freed after used.
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

// ignore all char between start and end from input stream. Move read pointer to
// end char position / EOF.
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

// iterate over input stream for c identifier [a-zA-Z\_]+. Move the read pointer
// to the end of the identifier. Set target params with identifier value and
// return identifier length.
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

// extract identifier from input stream into a token. Need trie struct to
// determine whether identifier is a keyword.
struct token *extract_ident(FILE *stream, struct trie_node *trie) {
  char *identifier;
  if (extract_word(stream, &identifier) == 0)
    return NULL;

  enum token_type type;
  if (!search(trie, identifier, &type))
    return new_token(IDENTIFIER, identifier);

  return new_token(type, identifier);
}
