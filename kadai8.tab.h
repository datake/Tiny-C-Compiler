/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     ADD = 258,
     SUB = 259,
     MULTI = 260,
     DIV = 261,
     OR = 262,
     AND = 263,
     EQUAL_TO = 264,
     NOT_EQUAL_TO = 265,
     GREATER_THAN = 266,
     LESS_THAN = 267,
     GREATER_THAN_EQUAL = 268,
     LESS_THAN_EQUAL = 269,
     LP = 270,
     RP = 271,
     SEMICOLON = 272,
     COMMA = 273,
     RIGHT_BRACKET = 274,
     LEFT_BRACKET = 275,
     OPT = 276,
     ASSIGN = 277,
     COMMMA = 278,
     INT = 279,
     RETURN = 280,
     WHILE = 281,
     ELSE = 282,
     IF = 283,
     CONS = 284,
     DECLARATION = 285,
     FUNDEF1 = 286,
     STATEMENT = 287,
     STATEMENT_EXPRESSION = 288,
     STATEMENT_IF = 289,
     STATEMENT_IF_ELSE = 290,
     STATEMENT_WHILE = 291,
     STATEMENT_RETURN = 292,
     COMP_STATE = 293,
     ASSIGN_EXPR = 294,
     POSTFIX = 295,
     POSTFIX2 = 296,
     DECLARATOR = 297,
     UNARY = 298,
     CONSTANT = 299,
     TOKEN = 300,
     IDENTIFIER = 301
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 2068 of yacc.c  */
#line 22 "kadai8.y"

  int i;
  char *str;
  tree n;

 


/* Line 2068 of yacc.c  */
#line 105 "kadai8.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


