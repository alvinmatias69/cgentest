#ifndef TOKEN
#define TOKEN

enum token_type {
  LEFT_PAREN = '(',
  RIGHT_PAREN = ')',
  LEFT_BRACE = '{',
  RIGHT_BRACE = '}',
  COMMA = ',',
  SEMICOLON = ';',
  AMPERSAND = '&',
  STAR = '*',
  TYPE_SPECIFIER,
  TYPE_QUALIFIER,
  STORAGE_CLASS_SPECIFIER,
  STRUCT_OR_UNION,
  IDENTIFIER
};

#endif
